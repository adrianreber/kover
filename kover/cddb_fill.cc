/** Hey emacs! This is: -*- adrian-c -*-
	 kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
	 Copyright (C) 1999-2000 by Denis Oliver Kropp
	 Copyright (C) 2000-2003 by Adrian Reber 
	   
	 This program is free software; you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation; either version 2 of the License, or
	 (at your option) any later version.
	 
	 This program is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 GNU General Public License for more details.
	 
	 You should have received a copy of the GNU General Public License
	 along with this program; if not, write to the Free Software
	 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
	 
	 File: cddb_fill.cc
	 
	 Description: the cddb code
	 
	 Changes: 
	 
	 14 Dec 1998: Initial release

	 11 Jan 2001: cddb over http

	 17 Jan 2001: threading support

	 14 Feb 2001: threading support disabled :(

	 20 Feb 2001: proxy support

	 17 Apr 2001: track duration

	 15 Jul 2001: 211(inexact match) support

	 30 Sep 2001: proxy authentification support

	 30 Oct 2001: Better error handling
	 
	 13 Nov 2001: CDDB without CD
	
*/

/* $Id: cddb_fill.cc,v 1.23 2004/04/20 21:04:28 adrian Exp $ */

#include "cddb_fill.moc"

#include "cddb_fill.h"

#include <string>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "cddb_211_item.h"
#include "inexact_dialog.h"
#include "proxy_auth.h"
#include "categories.h"

#ifdef __FreeBSD__
#include <sys/cdio.h>
#define CDROM_LEADOUT 0xAA
#endif

extern "C" {

#ifdef HAVE_LINUX_CDROM_H
#include <linux/cdrom.h>
#endif

#ifdef HAVE_LINUX_UCDROM_H
#include <linux/ucdrom.h>
#endif

} 

track_info::track_info(int _track, int _min, int _sec, int _frame)
{
    track = _track;
    min = _min;
    sec = _sec;
    length = min * 60 + sec;
    start = length * 75 + _frame;
}

CD_Info::CD_Info()
{
    trk.setAutoDelete(true);
}

CDDB_Fill::CDDB_Fill(KoverFile * _kover_file):QObject()
{
    kover_file = _kover_file;
    cd_fd = -1;
    code = 0;
}

CDDB_Fill::~CDDB_Fill()
{
}

bool CDDB_Fill::execute()
{

    char *message;

    if (!openCD()) {
        closeCD();
        return false;
    }
    if (!readTOC()) {
        closeCD();
        return false;
    }

    if (!getCDDBFromLocalFile()) {
        if (!cddb_connect()) {
            if (!cddb_query()) {
                closeCD();
                cddb_disconnect();
                return false;
            }
            cddb_disconnect();
        } else {
            if (errno < sys_nerr) {
                message =
                    (char *) malloc(strlen(strerror(errno)) +
                    strlen("Connecting to CDDB server failed: "));
                sprintf(message, "Connecting to CDDB server failed: %s",
                    strerror(errno));
                emit statusText(message);

                //free (message);
            } else if (errno == sys_nerr + 101) {
                emit statusText(tr
                    ("No http_proxy environment variable found... Giving up..."));
            } else if (errno == sys_nerr + 101) {
                emit statusText(tr
                    ("Don't understand http_proxy environment variable... Giving up..."));
            }
            closeCD();
            cddb_disconnect();
            return false;
        }
    }
    closeCD();
    return true;
}

bool CDDB_Fill::execute_without_cd(const char *id, int cat)
{
    char *message;
    bool without = true;
    categories *category = new categories();

    cdinfo.cddb_id = strtoul(id, NULL, 16);
    cdinfo.category =
        (QString((category->get_category(cat)).c_str())).lower();
    code = 200;
    cdinfo.artist = "Artist";
    cdinfo.cdname = "Title";
    cdinfo.length = 0;
    cdinfo.ntracks = 0;
    cdinfo.trk.clear();

    if (!getCDDBFromLocalFile(true)) {
        if (!cddb_connect()) {

            without = cddb_readcdinfo(sk_2, false, true, without);
            cddb_disconnect();

        } else {
            if (errno < sys_nerr) {
                message =
                    (char *) malloc(strlen(strerror(errno)) +
                    strlen("Connecting to CDDB server failed: "));
                sprintf(message, "Connecting to CDDB server failed: %s",
                    strerror(errno));
                emit statusText(message);

                //free (message);
            } else if (errno == sys_nerr + 101) {
                emit statusText(tr
                    ("No http_proxy environment variable found... Giving up..."));
            } else if (errno == sys_nerr + 102) {
                emit statusText(tr
                    ("Don't understand http_proxy environment variable... Giving up..."));
            }
            cddb_disconnect();
            if (category) {
                delete(category);
                category = NULL;
            }
            return false;
        }
    }
    if (category) {
        delete(category);
        category = NULL;
    }

    return without;
}

