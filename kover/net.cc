/** -*- adrian-c -*-
	 kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
	 Copyright (C) 2001-2002 by Adrian Reber 
	 
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
	 
	 File: net.cc
	 
	 Description: general network code
	 
	 Changes:

	 28 Apr 2002: Initial release

*/

/* $Id: net.cc,v 1.1 2002/04/28 09:33:51 adrian Exp $ */

#include "net.h"
#include "kover.h"

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <errno.h>

net::net() {
		  socket_1 = 0;
		  socket_2 = 0;
		  sock_mode = 0;
		  sk_1 = NULL;
		  sk_2 = NULL;
}

int net::connect() {
	 sockaddr_in sin;
	 hostent *h;
   
	 if (globals.use_proxy) {
		  if ((h = gethostbyname(globals.proxy_server)) == NULL)
				return errno;
	 } else {
		  if ((h = gethostbyname(globals.cddb_server)) == NULL)
				return errno;
	 }
 
	 bcopy(h->h_addr,(char *) &sin.sin_addr, h->h_length);
			
	 sin.sin_family = h->h_addrtype;

	 if (globals.use_proxy)
		  sin.sin_port   = htons(globals.proxy_port);
	 else
		  sin.sin_port   = htons(CDDB_PORT);

	 if ((socket_1 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		  return errno;

	 if ((socket_2 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		  return errno;
                 
	 if (::connect(socket_1,(struct sockaddr*)&sin, sizeof(sin)) < 0)
		  return errno;

	 if (::connect(socket_2,(struct sockaddr*)&sin, sizeof(sin)) < 0)
		  return errno;
     
	 sk_1 = fdopen(socket_1, "r+");
	 sk_2 = fdopen(socket_2, "r+");
	 if (sk_1 == NULL || sk_2 == NULL) {
		  close(socket_1);
		  close(socket_2);
		  return errno;
	 }

	 return 0;   
}

void net::disconnect() {
	 close(socket_1);
	 if (sk_1 != NULL)
		  fclose(sk_1);
	 close(socket_2);
	 if (sk_2 != NULL)
		  fclose(sk_2);
}
