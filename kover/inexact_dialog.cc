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
	 
	 File: inexact_dialog.cc
	 
	 Description: inexact (inexact match) dialog
	 
	 Changes:

	 15 Jul 2001: Initial release

	 09 Oct 2001: Better

	 30 Oct 2001: Bugfix

*/

/* $Id: inexact_dialog.cc,v 1.10 2003/02/07 16:44:40 adrian Exp $ */

#include "inexact_dialog.moc"
#include "inexact_dialog.h"

#include <qpushbutton.h>
#include <qlistbox.h>
#include <qstring.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>

/**
 * The constructor inexact_dialog::inexact_dialog
 * constructing the dialog
 * @param inexact_list is a STL list containing the 211 (inexact match) items
 */

inexact_dialog::inexact_dialog(list < cddb_211_item * >inexact_list):QDialog(0, 0, TRUE,
    0)
{
    local_list = inexact_list;
    QVBoxLayout *top_layout = new QVBoxLayout(this);

    top_layout->setMargin(7);
    top_layout->addSpacing(10);
    QLabel *label = new QLabel(tr("Choose wisely!"), this);

    top_layout->addWidget(label);
    top_layout->addSpacing(10);
    box = new QListBox(this);
    list < cddb_211_item * >::iterator item;

    for (item = inexact_list.begin(); item != inexact_list.end(); item++) {
        box->insertItem(((*item)->get()));
    }
    box->setMinimumWidth(box->maxItemWidth() + 30);

    connect(box, SIGNAL(doubleClicked(QListBoxItem *)),
        SLOT(double_clicked(QListBoxItem *)));

    top_layout->addWidget(box);
    top_layout->addSpacing(20);

    QBoxLayout *button_layout =
        new QBoxLayout(top_layout, QBoxLayout::RightToLeft, -10);

    QPushButton *ok = new QPushButton(tr("Ok"), this, "ok");

    ok->setDefault(TRUE);

    ok->setMaximumWidth(70);

    connect(ok, SIGNAL(clicked()), SLOT(accept()));
    button_layout->addWidget(ok, 0, AlignRight);
    button_layout->addSpacing(5);

    QPushButton *quit = new QPushButton(tr("Quit"), this, "quit");

    connect(quit, SIGNAL(clicked()), SLOT(quit()));
    quit->setMaximumWidth(70);
    button_layout->addWidget(quit, 0, AlignRight);
    button_layout->addStretch(20);
    adjustSize();
}

/**
 * The highly sophisticated destructor is doing nothing.
 */
inexact_dialog::~inexact_dialog()
{
}

/**
 * The accept() slot. Setting the return value.
 * reimplemented from QDialog
 */
void inexact_dialog::accept()
{
    QDialog::done(box->currentItem());
}

/**
 * The done() slot. Setting the return value.
 */
void inexact_dialog::quit()
{
    QDialog::done(-1);
}

/**
 * The double_clicked() slot. Setting the return value.
 * reimplemented from QDialog
 */
void inexact_dialog::double_clicked(QListBoxItem * item)
{
    QDialog::done(item->listBox()->currentItem());
}

/**
 * The exec() method. Executing the dialog.
 * reimplemented from QDialog
 * @see double_clicked(QListBoxItem *item)
 * @see accept()
 * @return an int representation of the selected item
 */
int inexact_dialog::exec()
{
    if (local_list.size() == 1) {
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
char *inexact_dialog::get(int index)
{
    int i = 0;

    list < cddb_211_item * >::iterator item;

    for (item = local_list.begin(); item != local_list.end(); item++) {
        if (i++ == index)
            return strdup((*item)->get());
    }
    return NULL;
}