void CDDB_Fill::setTitleAndContents()
{
    QString tracks, contents, cddb_id;

    kover_file->setTitle(cdinfo.artist + "\n" + cdinfo.cdname);
    for (int i = 0; i < cdinfo.ntracks; i++) {
        if (globals.display_track_duration) {
            int m = 0;
            int n = 0;

            m = cdinfo.trk.at(i)->length / 60;
            n = cdinfo.trk.at(i)->length - m * 60;
            tracks.sprintf("(%.2d:%.2d)-%.2d. ", m, n, i + 1);
        } else
            tracks.sprintf("%.2d. ", i + 1);

        tracks.append(cdinfo.trk.at(i)->songname);
        if (i != cdinfo.ntracks - 1)
            tracks.append("\n");
        contents.append(tracks);
    }
    kover_file->setContents(contents);
    cddb_id.sprintf("0x%lx", cdinfo.cddb_id);
    kover_file->set_cddb_id(cddb_id);
}

void CDDB_Fill::cdInfo()
{
    QString str;

    str.sprintf
        (tr("CD contains %d tracks, total time is %d:%02d, the magic number is 0x%lx"),
        cdinfo.ntracks, cdinfo.length / 60, cdinfo.length % 60,
        cdinfo.cddb_id);
    _DEBUG_ fprintf(stderr, "%s:%s\n", PACKAGE, str.latin1());
    emit statusText(str);
    emit update_id(cdinfo.cddb_id);

}

int CDDB_Fill::openCD()
{
    int ds;

    _DEBUG_ fprintf(stderr, "CD opening\n");

    if (cd_fd != -1) {
        emit statusText(tr
            ("Internal error: Filedescriptor is not -1, already opened?"));
        return false;
    }

    if (!globals.cdrom_device)
        globals.cdrom_device = strdup("/dev/cdrom");

    if ((cd_fd = open(globals.cdrom_device, O_RDONLY | O_NONBLOCK)) < 0) {
#ifdef __FreeBSD__
        emit statusText(QString(tr("Error while opening "))
                + QString(globals.cdrom_device));
#else
        switch (errno) {
        case EACCES:
            emit statusText(QString(tr
                    ("You don´t have permission to read from "))
                + QString(globals.cdrom_device));
            break;
        case ENOMEDIUM:
            emit statusText(QString(tr("There´s no medium in "))
                + QString(globals.cdrom_device));
            break;
        case EBUSY:
            emit statusText(QString(globals.cdrom_device)
                + QString(tr(" is busy!")));
            break;
        default:
            emit statusText(QString(tr("Unknown error while opening "))
                + QString(globals.cdrom_device));
        }
#endif
        return false;
    }

#ifndef __FreeBSD__
    ds = ioctl(cd_fd, CDROM_DISC_STATUS);

    switch (ds) {
    case CDS_AUDIO:
    case CDS_MIXED:
        break;
    case CDS_NO_INFO:
        emit statusText(tr
            ("Oops. No information about disc. Will keep trying..."));
        break;
    default:
        emit statusText(QString(tr("There´s no audio cd in "))
            + QString(globals.cdrom_device) + QString(tr("! (ignoring)")));
        return false;
    }
    _DEBUG_ fprintf(stderr, "CD opened: %d\n", ds);
#endif

    return true;
}

void CDDB_Fill::closeCD()
{
    if (cd_fd != -1) {
        close(cd_fd);
        cd_fd = -1;
    }
}

