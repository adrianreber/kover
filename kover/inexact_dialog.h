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
	 
	 File: inexact_dialog.h 
	 
	 Description: inexact (inexact match) dialog
	 
	 Changes:
	 
	 15 Jul 2001: Initial release
	 
*/

/* $Id: inexact_dialog.h,v 1.5 2002/09/15 21:20:27 adrian Exp $ */

#ifndef INEXACT_DIALOG_H
#define INEXACT_DIALOG_H

#include "../config.h"
#include <qdialog.h>
#include "cddb_211_item.h"
#include <list>
#include <qlistbox.h>

using namespace std;

/**
 * The inexact_dialog class
 * A dialog used to choose one of the 211 matches.
 */
class inexact_dialog:public QDialog {
  Q_OBJECT public:
      inexact_dialog(list < cddb_211_item * >inexact_list);
     ~inexact_dialog();
    int exec();
    char *get(int index);
  private:
      QListBox * box;
      list < cddb_211_item * >local_list;
    private slots:void accept();
    void quit();
    public slots:void double_clicked(QListBoxItem *);

};

#endif /* INEXACT_DIALOG_H */
