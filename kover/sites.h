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
	 
	 File: sites.h
	 
	 Description: entity for the sites

	 Changes:
	 
	 24 Apr 2002: Initial release
	 
*/

/* $Id: sites.h,v 1.1 2002/04/24 15:27:44 adrian Exp $ */

#ifndef SITES_H
#define SITES_H

#include "../config.h"
#include <list>
#include <string>

using namespace std;

class sites {

public:
	 sites();
	 ~sites();
	 int how_many();
	 string get_site(int id);
private:
	 list <string> site;

};

#endif /* SITE_H */
