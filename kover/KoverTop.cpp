/** hey emacs! try -*- adrian-c -*- mode
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
	 
	 File: KoverTop.cpp  
	 
	 Description: TopLevelWidget
	 
	 Changes:

	 05 Apr 2001: globals initialization methods

	 10 Jun 2001: bugfix for [modified] bug.

	 03 Jul 2001: now displaying CDDB id

	 29 Oct 2001: inlet title font stuff

	 11 Nov 2001: CDDB without CD
*/

#include "KoverTop.moc"

#include "KoverTop.h"
#include "imagedlg.h"
#include "without_cd.h"

#include <klocale.h>
#include <kmainwindow.h>
#include <kapp.h>
#include <kstdaction.h>
#include <kaction.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kfontdialog.h>
#include <kcolordialog.h>
#include <krecentdocument.h>

#define NORM_WIDTH 520
#define NORM_HEIGHT 440
#define PREV_WIDTH 695
#define PREV_HEIGHT 684

#define CDVIEW_X 224
#define CDVIEW_Y 40
#define CDVIEW_WIDTH	291
#define CDVIEW_HEIGHT 310

KoverTop::KoverTop(const char* name) : KMainWindow(0,name) {
	 main_frame = new QFrame(this);
	 
	 setCaption(i18n("[New Document]"), false);
	 setFixedSize( NORM_WIDTH, NORM_HEIGHT );

	 status_bar = statusBar();
	 status_bar->insertItem( "Kover "VERSION" - http://lisas.de/kover/", 1 );

	 KStdAction::openNew(this, SLOT(fileNew()), actionCollection());
	 KStdAction::open(this, SLOT(fileOpen()), actionCollection());
	 KStdAction::save(this, SLOT(fileSave()), actionCollection());
	 KStdAction::saveAs(this, SLOT(fileSaveAs()), actionCollection());
	 KStdAction::print(this, SLOT(filePrint()), actionCollection());
	 KStdAction::quit(this, SLOT(close()), actionCollection());
	 KStdAction::cut(this, SLOT(cut()), actionCollection());
	 KStdAction::copy(this, SLOT(copy()), actionCollection());
	 KStdAction::paste(this, SLOT(paste()), actionCollection());
	 recent = KStdAction::openRecent(this, SLOT(fileOpen(const KURL&)), actionCollection());
	 (void)new KAction(i18n("&Actual size"),"viewmag",0,this, SLOT(actualSize()),actionCollection(), "actual_size");
	 (void)new KAction(i18n("&CDDB lookup"),"network",0,this, SLOT(cddbFill()),actionCollection(), "cddb");
	 KStdAction::preferences(this,SLOT(preferences()),actionCollection());
	 (void)new KAction(i18n("&Image Embedding..."),"background",0,this,SLOT(imageEmbedding()),actionCollection(),"image_embedding");
	 (void)new KAction(i18n("Title Font..."),"fonts",0,this,SLOT(titleFont()),actionCollection(),"title_font");
	 (void)new KAction(i18n("Title Fontcolor..."),"colorize",0,this,SLOT(titleFontColor()),actionCollection(),"title_font_color");
	 (void)new KAction(i18n("Contents Font..."),"fonts",0,this,SLOT(contentsFont()),actionCollection(),"contents_font");
	 (void)new KAction(i18n("Contents Fontcolor..."),"colorize",0,this,SLOT(contentsFontColor()),actionCollection(),"contents_font_color");
	 (void)new KAction(i18n("Background Color..."),"colors",0,this,SLOT(backgroundColor()),actionCollection(),"background_color");
	 (void)new KAction(i18n("Inlet Title Font..."),"fonts",0,this,SLOT(inlet_title_font()),actionCollection(),"inlet_title_font");
	 (void)new KAction(i18n("Eject CD"),"player_eject",0,this,SLOT(cdrom_eject()),actionCollection(),"eject_cdrom");
 	 (void)new KAction(i18n("CDDB without CD"),"network",0,this, SLOT(cddb_without_cd()),actionCollection(), "cddb_without_cd");
	 
	 
	 createGUI();

	 number_spin = new QSpinBox( 1, 999, 1, main_frame, "numberspin" );
	 number_spin->setEnabled(false);
	 number_spin->resize( 50, 25 ); 
	 number_spin->move( 170, 5 );
	 connect( number_spin, SIGNAL(valueChanged(int)), SLOT(numberChanged(int)));

	 number_check = new QCheckBox(i18n("CD Number"), main_frame, "number_check");
	 number_check->resize( 100, 25 );
	 number_check->move( 50, 5 );
	 connect( number_check, SIGNAL(toggled(bool)), SLOT(numberChecked(bool)) );

	 title_label = new QLabel(i18n("Title"), main_frame,"title_label");
	 title_label->move( 5, 25 );

	 title_edit = new QMultiLineEdit( main_frame, "title_edit" );
	 title_edit->resize( 215, 50 );
	 title_edit->move( 5, 50 );
	 connect( title_edit, SIGNAL(textChanged()), SLOT(titleBoxChanged()) );
  
	 contents_label = new QLabel(i18n("Contents"), main_frame, "contents_label");
	 contents_label->move( 5, 105 );

	 contents_edit = new QMultiLineEdit( main_frame, "contents_edit" );
	 contents_edit->resize( 215, 215 );
	 contents_edit->move( 5, 135 );
	 connect(contents_edit, SIGNAL(textChanged()), SLOT(contentsBoxChanged()));

	 cddb_id = new QLabel("CDDB id:", main_frame, "cddb_id");
	 cddb_id->move( CDVIEW_X, CDVIEW_Y - 40 );
	 
	 cdview = new CDView( &kover_file, main_frame );
	 cdview->resize( CDVIEW_WIDTH, CDVIEW_HEIGHT );
	 cdview->move( CDVIEW_X, CDVIEW_Y );
	 connect( cdview, SIGNAL(stopPreview()), SLOT(stopPreview()) );
	 connect( cdview, SIGNAL(actualSize()), SLOT(actualSize()) );
  
	 connect( &kover_file, SIGNAL(dataChanged(bool)), SLOT(dataChanged(bool)) );

	 cddb_fill = new CDDB_Fill( &kover_file );

	 connect(cddb_fill, SIGNAL(statusText(const char*)), SLOT(setStatusText(const char*)));

	 connect(cddb_fill, SIGNAL(update_id(unsigned long)), SLOT(update_id(unsigned long)));

	 altered_data = false;
	 main_frame->move(0,70);
	 main_frame->adjustSize();
	 recent->loadEntries((KApplication::kApplication())->config());
}

