/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 2001, 2025 by Adrian Reber
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/* This is the dialog for freedb server selection */

#include "sd.h"
#include <cddb_fill.h>
#include <kover.h>

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

sd::sd():QDialog()
{
	cddb_fill *bla = new cddb_fill();
	bla->sites(server_list);
	delete(bla);
	QVBoxLayout *top_layout = new QVBoxLayout(this);

	top_layout->setContentsMargins(7, 7, 7, 7);
	top_layout->addSpacing(10);
	QLabel *label = new QLabel (tr("Choose wisely!"), this);

	top_layout->addWidget(label);
	top_layout->addSpacing(10);
	box = new QListWidget(this);
	list < server * >::iterator item;

	QString string;

	for (item = server_list.begin(); item != server_list.end(); item++) {
		if (verbose)
			(*item)->dump();
		if (((*item)->get_proto()).compare("http")) {
			string = QString::fromStdString((*item)->get_site());
			string += QStringLiteral(" (");
			string += QString::fromStdString((*item)->get_description());
			string += QStringLiteral(")");
			QListWidgetItem *newItem = new QListWidgetItem;
			newItem->setText(string);
			box->addItem(newItem);
		}

	}
	connect(box, SIGNAL(itemDoubleClicked(QListWidgetItem *)), SLOT(double_clicked(QListWidgetItem *)));

	top_layout->addWidget(box);
	top_layout->addSpacing(20);

	QBoxLayout *button_layout = new QBoxLayout(QBoxLayout::RightToLeft);

	top_layout->addLayout(button_layout);
	QPushButton *ok = new QPushButton(tr("Ok"), this);

	ok->setDefault(true);

	ok->setMaximumWidth(70);

	connect(ok, SIGNAL(clicked()), SLOT(accept()));
	button_layout->addWidget(ok, 0, Qt::AlignRight);
	button_layout->addSpacing(5);

	QPushButton *quit = new QPushButton(tr("Quit"), this);

	connect(quit, SIGNAL(clicked()), SLOT(quit()));
	quit->setMaximumWidth(70);
	button_layout->addWidget(quit, 0, Qt::AlignRight);
	button_layout->addStretch(20);
	adjustSize();
}

sd::~sd()
{
	while (server_list.size()) {
		delete((server_list.back()));
		server_list.pop_back();
	}
}

/**
 * The accept() slot. Setting the return value.
 * reimplemented from QDialog
 */
void
sd::accept()
{
	QDialog::done(box->currentRow());
}

/**
 * The done() slot. Setting the return value.
 */
void
sd::quit()
{
	QDialog::done(-1);
}

/**
 * The double_clicked() slot. Setting the return value.
 * reimplemented from QDialog
 */
void
sd::double_clicked(QListWidgetItem *item)
{
	QDialog::done(item->listWidget()->currentRow());
}

/**
 * The exec() method. Executing the dialog.
 * reimplemented from QDialog
 * @see double_clicked(QListBoxItem *item)
 * @see accept()
 * @return an int representation of the selected item
 */
int
sd::exec()
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
string sd::get(int index)
{
	int
		i = 0;

	list < server * >::iterator item;

	for (item = server_list.begin(); item != server_list.end(); item++) {
		if (i++ == index * 2)
			return (*item)->get_site();
	}
	return string();
}
