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
	 
	 File: without_cd.cc
	 
	 Description: dialog to execute a cddb query without a cd
	 
	 Changes:

	 11 Nov 2001: Initial release

*/

/* $Id: without_cd.cc,v 1.6 2004/04/20 20:58:07 adrian Exp $ */

#include "without_cd.moc"
#include "without_cd.h"
#include "categories.h"
#include "kover.h"

#include <qpushbutton.h>
#include <qstring.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

without_cd::without_cd():QDialog(0, 0, TRUE, 0)
{
    QString greeting;
    QVBoxLayout *top_layout = new QVBoxLayout(this);

    top_layout->setMargin(7);
    top_layout->addSpacing(10);
    greeting = tr("Select a category:");
    QLabel *label = new QLabel(greeting, this);

    category = new QComboBox(this, "categories");
    categories *cat = new categories();

    for (int i = 0; i <= cat->how_many(); i++) {
        string insert = cat->get_category(i);

        if (!insert.empty())
            category->insertItem(QString(insert.c_str()));
    }
    delete cat;
    top_layout->addWidget(label);
    top_layout->addSpacing(5);
    top_layout->addWidget(category);
    top_layout->addSpacing(10);

    cddb_id = new QLineEdit(this);
    cddb_id->setFocus();

    top_layout->addWidget(new QLabel(tr("Enter CDDB Id:"), this));
    top_layout->addSpacing(5);
    top_layout->addWidget(cddb_id);
    top_layout->addSpacing(10);

    QBoxLayout *button_layout =
        new QBoxLayout(top_layout, QBoxLayout::RightToLeft, -10);

    QPushButton *ok = new QPushButton(tr("Search"), this, "ok");

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

without_cd::~without_cd()
{
}

void without_cd::accept()
{
    QString tmp;

    tmp = cddb_id->text();
    _DEBUG_ fprintf(stderr, "%s:id: %s\n", PACKAGE, tmp.latin1());

    tmp = category->currentText();
    _DEBUG_ fprintf(stderr, "%s:category: %d %s\n", PACKAGE,
        category->currentItem(), tmp.latin1());
    QDialog::done(0);
}

void without_cd::quit()
{
    QDialog::done(-1);
}

void without_cd::handle_input()
{

}

int without_cd::get_category()
{
    return category->currentItem();
}

char *without_cd::get_id()
{
    return strdup((cddb_id->text()).latin1());
}
