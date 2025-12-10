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

#ifndef SERVER_H
#define SERVER_H

#include <string>
using namespace std;

class server
{
      public:

	server(string, string, int, string, string, string, string);
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
