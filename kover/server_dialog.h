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
	 
	 File: server_dialog.h 
	 
	 Description: dialog for freedb server selection
	 
	 Changes:
	 
	 05 May 2002: Initial release
	 
*/

/* $Id: server_dialog.h,v 1.2 2002/09/15 21:20:27 adrian Exp $ */

#ifndef SERVER_DIALOG_H
#define SERVER_DIALOG_H

#include "../config.h"
#include <qdialog.h>
#include "server.h"
#include "sites.h"
#include <list>
#include <qlistbox.h>

using namespace std;

/**
 * The server_dialog class
 * A dialog used to select one of the freedb servers.
 */
class server_dialog:public QDialog {
  Q_OBJECT public:
    server_dialog();
    ~server_dialog();
    int exec();
    string get(int index);
  private:
      QListBox * box;
      list < server * >server_list;
    sites *site_ref;
    private slots:void accept();
    void quit();
    public slots: void double_clicked(QListBoxItem *);

};

#endif /* SERVER_DIALOG_H */
