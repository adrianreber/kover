/**
	kover -
	
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
	
	File: CDDB.h
	
	Description: Header to the new CDDB class.
*/


#ifndef CDDB_H
#define CDDB_H

#include "kover.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>
#include <errno.h>

#define CDDB_HOST		"212.86.129.227"
#define CDDB_PORT		80


class CDDB
{

 public: 
  CDDB();
  ~CDDB();
	
	bool execute();

 private:
	int	connect(char *host, int port);
	void	disconnect();
	
 private:
	
#ifdef USE_THREADS
	friend void *cddbThread(void *parm);
	pthread_t cddb_thread;
	pthread_mutex_t cddb_lock;

	int semid;
	struct sembuf sops[2];
#endif
	int sock;		/* Descriptor for our socket */
	FILE *sk;		/* Stream descriptor for our socket */

};

#endif
