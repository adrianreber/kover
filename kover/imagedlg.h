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

#ifndef IMAGEDLG_H
#define IMAGEDLG_H

//Generated area. DO NOT EDIT!!!(begin)
#include <qwidget.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
//Generated area. DO NOT EDIT!!!(end)

#include <qdialog.h>

#include "koverfile.h"

/**
  *@author Denis Oliver Kropp
  */

class ImageDlg : public QDialog  {
   Q_OBJECT
public: 
	ImageDlg(QWidget* parent, KoverFile* _kover_file);
	~ImageDlg();

protected slots:
void OK();
		  void Cancel();
		  void Browse1();
		  void Browse2();
		  void Browse3();

protected: 
	void initDialog();
	//Generated area. DO NOT EDIT!!!(begin)
	QGroupBox *ImageGroup_1;
	QLineEdit *FileEdit_1;
	QLabel *FileLabel_1;
	QPushButton *BrowseButton_1;
	QComboBox *TargetCombo_1;
	QComboBox *AppearanceCombo_1;
	QLabel *QLabel_2;
	QLabel *QLabel_3;
	QGroupBox *QGroupBox_2;
	QLabel *QLabel_4;
	QLineEdit *FileEdit_2;
	QPushButton *BrowseButton_2;
	QLabel *QLabel_5;
	QComboBox *TargetCombo_2;
	QComboBox *AppearanceCombo_2;
	QLabel *QLabel_6;
	QGroupBox *QGroupBox_3;
	QLabel *QLabel_7;
	QLineEdit *FileEdit_3;
	QPushButton *BrowseButton_3;
	QLabel *QLabel_8;
	QComboBox *TargetCombo_3;
	QComboBox *AppearanceCombo_3;
	QLabel *QLabel_9;
	QPushButton *OKButton;
	QPushButton *CancelButton;
	//Generated area. DO NOT EDIT!!!(end)

private:
	KoverFile*	kover_file;
};

#endif


