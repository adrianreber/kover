/** -*- adrian-c -*-
	 kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
	 Copyright (C) 2001 by Adrian Reber 
	 
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
	 
	 File: cddb_211_item.h 
	 
	 Description: cddb 211 items
	 
	 Changes:
	 
	 8 Jul 2001: Initial release
	 
*/

/* $Id: cddb_211_item.h,v 1.3 2001/07/15 22:21:53 adrian Exp $ */

#ifndef CDDB_211_ITEM_H
#define CDDB_211_ITEM_H

#include "../config.h"

class cddb_211_item {
public:
	 cddb_211_item();	 
	 cddb_211_item(char *);
	 ~cddb_211_item();
	 char *get();
private:
	 char *cddb_data;
};

#endif /* CDDB_211_ITEM_H */
