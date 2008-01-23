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

#include "pa.moc"
#include "pa.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

pa::pa(char *host, int port):QDialog()
{
	password = NULL;
	username = NULL;
	QString greeting;
	QString tmp;
	QVBoxLayout *top_layout = new QVBoxLayout(this);

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
	QBoxLayout *button_layout = new QBoxLayout(QBoxLayout::RightToLeft);
	top_layout->addLayout(button_layout);
	QPushButton *ok = new QPushButton(tr("Ok"), this);

	ok->setDefault(TRUE);
	ok->setMaximumWidth(70);
	connect(ok, SIGNAL(clicked()), SLOT(accept()));
	button_layout->addWidget(ok, 0, Qt::AlignRight);
	button_layout->addSpacing(10);
	QPushButton *quit = new QPushButton(tr("Cancel"), this);

	connect(quit, SIGNAL(clicked()), SLOT(quit()));
	quit->setMaximumWidth(70);
	button_layout->addWidget(quit, 0, Qt::AlignRight);
	button_layout->addStretch(20);
	adjustSize();
}

pa::~pa()
{
	if (password)
		free(password);
	if (username)
		free(username);
	password = NULL;
	username = NULL;
}

void
pa::accept()
{
	QString tmp = user->text();
	username = strdup(tmp.toUtf8());
	tmp = pw->text();
	password = strdup(tmp.toUtf8());
	QDialog::done(0);
}

void
pa::quit()
{
	password = NULL;
	username = NULL;
	QDialog::done(-1);
}

char *
pa::get_password()
{
	if (!password)
		return NULL;
	int length = strlen(password);
	char *pw = strdup(password);

	memset(password, 0, length);

	return pw;
}

char *
pa::get_username()
{
	if (!username)
		return NULL;
	int length = strlen(username);
	char *un = strdup(username);

	memset(username, 0, length);

	return un;
}
