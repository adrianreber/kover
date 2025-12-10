/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 2001, 2025 by Adrian Reber
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "server.h"
#include <kover.h>
#include <iostream>

server::server(string _site, string _proto, int _port, string _path,
	       string _latitude, string _longitude, string _description)
{
	site = _site;
	proto = _proto;
	port = _port;
	path = _path;
	latitude = _latitude;
	longitude = _longitude;
	description = _description;
}

server::~server()
{
	kprintf("server::~server()\n");
}

string
server::get_site()
{
	return site;
}

string
server::get_proto()
{
	return proto;
}

int
server::get_port()
{
	return port;
}

string
server::get_path()
{
	return path;
}

string
server::get_latitude()
{
	return latitude;
}

string
server::get_longitude()
{
	return longitude;
}

string
server::get_description()
{
	return description;
}

void
server::dump()
{
	kprintf("");
	cerr << site << ":"
		<< proto << ":"
		<< port << ":" << path << ":" << latitude << ":" << longitude << ":" << description << endl;
}
