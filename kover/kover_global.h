/**  hey emacs! eat this: -*- adrian-c -*-
	 kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
	 Copyright (C) 2000-2002 by Adrian Reber
	 
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
	 
	 File: kover_global.h
	 
	 Description: Header for the global struct
*/

/* $Id: kover_global.h,v 1.15 2002/09/19 06:08:46 adrian Exp $ */

#ifndef _KOVER_GLOBAL_H
#define _KOVER_GLOBAL_H

#include <qfont.h>

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */
    typedef struct {
        char *cddb_server;
        char *cgi_path;
        int use_proxy;
        int proxy_from_env;
        char *proxy_server;
        int proxy_port;

        char *cdrom_device;
        int eject_cdrom;

        int read_local_cddb;
        int write_local_cddb;
        char *cddb_path;

        int trigger_actual_size;
        int display_track_duration;
        int its_a_slim_case;
        int inlet_only;
        int one_page;

        char *base64encoded;

        QFont *content_font;
        QFont *title_font;
        QFont *inlet_title_font;

        int xpos;
        int ypos;
        int save_position;

        int disable_animation;

    } kover_global;

    extern kover_global globals;

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* _KOVER_GLOBAL_H */
