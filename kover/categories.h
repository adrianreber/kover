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
	 
	 File: categories.h 
	 
	 Description: entity for the categories

	 Changes:
	 
	 11 Nov 2001: Initial release
	 
*/

/* $Id: categories.h,v 1.3 2002/01/13 00:33:21 adrian Exp $ */

#ifndef CATEGORIES_H
#define CATEGORIES_H

#include "../config.h"
#include <list>
#include <string>

using namespace std;

class categories {

public:
	 categories();
	 ~categories();
	 int how_many();
	 string get_category(int id);
private:
	 list <string> category;

};

#endif /* CATEGORIES_H */
