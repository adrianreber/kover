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
	 
	 File: cddb.cc
	 
	 Description: general cddb code
	 
	 Changes:

	 28 Apr 2002: Initial release


*/

/* $Id: cddb.cc,v 1.6 2003/06/14 15:49:14 adrian Exp $ */

#include "cddb.h"
#include "kover.h"

#include <unistd.h>

char *cddb::cddb_hello()
{
    char *hello_buffer = NULL;
    char *logname = NULL;
    char *hostname = NULL;

    logname = getenv("LOGNAME");
    hostname = getenv("HOSTNAME");
    if (!logname)
        logname = strdup("Kover_User");
    if (!hostname)
        hostname = strdup("Kover_Host");

    hello_buffer =
        (char *) malloc(strlen(logname) + strlen(hostname) +
        strlen(VERSION) + strlen(PACKAGE) + 12);

    if (!hello_buffer)
        return NULL;

    snprintf(hello_buffer,
        strlen(logname) + strlen(hostname) + strlen(VERSION) +
        strlen(PACKAGE) + 12, "&hello=%s+%s+%s+%s", logname, hostname,
        PACKAGE, VERSION);

    return hello_buffer;
}

int cddb::skip_http_header(int socket)
{
    char inchar, char2[4];
    int len, code = -1;

    _DEBUG_ fprintf(stderr, "%s:cddb::skip_http_header(int socket)\n",
        PACKAGE);

    /* The following is _really_ ugly... */
    /* don't look */

    while (1) {
        read(socket, &inchar, 1);
        if (inchar == 'H') {
            read(socket, &inchar, 1);
            if (inchar == 'T') {
                read(socket, &inchar, 1);
                if (inchar == 'T') {
                    read(socket, &inchar, 1);
                    if (inchar == 'P') {
                        read(socket, &inchar, 1);
                        if (inchar == '/')
                            break;
                    }
                }

            }
        }
    }

    /* remove 1.1 */
    read(socket, &char2, 4);

    read(socket, &char2, 3);

    code = atoi(char2);

    _DEBUG_ fprintf(stderr, "%s:cddb::skip_http_header():http code:%d\n",
        PACKAGE, code);

    do {
        len = 0;
        do {
            read(socket, &inchar, 1);
            len++;
            _DEBUG_ fprintf(stderr, "%c", inchar);
        } while (inchar != '\n');
    } while (len > 2);

    return code;
}

char *cddb::make_cddb_request(char *query_me, bool use_auth)
{
    char *hello_buffer = NULL;
    char *return_me = NULL;
    int length;

    hello_buffer = cddb_hello();

    if (!hello_buffer) {
        return strdup("ERRGREET");
    }

    length = strlen(hello_buffer);

    if (globals.use_proxy) {
        if (use_auth && globals.base64encoded) {
            length +=
                strlen
                ("GET http://  /  ?cmd=    &proto=   HTTP/1.1    Host:       User-Agent:   /   Accept: text/plain  Proxy-authorization: Basic %s")
                + strlen(globals.cddb_server)
                + strlen(globals.cgi_path) + strlen(query_me) +
                strlen(KOVER_CDDB_LEVEL) + strlen(globals.cddb_server) +
                strlen(PACKAGE) + strlen(VERSION) +
                strlen(globals.base64encoded) + 10;
        } else {
            length +=
                strlen
                ("GET http://  /  ?cmd=    &proto=   HTTP/1.1    Host:       User-Agent:   /   Accept: text/plain  ")
                + strlen(globals.cddb_server)
                + strlen(globals.cgi_path) + strlen(query_me) +
                strlen(KOVER_CDDB_LEVEL) + strlen(globals.cddb_server) +
                strlen(PACKAGE) + strlen(VERSION) + 10;
        }
        return_me = (char *) malloc(length);
        if (use_auth && globals.base64encoded) {
            snprintf(return_me, length,
                "GET http://%s/%s?cmd=%s%s&proto=%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s/%s\r\nAccept: text/plain\nProxy-authorization: Basic %s\n\n",
                globals.cddb_server, globals.cgi_path, query_me, hello_buffer,
                KOVER_CDDB_LEVEL, globals.cddb_server, PACKAGE, VERSION,
                globals.base64encoded);
        } else {
            snprintf(return_me, length,
                "GET http://%s/%s?cmd=%s%s&proto=%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s/%s\r\nAccept: text/plain\n\n",
                globals.cddb_server, globals.cgi_path, query_me, hello_buffer,
                KOVER_CDDB_LEVEL, globals.cddb_server, PACKAGE, VERSION);
        }
    } else {
        length +=
            strlen
            ("GET /  ?cmd=    &proto=   HTTP/1.1    Host:       User-Agent:   /   Accept: text/plain  ")
            + strlen(globals.cgi_path) + strlen(query_me) +
            strlen(KOVER_CDDB_LEVEL) + strlen(globals.cddb_server) +
            strlen(PACKAGE) + strlen(VERSION) + 10;
        return_me = (char *) malloc(length);
        snprintf(return_me, length,
            "GET /%s?cmd=%s%s&proto=%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s/%s\r\nAccept: text/plain\n\n",
            globals.cgi_path, query_me, hello_buffer, KOVER_CDDB_LEVEL,
            globals.cddb_server, PACKAGE, VERSION);
    }

    free(hello_buffer);
    return return_me;
}
