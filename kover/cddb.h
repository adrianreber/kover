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
	 
	 File: cddb.h
	 
	 Description: general cddb code

	 Changes:
	 
	 28 Apr 2002: Initial release
	 
*/

/* $Id: cddb.h,v 1.1 2002/04/28 21:56:12 adrian Exp $ */

#ifndef CDDB_H
#define CDDB_H

#include "../config.h"

#include <stdio.h>

#define KOVER_CDDB_LEVEL "3"  /* Current CDDB protocol level supported */

using namespace std;

class cddb {

public:
		  char * cddb_hello();
		  int skip_http_header(int socket);
		  char * make_cddb_request(char *query_me, bool use_auth);
};

#endif /* NET_H */
