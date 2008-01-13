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
 * $Id: proxy_auth.cc,v 1.7 2005/06/25 18:56:55 adrian Exp $ */

#include "proxy_auth.moc"
#include "proxy_auth.h"

#include <qpushbutton.h>
#include <qstring.h>
#include <qlayout.h>
#include <q3groupbox.h>
#include <qlabel.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <stdlib.h>
#include <stdio.h>

proxy_auth::proxy_auth(char *host, int port):QDialog(0, 0, TRUE, 0)
{
	password = NULL;
	username = NULL;
	QString greeting;
	QString tmp;
	Q3VBoxLayout *top_layout = new Q3VBoxLayout(this);

	top_layout->setMargin(7);
	top_layout->addSpacing(10);
	greeting = tr("Enter username and password for proxy at ");
	tmp.sprintf("%s:%d", host, port);
	greeting += tmp;
	QLabel *label = new QLabel(greeting, this);

	top_layout->addWidget(label);
	top_layout->addSpacing(5);

	user = new QLineEdit(this);
	user->setFocus();
	pw = new QLineEdit(this);
	pw->setEchoMode(QLineEdit::Password);

	top_layout->addWidget(new QLabel(tr("User Name:"), this));
	top_layout->addSpacing(5);
	top_layout->addWidget(user);
	top_layout->addSpacing(5);
	top_layout->addWidget(new QLabel(tr("Password:"), this));
	top_layout->addSpacing(5);
	top_layout->addWidget(pw);
	top_layout->addSpacing(10);
	Q3BoxLayout *button_layout = new Q3BoxLayout(top_layout, Q3BoxLayout::RightToLeft, -10);
	QPushButton *ok = new QPushButton(tr("Ok"), this, "ok");

	ok->setDefault(TRUE);
	ok->setMaximumWidth(70);
	connect(ok, SIGNAL(clicked()), SLOT(accept()));
	button_layout->addWidget(ok, 0, AlignRight);
	button_layout->addSpacing(10);
	QPushButton *quit = new QPushButton(tr("Cancel"), this, "quit");

	connect(quit, SIGNAL(clicked()), SLOT(quit()));
	quit->setMaximumWidth(70);
	button_layout->addWidget(quit, 0, AlignRight);
	button_layout->addStretch(20);
	adjustSize();
}

proxy_auth::~proxy_auth()
{
	if (password)
		free(password);
	if (username)
		free(username);
	password = NULL;
	username = NULL;
}

void proxy_auth::accept()
{
	QString tmp = user->text();
	username = strdup(tmp.latin1());
	tmp = pw->text();
	password = strdup(tmp.latin1());
	QDialog::done(0);
}

void proxy_auth::quit()
{
	password = NULL;
	username = NULL;
	QDialog::done(-1);
}

char *proxy_auth::get_password()
{
	if (!password)
		return NULL;
	int length = strlen(password);
	char *pw = strdup(password);

	memset(password, 0, length);

	return pw;
}

char *proxy_auth::get_username()
{
	if (!username)
		return NULL;
	int length = strlen(username);
	char *un = strdup(username);

	memset(username, 0, length);

	return un;
}
