/***************************************************************************
                          FILENAME  -  description                              
                             -------------------                                         

    version              :                                   
    begin                : DATE                                           
    copyright            : (C) YEAR by AUTHOR                         
    email                : EMAIL                                     
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
#include "imagedlg.h"
#include <kfiledialog.h>

ImageDlg::ImageDlg(QWidget *parent, KoverFile* _kover_file) 
  : QDialog(parent,"image_dlg",true) {
		  kover_file = _kover_file;

		  initDialog();
		  setCaption(tr("Image Embedding"));
	
		  connect( OKButton, SIGNAL(clicked()), SLOT(OK()) );
		  connect( CancelButton, SIGNAL(clicked()), SLOT(Cancel()) );
	
		  connect( BrowseButton_1, SIGNAL(clicked()), SLOT(Browse1()) );
		  connect( BrowseButton_2, SIGNAL(clicked()), SLOT(Browse2()) );
		  connect( BrowseButton_3, SIGNAL(clicked()), SLOT(Browse3()) );
	
		  FileEdit_1->setText( kover_file->imageFile(0) );
		  FileEdit_2->setText( kover_file->imageFile(1) );
		  FileEdit_3->setText( kover_file->imageFile(2) );
	
		  AppearanceCombo_1->setCurrentItem( kover_file->imageMode(0) );
		  AppearanceCombo_2->setCurrentItem( kover_file->imageMode(1) );
		  AppearanceCombo_3->setCurrentItem( kover_file->imageMode(2) );

		  TargetCombo_1->setCurrentItem( kover_file->imageTarget(0) );
		  TargetCombo_2->setCurrentItem( kover_file->imageTarget(1) );
		  TargetCombo_3->setCurrentItem( kover_file->imageTarget(2) );
}

ImageDlg::~ImageDlg() {
}


void ImageDlg::OK() {
		  kover_file->setImageFile( 0, FileEdit_1->text() );
		  kover_file->setImageFile( 1, FileEdit_2->text() );
		  kover_file->setImageFile( 2, FileEdit_3->text() );
	
		  kover_file->setImageMode( 0, AppearanceCombo_1->currentItem() );
		  kover_file->setImageMode( 1, AppearanceCombo_2->currentItem() );
		  kover_file->setImageMode( 2, AppearanceCombo_3->currentItem() );
	
		  kover_file->setImageTarget( 0, TargetCombo_1->currentItem() );
		  kover_file->setImageTarget( 1, TargetCombo_2->currentItem() );
		  kover_file->setImageTarget( 2, TargetCombo_3->currentItem() );
	
		  accept();
}

void ImageDlg::Cancel()
{
		  reject();
}

void ImageDlg::Browse1()
{
		  QString filename = KFileDialog::getOpenFileName();
		  if (!filename.isEmpty())
		  {
					 FileEdit_1->setText( filename );
		  }
}

void ImageDlg::Browse2()
{
		  QString filename = KFileDialog::getOpenFileName();
		  if (!filename.isEmpty())
		  {
					 FileEdit_2->setText( filename );
		  }
}

void ImageDlg::Browse3()
{
		  QString filename = KFileDialog::getOpenFileName();
		  if (!filename.isEmpty())
		  {
					 FileEdit_3->setText( filename );
		  }
}


#include "imagedlg.moc"
