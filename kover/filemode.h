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
	 
	 File: filemode.h 
	 
	 Description: inexact (inexact match) dialog
	 
	 Changes:
	 
	 15 Jul 2001: Initial release
	 
*/

/* $Id: filemode.h,v 1.1 2003/03/23 22:25:28 adrian Exp $ */

#ifndef FILEMODE_H
#define FILEMODE_H

#include "../config.h"
#include <qdialog.h>
#include "cddb_211_item.h"
#include <list>
#include <qlistbox.h>
#include <qlineedit.h>

using namespace std;

/**
 * The filemode class
 * A dialog used to choose one of the 211 matches.
 */
class filemode:public QDialog {
  Q_OBJECT public:
      filemode();
     ~filemode();
    int exec();
    char *get(int index);
  private:
      QListBox * box;
      QLineEdit *directory;
    private slots:void accept();
    void quit();
    public slots:void double_clicked(QListBoxItem *);

};

#endif /* FILEMODE_H */
