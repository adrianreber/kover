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
	 
	 File: sites.cc
	 
	 Description: entity for the sites
	 
	 Changes:

	 24 Apr 2002: Initial release

*/

/* $Id: sites.cc,v 1.6 2003/02/07 16:44:40 adrian Exp $ */

#include "sites.h"

#include <unistd.h>

sites::sites()
{
    connect();
}

sites::~sites()
{
    disconnect();
}

bool sites::gen_server_list(list < server * >&server_list)
{
    char cmd[] = "sites";
    char *request = NULL;
    char *code_string = NULL;
    int code = 0;
    char s[256];

    request = make_cddb_request(cmd, false);

    _DEBUG_ fprintf(stderr, "%s:%s\n", PACKAGE, request);

    write(socket_1, request, strlen(request));

    code = skip_http_header(socket_1);

    _DEBUG_ fprintf(stderr, "%s:sites::gen_server_list():http code:%d\n",
        PACKAGE, code);
    code = 0;
    code_string = (char *) malloc(21);
    if (read(socket_1, code_string, 20) < 0)
        return false;

    code_string[20] = 0;

    _DEBUG_ fprintf(stderr, "%s:cddb answer: %s\n", PACKAGE, code_string);

    code = atoi(code_string);
    if (!code)
        return false;

    while (read(socket_1, code_string, 1) > 0) {
        if (code_string[0] == 10)
            break;
    }

    free(request);
    free(code_string);

    while (1) {
        if (!fgets(s, 255, sk_1) || !strncmp(s, ".", 1))
            break;
        server_list.push_back(new server(s));
        _DEBUG_ fprintf(stderr, "answer: %s", s);
    }

    return true;
}