KoverTop::~KoverTop() {
	 recent->saveEntries((KApplication::kApplication())->config());
	 delete status_bar;
	 delete cddb_fill;
	 delete cdview;
	 _DEBUG_ fprintf(stderr,"~KoverTop()\n");
}

void KoverTop::dataChanged(bool image) {
	 bool bla = image;
	 bla = true;
		  
	 setStatusText(i18n("Data changed"));
	 if (m_url.isEmpty() )
		  setCaption(i18n("[New Document]"), true);
	 else
		  setCaption(i18n(m_url.url()), true);
	 
	 altered_data = true;
}

void KoverTop::setStatusText( const char* _status_text ) {
	 status_bar->changeItem( _status_text, 1 );
	 kapp->processEvents();
}

void KoverTop::update_id(unsigned long id) {
	 QString string="CDDB id:";
	 if (id!=0)
		  string.sprintf("CDDB id: 0x%08lx",id);
	 cddb_id->setText(string);
	 cddb_id->adjustSize();
}

void KoverTop::titleBoxChanged() {
	 kover_file.setTitle(title_edit->text());
}

void KoverTop::contentsBoxChanged() {
	 kover_file.setContents(contents_edit->text());
}

void KoverTop::numberChanged(int value) {
	 kover_file.setNumber(value);
}

void KoverTop::numberChecked(bool checked) {
	 number_spin->setEnabled(checked);
	 
	 if (checked)
		  kover_file.setNumber(number_spin->value());
	 else
		  kover_file.setNumber(0);
}

