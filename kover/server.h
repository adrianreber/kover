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
	 
	 File: server.h
	 
	 Description: entity for the server

	 Changes:
	 
	 04 May 2002: Initial release
	 
*/

/* $Id: server.h,v 1.4 2002/08/14 08:10:12 adrian Exp $ */

#ifndef SERVER_H
#define SERVER_H

#include "../config.h"
#include "kover.h"

using namespace std;

class server {

  public:
    server(const char *cddb_answer);
     ~server();
    string get_site();
    string get_proto();
    int get_port();
    string get_path();
    string get_latitude();
    string get_longitude();
    string get_description();
    void dump();
  private:
      string site;
    string proto;
    int port;
    string path;
    string latitude;
    string longitude;
    string description;

};

#endif /* SERVER_H */
