/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 1998, 2000 by Denis Oliver Kropp
 * Copyright (C) 2000, 2008 by Adrian Reber
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * 14 Dec 1998: Initial release
 */

#ifndef IMAGEDLG_H
#define IMAGEDLG_H

#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QDialog>

#include "koverfile.h"

class ImageDlg : public QDialog {
	Q_OBJECT public:
	ImageDlg (KoverFile *_kover_file);
	~ImageDlg ();

 protected slots:
	void OK();
	void Cancel();
	void Browse1();
	void Browse2();
	void Browse3();

 private:
	void initDialog();
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
	KoverFile *kover_file;
};

#endif /* IMAGEDLG_H */
