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
	 
	 File: KoverTop.cpp  
	 
	 Description: TopLevelWidget
	 
	 Changes:

	 05 Apr 2001: globals initialization methods

*/


#include "kover.h"
#include "KoverTop.h"
#include "cddb_fill.h"
#include "imagedlg.h"

#define NORM_WIDTH	520
#define NORM_HEIGHT	400
#define PREV_WIDTH	695
#define PREV_HEIGHT	684

#define CDVIEW_X	224
#define CDVIEW_Y	 0
#define CDVIEW_WIDTH	291
#define CDVIEW_HEIGHT	310



KoverTop::KoverTop(const char* name) : KMainWindow(0,name)
{
#ifdef USE_THREADS
	 if(( semid = semget( SEMKEY, SEM_CNT, MODE | IPC_CREAT )) < 0 )
    {
        perror("ERROR");
        exit(-1);
    }
	 update_display = false;
	 end_loop = false;
#endif

	 main_frame = new QFrame(this);
  
	 setCaption(i18n("[New Document]"), false);
	 setFixedSize( NORM_WIDTH, NORM_HEIGHT );
	 filename = "untitled";

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
	 (void)new KAction(i18n("&Actual size"),"viewmag",0,this, SLOT(actualSize()),actionCollection(), "actual_size");
	 (void)new KAction(i18n("&CDDB lookup"),"network",0,this, SLOT(cddbFill()),actionCollection(), "cddb");
	 KStdAction::preferences(this,SLOT(preferences()),actionCollection());
	 (void)new KAction(i18n("&Image Embedding..."),"background",0,this,SLOT(imageEmbedding()),actionCollection(),"image_embedding");
	 (void)new KAction(i18n("Title Font..."),"fonts",0,this,SLOT(titleFont()),actionCollection(),"title_font");
	 (void)new KAction(i18n("Title Fontcolor..."),"colorize",0,this,SLOT(titleFontColor()),actionCollection(),"title_font_color");
	 (void)new KAction(i18n("Contents Font..."),"fonts",0,this,SLOT(contentsFont()),actionCollection(),"contents_font");
	 (void)new KAction(i18n("Contents Fontcolor..."),"colorize",0,this,SLOT(contentsFontColor()),actionCollection(),"contents_font_color");
	 (void)new KAction(i18n("Background Color..."),"colors",0,this,SLOT(backgroundColor()),actionCollection(),"background_color");

	 createGUI();

	 number_spin = new QSpinBox( 1, 999, 1, main_frame, "numberspin" );
	 number_spin->setEnabled(false);
	 number_spin->resize( 50, 25 );
	 number_spin->move( 170, 5 );
	 connect( number_spin, SIGNAL(valueChanged(int)), SLOT(numberChanged(int)) );

	 number_check = new QCheckBox( "CD Number", main_frame, "number_check" );
	 number_check->resize( 100, 25 );
	 number_check->move( 50, 5 );
	 connect( number_check, SIGNAL(toggled(bool)), SLOT(numberChecked(bool)) );


	 title_label = new QLabel( "Title", main_frame, "title_label" );
	 title_label->move( 5, 25 );

	 title_edit = new QMultiLineEdit( main_frame, "title_edit" );
	 title_edit->resize( 215, 50 );
	 title_edit->move( 5, 50 );
	 connect( title_edit, SIGNAL(textChanged()), SLOT(titleBoxChanged()) );
  
	 contents_label = new QLabel( "Contents", main_frame, "contents_label" );
	 contents_label->move( 5, 105 );

	 contents_edit = new QMultiLineEdit( main_frame, "contents_edit" );
	 contents_edit->resize( 215, 175 );
	 contents_edit->move( 5, 135 );
	 connect(contents_edit, SIGNAL(textChanged()), SLOT(contentsBoxChanged()));
          
	 cdview = new CDView( &kover_file, main_frame );
	 cdview->resize( CDVIEW_WIDTH, CDVIEW_HEIGHT );
	 cdview->move( CDVIEW_X, CDVIEW_Y );
	 connect( cdview, SIGNAL(stopPreview()), SLOT(stopPreview()) );
	 connect( cdview, SIGNAL(actualSize()), SLOT(actualSize()) );
  
	 connect( &kover_file, SIGNAL(dataChanged(bool)), SLOT(dataChanged(bool)) );

#ifdef USE_THREADS
	 cddb_fill = new CDDBFill();
#else
	 cddb_fill = new CDDB_Fill( &kover_file );
#endif
	 connect(cddb_fill, SIGNAL(statusText(const char*)), SLOT(setStatusText(const char*)));
	 connect(cddb_fill, SIGNAL(updateDisplay(bool)), SLOT(updateDisplay(bool)) );

	 altered_data = false;
	 main_frame->move(0,70);
	 main_frame->adjustSize();
}