void KoverTop::stopPreview() {
	 main_frame->move(0,70);
	 setFixedSize(NORM_WIDTH, NORM_HEIGHT);
	 cdview->resize( CDVIEW_WIDTH, CDVIEW_HEIGHT );
	 cdview->move( CDVIEW_X, CDVIEW_Y );
	 cdview->showPreview( false );
	 menuBar()->show();
	 statusBar()->show(); 
	 toolBar("koverToolBar")->show();
	 toolBar("mainToolBar")->show();
} 

bool KoverTop::queryClose() {
	 if (altered_data) {
		  if(how_about_saving())
				return false;
	 }
	 return true;
}

void KoverTop::fileNew() {
	 if (altered_data) {
		  if(how_about_saving())
				return;
	 }
  
	 title_edit->clear();
	 contents_edit->clear();
	 number_spin->setValue( 1 );
	 number_spin->setEnabled( false );
	 number_check->setChecked( false );
	 title_edit->setFocus();
	 kover_file.reset();
	 setStatusText("Chop!");
	 altered_data = false;
	 m_url = KURL();
	 setCaption(i18n("[New Document]"), false);
	 update_id();
}

void KoverTop::fileOpen() {
	 if (altered_data) {
		  if (how_about_saving())
				return;
	 }

	 KURL url = KFileDialog::getOpenURL( QString::null, "*.kover" );
	 if( !url.isEmpty() ) {
		  fileOpen( url );
	 }
}

void KoverTop::fileOpen( const KURL& url ) {
	 if (!url.isEmpty()) {
		  if (kover_file.openFile( url )) {
				m_url = url;
				
				setCaption(i18n(m_url.url()), false);
				
				disconnect( contents_edit, SIGNAL(textChanged()), this, SLOT(contentsBoxChanged()) );
				title_edit->setText( kover_file.title() );
				contents_edit->setText( kover_file.contents() );
				connect( contents_edit, SIGNAL(textChanged()), SLOT(contentsBoxChanged()) );

				if (kover_file.number()) {
					 number_spin->setValue( kover_file.number() );
					 number_spin->setEnabled( true );
					 number_check->setChecked( true );
				} else {
					 number_spin->setEnabled( false );
					 number_check->setChecked( false );
				}
              
				setStatusText(i18n("File loaded"));
				altered_data = false;
		  } else 
				KMessageBox::error( this, i18n("Error while opening/reading file!"));
	 }
}

int KoverTop::how_about_saving() {
	 switch (KMessageBox::warningYesNoCancel( this, i18n("Data changed since last saving!\nDo you want to save the changes?"))) {
	 case 3: //YES
		  fileSave();
		  if (altered_data)
				return -1;
		  return 0;
	 case 4: //NO
		  return 0;
	 case 2: //CANCEL
		  return -1;
	 }
	 return -1;
}

void KoverTop::saveFile( const KURL& url )
{
	 if (kover_file.saveFile( url )) {
		  setCaption(i18n(url.url()), false);
		  setStatusText(i18n("File saved"));
		  altered_data = false;
		  recent->addURL(url);
		  m_url = url;
	 } else
		  KMessageBox::error( this, i18n("Error while opening/reading file!"));
}

void KoverTop::fileSave()
{
	 if( m_url.isEmpty() )
		  fileSaveAs();
	 else
		  saveFile( m_url );
}

void KoverTop::fileSaveAs()
{
	 KURL url = KFileDialog::getSaveURL( QString::null, "*.kover" );

	 if (!url.isEmpty())
		  saveFile( url );
}

void KoverTop::filePrint() {
	 cdview->printKover();
	 _DEBUG_ fprintf(stderr,"Printing done\n");
}

void KoverTop::cut() {
	 if (title_edit->hasFocus())
		  title_edit->cut();
	 if (contents_edit->hasFocus())
		  contents_edit->cut();
}

void KoverTop::copy() {
	 if (title_edit->hasFocus())
		  title_edit->copy();
	 if (contents_edit->hasFocus())
		  contents_edit->copy();
}

void KoverTop::paste() {
	 if (title_edit->hasFocus())
		  title_edit->paste();
	 if (contents_edit->hasFocus())
		  contents_edit->paste();
}

