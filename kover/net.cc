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
	 
	 File: net.cc
	 
	 Description: general network code
	 
	 Changes:

	 28 Apr 2002: Initial release
     11 Sep 2002: Support to read proxy from http_proxy env

*/

/* $Id: net.cc,v 1.7 2004/04/20 21:04:28 adrian Exp $ */

#include "net.h"
#include "kover.h"

#include <netdb.h>
#ifdef __FreeBSD__
#include <sys/param.h>
#endif
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

net::net()
{
    socket_1 = 0;
    socket_2 = 0;
    sock_mode = 0;
    sk_1 = NULL;
    sk_2 = NULL;
}

int net::connect()
{
    sockaddr_in sin;
    hostent *h;
    char *proxy_server = NULL;
    int proxy_port = 0;
    char *tmp = NULL;
    char *s, *ss;

    if (globals.proxy_from_env && globals.use_proxy) {
        //saving the proxy configuration to temporary variables
        if (globals.proxy_server) {
            proxy_server = strdup(globals.proxy_server);
            free(globals.proxy_server);
            globals.proxy_server = NULL;
        }
        proxy_port = globals.proxy_port;
        //reading from environment
        if (getenv("http_proxy"))
            tmp = strdup(getenv("http_proxy"));
        if (!tmp) {
            errno = sys_nerr + 101;
            return errno;
        }
        if (strncmp(tmp, "http://", 7)) {
            errno = sys_nerr + 102;
            return errno;
        } else {
            //finding proxy server and port
            s = strchr(tmp + 7, 58);
            if (!s) {
                errno = sys_nerr + 102;
                return errno;
            }
            *s = 0;
            ss = strchr(s + 1, 47);
            if (!ss) {
                errno = sys_nerr + 102;
                return errno;
            }
            *ss = 0;
            //now globals has the environment proxy information
            globals.proxy_server = strdup(tmp + 7);
            globals.proxy_port = atoi(s + 1);
        }
    }

    if (globals.use_proxy) {
        if ((h = gethostbyname(globals.proxy_server)) == NULL)
            return errno;
    } else {
        if ((h = gethostbyname(globals.cddb_server)) == NULL)
            return errno;
    }

    bcopy(h->h_addr, (char *) &sin.sin_addr, h->h_length);

    sin.sin_family = h->h_addrtype;

    if (globals.use_proxy)
        sin.sin_port = htons(globals.proxy_port);
    else
        sin.sin_port = htons(CDDB_PORT);

    //copying the original values into globals structure
    if (globals.proxy_from_env && globals.use_proxy) {
        if (globals.proxy_server) {
            free(globals.proxy_server);
            globals.proxy_server = NULL;
        }
        if (proxy_server) {
            globals.proxy_server = strdup(proxy_server);
            free(proxy_server);
            proxy_server = NULL;
        }
        if (tmp)
            free(tmp);
        tmp = NULL;
        globals.proxy_port = proxy_port;
    }

    if ((socket_1 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return errno;

    if ((socket_2 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return errno;

    if (::connect(socket_1, (struct sockaddr *) &sin, sizeof(sin)) < 0)
        return errno;

    if (::connect(socket_2, (struct sockaddr *) &sin, sizeof(sin)) < 0)
        return errno;

    sk_1 = fdopen(socket_1, "r+");
    sk_2 = fdopen(socket_2, "r+");
    if (sk_1 == NULL || sk_2 == NULL) {
        close(socket_1);
        close(socket_2);
        return errno;
    }

    return 0;
}

void net::disconnect()
{
    close(socket_1);
    if (sk_1 != NULL)
        fclose(sk_1);
    close(socket_2);
    if (sk_2 != NULL)
        fclose(sk_2);
}

char *net::readline(int socket)
{
    char inchar, char2[255];
    int i = 0;

    while (1) {
        read(socket, &inchar, 1);
        if (inchar == 10)
            break;
        char2[i++] = inchar;
        if (i >= 250)
            break;
    }
    char2[i] = 0;
    return strdup(char2);
}
