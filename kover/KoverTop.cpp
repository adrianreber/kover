/***************************************************************************
                          KoverTop.cpp  -  TopLevelWidget
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
#include "kover.h"
#include "KoverTop.h"
#include "cddb_fill.h"
#include "imagedlg.h"
#include "../config.h"

#include <kiconloader.h>
#include <kmessagebox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <kapp.h>
#include <kfontdialog.h>
#include <kcolordlg.h>
//#include <ktopwidget.h>
#include <ktmainwindow.h>
#include <qmultilinedit.h>
#include <kfiledialog.h>
#include <qregexp.h>

#define NORM_WIDTH	520
#define NORM_HEIGHT	400
#define PREV_WIDTH	695
#define PREV_HEIGHT	684

#define CDVIEW_X	224
#define CDVIEW_Y	 0
#define CDVIEW_WIDTH	291
#define CDVIEW_HEIGHT	310

#define DATAICON(x) QPixmap( KApplication::kde_datadir() + "/kover/toolbar/" + x )

KoverTop::KoverTop(QApplication *parent, const char* name=NULL ) : KMainWindow(0,name)
{
  main_frame = new QFrame(this);
  //main_frame->resize(200,200);

  this->parent=parent;

  //setCaption("kover",true);
  cddb_thread=0;
  update_display = false;
  end_loop = false;
  setCaption(i18n("[New Document]"), false);
  setFixedSize( NORM_WIDTH, NORM_HEIGHT );
  filename = "untitled";

  status_bar = statusBar();
  status_bar->insertItem( "Kover "VERSION" - http://www.fischlustig.de/kover - dok@fischlustig.de (Denis Oliver Kropp)", 1 );


  KStdAction::openNew(this, SLOT(fileNew()), actionCollection());
  KStdAction::open(this, SLOT(fileOpen()), actionCollection());
  KStdAction::save(this, SLOT(fileSave()), actionCollection());
  KStdAction::saveAs(this, SLOT(fileSaveAs()), actionCollection());
  KStdAction::print(this, SLOT(filePrint()), actionCollection());
  KStdAction::quit(this, SLOT(close()), actionCollection());
  KStdAction::cut(this, SLOT(cut()), actionCollection());
    KStdAction::copy(this, SLOT(copy()), actionCollection());
    KStdAction::paste(this, SLOT(paste()), actionCollection());
	 //KStdAction::actualSize(this, SLOT(actualSize()), actionCollection());
	 //  (void)new KAction(i18n("&Edit mode"), 0, this, SLOT(stopPreview()),
	 //                actionCollection(), "edit");
	  //toolbarAction = KStdAction::showToolbar(this, SLOT(toggleToolBar()), actionCollection());
	  //statusbarAction = KStdAction::showStatusbar(this, SLOT(toggleStatusBar()), actionCollection());
	  //KStdAction::configureToolbars(this, SLOT(editToolbars()), actionCollection());
	 (void)new KAction(i18n("&Actual size"),"viewmag",0,this, SLOT(actualSize()),actionCollection(), "actual_size");
	 (void)new KAction(i18n("&CDDB lookup"),dataIcon("cddb.png"),0,this, SLOT(cddbFill()),actionCollection(), "cddb");
	 KStdAction::preferences(this,SLOT(preferences()),actionCollection());
	 (void)new KAction(i18n("&Image Embedding"),"background",0,this,SLOT(imageEmbedding()),actionCollection(),"image_embedding");


  //menu_bar = menuBar();
  //file_menu = new KPopupMenu(this,"File");
//   file_menu->insertItem("New",this, SLOT(fileNew()),CTRL+Key_N);
//   file_menu->insertItem("Open",this, SLOT(fileOpen()));
//   file_menu->insertSeparator();
//   file_menu->insertItem("Save",this, SLOT(fileSave()));
//   file_menu->insertItem("Save As...",this, SLOT(fileSaveAs()));
//   file_menu->insertSeparator();
//   menu_bar->insertItem("File", file_menu);
//   KStdAction::undo(this, SLOT(fileNew()));

  


	 //cd_toolbar =  toolBar("CD_TOOLBAR");
	 //cd_toolbar->insertButton("colors", ID_CD_BGCOLOR, true, "Background Color" );
//   cd_toolbar->insertSeparator();
//   cd_toolbar->insertButton(dataIcon("fonts.png"), ID_CD_TITLEFONT, true, "Title Font" );
//   cd_toolbar->insertButton(dataIcon("text_color.png"), ID_CD_TITLECOLOR, true, "Title Textcolor" );
//   cd_toolbar->insertSeparator();
//   cd_toolbar->insertButton(dataIcon("contentsfont.xpm"), ID_CD_CONTENTSFONT, true, "Contents Font" );
//   cd_toolbar->insertButton("colors", ID_CD_CONTENTSCOLOR, true, "Contents Textcolor" );
//   cd_toolbar->insertSeparator();
//   cd_toolbar->insertButton("background", ID_CD_IMAGES, true, "Image Embedding" );
//   cd_toolbar->insertSeparator();

//   cd_toolbar->insertButton(dataIcon("cddb.xpm"),ID_CD_CDDBFILL,true,"CDDB Fill");
	
//   connect( cd_toolbar, SIGNAL(clicked(int)), SLOT(handleCdToolBar(int)) );
  
  //newAct->plug(cd_toolbar);

  createGUI();

//   main_toolbar = toolBar(MAIN_TOOLBAR);
//   //	main_toolbar->setBarPos(KToolBar::Top);
//   main_toolbar->insertButton("filenew", ID_MAIN_NEW, SIGNAL(clicked(int)),this, SLOT(fileNew()),true, "Clear all" );
//   main_toolbar->insertButton("fileopen", ID_MAIN_OPEN,SIGNAL(clicked(int)),this, SLOT(fileOpen()), true, "Open" );
//   main_toolbar->insertButton("filesave", ID_MAIN_SAVE, SIGNAL(clicked(int)),this, SLOT(fileSave()),true, "Save" );
//   //main_toolbar->insertButton("filesaveas", ID_MAIN_SAVEAS, SIGNAL(clicked(int)),this, SLOT(fileSaveAs()),true, "Save As" );
//   main_toolbar->insertSeparator();
//   main_toolbar->insertButton("fileprint", ID_MAIN_PRINT, true, "Print" );
//   main_toolbar->insertButton("viewmag", ID_MAIN_VIEW, true, "Full View" );
//   main_toolbar->insertSeparator();
//   main_toolbar->insertButton("penguin", ID_MAIN_ABOUT, true, "About" );
//   main_toolbar->insertButton("exit", ID_MAIN_EXIT, true, "Exit" );
//   main_toolbar->alignItemRight( ID_MAIN_ABOUT );

//   connect( main_toolbar, SIGNAL(clicked(int)), SLOT(handleMainToolBar(int)) );


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
  connect( contents_edit, SIGNAL(textChanged()), SLOT(contentsBoxChanged()) );
  
        
  cdview = new CDView( &kover_file, main_frame );
  cdview->resize( CDVIEW_WIDTH, CDVIEW_HEIGHT );
  cdview->move( CDVIEW_X, CDVIEW_Y );
  connect( cdview, SIGNAL(stopPreview()), SLOT(stopPreview()) );
  connect( cdview, SIGNAL(actualSize()), SLOT(actualSize()) );
  
  connect( &kover_file, SIGNAL(dataChanged(bool)), SLOT(dataChanged(bool)) );

  cddb_fill = new CDDB_Fill( &kover_file );
  connect( cddb_fill, SIGNAL(statusText(const char*)), SLOT(setStatusText(const char*)) );
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
  //setStatusText( "Data changed" );
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
	
	 for (i=0; i<filename.length(); i++)
	 {
	 c = filename.at(i);
	 if (c == 37)
	 {
	 c = hexToChar(filename.at(i+1)) * 16 + hexToChar(filename.at(i+2));
	 i += 2;
	 }
	 tmp += c;
	 }
	
	 filename = tmp;*/
}

