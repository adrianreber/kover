/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 2001, 2008 by Adrian Reber
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

/* This is the dialog for freedb server selection */

#include "server_dialog.moc"

#include "server_dialog.h"
//#include <qpushbutton.h>
//#include <qstring.h>
//#include <qlayout.h>
//#include <q3groupbox.h>
//#include <qlabel.h>
//Added by qt3to4:
//#include <Q3VBoxLayout>

/**
 * The constructor server_dialog::server_dialog
 * constructing the dialog
 */

server_dialog::server_dialog():QDialog()
{
	site_ref = new sites();
	site_ref->gen_server_list(server_list);
	QVBoxLayout *top_layout = new QVBoxLayout(this);

	top_layout->setMargin(7);
	top_layout->addSpacing(10);
	QLabel *label = new QLabel(tr("Choose wisely!"), this);

	top_layout->addWidget(label);
	top_layout->addSpacing(10);
	box = new QListWidget(this);
	list < server * >::iterator item;

	QString string;

	for (item = server_list.begin(); item != server_list.end(); item++) {
		if (((*item)->get_proto()).compare("http")) {
			string = ((*item)->get_site()).c_str();
			string += " (";
			string += ((*item)->get_description()).c_str();
			string += ")";
			QListWidgetItem *newItem = new QListWidgetItem;
			newItem->setText(string);
			box->addItem(newItem);
		}

	}
	//box->setMinimumWidth(box->maxItemWidth() + 30);
	//connect(box, SIGNAL(doubleClicked(Q3ListBoxItem *)), SLOT(double_clicked(Q3ListBoxItem *)));

	top_layout->addWidget(box);
	top_layout->addSpacing(20);

	QBoxLayout * button_layout = new QBoxLayout(QBoxLayout::RightToLeft);

	QPushButton * ok = new QPushButton(tr("Ok"), this);

	ok->setDefault(TRUE);

	ok->setMaximumWidth(70);

	//connect(ok, SIGNAL(clicked()), SLOT(accept()));
	button_layout->addWidget(ok, 0, Qt::AlignRight);
	button_layout->addSpacing(5);

	QPushButton * quit = new QPushButton(tr("Quit"), this);

	//connect(quit, SIGNAL(clicked()), SLOT(quit()));
	quit->setMaximumWidth(70);
	button_layout->addWidget(quit, 0, Qt::AlignRight);
	button_layout->addStretch(20);
	adjustSize();
}

/**
 * The highly sophisticated destructor is doing nothing.
 */
server_dialog::~server_dialog()
{
	fprintf(stderr, "%s:server_dialog::~server_dialog()\n", PACKAGE);

	while (server_list.size()) {
		delete((server_list.back()));
		server_list.pop_back();
	}
	delete site_ref;
}

/**
 * The accept() slot. Setting the return value.
 * reimplemented from QDialog
 */
void
server_dialog::accept()
{
	//QDialog::done(box->currentItem());
}

/**
 * The done() slot. Setting the return value.
 */
void
server_dialog::quit()
{
	QDialog::done(-1);
}

/**
 * The double_clicked() slot. Setting the return value.
 * reimplemented from QDialog
 */
//void
//server_dialog::double_clicked(Q3ListBoxItem * item)
//{
//	QDialog::done(item->listBox()->currentItem());
//}

/**
 * The exec() method. Executing the dialog.
 * reimplemented from QDialog
 * @see double_clicked(QListBoxItem *item)
 * @see accept()
 * @return an int representation of the selected item
 */
int
server_dialog::exec()
{
	if (server_list.size() == 1) {
		return 0;
	} else {
		return QDialog::exec();
	}
}

/**
 * The get() method.
 * @param index the int value returned by exec()
 * @see exec()
 * @return the string containing the cddb id and category. Can be freed with free(3).
 */
string
server_dialog::get(int index)
{
	int i = 0;

	list < server * >::iterator item;

	for (item = server_list.begin(); item != server_list.end(); item++) {
		if (i++ == index * 2)
			return (*item)->get_site();
	}
	return string();
}
