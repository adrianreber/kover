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
	
	File: CDDB.cpp
	
	Description: CDDB methods
	
	Created: 07 Jan 2001
	
	Changes:
	
	07 Jan 2001: created... mainly cut and paste from cddb_fill.h
	             with rewrite, some new methods
					 -now using the local cddb data if available in ~/.cddb
					 -saving the received cddb data in ~/.cddb
					 -tried to use threads... but failed
*/

#include "CDDB.h"

CDDB::CDDB()
{
  sock = 0;
#ifdef USE_THREADS
  cddb_thread=0;
  pthread_mutex_init(&cddb_lock,NULL);
  if(( semid = semget( SEMKEY, SEM_CNT, MODE | IPC_CREAT )) < 0 )
    {
		perror("Fehler beim Anfordern der Semaphoren");
		exit(-1);
    }
  sops[0].sem_num = 1;
  sops[0].sem_flg = IPC_NOWAIT;
  sops[0].sem_op  = 1;
 
  semop(semid,sops,1);
  printf("sema set\n");
  pthread_create(&cddb_thread, NULL, cddbThread, (void *)this);
#endif
}

CDDB::~CDDB()
{
  
}

bool CDDB::execute()
{
#ifdef USE_THREADS
  sops[0].sem_num = 1;
  sops[0].sem_flg = IPC_NOWAIT;
  sops[0].sem_op  = -1;
  semop(semid,sops,1);
#endif
  return true;
}

#ifdef USE_THREADS

void *cddbThread(void *parm)
{
  CDDB *cddb = (CDDB *) parm;
  
  if (0!=pthread_setcanceltype(PTHREAD_CANCEL_ENABLE,NULL))
    qWarning("pthread_setcanceltype failed!");
  if (0!= pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL))
    qWarning("pthread_setcanceltype failed!");

  while(1)
	 {
		cddb->sops[0].sem_num = 1;
		cddb->sops[0].sem_flg = SEM_UNDO;
		cddb->sops[0].sem_op  = 0;
		semop(cddb->semid,cddb->sops,1);

		cddb->connect(CDDB_HOST, CDDB_PORT);
		cddb->disconnect();

		cddb->sops[0].sem_num = 0;
		cddb->sops[0].sem_flg = IPC_NOWAIT;
		cddb->sops[0].sem_op  = -1;
		semop(cddb->semid,cddb->sops,1);
		cddb->sops[0].sem_num = 1;
		cddb->sops[0].sem_flg = IPC_NOWAIT;
		cddb->sops[0].sem_op  = 1;
		semop(cddb->semid,cddb->sops,1);


	 }
  return NULL;
}

#endif

int CDDB::connect(char *host, int port)
{
  sockaddr_in sin;
  hostent *h;
   
  //emit statusText(QString("Connecting to ") + QString(host) + "..." );

  if ((h = gethostbyname(host)) == NULL)
	 return errno;
 
  bcopy(h->h_addr, (char *) &sin.sin_addr, h->h_length);
                    
  sin.sin_family = h->h_addrtype;
  sin.sin_port   = htons(port);

  //emit statusText( "Connecting to CDDB server..." );
                            
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	 return errno;
                 
  if (::connect(sock,(struct sockaddr*)&sin, sizeof(sin)) < 0)
	 return errno;
     
  sk = fdopen(sock, "r+");
  if (sk == NULL)
	 {
		close(sock);
		return errno;
	 }

  return 0;   
}

void CDDB::disconnect()
{
  close(sock);
  if (sk != NULL)
	 fclose(sk);
}
