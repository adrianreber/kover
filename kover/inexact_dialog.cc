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
	 
	 File: inexact_dialog.cc
	 
	 Description: inexact (inexact match) dialog
	 
	 Changes:

	 15 Jul 2001: Initial release

*/

/* $Id: inexact_dialog.cc,v 1.1 2001/07/15 22:21:53 adrian Exp $ */

#include "inexact_dialog.h"
#include <qpushbutton.h>
#include <qlistbox.h>
#include <qstring.h>
#include <qlayout.h>
#include <qgroupbox.h>

inexact_dialog::inexact_dialog(list <cddb_211_item *> inexact_list) : QDialog(0,0,TRUE,0) {
	 QVBoxLayout *top_layout = new QVBoxLayout(this);
	 box = new QListBox(this);
	 list <cddb_211_item *> :: iterator item;
	 for (item = inexact_list.begin(); item != inexact_list.end(); item++) {
		  box->insertItem(((*item)->get()));
		  box->insertItem("haha");
	 }
	 box->setMinimumWidth(box->maxItemWidth()+30);
	 
	 connect(box, SIGNAL(doubleClicked(QListBoxItem *)), SLOT (double_clicked(QListBoxItem *)));

	 top_layout->addWidget(box);
	 top_layout->addSpacing(20);

	 QBoxLayout *button_layout = new QBoxLayout(top_layout, QBoxLayout::RightToLeft, -10);
	 //button_layout->setResizeMode(QLayout::Minimum);

	 //top_layout->addLayout(button_layout);
	 
	 QPushButton *ok = new QPushButton( "ok",this , "quitok" );
	 
	 ok->setMaximumWidth(70);

    connect( ok, SIGNAL(clicked()), SLOT(accept()) );
	 button_layout->addSpacing(5);
	 button_layout->addWidget(ok,0,AlignRight);
	 button_layout->addSpacing(5);

	 QPushButton *quit = new QPushButton( "Quit", this, "quit" );
	 
    connect( quit, SIGNAL(clicked()), SLOT(accept()) );
	 quit->setMaximumWidth(70);
	 button_layout->addWidget(quit,0,AlignRight);
	 button_layout->addStretch(20);
	 adjustSize();
}

inexact_dialog::~inexact_dialog() {
}

void inexact_dialog::accept() {
	 QDialog::done(box->currentItem());
}

void inexact_dialog::double_clicked(QListBoxItem *item) {
	 QDialog::done(item->listBox()->currentItem());
}