KoverTop::~KoverTop()
{
	 delete status_bar;
	 delete cddb_fill;
	 delete cdview;
}

void KoverTop::dataChanged(bool image)
{
	 bool bla = image;
	 bla = true;
		  
	 setStatusText( "Data changed" );
	 if (filename == "untitled")
		  setCaption(i18n("[New Document]"), true);
	 else
		  setCaption(i18n(filename), true);
	 
	 altered_data = true;
}

void KoverTop::setStatusText( const char* _status_text )
{
	 status_bar->changeItem( _status_text, 1 );
	 kapp->processEvents();
}

void KoverTop::titleBoxChanged()
{
	 kover_file.setTitle( title_edit->text() );
}

void KoverTop::contentsBoxChanged()
{
	 kover_file.setContents( contents_edit->text() );
}

void KoverTop::numberChanged(int value)
{
	 kover_file.setNumber( value );
}

void KoverTop::numberChecked(bool checked)
{
	 number_spin->setEnabled(checked);
  
	 if (checked)
	 {
		  kover_file.setNumber( number_spin->value() );
	 } else
	 {
		  kover_file.setNumber( 0 );
	 }
}


char KoverTop::hexToChar( char hexc )
{
	 switch (hexc)
	 {
	 case 48:
		  return 0;
	 case 49:
		  return 1;
	 case 50:
		  return 2;
	 case 51:
		  return 3;
	 case 52:
		  return 4;
	 case 53:
		  return 5;
	 case 54:
		  return 6;
	 case 55:
		  return 7;
	 case 56:
		  return 8;
	 case 57:
		  return 9;
	 case 97:
	 case 65:
		  return 10;
	 case 98:
	 case 66:
		  return 11;
	 case 99:
	 case 67:
		  return 12;
	 case 100:
	 case 68:
		  return 13;
	 case 101:
	 case 69:
		  return 14;
	 case 102:
	 case 70:
		  return 15;
	 }

	 return 0;
}

void KoverTop::parseFilename( QString& filename )
{
	 /*
		unsigned int	i;
		char		c;
		QString		tmp = "";
	 */
	 for (uint i=0; i<filename.length(); i++)
	 {
		  // c = filename.at(i);
// 	 if (c == 37)
// 	 {
// 	 c = hexToChar(filename.at(i+1)) * 16 + hexToChar(filename.at(i+2));
// 	 i += 2;
// 	 }
// 	 tmp += c;
	 }
	 /*
		filename = tmp;*/
}

QString KoverTop::dataIcon(QString filename)
{
	 return locate("data","kover/" + filename);
}

