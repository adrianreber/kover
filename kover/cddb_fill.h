/** -*- adrian-c -*-
	 kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
	 Copyright (C) 1999, 2000 by Denis Oliver Kropp
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
	
	 File: cddb_fill.h 

	 Description: the cddb header

	 Changes: 

	 10 Jan 1999: Initial release

*/

/* $Id: cddb_fill.h,v 1.25 2003/02/07 16:44:40 adrian Exp $ */

#ifndef CDDB_FILL_H
#define CDDB_FILL_H

#include "kover.h"
#include "koverfile.h"
#include "net.h"
#include "cddb.h"

#include <stdio.h>
#include <errno.h>
#include <qlist.h>

#define CDDB_PORT		80

#define KOVER_CDDB_LEVEL "3"    /* Current CDDB protocol level supported */

class track_info {
  public:
    track_info(int _track, int _min, int _sec, int _frame);

    QString songname;           /* Name of song, dynamically allocated */
    int min;                    //starting minute of the track
    int sec;                    //starting second of the track
    int length;                 /* Length of track in seconds or Kbytes */
    int start;                  /* Starting position */
    int track;                  /* Physical track number */
};

class CD_Info {
  public:
    CD_Info();
    /* Artist's name */
    QString artist;
    /* Disc's name */
    QString cdname;
    /* CDDB artist and CD name string */
    QString cdnames;
    /* CDDB category */
    QString category;
    /* Number of tracks on the disc */
    int ntracks;
    /* Total running time in seconds */
    int length;
    /* struct trackinfo[ntracks] */
      QList < track_info > trk;
    /* CDDB ID which gets calculated */
    unsigned long cddb_id;
};

class CDDB_Fill:public QObject, net, cddb {
  Q_OBJECT public:
      CDDB_Fill(KoverFile * _kover_file);
     ~CDDB_Fill();
    bool execute();
    bool execute_without_cd(const char *, int);
    void cdInfo();
    void setTitleAndContents();
      signals:void statusText(const char *);
    void update_id(unsigned long);

  private:
      KoverFile * kover_file;
    CD_Info cdinfo;
    int cd_fd;
    char cddb_msg[255];         /* Return message of server info */
    int code;                   /* Return value for sending data to the server */

    int openCD();
    void closeCD();
    bool readTOC();
    unsigned long calcID();
    int cddb_sum(int n);
    void parse_trails(char *ss);
    int cddb_connect();
    void cddb_disconnect();
    char *cddbHello();
    bool cddb_query();
    bool cddb_readcdinfo(FILE * desc, bool local, bool save_as_file,
        bool without = false);
    bool do_authentification(char *query_buffer, int socket);
    bool getCDDBFromLocalFile(bool without = false);
    int CDDBSkipHTTP(int socket);
    char *make_cddb_request(char *query_me, bool use_auth = false);
};

#endif /* CDDB_FILL_H */