QString KoverTop::dataIcon(QString filename)
{
  return locate("data","kover/" + filename);
}

void KoverTop::handleMainToolBar(int id)
{
  QString newfilename;

  switch (id)
	 {
	 case ID_MAIN_NEW:
		// if (altered_data)
// 		  {
// 			 switch (KMessageBox::warningYesNoCancel( this, "Data changed since last saving!\nDo you want to save the changes?"))
// 				{
// 				case 1:
// 				  handleMainToolBar( ID_MAIN_SAVE );
// 				  if (altered_data)
// 					 {
// 						return;
// 					 }
// 				  break;
// 				case 2:
// 				  break;
// 				case 3:
// 				  return;
// 				}
// 		  }
// 		title_edit->clear();
// 		contents_edit->clear();
// 		number_spin->setValue( 1 );
// 		number_spin->setEnabled( false );
// 		number_check->setChecked( false );
// 		title_edit->setFocus();
// 		kover_file.reset();
// 		setStatusText( "Chop!" );
// 		altered_data = false;
// 		filename = "untitled";
// 		setCaption( "Kover - untitled" );
		break;
	 case ID_MAIN_OPEN:
// 		if (altered_data)
// 		  {
// 			 switch (KMessageBox::warningYesNoCancel( this, "Data changed since last saving!\nDo you want to save the changes?"))
// 				{
// 				case 1:
// 				  handleMainToolBar( ID_MAIN_SAVE );
// 				  if (altered_data)
// 					 {
// 						return;
// 					 }
// 				  break;
// 				case 2:
// 				  break;
// 				case 3:
// 				  return;
// 				}
// 		  }
// 		newfilename = KFileDialog::getOpenFileName( NULL, NULL, this );
// 		if (newfilename.length())
// 		  {
// 			 parseFilename( newfilename );
// 			 if (kover_file.openFile( newfilename ))
// 				{
// 				  filename = newfilename;
              
// 				  QString newCaption;
// 				  newCaption.append( "Kover - " );
// 				  newCaption.append( filename );
// 				  setCaption( newCaption );
              
// 				  disconnect( contents_edit, SIGNAL(textChanged()), this, SLOT(contentsBoxChanged()) );
// 				  title_edit->setText( kover_file.title() );
// 				  contents_edit->setText( kover_file.contents() );
// 				  connect( contents_edit, SIGNAL(textChanged()), SLOT(contentsBoxChanged()) );

// 				  if (kover_file.number())
// 					 {
// 						number_spin->setValue( kover_file.number() );
// 						number_spin->setEnabled( true );
// 						number_check->setChecked( true );
// 					 } else
// 						{
// 						  number_spin->setEnabled( false );
// 						  number_check->setChecked( false );
// 						}
              
// 				  setStatusText( "File loaded" );
// 				  altered_data = false;
// 				} else
// 				  {
// 					 KMessageBox::error( this, "Error while opening/reading file!");
// 				  }
// 		  }
		break;
	 case ID_MAIN_SAVE:
	// 	if (filename == "untitled")
// 		  {
// 			 handleMainToolBar( ID_MAIN_SAVEAS );
// 		  } else
// 			 {
// 				if (kover_file.saveFile( filename ))
// 				  {
// 					 setStatusText( "File saved" );
// 					 altered_data = false;
// 				  } else
// 					 {
// 						KMessageBox::error( this, "Error while opening/reading file!");
// 					 }
// 			 }
		break;
	 case ID_MAIN_SAVEAS:
		// newfilename = KFileDialog::getSaveFileName( 0, 0, this);
// 		if (newfilename.length())
// 		  {
// 			 parseFilename( newfilename );
// 			 if (kover_file.saveFile( newfilename ))
// 				{
// 				  filename = newfilename;

// 				  QString newCaption;
// 				  newCaption.append( "Kover - " );
// 				  newCaption.append( filename );
// 				  setCaption( newCaption );
// 				  setStatusText( "File saved" );
// 				  altered_data = false;
// 				} else
// 				  {
// 					 KMessageBox::error( this, "Error while opening/reading file!");
// 				  }
// 		  }
		break;
	 case ID_MAIN_PRINT:
		//		cdview->printKover();
		break;
	 case ID_MAIN_VIEW:
// 		cdview->resize( PREV_WIDTH, PREV_HEIGHT );
// 		cdview->move( 0, 0 );
// 		setFixedSize( PREV_WIDTH, PREV_HEIGHT );
// 		resize( PREV_WIDTH, PREV_HEIGHT );
// 		cdview->showPreview();
// 		cdview->setFocus();
		break;
	 case ID_MAIN_ABOUT:
		showAboutApplication();
		break;
	 case ID_MAIN_EXIT:
		close();
		break;
	 }
}

