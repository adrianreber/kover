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
	
	 File: direntry.cc
	
	 Description: header for the config class
	
	 Changes:
	
	 01 Dec 2001: Initial creation
*/

/* $Id: direntry.cc,v 1.1 2001/12/01 23:17:42 adrian Exp $ */

#include "direntry.h"

direntry::direntry(string name, long long size, file_type type){
}

direntry::~direntry(){
}

/** Read property of string name. */
string direntry::get_name(){
	 return name;
}

long long direntry::get_size() {
	 return size;
}

enum direntry::file_type direntry::get_type() {
	 return type;
}

