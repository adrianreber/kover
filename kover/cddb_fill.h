/***************************************************************************
                          cddb_fill.h  -  description                              
                             -------------------                                         

    version              :                                   
    begin                : Sun Jan 10 1999                                           
    copyright            : (C) 1999 by Denis Oliver Kropp                         
    email                : dok@fischlustig.de                                     
 ***************************************************************************/

/*************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

// alter mach kaffe

#ifndef CDDB_FILL_H
#define CDDB_FILL_H

#include "kover.h"

#include <qobject.h>
#include <qstring.h>
#include <qlist.h>
#include "koverfile.h"

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define CD_DEVICE		"/dev/cdrom"

#define CDDB_HOST		"212.86.129.227"
#define CDDB_PORT		80


#define SEND(x)	fprintf(sk, "%s\n", x); fflush(sk);
/**
  *@author Denis Oliver Kropp
  */

class Track_Info
{
public:
	Track_Info( int _track, int _min, int _sec, int _frame );

	QString	songname;	/* Name of song, dynamically allocated */
	int	min; //starting minute of the track
	int	sec; //starting second of the track
	int	length;		/* Length of track in seconds or Kbytes */
	int	start;		/* Starting position */
	int	track;		/* Physical track number */
};

class CD_Info
{
public:
	CD_Info();

	QString	artist;		/* Artist's name */
	QString	cdname;		/* Disc's name */
	QString	cdnames;	/* CDDB artist and CD name string */
	QString	catagory;	/* CDDB catagory */
	int	ntracks;	/* Number of tracks on the disc */
	int	length;		/* Total running time in seconds */

	QList<Track_Info>	trk;		/* struct trackinfo[ntracks] */
	unsigned long		cddb_id;	/* CDDB ID which gets calculated */
};

class CDDB_Fill : public QObject
{
	Q_OBJECT
public: 
	CDDB_Fill( KoverFile* _kover_file );
	~CDDB_Fill();
	
	bool execute();

	void cdInfo();
	
signals:
	void statusText( const char* );
	void updateDisplay(bool update_really = false);

protected:
	int			openCD();
	void			closeCD();
	bool			readTOC();
	unsigned long	calcID();
	
	KoverFile*	kover_file;
	CD_Info		cdinfo;
	int			cd_fd;
	
	int	cddb_sum(int n);
	
	void	parse_trails(char *ss);
	int	cddb_code();
	int	cddb_connect(char *host, int port);
	void	cddb_disconnect();
	char *cddb_hello();
	
	void	cddb_query();
	void	cddb_readcdinfo(FILE *desc, bool local);
	int CDDBReadLine(int sock,char *inbuffer,int len);
	void remove_line(char *string);

public:
	void setTitleAndContents();
	void killThread();

private:
#ifdef USE_THREADS
	friend void *cddbThread(void *parm);
	pthread_t cddb_thread;
	pthread_mutex_t cddb_lock;
#endif
	bool getCDDBFromLocalFile();


};

#endif
