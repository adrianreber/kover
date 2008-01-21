/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 2000-2005 by Adrian Reber 
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 * $Id: proxy_auth.h,v 1.3 2005/06/25 18:56:56 adrian Exp $ */

#ifndef PROXY_AUTH_H
#define PROXY_AUTH_H

#include <config.h>
#include <QDialog>
#include <QLineEdit>

class proxy_auth:public QDialog {
      Q_OBJECT public:
	proxy_auth(char *host, int port);
	~proxy_auth();
	char *get_username();
	char *get_password();

      private:
	QLineEdit * user;
	QLineEdit *pw;
	char *password;
	char *username;

      private slots: 
	void accept();
	void quit();
};

#endif				/* PROXY_AUTH_H */
