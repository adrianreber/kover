/**  hey emacs! try -*- adrian-c -*- mode
	 kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
	 Copyright (C) 2001, 2002 by Adrian Reber 
	 
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
	 
	 File: config.cc  
	 
	 Description: reads and write the config file
	 
	 Changes:

	 05 Apr 2001: initial thingy
*/

/* $Id: config.cc,v 1.6 2002/08/07 17:48:42 adrian Exp $ */

#include "config.h"
#include <kconfig.h>
#include <sys/stat.h>
#include <stdio.h>

config_class::config_class(KApplication * kover)
{
    this->kover = kover;
}

void config_class::load_globals()
{
    struct stat stat_struct;
    KConfig *config = kover->config();
    QString *string = new QString();

    config->setGroup("CDDB");

    if ((config->readEntry("cddb_server")).isEmpty())
        globals.cddb_server = strdup("freedb.freedb.org");
    else
        globals.cddb_server =
            strdup((config->readEntry("cddb_server")).latin1());

    if ((config->readEntry("cgi_path")).isEmpty())
        globals.cgi_path = strdup("~cddb/cddb.cgi");
    else
        globals.cgi_path = strdup((config->readEntry("cgi_path")).latin1());

    if ((config->readEntry("proxy_server")).isEmpty())
        globals.proxy_server = NULL;
    else
        globals.proxy_server =
            strdup((config->readEntry("proxy_server")).latin1());

    *string = config->readEntry("proxy_port");
    if (string->isEmpty())
        globals.proxy_port = 3128;
    else
        globals.proxy_port = string->toInt();

    _DEBUG_ fprintf(stderr, "proxy port: %d\n", globals.proxy_port);

    *string = config->readEntry("use_proxy");
    if (string->isNull())
        globals.use_proxy = 0;
    else
        globals.use_proxy = string->toInt();

    *string = config->readEntry("proxy_from_env");
    if (string->isNull())
        globals.proxy_from_env = 0;
    else
        globals.proxy_from_env = string->toInt();

    config->setGroup("CDROM");

    if ((config->readEntry("cdrom_device")).isEmpty())
        globals.cdrom_device = strdup("/dev/cdrom");
    else
        globals.cdrom_device =
            strdup((config->readEntry("cdrom_device")).latin1());

    *string = config->readEntry("eject_cdrom");
    if (string->isNull())
        globals.eject_cdrom = 0;
    else
        globals.eject_cdrom = string->toInt();

    config->setGroup("CDDB_files");
    *string = config->readEntry("read_local_cddb");
    if (string->isNull())
        globals.read_local_cddb = 0;
    else
        globals.read_local_cddb = string->toInt();
    *string = config->readEntry("write_local_cddb");
    if (string->isNull())
        globals.write_local_cddb = 0;
    else
        globals.write_local_cddb = string->toInt();
    if ((config->readEntry("cddb_path")).isEmpty()) {
        globals.cddb_path = check_cddb_dir();
    } else {
        globals.cddb_path = strdup((config->readEntry("cddb_path")).latin1());
        /* checking for "/" at the end */
        if (globals.cddb_path[strlen(globals.cddb_path) - 1] != '/') {
            char *dummy = strdup(globals.cddb_path);

            free(globals.cddb_path);
            globals.cddb_path = (char *) malloc(strlen(dummy) + 1);
            strcpy(globals.cddb_path, dummy);
            strcat(globals.cddb_path, "/");
            free(dummy);
        }
        /* does the directory exist */
        if (stat(globals.cddb_path, &stat_struct) == -1)
            /* no it doesn't... let's create one */
            if (mkdir(globals.cddb_path, 0777) == -1) {
                /* failed */
                free(globals.cddb_path);
                globals.cddb_path = NULL;
            }
    }

    config->setGroup("misc");
    *string = config->readEntry("trigger_actual_size");
    if (string->isNull())
        globals.trigger_actual_size = 1;
    else
        globals.trigger_actual_size = string->toInt();

    *string = config->readEntry("display_track_duration");
    if (string->isNull())
        globals.display_track_duration = 1;
    else
        globals.display_track_duration = string->toInt();

    *string = config->readEntry("its_a_slim_case");
    if (string->isNull())
        globals.its_a_slim_case = 0;
    else
        globals.its_a_slim_case = string->toInt();

    *string = config->readEntry("one_page");
    if (string->isNull())
        globals.one_page = 0;
    else
        globals.one_page = string->toInt();

    *string = config->readEntry("xpos");
    if (string->isNull())
        globals.xpos = 0;
    else
        globals.xpos = string->toInt();

    *string = config->readEntry("ypos");
    if (string->isNull())
        globals.ypos = 0;
    else
        globals.ypos = string->toInt();

    *string = config->readEntry("save_position");
    if (string->isNull())
        globals.save_position = 1;
    else
        globals.save_position = string->toInt();

    *string = config->readEntry("disable_animation");
    if (string->isNull())
        globals.disable_animation = 0;
    else
        globals.disable_animation = string->toInt();

    config->setGroup("fonts");
    if ((config->readEntry("content_font")).isEmpty())
        globals.content_font = new QFont("helvetica", 16);
    else {
        QFont *bla = new QFont();

        bla->setRawName(config->readEntry("content_font"));
        globals.content_font = new QFont(*bla);
        delete(bla);
        _DEBUG_ fprintf(stderr, "kover:font loaded: %s\n",
            ((globals.content_font)->rawName()).latin1());
    }

    if ((config->readEntry("title_font")).isEmpty())
        globals.title_font = new QFont("helvetica", 32);
    else {
        QFont *bla = new QFont();

        bla->setRawName(config->readEntry("title_font"));
        globals.title_font = new QFont(*bla);
        delete(bla);
        _DEBUG_ fprintf(stderr, "kover:font loaded: %s\n",
            ((globals.title_font)->rawName()).latin1());
    }

    if ((config->readEntry("inlet_title_font")).isEmpty())
        globals.inlet_title_font = new QFont("helvetica", 32);
    else {
        QFont *bla = new QFont();

        bla->setRawName(config->readEntry("inlet_title_font"));
        globals.inlet_title_font = new QFont(*bla);
        delete(bla);
        _DEBUG_ fprintf(stderr, "kover:font loaded: %s\n",
            ((globals.inlet_title_font)->rawName()).latin1());
    }

    delete(string);
    globals.base64encoded = NULL;
}