void KoverTop::handleCdToolBar(int id)
{
  QColor        new_color;
  QFont         new_font;
  KColorDialog* kc;
  KFontDialog*  kf;
  QString       tracks;
  
  switch (id)
	 {
	 case ID_CD_BGCOLOR:
		kc = new KColorDialog(this, "kc", true);
		new_color = kover_file.backColor();
		if (kc->getColor( new_color ))
		  {
			 kover_file.setBackColor( new_color );
		  }
		delete kc;
		break;
	 case ID_CD_TITLEFONT:
		kf = new KFontDialog(this, "kf", true);
		new_font = kover_file.titleFont();
		if (kf->getFont( new_font ))
		  {
			 kover_file.setTitleFont( new_font );
		  }
		delete kf;
		break;
	 case ID_CD_TITLECOLOR:
		kc = new KColorDialog(this, "kc", true);
		new_color = kover_file.titleColor();
		if (kc->getColor( new_color ))
		  {
			 kover_file.setTitleColor( new_color );
		  }
		delete kc;
		break;
	 case ID_CD_CONTENTSFONT:
		kf = new KFontDialog(this, "kf", true);
		new_font = kover_file.contentsFont();
		if (kf->getFont( new_font ))
		  {
			 kover_file.setContentsFont( new_font );
		  }
		delete kf;
		break;
	 case ID_CD_CONTENTSCOLOR:
		kc = new KColorDialog(this, "kc", true);
		new_color = kover_file.contentsColor();
		if (kc->getColor( new_color ))
		  {
			 kover_file.setContentsColor( new_color );
		  }
		delete kc;
		break;
	 case ID_CD_IMAGES:
		ImageDlg* imgdlg;
		imgdlg = new ImageDlg(this,&kover_file);
		imgdlg->exec();
		delete imgdlg;
		break;
	 case ID_CD_CDDBFILL:
	// 	if (altered_data)
// 		  {
// 			 if(howAboutSaving())
// 		  return;
			 
// 		  }
// 		if (cddb_fill->execute())
// 		  {
// 			 disconnect( contents_edit, SIGNAL(textChanged()), this, SLOT(contentsBoxChanged()) );
// 			 title_edit->setText( kover_file.title() );
// 			 contents_edit->setText( kover_file.contents() );
// 			 connect( contents_edit, SIGNAL(textChanged()), SLOT(contentsBoxChanged()) );
// 			 altered_data = false;
// 			 cddb_fill->cdInfo();
// 		  }
		break;
	 }
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

// void KoverTop::closeEvent(QCloseEvent* event)
// {
//   if (altered_data)
// 	 {
// 		if(howAboutSaving())
// 		  return;
// 	 }
//   event->accept();
	
// }

bool KoverTop::queryClose()
{
   if (altered_data)
	  {
		 if(howAboutSaving())
			return false;
	  }
	cddb_fill->killThread();
	return true;
}

void KoverTop::showAboutApplication()
{
  KMessageBox::about( this,"Kover "VERSION"\n(C) 1998 Denis Oliver Kropp, a one-man division of Fischlustig\n\ndok@fischlustig.de\nCheck www.fischlustig.de/kover\n" );
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
  setCaption( "- untitled",true );
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
              
			 QString newCaption;
			 newCaption.append( "Kover - " );
			 newCaption.append( filename );
			 setCaption( newCaption );
              
			 disconnect( contents_edit, SIGNAL(textChanged()), this, SLOT(contentsBoxChanged()) );
			 title_edit->setText( kover_file.title() );
			 contents_edit->setText( kover_file.contents() );
			 connect( contents_edit, SIGNAL(textChanged()), SLOT(contentsBoxChanged()) );

			 if (kover_file.number())
				{
				  number_spin->setValue( kover_file.number() );
				  number_spin->setEnabled( true );
				  number_check->setChecked( true );
				} else
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
		  } else
			 {
				if (kover_file.saveFile( filename ))
				  {
					 setStatusText( "File saved" );
					 altered_data = false;
				  } else
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

				  QString newCaption;
				  newCaption.append( "Kover - " );
				  newCaption.append( filename );
				  setCaption( newCaption );
				  setStatusText( "File saved" );
				  altered_data = false;
				} else
				  {
					 KMessageBox::error( this, "Error while opening/reading file!");
				  }
		  }
}

