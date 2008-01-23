/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 2000, 2008 by Adrian Reber
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
 */

/* PA proxy auth */

#ifndef PA_H
#define PA_H

#include <config.h>
#include <QDialog>
#include <QLineEdit>

class pa:public QDialog
{
      Q_OBJECT public:

	pa(char *host, int port);
	~pa();
	char *get_username();
	char *get_password();

      private:

	QLineEdit *user;
	QLineEdit *pw;
	char *password;
	char *username;

      private slots:

	void accept();
	void quit();
};

#endif /* PA_H */
