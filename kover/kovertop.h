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
	 
	 29 Oct 2001: inlet title font stuff

	 11 Nov 2001: CDDB without CD
*/

/* $Id: kovertop.h,v 1.10 2004/04/20 20:58:07 adrian Exp $ */

#ifndef KOVER_TOP_H
#define KOVER_TOP_H

#include "kover.h"
#include "cdview.h"
#include "koverfile.h"
#include "cddb_fill.h"
#include "cdrom.h"

#include "PreferencesDialog.h"

#include <kapp.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <kmainwindow.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qmultilinedit.h>
#include <ktoolbar.h>
#include <kstatusbar.h>
#include <kstddirs.h>
#include <kurl.h>
#include <kaction.h>

/** KoverTop is the TopLevelWidget. */

class KoverTop:public KMainWindow {
  Q_OBJECT public:
      KoverTop(const char *name = NULL);
     ~KoverTop();

    public slots:void contentsBoxChanged();
    void titleBoxChanged();
    void stopPreview();
    void numberChecked(bool checked);
    void numberChanged(int number);
    void dataChanged(bool image);
    void setStatusText(const char *_status_text);
    void actualSize();
    void update_id(unsigned long = 0);
    void fileOpen(const KURL &);

  private:
    void saveFile(const KURL &);

    private slots:void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void filePrint();
    void cut();

    void copy();
    void paste();
    void cddbFill();
    void preferences();
    void imageEmbedding();
    void titleFont();
    void titleFontColor();
    void contentsFont();
    void contentsFontColor();
    void backgroundColor();
    void cdrom_eject();
    void inlet_title_font();
    void cddb_without_cd();
    void more_or_less();
    void display_title_signal();
    void spine_text_method();
    void spine_text_changed_method(const QString & s);
    void file_mode();
    void slotConfigureKeys();

  private:

      bool queryClose();
    int how_about_saving();

    void make_menu();
    void make_main_frame();
    void make_more_frame();
    void make_option_frame();
    void update_id(QString id);

    QFrame *option_frame;
    QCheckBox *display_title;
    QLineEdit *the_spine_text;
    QCheckBox *spine_text;

    KRecentFilesAction *recent;
    QCheckBox *slim_case;

    QFrame *main_frame;

    KoverFile kover_file;
    CDDB_Fill *cddb_fill;

    KURL m_url;
    bool altered_data;

    QLabel *title_label;
    QMultiLineEdit *title_edit;

    QLabel *contents_label;
    QMultiLineEdit *contents_edit;

    QCheckBox *number_check;
    QSpinBox *number_spin;

    KToolBar *main_toolbar;
    KToolBar *cd_toolbar;
    KStatusBar *status_bar;

    QLabel *cddb_id;
    CDView *cdview;

    QBoxLayout *button_layout;
    QFrame *more_frame;
    QPushButton *more_button;
    bool more;
};

#endif /* KOVER_TOP_H */