void KoverTop::filePrint()
{
  cdview->printKover();
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
  if (altered_data)
	 {
		if(howAboutSaving())
		  return;
	 }

  cddb_fill->execute();

  printf("before while\n");
	

  while (!end_loop) 
	 {
		parent->processEvents();
	 }
  
  printf("after while\n");

  if (update_display)
	 {
		
		cddb_fill->setTitleAndContents();
		disconnect( contents_edit, SIGNAL(textChanged()), this, SLOT(contentsBoxChanged()) );
		title_edit->setText( kover_file.title() );
		contents_edit->setText( kover_file.contents() );
		connect( contents_edit, SIGNAL(textChanged()), SLOT(contentsBoxChanged()) );
		altered_data = false;
		cddb_fill->cdInfo();
	 }
  update_display = false;
  end_loop = false;
  cddb_fill->killThread(); // actualy just joining... to prevent memory leaks

  //if (cddb_fill->start())
	 {
//  disconnect( contents_edit, SIGNAL(textChanged()), this, SLOT(contentsBoxChanged()) );
//   title_edit->setText( kover_file.title() );
//   contents_edit->setText( kover_file.contents() );
//   connect( contents_edit, SIGNAL(textChanged()), SLOT(contentsBoxChanged()) );
//   altered_data = false;
//   cddb_fill->cdInfo();
	 }
  //pthread_create(&cddb_thread, NULL,cddbThread,(void *)this);
}

