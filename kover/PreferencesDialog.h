/** hey emacs! check this out: -*- adrian-c -*-
	kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
	Copyright (C) 2000, 2001 by Adrian Reber
	
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
	
	File: PreferencesDialog.h
	
	Description: Header to PreferencesDialog.cpp
*/

/* $Id: PreferencesDialog.h,v 1.11 2001/11/11 22:58:38 adrian Exp $ */

#ifndef PREFERENCES_DIALOG_H
#define PREFERENCES_DIALOG_H

#include "kover.h"
#include <qradiobutton.h>
#include <kdialogbase.h>
#include <qlineedit.h>
#include <qcheckbox.h>

class PreferencesDialog : public KDialogBase {
	 Q_OBJECT

public:
	 PreferencesDialog(QWidget *parent, const QString &caption);
	 //~PreferencesDialog();

private:
	 void setupCDDBPage();
	 void setupCDROMPage();
	 void setup_cddb_files_page();
	 void setup_misc_page();

	 void apply_settings();

	 void set_cddb();
	 void set_cdrom();
	 void set_cddb_files();
	 void set_misc();

	 void save_cddb_files();
	 void save_misc();

	 struct cddb_widgets {
		  QLineEdit *cddb_server;
		  QLineEdit *cgi_path;
		  QCheckBox *use_proxy;
		  QCheckBox *proxy_from_env;
		  QLineEdit *proxy_server;
		  QLineEdit *proxy_port;
	 } cddb_widgets;
		  
	 QWidget *parent;

	 enum Page {
		  page_cddb = 0,
		  page_cdrom,
		  page_cddb_files,
		  page_misc,
		  page_max
	 };

	 struct cdrom_widgets {
		  QLineEdit *cdrom_device;
		  QCheckBox *eject_cdrom;
	 } cdrom_widgets;

	 struct cddb_files_widgets {
		  QCheckBox *read_local_cddb;
		  QCheckBox *write_local_cddb;
		  QLineEdit *cddb_path;
	 } cddb_files_widgets;

	 struct misc_widgets {
		  QCheckBox * trigger_actual_size;
		  QCheckBox * display_track_duration;
		  QRadioButton * its_a_slim_case;
		  QRadioButton * inlet_only;
		  QRadioButton * its_normal;
	 } misc_widgets;
		  
private slots:
void use_proxy(bool status);
	 virtual void slotOk(void);
	 virtual void slotDefault();
};

#endif /* PREFERENCES_DIALOG_H */