bool CDDB_Fill::readTOC()
{
#ifdef __FreeBSD__
    ioc_toc_header hdr;
    ioc_read_toc_single_entry entry;
#else
    cdrom_tochdr hdr;
    cdrom_tocentry entry;
#endif
    int i, pos;
    _DEBUG_ fprintf(stderr, "Reading TOC\n");

    if (cd_fd < 0) {
        emit statusText(tr
            ("Internal error: Filedescriptor is -1, not opened?"));
        return false;
    }

    emit statusText(tr("Reading table of contents..."));

#ifdef __FreeBSD__
    if (ioctl(cd_fd, CDIOREADTOCHEADER, &hdr) == -1) {
#else
    if (ioctl(cd_fd, CDROMREADTOCHDR, &hdr)) {
#endif
        emit statusText(tr("Error while reading table of contents!"));

        return false;
    }

    cdinfo.artist = "Artist";
    cdinfo.cdname = "Title";
    cdinfo.length = 0;
#ifdef __FreeBSD__
    cdinfo.ntracks = hdr.ending_track;
#else
    cdinfo.ntracks = hdr.cdth_trk1;
#endif
    cdinfo.trk.clear();

    for (i = 0; i <= cdinfo.ntracks; i++) {
        if (i == cdinfo.ntracks)
#ifdef __FreeBSD__
            entry.track = CDROM_LEADOUT;
        else
            entry.track = i + 1;
        entry.address_format = CD_MSF_FORMAT;
        if (ioctl(cd_fd, CDIOREADTOCENTRY, &entry) == -1) {
#else
            entry.cdte_track = CDROM_LEADOUT;
        else
            entry.cdte_track = i + 1;
        entry.cdte_format = CDROM_MSF;
        if (ioctl(cd_fd, CDROMREADTOCENTRY, &entry)) {
#endif
            emit statusText(tr("Error while reading TOC entry!"));

            return false;
        }

#ifdef __FreeBSD__
        cdinfo.trk.append(new track_info(i + 1, entry.entry.addr.msf.minute,
                entry.entry.addr.msf.second, entry.entry.addr.msf.frame));

#else
        cdinfo.trk.append(new track_info(i + 1, entry.cdte_addr.msf.minute,
                entry.cdte_addr.msf.second, entry.cdte_addr.msf.frame));
#endif
    }

    pos = cdinfo.trk.first()->length;

    for (i = 0; i < cdinfo.ntracks; i++) {
        cdinfo.trk.at(i)->length = cdinfo.trk.at(i + 1)->length - pos;
        pos = cdinfo.trk.at(i + 1)->length;
    }

    cdinfo.length = cdinfo.trk.last()->length;

    cdinfo.cddb_id = calcID();

    emit statusText(tr("Table of contents successfully read"));
    _DEBUG_ fprintf(stderr, "Table of contents successfully read: %08lx\n",
        cdinfo.cddb_id);
    return true;
}

int CDDB_Fill::cddb_sum(int n)
{
    char buf[12], *p;
    uint ret = 0;

    sprintf(buf, "%lu", (unsigned long) n);
    for (p = buf; *p != '\0'; p++)
        ret += (*p - '0');

    return (ret);
}

unsigned long CDDB_Fill::calcID()
{
    int i, t = 0, n = 0;

    for (i = 0; i < cdinfo.ntracks; i++) {
        n += cddb_sum((cdinfo.trk.at(i)->min * 60) + cdinfo.trk.at(i)->sec);
    }

    t = ((cdinfo.trk.last()->min * 60) + cdinfo.trk.last()->sec) -
        ((cdinfo.trk.first()->min * 60) + cdinfo.trk.first()->sec);

    return ((n % 0xff) << 24 | t << 8 | cdinfo.ntracks);
}

void CDDB_Fill::parse_trails(char *ss)
{
    unsigned int i;

    for (i = 0; i < strlen(ss); i++) {
        if (ss[i] == '\r' || ss[i] == '\n')
            ss[i] = 0;
    }
}

int CDDB_Fill::cddb_connect()
{
    if (globals.use_proxy)
        emit statusText(QString(tr("Connecting to "))
        + QString(globals.proxy_server) + "...");

    else
    emit statusText(QString(tr("Connecting to "))
        + QString(globals.cddb_server) + "...");

    return net::connect();
}

void CDDB_Fill::cddb_disconnect()
{
    net::disconnect();
}

char *CDDB_Fill::cddbHello()
{
    return cddb_hello();
}

/** getCDDBFromLocalFile checks if there is already a file containing the disc info we are looking for.
 *  Currently the .cddb directory is used to check for available cddb entries
 *
 *
 */

bool CDDB_Fill::getCDDBFromLocalFile(bool without)
{
    if (!globals.read_local_cddb)
        return false;

    char *cddb_file;
    FILE *cddb_file_descriptor = NULL;
    char help_string[10];

    if (globals.cddb_path) {
        _DEBUG_ fprintf(stderr, "CDDBDIR=%s\n", globals.cddb_path);
	
	//if not locally, then category is = NULL 
	//FIXME
	cdinfo.category = "";
        cddb_file =
            (char *) malloc(strlen(globals.cddb_path) + 10 +
            strlen(cdinfo.category));
        strcpy(cddb_file, globals.cddb_path);
        strncat(cddb_file, cdinfo.category, strlen(cdinfo.category));
        strncat(cddb_file, "/", 1);
        snprintf(help_string, 9, "%08lx", cdinfo.cddb_id);
        strncat(cddb_file, help_string, 8);

        _DEBUG_ fprintf(stderr, "file : %s\n", cddb_file);

        cddb_file_descriptor = fopen(cddb_file, "r");

        free(cddb_file);

        if (cddb_file_descriptor) {
            emit statusText(tr("Using local values for disc"));

            cddb_readcdinfo(cddb_file_descriptor, true, true, without);
            fclose(cddb_file_descriptor);
            return true;
        }
    }
    return false;
}

/* Sends query to server -- this is the first thing to be done */
bool CDDB_Fill::cddb_query()
{
    char *code_string = NULL;
    char *query_buffer = NULL;
    char *http_buffer = NULL;
    char *offset_buffer = NULL;
    char *ss, *sss, *ssss;
    int i;
    int tot_len, len;

    emit statusText(tr("Querying database..."));

    /* Figure out a good buffer size -- 7 chars per track, plus 256 for the rest
       of the query */

    tot_len = (cdinfo.ntracks * 7) + 256;
    offset_buffer = (char *) malloc(tot_len);
    len = 0;

    // number ot tracks
    len = snprintf(offset_buffer, tot_len, "%d", cdinfo.ntracks);

    //all the offsets
    for (i = 0; i < cdinfo.ntracks; i++)
        len +=
            snprintf(offset_buffer + len, tot_len - len, "+%d",
            cdinfo.trk.at(i)->start);

    query_buffer = (char *) malloc(tot_len);

    //the query string
    len += snprintf(query_buffer, tot_len, "cddb+query+%08x+%s+%d",
        (unsigned int) cdinfo.cddb_id, offset_buffer, cdinfo.length);
    _DEBUG_ fprintf(stderr, "the query string : %s\n", query_buffer);

    http_buffer = make_cddb_request(query_buffer);

    if (!http_buffer)
        return false;

    _DEBUG_ fprintf(stderr, "Query is [%s]\n", http_buffer);

    write(socket_1, http_buffer, strlen(http_buffer));

    /* free free free */

    free(http_buffer);
    free(offset_buffer);

    // now using code of the http answer
    // to determine if we need a user name and pw
    code = 0;

    code = CDDBSkipHTTP(socket_1);

    _DEBUG_ fprintf(stderr, "kover:CDDB_Fill::cddb_query():http code:%d\n",
        code);

    switch (code) {
    case 407:
        if (!do_authentification(query_buffer, socket_1))
            return false;
        break;
    case 200:
        _DEBUG_ fprintf(stderr, "kover:sweeeeet!\n");
        // proceeding with standard operation
        break;
    default:
        _DEBUG_ fprintf(stderr, "O, I die, Horatio!\n");
        emit statusText("O, I die, Horatio!");

        fprintf(stderr, "kover:%s:%d: this should not happen\n", __FILE__,
            __LINE__);
        return false;
    }

    free(query_buffer);

    // determining the code of the cddb answer see http://www.freedb.org/sections.php?op=viewarticle&artid=28 for details
    code = 0;
    
    // finally some STL
    list < cddb_211_item * >inexact_list;
    inexact_dialog *inexact;
    int aber = 0;
    int bye = 1;

    //what category
    while (bye) {
        code_string = readline(socket_1);
        code = atoi(code_string);
        
        if (strlen(code_string) < 5)
            continue;

        _DEBUG_ fprintf(stderr, "answer: %d %s \n", code, code_string);

        if (strchr(code_string, 32))
            strcpy(cddb_msg, strchr(code_string, 32) + 1);
        free(code_string);
        switch (code) {
        case 200:              /* Success, get the category ID */
            //cddb_msg looks like : "newage 670db908....."
            _DEBUG_ fprintf(stderr, "code 200... %s\n", cddb_msg);
            ss = strchr(cddb_msg, 32);  //searching for " "
            *ss = 0;            // terminating the string cddb_msg after the first " "
            cdinfo.category = cddb_msg; //bla bla. cddb_msg now contains only the category
            bye = 0;            // leave the while loop
            break;
        case 211:
            _DEBUG_ fprintf(stderr,
                "Found inexact matches, list follows (until terminating marker)\n");
            emit statusText(tr
                ("Found inexact matches, list follows (until terminating marker)"));
            char s[256];

            s[0] = 0;

            cddb_211_item *ref_211;

            //reading all possible matches into ref_211
            while (strncmp(s, ".", 1) != 0) {
                if (!fgets(s, 255, sk_1)) {
                    bye = 0;    // leave the while loop
                    break;
                }
                //not the first element
                if (!aber++)
                    continue;
                if (s[0] != 48) {
                    ss = strchr(s, 13); // searching \r
                    *ss = 0;
                    ref_211 = new cddb_211_item(s);
                    _DEBUG_ fprintf(stderr, "%s:read:%s\n", PACKAGE, s);

                    //pushing everything in a list
                    inexact_list.push_back(ref_211);
                }
            }
            //removing the last element. it is just a "."
            inexact_list.remove(ref_211);
            inexact = new inexact_dialog(inexact_list);
            //dialog to choose one of the matches
            aber = inexact->exec();
            _DEBUG_ fprintf(stderr, "kover:inexact_dialog returns: %d\n",
                aber);

            if (aber == -1) {
                emit statusText("Our duty to your honour.");

                return false;
            }
            //getting the string
            ssss = inexact->get(aber);
            if (!ssss) {
                fprintf(stderr, "kover:%s:%d: this should not happen\n",
                    __FILE__, __LINE__);
                emit statusText("Why does the drum come hither?");

                return false;
            }

            _DEBUG_ fprintf(stderr, "kover:inexact->get(%d) returns:%s\n",
                aber, ssss);
            //string looks like : rock bd09280d Pink Floyd / The Wall (CD1)
            //searching first space
            ss = strchr(ssss, 32);
            *ss = 0;            // terminating the string after the first " "
            //cdinfo.category is now "rock"
            cdinfo.category = ssss;
            free(ssss);
            ssss = inexact->get(aber);
            if (!ssss) {
                fprintf(stderr, "kover:%s:%d: this should not happen\n",
                    __FILE__, __LINE__);
                emit statusText("Why does the drum come hither?");

                return false;
            }

            ss = strchr(ssss, 32);      //searching for " "
            //ss is now " bd09280d Pink Floyd / The Wall (CD1)"
            //watch out, a leading space (32)
            sss = strchr(ss + 1, 32);
            *sss = 0;
            //strtoul - convert a string to an unsigned long integer
            cdinfo.cddb_id = strtoul(ss + 1, NULL, 16);
            free(ssss);
            if (cdinfo.cddb_id == ULONG_MAX) {
                fprintf(stderr, "kover:%s:%d: this should not happen\n",
                    __FILE__, __LINE__);
                emit statusText("Why does the drum come hither?");

                return false;
            }
            _DEBUG_ fprintf(stderr, "kover:new id:0x%lx:category:%s\n",
                cdinfo.cddb_id, cdinfo.category.latin1());

            delete inexact;

            bye = 0;            // leave the while loop
            break;
            //return false;
        case 202:
            emit statusText(tr("No match found."));
            return false;
        case 403:
            emit statusText(tr("Database entry is corrupt."));
            return false;
        case 409:
            emit statusText(tr("No handshake."));
            return false;
        default:
            cddb_msg[strlen(cddb_msg) - 1] = 0;
            _DEBUG_ fprintf(stderr, "here(%02d): %s\n", code, cddb_msg);
        }
    }
    cddb_readcdinfo(sk_2, false, true);

    return true;
}

bool CDDB_Fill::cddb_readcdinfo(FILE * desc, bool local, bool save_as_file,
    bool without)
{
    char *cddb_file = NULL;
    FILE *cddb_file_descriptor = NULL;
    char *query_buffer = NULL;
    char *cddb_request = NULL;
    char help_string[10];
    bool file_opened = false;
    char s[256], *ss;
    int t;
    int track = 0;
    char *code_string = NULL;

    if (!local) {
        if (code == 200 || code == 211) {
            // cddb_query was a success, request info

            emit statusText(tr("Downloading CD info..."));

            t = strlen("cddb+read+%s+%08x") + strlen(cdinfo.category) +
                sizeof(cdinfo.cddb_id) + 10;

            query_buffer = (char *) malloc(t);

            snprintf(query_buffer, t, "cddb+read+%s+%08x",
                (const char *) cdinfo.category,
                (unsigned int) cdinfo.cddb_id);

            if (!globals.base64encoded)
                cddb_request = make_cddb_request(query_buffer);
            else
                cddb_request = make_cddb_request(query_buffer, true);

            if (!cddb_request) {
                without = false;
                return without;
            }
            _DEBUG_ fprintf(stderr, "Reading : %s\n", cddb_request);

            write(socket_2, cddb_request, strlen(cddb_request));

            free(cddb_request);

            // use proxy stuff needs to be included ... done

            code = CDDBSkipHTTP(socket_2);

            _DEBUG_ fprintf(stderr,
                "kover:CDDB_Fill::cddb_readcdinfo():http code:%d\n", code);

            switch (code) {
            case 200:
                _DEBUG_ fprintf(stderr, "kover:no comment\n");
                break;
            case 407:
                _DEBUG_ fprintf(stderr,
                    "kover:once more we need some authorization.\n");
                if (!do_authentification(query_buffer, socket_2))
                    return false;
                desc = sk_2;
                _DEBUG_ fprintf(stderr,
                    "kover:CDDB_Fill::cddb_readcdinfo():http code:%d\n",
                    code);
                break;
            default:
                fprintf(stderr, "kover:%s:%d: this should not happen\n",
                    __FILE__, __LINE__);
                return false;
            }

            free(query_buffer);

            int bye = 1;

            while (bye) {
                code_string = readline(socket_2);
                code = atoi(code_string);
                _DEBUG_ fprintf(stderr, "answer: %d %s \n", code,
                    code_string);
                if (strlen(code_string) < 5)
                    continue;
                if (strchr(code_string, 32))
                    strcpy(cddb_msg, strchr(code_string, 32) + 1);
                free(code_string);
                switch (code) {
                case 210:
                    emit statusText(tr("OK, CDDB database entry follows."));
                    bye = 0;    // leave the while loop
                    break;
                case 401:
                    emit statusText(tr("Specified CDDB entry not found."));
                    // maybe here without + search in all categories...
                    return false;
                case 402:
                    emit statusText(tr("Server error."));
                    return false;
                case 403:
                    emit statusText(tr("Database entry is corrupt."));
                    return false;
                case 409:
                    emit statusText(tr("No handshake."));
                    return false;
                default:
                    break;
                }
            }

        } else {
            fprintf(stderr, "kover:%s:%d: this should not happen\n",
                __FILE__, __LINE__);
            return false;
        }

    }

    s[0] = 0;

    while (strncmp(s, ".", 1) != 0) {
        if (!fgets(s, 255, desc))
            break;

        _DEBUG_ fprintf(stderr, "answer: %s", s);

        if (!local && save_as_file && globals.write_local_cddb) {
            if (!file_opened) {
                if (globals.cddb_path) {
                    cddb_file =
                        (char *) malloc(strlen(globals.cddb_path) + 10 +
                        strlen(cdinfo.category));
                    strcpy(cddb_file, globals.cddb_path);

                    strncat(cddb_file, cdinfo.category,
                        strlen(cdinfo.category));
                    strncat(cddb_file, "/", 1);
                    snprintf(help_string, 9, "%08lx", cdinfo.cddb_id);
                    strncat(cddb_file, help_string, 8);

                    _DEBUG_ fprintf(stderr, "using file: %s\n", cddb_file);

                    cddb_file_descriptor = fopen(cddb_file, "w");
                    free(cddb_file);

                    if (cddb_file_descriptor) {
                        file_opened = true;
                    }
                }
            }
            if (cddb_file_descriptor) {
                if (strstr(s, "#") || strstr(s, "=")) {
                    if (strstr(s, "# Submitted via:"))
                        fprintf(cddb_file_descriptor,
                            "# Submitted via: %s %s\n", PACKAGE, VERSION);
                    else if (strstr(s, "# xmcd CD database file"))
                        fprintf(cddb_file_descriptor,
                            "# xmcd CD database file generated by %s %s\n",
                            PACKAGE, VERSION);
                    else if (strstr(s, "# Processed by")
                        || strstr(s, "# Generated: ")
                        || strstr(s, "Copyright (C)"))
                        fprintf(cddb_file_descriptor,
                            "#\n# Revenge his foul and most unnatural murder. (Hamlet I.5.25)\n#\n");
                    else if (strstr(s, "DISCID"))
                        fprintf(cddb_file_descriptor, "DISCID=%08lx\n",
                            cdinfo.cddb_id);
                    else
                        fprintf(cddb_file_descriptor, "%s", s);
                }
            }
        }

        if ((ss = strstr(s, "DTITLE")) != NULL) {
            ss += 7;
            ss[strlen(ss) - 1] = 0;
            cdinfo.cdnames = ss;

            ss = strchr(s, '/');
            *ss = 0;
            cdinfo.artist = s + 7;
            cdinfo.cdname = ss + 2;
        }

        if (((ss = strstr(s, "Disc length:")) != NULL) && without) {
            cdinfo.length = atoi(ss + 13);
            _DEBUG_ fprintf(stderr, "%s:length:%d\n", PACKAGE, cdinfo.length);
        }

        if ((ss = strstr(s, "TTITLE")) == NULL)
            continue;

        /* Yeah, yeah. Cheap hack, but it's guarenteed to work! :)
         * The following just hacks the returned track name into the variable 
         */

        ss += 6;
        t = atoi(ss);
        if (t < 100)
            ss += t < 10 ? 2 : 3;
        else
            ss += 4;

        parse_trails(ss);

        if (without) {
            cdinfo.trk.append(new track_info(++track, 0, 0, 0));
            cdinfo.ntracks++;
        }
        if (cdinfo.trk.at(t)->songname.length())
            cdinfo.trk.at(t)->songname += ss;
        else
            cdinfo.trk.at(t)->songname = ss;
    }

    if (file_opened && cddb_file_descriptor)
        fclose(cddb_file_descriptor);

    return true;
}

int CDDB_Fill::CDDBSkipHTTP(int socket)
{
    return skip_http_header(socket);
}

char *CDDB_Fill::make_cddb_request(char *query_me, bool use_auth)
{
    char *return_me = NULL;

    return_me = cddb::make_cddb_request(query_me, use_auth);

    if (!strcmp(return_me, "ERRGREET")) {
        emit statusText(tr("Trouble creating cddb greeting... Giving up..."));

        free(return_me);
        return NULL;
    }

    return return_me;
}

bool CDDB_Fill::do_authentification(char *query_buffer, int socket)
{
    proxy_auth *proxy_auth_dialog = NULL;
    int aber = 0;
    int code = 0;
    char *http_buffer = NULL;

    //loop until the correct password is entered
    while (1) {
        if (!globals.base64encoded) {
            _DEBUG_ fprintf(stderr, "kover:now requesting authorization.\n");
            emit statusText(tr("Initializing authorization"));

            proxy_auth_dialog =
                new proxy_auth(globals.proxy_server, globals.proxy_port);
            // invoking passwd dialog...
            aber = proxy_auth_dialog->exec();
            _DEBUG_ fprintf(stderr, "kover:proxy_auth returns: %d\n", aber);

            if (aber) {
                emit statusText(tr("Operation aborted."));

                //canceled
                return false;
            }
            globals.base64encoded = proxy_auth_dialog->get_authentification();
            delete proxy_auth_dialog;

            proxy_auth_dialog = NULL;
        }
        cddb_disconnect();
        cddb_connect();
        http_buffer = NULL;
        http_buffer = make_cddb_request(query_buffer, true);

        if (!http_buffer)
            return false;
        _DEBUG_ fprintf(stderr, "Query is [%s]\n", http_buffer);

        write(socket, http_buffer, strlen(http_buffer));
        free(http_buffer);
        code = CDDBSkipHTTP(socket);
        if (code != 407)
            break;
        else {
            if (globals.base64encoded)
                free(globals.base64encoded);
            globals.base64encoded = NULL;
        }
    }
    return true;
}
