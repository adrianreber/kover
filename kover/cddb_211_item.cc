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
	 
	 File: cddb_211_item.cc
	 
	 Description: wrapper class for the 211 match
	 
	 Changes:

	 8 Jul 2001: Initial release

*/

/* $Id: cddb_211_item.cc,v 1.6 2003/02/07 16:44:40 adrian Exp $ */

#include "cddb_211_item.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

cddb_211_item::cddb_211_item() {
	 cddb_data = NULL;
}

cddb_211_item::cddb_211_item(char *input) {
	 cddb_data = strdup(input);
}

cddb_211_item::~cddb_211_item() {
	 if (cddb_data)
		  free (cddb_data);
}

char *cddb_211_item::get() {
	 return strdup(cddb_data);
}
