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
	 
	 File: net.h
	 
	 Description: general network code

	 Changes:
	 
	 28 Apr 2002: Initial release
	 
*/

/* $Id: net.h,v 1.2 2002/09/13 21:32:06 adrian Exp $ */

#ifndef NET_H
#define NET_H

#include "../config.h"

#include <stdio.h>

using namespace std;

#define CDDB_PORT	80

class net {

public:
	 net();
	 int connect();
	 void disconnect();
     char *readline(int socket);
protected:
	 int socket_1;    /* Descriptor for our first socket */
	 int socket_2;    /* Descriptor for our second socket */
	 int sock_mode;   /* Server read/write status */
	 FILE *sk_1;      /* Stream descriptor for our first socket */
	 FILE *sk_2;      /* Stream descriptor for our second socket */

};

#endif /* NET_H */
