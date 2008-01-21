/** -*- adrian-c -*-
	 kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
	 Copyright (C) 2002-2003 by Adrian Reber 
	 
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
	 
	 File: categories.cc
	 
	 Description: entity for the categories
	 
	 Changes:

	 11 Nov 2001: Initial release

*/

/* $Id: categories.cc,v 1.7 2003/02/07 16:44:40 adrian Exp $ */

#include "categories.h"

categories::categories() {
	 //category.push_back("All");
     category.push_back("Blues");
	 category.push_back("Classical");
	 category.push_back("Country");
	 category.push_back("Data");
	 category.push_back("Folk");
	 category.push_back("Jazz");
	 category.push_back("Misc");
	 category.push_back("Newage");
	 category.push_back("Reggae");
	 category.push_back("Rock");
	 category.push_back("Soundtrack");
}

categories::~categories() {

}

int categories::how_many() {
	 return (int)category.size();
}

string categories::get_category(int id) {
	 if (id < 0 || id >= how_many())
		  return string();
	 list <string> :: iterator cat;
	 int i = 0;
	 for (cat = category.begin(); cat != category.end(); cat++) {
		  if (i++ == id)
				return *cat;
    }
	 return string();
}