void config_class::store_globals()
{
    _DEBUG_ fprintf(stderr,
        "kover: entering config_class::store_globals()\n");
    KConfig *config = kover->config();
    QString *string = new QString();

    config->setGroup("CDDB");
    config->writeEntry("cddb_server", globals.cddb_server);
    config->writeEntry("cgi_path", globals.cgi_path);
    config->writeEntry("proxy_server", globals.proxy_server);
    string->sprintf("%d", globals.proxy_port);
    config->writeEntry("proxy_port", *string);
    string->sprintf("%d", globals.use_proxy);
    config->writeEntry("use_proxy", *string);
    string->sprintf("%d", globals.proxy_from_env);
    config->writeEntry("proxy_from_env", *string);

    config->setGroup("CDROM");
    string->sprintf("%d", globals.eject_cdrom);
    config->writeEntry("eject_cdrom", *string);
    config->writeEntry("cdrom_device", globals.cdrom_device);

    config->setGroup("CDDB_files");
    string->sprintf("%d", globals.read_local_cddb);
    config->writeEntry("read_local_cddb", *string);
    string->sprintf("%d", globals.write_local_cddb);
    config->writeEntry("write_local_cddb", *string);
    config->writeEntry("cddb_path", globals.cddb_path);

    config->setGroup("misc");
    string->sprintf("%d", globals.trigger_actual_size);
    config->writeEntry("trigger_actual_size", *string);

    string->sprintf("%d", globals.display_track_duration);
    config->writeEntry("display_track_duration", *string);

    string->sprintf("%d", globals.its_a_slim_case);
    config->writeEntry("its_a_slim_case", *string);

    string->sprintf("%d", globals.one_page);
    config->writeEntry("one_page", *string);

    string->sprintf("%d", globals.xpos);
    config->writeEntry("xpos", *string);

    string->sprintf("%d", globals.ypos);
    config->writeEntry("ypos", *string);

    string->sprintf("%d", globals.save_position);
    config->writeEntry("save_position", *string);

    string->sprintf("%d", globals.disable_animation);
    config->writeEntry("disable_animation", *string);

    config->setGroup("fonts");
    config->writeEntry("content_font",
        ((globals.content_font)->rawName()).latin1());
    config->writeEntry("title_font",
        ((globals.title_font)->rawName()).latin1());
    config->writeEntry("inlet_title_font",
        ((globals.inlet_title_font)->rawName()).latin1());

    delete(string);
    _DEBUG_ fprintf(stderr, "kover: leaving config_class::store_globals()\n");
}

char *config_class::check_cddb_dir()
{
    char *home_dir = NULL;
    char *cddb_file = NULL;
    struct stat stat_struct;

    home_dir = getenv("HOME");
    if (home_dir) {
        if (home_dir[strlen(home_dir) - 1] != '/') {
            cddb_file = (char *) malloc(strlen(home_dir) + 7 + 8);
            strcpy(cddb_file, home_dir);
            strcat(cddb_file, "/.cddb/");
        } else {
            cddb_file = (char *) malloc(strlen(home_dir) + 6 + 8);
            strcpy(cddb_file, home_dir);
            strcat(cddb_file, ".cddb/");
        }

        /* does the directory exist */
        if (stat(cddb_file, &stat_struct) == -1)
            /* no it doesn't... let's create one */
            if (mkdir(cddb_file, 0777) == -1) {
                /* failed */
                free(cddb_file);
                return NULL;
            }

        return cddb_file;
    }

    return NULL;
}

void config_class::sync()
{
    kover->config()->sync();
}
