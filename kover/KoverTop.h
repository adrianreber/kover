/** -*- adrian-c -*-
	 kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
	 Copyright (C) 1999, 2000 by Denis Oliver Kropp
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
	 
	 File: KoverTop.h 
	 
	 Description: TopLevelWidget
	 
	 Changes:

*/

#ifndef KOVER_TOP_H
#define KOVER_TOP_H

#include "kover.h"
#include "CDView.h"
#include "koverfile.h"
#include "cddb_fill.h"
#include "CDDB.h"
#include "cdrom.h"

#include "PreferencesDialog.h"

#include <kapp.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <ktmainwindow.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qmultilinedit.h>
#include <ktoolbar.h>
#include <kstatusbar.h>
#include <kstddirs.h>

/** KoverTop is the TopLevelWidget. */

class KoverTop : public KMainWindow
{
	 Q_OBJECT
public:
	 KoverTop(cdrom *cdrom_device=NULL, const char* name=NULL );
	 ~KoverTop();
// 	 void load_globals();
// 	 void store_globals();
	 void set_cdrom(cdrom *);

	 
public slots:
void contentsBoxChanged();
	 void titleBoxChanged();
	 void stopPreview();
	 void numberChecked(bool checked);
	 void numberChanged(int number);
	 void dataChanged(bool image);
	 void setStatusText( const char* _status_text );
	 void actualSize();
	 void updateDisplay( bool update_really = false);

private slots:

void fileNew();
	 void fileOpen();
	 void fileSave();
	 void fileSaveAs();
	 void filePrint();
	 void cut();
	 void copy();
	 void paste();
	 void editToolbars();
	 void cddbFill();
	 void preferences();
	 void imageEmbedding();
	 void titleFont();
	 void titleFontColor();
	 void contentsFont();
	 void contentsFontColor();
	 void backgroundColor();
	 void cddbDone();
	 void cdrom_eject();

private:
	 cdrom *cdrom_device;
	 char hexToChar( char hexc );
	 bool queryClose();
	 void parseFilename( QString& filename );
	 int how_about_saving();
// 	 char *check_cddb_dir();

	 bool update_display;
	 bool end_loop;

	 QFrame *main_frame;
 
	 QString dataIcon (QString filename);
	
	 KoverFile	kover_file;
	 CDDB_Fill*	cddb_fill;

	 CDDB *cddb__fill;
  
	 QString		filename;
	 bool		altered_data;

	 QLabel*		title_label;
	 QMultiLineEdit*	title_edit;

	 QLabel*		contents_label;
	 QMultiLineEdit*	contents_edit;
  
	 QCheckBox*	number_check;
	 QSpinBox*	number_spin;

	 KToolBar*	main_toolbar;
	 KToolBar*	cd_toolbar;
	 KStatusBar*	status_bar;

	 CDView*		cdview;
};

#endif /* KOVER_TOP_H */