void KoverTop::stopPreview()
{
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

bool KoverTop::queryClose()
{
	 if (altered_data)
	 {
		  if(howAboutSaving())
				return false;
	 }
#ifdef USE_THREADS
	 cddb_fill->killThread();
	 if( semctl( semid, 0, IPC_RMID) < 0 )
    {
        perror("ERROR");
	 }
#endif
	 return true;
}

void KoverTop::fileNew()
{
	 if (altered_data)
	 {
		  if(howAboutSaving())
				return;
	 }
  
	 title_edit->clear();
	 contents_edit->clear();
	 number_spin->setValue( 1 );
	 number_spin->setEnabled( false );
	 number_check->setChecked( false );
	 title_edit->setFocus();
	 kover_file.reset();
	 setStatusText( "Chop!" );
	 altered_data = false;
	 filename = "untitled";
	 setCaption(i18n("[New Document]"), false);
	 cddb_fill->killThread();
}

void KoverTop::fileOpen()
{
	 QString newfilename;
  
	 if (altered_data)
	 {
		  if (howAboutSaving())
				return;
	 }
  
	 newfilename = KFileDialog::getOpenFileName();
	 if (newfilename.length())
	 {
		  parseFilename( newfilename );
		  if (kover_file.openFile( newfilename ))
		  {
				filename = newfilename;
              
				setCaption(i18n(filename), false);

				disconnect( contents_edit, SIGNAL(textChanged()), this, SLOT(contentsBoxChanged()) );
				title_edit->setText( kover_file.title() );
				contents_edit->setText( kover_file.contents() );
				connect( contents_edit, SIGNAL(textChanged()), SLOT(contentsBoxChanged()) );

				if (kover_file.number())
				{
					 number_spin->setValue( kover_file.number() );
					 number_spin->setEnabled( true );
					 number_check->setChecked( true );
				} 
				else
				{
					 number_spin->setEnabled( false );
					 number_check->setChecked( false );
				}
              
				setStatusText( "File loaded" );
				altered_data = false;
		  } 
		  else
		  {
				KMessageBox::error( this, "Error while opening/reading file!");
		  }
	 }
}

int KoverTop::howAboutSaving()
{
	 switch (KMessageBox::warningYesNoCancel( this, "Data changed since last saving!\nDo you want to save the changes?"))
	 {
	 case 3: //YES
		  fileSave();
		  if (altered_data)
		  {
				return -1;
		  }
		  return 0;
	 case 4: //NO
		  return 0;
	 case 2: //CANCEL
		  return -1;
	 }
	 return -1;
}

void KoverTop::fileSave()
{
	 if (filename == "untitled")
	 {
		  fileSaveAs();
	 } 
	 else
	 {
		  if (kover_file.saveFile( filename ))
		  {
				setStatusText( "File saved" );
				altered_data = false;
		  } 
		  else
		  {
				KMessageBox::error( this, "Error while opening/reading file!");
		  }
	 }
}

void KoverTop::fileSaveAs()
{
	 QString newfilename;

	 newfilename = KFileDialog::getSaveFileName();
	 if (newfilename.length())
	 {
		  parseFilename( newfilename );
		  if (kover_file.saveFile( newfilename ))
		  {
				filename = newfilename;

				setCaption(i18n(filename), false);
				setStatusText( "File saved" );
				altered_data = false;
		  } 
		  else
		  {
				KMessageBox::error( this, "Error while opening/reading file!");
		  }
	 }
}

void KoverTop::filePrint()
{
	 cdview->printKover();
#ifdef ENABLE_DEBUG_OUTPUT
	 fprintf(stderr,"Printing done\n");
#endif
}

void KoverTop::cut()
{
	 if (title_edit->hasFocus())
		  title_edit->cut();
	 if (contents_edit->hasFocus())
		  contents_edit->cut();
}

void KoverTop::copy()
{
	 if (title_edit->hasFocus())
		  title_edit->copy();
	 if (contents_edit->hasFocus())
		  contents_edit->copy();
}

void KoverTop::paste()
{
	 if (title_edit->hasFocus())
		  title_edit->paste();
	 if (contents_edit->hasFocus())
		  contents_edit->paste();
}

void KoverTop::actualSize()
{
	 if (!globals.trigger_actual_size)
		  return;
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

// void KoverTop::toggleToolBar()
// {
//   if (toolbarAction->isChecked())
// 	 {
// 		toolBar("koverToolBar")->show();
// 		toolBar("mainToolBar")->show();
//   		main_frame->move(0,70);
// 		setFixedSize(NORM_WIDTH, NORM_HEIGHT);
// 	 }
//   else
// 	 {
// 		toolBar("mainToolBar")->hide();
// 		toolBar("koverToolBar")->hide();
// 		main_frame->move(0,38);
// 		setFixedSize(NORM_WIDTH, NORM_HEIGHT toolBar);
// 	 }
// }


// void KoverTop::toggleStatusBar()
// {
//   if (statusbarAction->isChecked())
// 	 {
// 		statusBar()->show();
// 		setFixedSize(NORM_WIDTH, NORM_HEIGHT);
// 	 }
//   else
//     { 
// 		statusBar()->hide();
// 		setFixedSize(NORM_WIDTH, NORM_HEIGHT-20);
// 	 }

// }

void KoverTop::editToolbars()
{
	 KEditToolbar dlg(actionCollection());

	 if (dlg.exec())
		  createGUI();
}

void KoverTop::cddbFill()
{

	 setStatusText("Initiating CDDB lookup!");

#ifdef USE_THREADS
	 timer = new QTimer(this);
	 connect(timer, SIGNAL(timeout()),this, SLOT(cddbDone()));
#endif

	 if (altered_data)
	 {
		  if(howAboutSaving())
				return;
	 }

	 if(cddb_fill->execute())
	 {
#ifdef USE_THREADS
		  sops[0].sem_num = 0;
		  sops[0].sem_flg = IPC_NOWAIT;
		  sops[0].sem_op  = 1;
		  semop(semid,sops,1);
		  timer->start(20);

		  while (!end_loop) 
		  {
				kapp->processEvents();
		  }

		  if (update_display)
		  {
#endif
				cddb_fill->setTitleAndContents();
				disconnect( contents_edit, SIGNAL(textChanged()), this, SLOT(contentsBoxChanged()) );
				title_edit->setText( kover_file.title() );
				contents_edit->setText( kover_file.contents() );
				connect( contents_edit, SIGNAL(textChanged()), SLOT(contentsBoxChanged()) );
				altered_data = false;
				cddb_fill->cdInfo();
#ifdef USE_THREADS
				cddb_fill->killThread(); // actualy just joining... to prevent memory leaks
		  }
#endif
	 }
	 update_display = false;
	 end_loop = false;
}

void KoverTop::cddbDone()
{

#ifdef USE_THREADS

	 sops[0].sem_num = 0;
	 sops[0].sem_flg = IPC_NOWAIT;
	 sops[0].sem_op  = 0;
  
	 parent->processEvents();
	 if (semop(semid,sops,1) > 0);
	 {
#ifdef ENABLE_DEBUG_OUTPUT
		  printf("cddb lookup done\n");
#endif
		  timer->stop();
		  cddb_fill->setTitleAndContents();
		  disconnect( contents_edit, SIGNAL(textChanged()), this, SLOT(contentsBoxChanged()) );
		  title_edit->setText( kover_file.title() );
		  contents_edit->setText( kover_file.contents() );
		  connect( contents_edit, SIGNAL(textChanged()), SLOT(contentsBoxChanged()) );
		  altered_data = false;
		  cddb_fill->cdInfo();
	 }

#endif
}

void KoverTop::preferences()
{
	 PreferencesDialog *dialog = new PreferencesDialog(this, "config me");
	 dialog->exec();
	 delete dialog;
}

void KoverTop::titleFont()
{
	 QFont new_font;
	 KFontDialog *kf = new KFontDialog(this, "kf", true);
	 new_font = kover_file.titleFont();
	 if (kf->getFont( new_font ))
	 {
		  kover_file.setTitleFont( new_font );
	 }
	 delete kf;
}

void KoverTop::imageEmbedding()
{
	 ImageDlg *imgdlg = new ImageDlg(this,&kover_file);
	 imgdlg->exec();
	 delete imgdlg;
}

void KoverTop::updateDisplay(bool update_really)
{
	 update_display = update_really;
	 end_loop = true;
}

void KoverTop::titleFontColor()
{
	 QColor new_color;
	 KColorDialog *kc = new KColorDialog(this, "kc", true);
	 new_color = kover_file.titleColor();
	 if (kc->getColor( new_color ))
	 {
		  kover_file.setTitleColor( new_color );
	 }
	 delete kc; 
}

void KoverTop::contentsFont()
{
	 QFont new_font;
	 KFontDialog *kf = new KFontDialog(this, "kf", true);
	 new_font = kover_file.contentsFont();
	 if (kf->getFont( new_font ))
	 {
		  kover_file.setContentsFont( new_font );
	 }
	 delete kf;
}

void KoverTop::contentsFontColor()
{
	 QColor new_color;
	 KColorDialog *kc = new KColorDialog(this, "kc", true);
	 new_color = kover_file.contentsColor();
	 if (kc->getColor( new_color ))
	 {
		  kover_file.setContentsColor( new_color );
	 }
	 delete kc;
}

void KoverTop::backgroundColor()
{
	 QColor new_color;
	 KColorDialog *kc = new KColorDialog(this, "kc", true);
	 new_color = kover_file.backColor();
	 if (kc->getColor( new_color ))
	 {
		  kover_file.setBackColor( new_color );
	 }
	 delete kc;
}

void KoverTop::load_globals()
{
	 struct stat stat_struct;
	 KConfig *config = kapp->config();
	 QString *string = new QString();
	 config->setGroup("CDDB");
		  
	 if ((config->readEntry("cddb_server")).isEmpty())
		  globals.cddb_server = strdup("freedb.freedb.org");		 
	 else
		  globals.cddb_server = strdup((config->readEntry("cddb_server")).latin1());
		  
	 if ((config->readEntry("cgi_path")).isEmpty())
		  globals.cgi_path = strdup("~cddb/cddb.cgi");
	 else
		  globals.cgi_path = strdup((config->readEntry("cgi_path")).latin1());
		  
	 if ((config->readEntry("proxy_server")).isEmpty())
		  globals.proxy_server = NULL;
	 else
		  globals.proxy_server = strdup((config->readEntry("proxy_server")).latin1());

	 *string = config->readEntry("proxy_port");
	 if (string->isEmpty())
		  globals.proxy_port = 3128;
	 else
		  globals.proxy_port = string->toInt();

#ifdef ENABLE_DEBUG_OUTPUT
	 fprintf(stderr,"proxy port: %d\n",globals.proxy_port);
#endif

	 *string = config->readEntry("use_proxy");
	 if (string->isNull())
		  globals.use_proxy = 0;
	 else
		  globals.use_proxy = string->toInt();
		  
	 *string = config->readEntry("proxy_from_env");
	 if (string->isNull())
		  globals.proxy_from_env = 0;
	 else
		  globals.proxy_from_env = string->toInt();

	 config->setGroup("CDROM");
		  
	 if ((config->readEntry("cdrom_device")).isEmpty())
		  globals.cdrom_device = strdup("/dev/cdrom");
	 else
		  globals.cdrom_device = strdup((config->readEntry("cdrom_device")).latin1());
		    
	 *string = config->readEntry("eject_cdrom");
	 if (string->isNull())
		  globals.eject_cdrom = 0;
	 else
		  globals.eject_cdrom = string->toInt();
		  
	 config->setGroup("CDDB_files");
	 *string = config->readEntry("read_local_cddb");
	 if (string->isNull())
		  globals.read_local_cddb = 0;
	 else
		  globals.read_local_cddb = string->toInt();
	 *string = config->readEntry("write_local_cddb");
	 if (string->isNull())
		  globals.write_local_cddb = 0;
	 else
		  globals.write_local_cddb = string->toInt();
	 if ((config->readEntry("cddb_path")).isEmpty())
	 {
		  globals.cddb_path = check_cddb_dir();
	 }
	 else
	 {
		  globals.cddb_path = strdup((config->readEntry("cddb_path")).latin1());
		  /* checking for "/" at the end */
		  if (globals.cddb_path[strlen(globals.cddb_path)-1]!='/')
		  {
				char *dummy = strdup(globals.cddb_path);
				free(globals.cddb_path);
				globals.cddb_path = (char *) malloc(strlen(dummy)+1);
				strcpy(globals.cddb_path,dummy);
				strcat(globals.cddb_path,"/");
				free(dummy);
		  }
		  /* does the directory exist */
		  if (stat(globals.cddb_path,&stat_struct)==-1)
				/* no it doesn't... let's create one */
				if(mkdir(globals.cddb_path,0777)==-1)
				{
					 /* failed */
					 free(globals.cddb_path);
					 globals.cddb_path = NULL;
				}
					
	 }

	 config->setGroup("misc");
	 *string = config->readEntry("trigger_actual_size");
	 if (string->isNull())
		  globals.trigger_actual_size = 1;
	 else
		  globals.trigger_actual_size = string->toInt();
		  
	 *string = config->readEntry("display_track_duration");
	 if (string->isNull())
		  globals.display_track_duration = 1;
	 else
		  globals.display_track_duration = string->toInt();

	 delete (string);
}

void KoverTop::store_globals()
{
	 KConfig *config = kapp->config();
	 QString *string = new QString();
	 config->setGroup("CDDB");
	 config->writeEntry("cddb_server",globals.cddb_server);
	 config->writeEntry("cgi_path",globals.cgi_path);
	 config->writeEntry("proxy_server",globals.proxy_server);
	 string->sprintf("%d",globals.proxy_port);
	 config->writeEntry("proxy_port",*string);
	 string->sprintf("%d",globals.use_proxy);
	 config->writeEntry("use_proxy",*string);
	 string->sprintf("%d",globals.proxy_from_env);
	 config->writeEntry("proxy_from_env",*string);
		  
	 config->setGroup("CDROM");
	 string->sprintf("%d",globals.eject_cdrom);
	 config->writeEntry("eject_cdrom",*string);
	 config->writeEntry("cdrom_device",globals.cdrom_device);

	 config->setGroup("CDDB_files");
	 string->sprintf("%d",globals.read_local_cddb);
	 config->writeEntry("read_local_cddb",*string);
	 string->sprintf("%d",globals.write_local_cddb);
	 config->writeEntry("write_local_cddb",*string);
	 config->writeEntry("cddb_path",globals.cddb_path);
		  
	 config->setGroup("misc");
	 string->sprintf("%d",globals.trigger_actual_size);
	 config->writeEntry("trigger_actual_size",*string);
		  
	 string->sprintf("%d",globals.display_track_duration);
	 config->writeEntry("display_track_duration",*string);

	 delete (string);	
}

char *KoverTop::check_cddb_dir()
{
	 char *home_dir = NULL;
	 char *cddb_file = NULL;
	 struct stat stat_struct;
	 home_dir = getenv("HOME");
	 if (home_dir)
	 {
		  if (home_dir[strlen(home_dir)-1]!='/')
		  {
				cddb_file = (char *) malloc(strlen(home_dir)+7+8);
				strcpy(cddb_file,home_dir);
				strcat(cddb_file, "/.cddb/");
		  }
		  else
		  {
				cddb_file = (char *) malloc(strlen(home_dir)+6+8);
				strcpy(cddb_file,home_dir);
				strcat(cddb_file, ".cddb/");
		  }

		  /* does the directory exist */
		  if (stat(cddb_file,&stat_struct)==-1)
				/* no it doesn't... let's create one */
				if(mkdir(cddb_file,0777)==-1)
				{
					 /* failed */
					 free(cddb_file);
					 return NULL;
				}

		  //free(cddb_file);
		  return cddb_file;
	 }
		  
	 return NULL;
}
