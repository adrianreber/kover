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
	 
	 File: sites.cc
	 
	 Description: entity for the sites
	 
	 Changes:

	 24 Apr 2002: Initial release

*/

/* $Id: sites.cc,v 1.2 2002/04/28 21:55:24 adrian Exp $ */

#include "sites.h"

sites::sites() {
	 //category.push_back("All");
	 site.push_back("Classical");
	 site.push_back("Country");
	 site.push_back("Data");
	 site.push_back("Folk");
	 site.push_back("Jazz");
	 site.push_back("Misc");
	 site.push_back("Newage");
	 site.push_back("Reggae");
	 site.push_back("Rock");
	 site.push_back("Soundtrack");
	 connect();
}

sites::~sites() {
	disconnect();
}

int sites::how_many() {
	 //return (int)site.size();
	 char * bla = "sites";
	 char *test = NULL;
	 test = make_cddb_request(bla,false);

	 printf("%s\n",test);
	 
	 return 0;
}

string sites::get_site(int id) {
	 if (id < 0 || id >= how_many())
		  return string();
	 list <string> :: iterator sit;
	 int i = 0;
	 for (sit = site.begin(); sit != site.end(); sit++) {
		  if (i++ == id)
				return *sit;
    }
	 return string();
}