void KoverTop::actualSize() {
	 cdview->resize( PREV_WIDTH, PREV_HEIGHT );
	 cdview->move( 0, 0 );
	 main_frame->move(0,0);
	 setFixedSize( PREV_WIDTH, PREV_HEIGHT );
	 cdview->showPreview();
	 cdview->setFocus();
	 menuBar()->hide();
	 statusBar()->hide();
	 toolBar("mainToolBar")->hide();
	 toolBar("koverToolBar")->hide();
	 main_frame->adjustSize();
}

void KoverTop::cddbFill() {
	 setStatusText(i18n("Initiating CDDB lookup!"));
	 if (altered_data) {
		  if(how_about_saving())
				return;
	 }

	 if(cddb_fill->execute()) {
		  cddb_fill->setTitleAndContents();
		  disconnect( contents_edit, SIGNAL(textChanged()), this, SLOT(contentsBoxChanged()) );
		  title_edit->setText( kover_file.title() );
		  contents_edit->setText( kover_file.contents() );
		  connect( contents_edit, SIGNAL(textChanged()), SLOT(contentsBoxChanged()) );
		  altered_data = false;
		  cddb_fill->cdInfo();
	 }
}

void KoverTop::preferences() {
	 PreferencesDialog *dialog = new PreferencesDialog(this,i18n("config me"));
	 dialog->exec();
	 delete dialog;
}

void KoverTop::titleFont() {
	 QFont new_font;
	 KFontDialog *kf = new KFontDialog(this, "kf", true);
	 new_font = kover_file.titleFont();
	 if (kf->getFont( new_font ))
		  kover_file.setTitleFont( new_font );
	 delete kf;
}

void KoverTop::inlet_title_font() {
	 QFont new_font;
	 KFontDialog *kf = new KFontDialog(this, "inlet_font", true);
	 new_font = kover_file.inlet_title_font();
	 if (kf->getFont(new_font))
		  kover_file.set_inlet_title_font(new_font);
	 delete kf;
}

void KoverTop::imageEmbedding() {
	 ImageDlg *imgdlg = new ImageDlg(this,&kover_file);
	 imgdlg->exec();
	 delete imgdlg;
}

void KoverTop::titleFontColor() {
	 QColor new_color;
	 KColorDialog *kc = new KColorDialog(this, "kc", true);
	 new_color = kover_file.titleColor();
	 if (kc->getColor( new_color ))
		  kover_file.setTitleColor( new_color );
	 delete kc; 
}

void KoverTop::contentsFont() {
	 QFont new_font;
	 KFontDialog *kf = new KFontDialog(this, "kf", true);
	 new_font = kover_file.contentsFont();
	 if (kf->getFont( new_font ))
		  kover_file.setContentsFont( new_font );
	 delete kf;
}

void KoverTop::contentsFontColor() {
	 QColor new_color;
	 KColorDialog *kc = new KColorDialog(this, "kc", true);
	 new_color = kover_file.contentsColor();
	 if (kc->getColor( new_color ))
		  kover_file.setContentsColor( new_color );
	 delete kc;
}

void KoverTop::backgroundColor() {
	 QColor new_color;
	 KColorDialog *kc = new KColorDialog(this, "kc", true);
	 new_color = kover_file.backColor();
	 if (kc->getColor( new_color ))
		  kover_file.setBackColor( new_color );
	 delete kc;
}

void KoverTop::cdrom_eject() {
	 cdrom *cdrom_class = new cdrom(globals.cdrom_device);
	 cdrom_class->eject();
	 delete cdrom_class;
}

void KoverTop::cddb_without_cd() {
	 int display_track_duration = globals.display_track_duration;
	 globals.display_track_duration = 0;
	 without_cd * without = new without_cd();
	 without->exec();
	 int category = without->get_category();
	 char * id = without->get_id();
	 if(cddb_fill->execute_without_cd(id, category)) {
		  cddb_fill->setTitleAndContents();
		  disconnect( contents_edit, SIGNAL(textChanged()), this, SLOT(contentsBoxChanged()) );
		  title_edit->setText( kover_file.title() );
		  contents_edit->setText( kover_file.contents() );
		  connect( contents_edit, SIGNAL(textChanged()), SLOT(contentsBoxChanged()) );
		  altered_data = false;
		  cddb_fill->cdInfo();
	 }
	 delete(without);
	 free(id);
	 globals.display_track_duration = display_track_duration;
}


