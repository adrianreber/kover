/** -*- adrian-c -*-
	 kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
	 Copyright (C) 2001-2003 by Adrian Reber 
	 
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
	 
	 File: cdrom_cddb.cc
	 
	 Description: class for all cdrom cddb related stuff
	 
	 Changes:

	 2 Jul 2001: Initial release

*/

/* $Id: cdrom_cddb.cc,v 1.4 2004/04/20 21:04:28 adrian Exp $ */

#include "cdrom_cddb.h"
#include <stdio.h>

#ifdef __FreeBSD__
#include <sys/cdio.h>
#define CDROM_LEADOUT 0xAA
#endif

cdrom_cddb::cdrom_cddb(char *path):cdrom(path)
{
    open();
}

cdrom_cddb::~cdrom_cddb()
{
    close();
}

unsigned long cdrom_cddb::get_cddb_id()
{
    calc_cddb_id();
    return cddb_id;
}

void cdrom_cddb::calc_cddb_id()
{
#ifdef __FreeBSD__
    ioc_toc_header hdr;
    ioc_read_toc_single_entry entry;
#else
    cdrom_tochdr hdr;
    cdrom_tocentry entry;
#endif
    int i = 0;
    int pos = 0;
    int length = 0;
    int ntracks = 0;

    if (cdrom_fd < 0) {
        fprintf(stderr, "%s:negative filedescriptor. Exiting now!\n",
            PACKAGE);
        exit(0);
    }

#ifdef __FreeBSD__
    if (ioctl(cdrom_fd, CDIOREADTOCHEADER, &hdr) == -1) {
#else
    if (ioctl(cdrom_fd, CDROMREADTOCHDR, &hdr)) {
#endif
        fprintf(stderr,
            "%s:error while reading table of contents. exiting now!\n",
            PACKAGE);
        exit(0);
    }

#ifdef __FreeBSD__
    i = ntracks = hdr.ending_track;

    do {
        if (i == ntracks)
            entry.track = CDROM_LEADOUT;
        else
            entry.track = i + 1;
        entry.address_format = CD_MSF_FORMAT;
        if (ioctl(cdrom_fd, CDIOREADTOCENTRY, &entry)) {
#else
    i = ntracks = hdr.cdth_trk1;

    do {
        if (i == ntracks)
            entry.cdte_track = CDROM_LEADOUT;
        else
            entry.cdte_track = i + 1;
        entry.cdte_format = CDROM_MSF;
        if (ioctl(cdrom_fd, CDROMREADTOCENTRY, &entry)) {
#endif
            fprintf(stderr,
                "%s:error while reading toc entry. exiting now!\n", PACKAGE);
            exit(0);
        }
        if (i != ntracks)
            pos +=
#ifdef __FreeBSD__
                cddb_sum((entry.entry.addr.msf.minute * 60) +
                entry.entry.addr.msf.second);
        if (i == 0)
            length =
                length - ((entry.entry.addr.msf.minute * 60) +
                entry.entry.addr.msf.second);
        if (i == ntracks)
            length =
                (entry.entry.addr.msf.minute * 60) +
                entry.entry.addr.msf.second;
#else
                cddb_sum((entry.cdte_addr.msf.minute * 60) +
                entry.cdte_addr.msf.second);
        if (i == 0)
            length =
                length - ((entry.cdte_addr.msf.minute * 60) +
                entry.cdte_addr.msf.second);
        if (i == ntracks)
            length =
                (entry.cdte_addr.msf.minute * 60) +
                entry.cdte_addr.msf.second;
#endif

    } while (i--);

    cddb_id = (pos % 0xff) << 24 | length << 8 | ntracks;
}

int cdrom_cddb::cddb_sum(int n)
{
    int ret = 0;

    while (n) {
        ret += n % 10;
        n = n / 10;
    }

    return ret;
}
