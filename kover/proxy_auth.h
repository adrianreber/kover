/** -*- adrian-c -*-
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
	 
	 File: proxy_auth.h 
	 
	 Description: proxy authentification dialog
	 
	 Changes:
	 
	 30 Sep 2001: Initial release
	 
*/

/* $Id: proxy_auth.h,v 1.2 2001/09/30 18:21:55 adrian Exp $ */

#ifndef PROXY_AUTH_H
#define PROXY_AUTH_H

#include "../config.h"
#include <qdialog.h>
#include <qlineedit.h>

class proxy_auth : public QDialog {
	 Q_OBJECT
public:
	 proxy_auth(char *host, int port);
	 ~proxy_auth();
	 char * get_authentification();
	 
private:
	 QLineEdit *user;
	 QLineEdit *pw;
	 void handle_input();
	 char *base64encoded;

private slots:
void accept();
	 void quit();
	 
};

#endif /* PROXY_AUTH_H */
