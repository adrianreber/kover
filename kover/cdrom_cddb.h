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
	 
	 File: cdrom_cddb.h 
	 
	 Description: cdrom cddb class header
	 
	 Changes:
	 
	 2 Jul 2001: Initial release
	 
*/

/* $Id: cdrom_cddb.h,v 1.1 2001/07/09 09:15:27 adrian Exp $ */

#ifndef CDROM_CDDB_H
#define CDROM_CDBB_H

#include "../config.h"
#include "cdrom.h"


class cdrom_cddb : public cdrom {
public:
	 cdrom_cddb(char *path = "/dev/cdrom");
	 ~cdrom_cddb();
	 unsigned long get_cddb_id();
private:
	 void calc_cddb_id();
	 unsigned long cddb_id;	
	 int cddb_sum(int n);
};

#endif /* CDROM_CDBB_H */
