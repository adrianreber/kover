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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * $Id: filemode.cc,v 1.3 2005/06/25 19:38:13 adrian Exp $ */

#include "filemode.h"

#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>

/**
 * The constructor filemode::filemode
 * constructing the dialog
 * @param inexact_list is a STL list containing the 211 (inexact match) items
 */

filemode::filemode():QDialog(nullptr)
{
	setModal(true);

	QVBoxLayout *top_layout = new QVBoxLayout(this);

	top_layout->setContentsMargins(7, 7, 7, 7);
	top_layout->addSpacing(10);
	QLabel *label = new QLabel(tr("Choose wisely!"), this);

	top_layout->addWidget(label);
	top_layout->addSpacing(10);
	box = new QListWidget(this);

	box->setMinimumWidth(300);

	connect(box, &QListWidget::itemDoubleClicked, this, &filemode::double_clicked);

	top_layout->addWidget(box);
	top_layout->addSpacing(10);

	directory = new QLineEdit(this);
	top_layout->addWidget(directory);
	top_layout->addSpacing(20);

	QHBoxLayout *button_layout = new QHBoxLayout();

	QPushButton *ok = new QPushButton(tr("Ok"), this);

	ok->setDefault(true);

	ok->setMaximumWidth(70);

	connect(ok, &QPushButton::clicked, this, &filemode::accept);

	QPushButton *quit = new QPushButton(tr("Quit"), this);

	connect(quit, &QPushButton::clicked, this, &filemode::quit);
	quit->setMaximumWidth(70);

	button_layout->addStretch();
	button_layout->addWidget(quit);
	button_layout->addSpacing(5);
	button_layout->addWidget(ok);

	top_layout->addLayout(button_layout);
	adjustSize();
}

/**
 * The highly sophisticated destructor is doing nothing.
 */
filemode::~filemode()
{
}

/**
 * The accept() slot. Setting the return value.
 * reimplemented from QDialog
 */
void filemode::accept()
{
	QDialog::done(box->currentRow());
}

/**
 * The done() slot. Setting the return value.
 */
void filemode::quit()
{
	QDialog::done(-1);
}

/**
 * The double_clicked() slot. Setting the return value.
 * reimplemented from QDialog
 */
void filemode::double_clicked(QListWidgetItem * item)
{
	Q_UNUSED(item);
	QDialog::done(box->currentRow());
}

/**
 * The exec() method. Executing the dialog.
 * reimplemented from QDialog
 * @see double_clicked(QListBoxItem *item)
 * @see accept()
 * @return an int representation of the selected item
 */
int filemode::exec()
{
	if (0) {
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
char *filemode::get(int index __attribute__((unused)))
{
	return strdup(directory->text().toUtf8().constData());
}