void KoverTop::preferences()
{
 //  KDialogBase *test = new KDialogBase(KJanusWidget::IconList,"config me",KDialogBase::Default,KDialogBase::Default,this);
  
//   //  QWidget *page = new QWidget( test ); 
//   QFrame *layout = test->addPage(i18n("Layout"));
//    test->addPage(i18n("Devices"));
//  QVBoxLayout *topLayout = new QVBoxLayout( layout, 0, 6 );
//  QLabel *label = new QLabel( i18n("Layout type"), layout );
//  topLayout->addWidget( label );
  //QVBoxLayout *topLayout = new QVBoxLayout( page, 0, test->spacingHint() );
 
//   QLabel *label = new QLabel( "config me", page, "caption" );
//   topLayout->addWidget( label );
 
//   QLineEdit *lineedit = new QLineEdit( "lalala", page, "lineedit" );
//   lineedit->setMinimumWidth(fontMetrics().maxWidth()*20);
//   topLayout->addWidget( lineedit );

//   topLayout->addStretch(10); 

  PreferencesDialog *test = new PreferencesDialog(this, "config me");

 test->adjustSize();
  int result = test->exec();
  printf ("lalala... %d\n",result);
  delete test;
  
}

void KoverTop::imageEmbedding()
{
  	ImageDlg* imgdlg;
	imgdlg = new ImageDlg(this,&kover_file);
	imgdlg->exec();
	delete imgdlg;
}

void KoverTop::updateDisplay(bool update_really = false)
{
  printf("updateDisplay()\n");
  printf("update_display = %d\n",update_display);  
  printf("end_loop = %d\n",end_loop);
  update_display = update_really;
  end_loop = true;
  printf("update_display = %d\n",update_display);
 printf("end_loop = %d\n",end_loop);
//   cddb_fill->setTitleAndContents();
//   disconnect( contents_edit, SIGNAL(textChanged()), this, SLOT(contentsBoxChanged()) );
//   title_edit->setText( kover_file.title() );
//   contents_edit->setText( kover_file.contents() );
//   connect( contents_edit, SIGNAL(textChanged()), SLOT(contentsBoxChanged()) );
//   altered_data = false;
//   cddb_fill->cdInfo();
  
}

void *cddbThread_old(void *parm)
{
  KoverTop *kover_top = (KoverTop *) parm;

#ifdef ENABLE_DEBUG_OUTPUT
	fprintf(stderr, "[cddbThread()] CDDB thread up, PID: %i\n", getpid());
#endif
  
  // if (kover_top->cddb_fill->execute())
//  		  {
//  			 kover_top->disconnect( kover_top->contents_edit, SIGNAL(textChanged()), kover_top, SLOT(contentsBoxChanged()) );
//  			 kover_top->title_edit->setText( kover_top->kover_file.title() );
// 			 kover_top->contents_edit->setText( kover_top->kover_file.contents() );
//  			 kover_top->connect( kover_top->contents_edit, SIGNAL(textChanged()), SLOT(contentsBoxChanged()) );
//  			 kover_top->altered_data = false;
//  			 kover_top->cddb_fill->cdInfo();
//  		  } 
	
	
	//	kover_top->cddb_fill->execute();

  
	return NULL;
}
