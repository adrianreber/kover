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
	 
	 File: server.cc
	 
	 Description: entity for the servers
	 
	 Changes:
	 
	 04 May 2002: Initial release
	 
*/

/* $Id: server.cc,v 1.6 2003/02/07 16:44:40 adrian Exp $ */

#include "server.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

server::server(const char *cddb_answer)
{
    string answer = cddb_answer;
    int start = 0;
    int end = 0;

    // site
    end = answer.find(" ", start);
    site = answer.substr(start, end);

    //proto
    start = end;
    end = answer.find(" ", ++start);
    proto = answer.substr(start, end - start);

    //port
    start = end;
    end = answer.find(" ", ++start);
    port = atoi((answer.substr(start, end - start)).c_str());

    //path
    start = end;
    end = answer.find(" ", ++start);
    path = answer.substr(start, end - start);

    //latitude
    start = end;
    end = answer.find(" ", ++start);
    latitude = answer.substr(start, end - start);

    //longitude
    start = end;
    end = answer.find(" ", ++start);
    longitude = answer.substr(start, end - start);

    //description
    start = end;
    end = answer.find("\r", ++start);
    description = answer.substr(start, end - start);
}

server::~server()
{
    _DEBUG_ fprintf(stderr, "%s:server::~server()\n", PACKAGE);
}

string server::get_site()
{
    return site;
}

string server::get_proto()
{
    return proto;
}

int server::get_port()
{
    return port;
}

string server::get_path()
{
    return path;
}

string server::get_latitude()
{
    return latitude;
}

string server::get_longitude()
{
    return longitude;
}

string server::get_description()
{
    return description;
}

void server::dump()
{
    cerr << PACKAGE << ":"
        << site << ":"
        << proto << ":"
        << port << ":"
        << path << ":"
        << latitude << ":" << longitude << ":" << description << endl;
}
