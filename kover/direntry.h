/** -*- adrian-c -*-, ho!
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
	
	 File: direntry.h
	
	 Description: header for the direntry class
	
	 Changes:
	
	 01 Dec 2001: Initial creation
*/

/* $Id: direntry.h,v 1.2 2002/01/13 00:33:21 adrian Exp $ */

#ifndef DIRENTRY_H
#define DIRENTRY_H

#include <string>

using namespace std;

class direntry {
public: 
	 enum file_type {
		  file = 1,
		  dir = 2,
		  link = 3,
		  unknown = 4
	 };
	 direntry(string name, long long size, file_type type);
	 ~direntry();
	 /** Read property of string name. */
	 string get_name();
	 long long get_size();
	 file_type get_type();
private: // Private attributes
	 string name;
	 long long size;
	 file_type type; 
};

#endif /* DIRENTRY_H */
