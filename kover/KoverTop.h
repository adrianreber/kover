/***************************************************************************
                          KoverTop.h  -  TopLevelWidget                              
                             -------------------                                         

    version              :                                   
    begin                : Mon Dez 14 19:01:57 CET 1998
                                           
    copyright            : (C) 1998 by Denis Oliver Kropp                         
    email                : dok@fischlustig.de                                     
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

// alter mach kaffe
#ifndef KOVER_TOP_H
#define KOVER_TOP_H

#include "kover.h"
#include "CDView.h"
#include "koverfile.h"
#include "cddb_fill.h"
#include "CDDB.h"

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
		  KoverTop(const char* name=NULL );
		  ~KoverTop();
		  void load_globals();
		  void store_globals();
public slots:
void contentsBoxChanged();
		  void titleBoxChanged();
		  //void handleMainToolBar(int id);
		  //void handleCdToolBar(int id);
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
/* 	void toggleStatusBar(); */
/* 	void toggleToolBar(); */
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

private:
		  char hexToChar( char hexc );
		  bool queryClose();
		  void parseFilename( QString& filename );
		  int howAboutSaving();
		  char *check_cddb_dir();

#ifdef USE_THREADS	
		  int semid;
		  struct sembuf sops[1];
		  QTimer *timer;
#endif

		  //QApplication *parent;

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

/* 	KMenuBar *menu_bar; */
/* 	KPopupMenu *file_menu; */
/* 	KPopupMenu *cd_menu; */
/* 	KPopupMenu *settings_menu; */
/* 	KPopupMenu *help_menu; */
	
/* 	KToggleAction *statusbarAction; */
/* 	KToggleAction *toolbarAction; */


		  CDView*		cdview;
	

/* 	bool toolbar_active; */
/* 	bool statusbar_active; */

};



#endif
