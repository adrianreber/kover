/**  Hey emacs! This is: -*- adrian-c -*-
	 kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
	 Copyright (C) 1998-2000 by Denis Oliver Kropp
	 Copyright (C) 2000-2003 by Adrian Reber 
	 
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
	 
	 File: imagedlg.cc
	 
	 Description: Image Dialog
	 
	 Changes: 
	 
	 14 Dec 1998: Initial release
	
*/

/* $Id: imagedlg.cc,v 1.2 2003/02/07 16:44:40 adrian Exp $ */

#include "imagedlg.moc"
#include "imagedlg.h"
#include <kfiledialog.h>

ImageDlg::ImageDlg(QWidget * parent, KoverFile * _kover_file)
:  QDialog(parent, "image_dlg", true)
{
    kover_file = _kover_file;
    initDialog();
    setCaption(tr("Image Embedding"));

    connect(OKButton, SIGNAL(clicked()), SLOT(OK()));
    connect(CancelButton, SIGNAL(clicked()), SLOT(Cancel()));

    connect(BrowseButton_1, SIGNAL(clicked()), SLOT(Browse1()));
    connect(BrowseButton_2, SIGNAL(clicked()), SLOT(Browse2()));
    connect(BrowseButton_3, SIGNAL(clicked()), SLOT(Browse3()));

    FileEdit_1->setText(kover_file->imageFile(0));
    FileEdit_2->setText(kover_file->imageFile(1));
    FileEdit_3->setText(kover_file->imageFile(2));

    AppearanceCombo_1->setCurrentItem(kover_file->imageMode(0));
    AppearanceCombo_2->setCurrentItem(kover_file->imageMode(1));
    AppearanceCombo_3->setCurrentItem(kover_file->imageMode(2));

    TargetCombo_1->setCurrentItem(kover_file->imageTarget(0));
    TargetCombo_2->setCurrentItem(kover_file->imageTarget(1));
    TargetCombo_3->setCurrentItem(kover_file->imageTarget(2));
}

ImageDlg::~ImageDlg()
{
}

void ImageDlg::OK()
{
    kover_file->setImageFile(0, FileEdit_1->text());
    kover_file->setImageFile(1, FileEdit_2->text());
    kover_file->setImageFile(2, FileEdit_3->text());

    kover_file->setImageMode(0, AppearanceCombo_1->currentItem());
    kover_file->setImageMode(1, AppearanceCombo_2->currentItem());
    kover_file->setImageMode(2, AppearanceCombo_3->currentItem());

    kover_file->setImageTarget(0, TargetCombo_1->currentItem());
    kover_file->setImageTarget(1, TargetCombo_2->currentItem());
    kover_file->setImageTarget(2, TargetCombo_3->currentItem());

    accept();
}

void ImageDlg::Cancel()
{
    reject();
}

void ImageDlg::Browse1()
{
    QString filename = KFileDialog::getOpenFileName();

    if (!filename.isEmpty()) {
        FileEdit_1->setText(filename);
    }
}

void ImageDlg::Browse2()
{
    QString filename = KFileDialog::getOpenFileName();

    if (!filename.isEmpty()) {
        FileEdit_2->setText(filename);
    }
}

void ImageDlg::Browse3()
{
    QString filename = KFileDialog::getOpenFileName();

    if (!filename.isEmpty()) {
        FileEdit_3->setText(filename);
    }
}

void ImageDlg::initDialog()
{
    this->resize(500, 440);
    ImageGroup_1 = new QGroupBox(this, "ImageGroup_1");
    ImageGroup_1->setGeometry(10, 10, 480, 120);
    ImageGroup_1->setTitle(tr("First Image"));

    FileEdit_1 = new QLineEdit(this, "FileEdit_1");
    FileEdit_1->setGeometry(80, 40, 280, 30);

    FileLabel_1 = new QLabel(this, "FileLabel_1");
    FileLabel_1->setGeometry(30, 40, 30, 30);
    FileLabel_1->setText(tr("File"));

    BrowseButton_1 = new QPushButton(this, "BrowseButton_1");
    BrowseButton_1->setGeometry(370, 40, 100, 30);
    BrowseButton_1->setText(tr("Browse"));

    TargetCombo_1 = new QComboBox(this, "NoName");
    TargetCombo_1->setGeometry(80, 80, 150, 30);
    TargetCombo_1->insertItem(tr("Front, left side"));
    TargetCombo_1->insertItem(tr("Front, right side"));
    TargetCombo_1->insertItem(tr("Front, both sides"));
    TargetCombo_1->insertItem(tr("Back, without sides"));
    TargetCombo_1->insertItem(tr("Back, with sides"));

    AppearanceCombo_1 = new QComboBox(this, "NoName");
    AppearanceCombo_1->setGeometry(320, 80, 150, 30);
    AppearanceCombo_1->insertItem(tr("Centered"));
    AppearanceCombo_1->insertItem(tr("Tiled"));
    AppearanceCombo_1->insertItem(tr("Stretched"));

    QLabel_2 = new QLabel(this, "NoName");
    QLabel_2->setGeometry(30, 80, 40, 30);
    QLabel_2->setText(tr("Target"));

    QLabel_3 = new QLabel(this, "NoName");
    QLabel_3->setGeometry(240, 80, 70, 30);
    QLabel_3->setText(tr("Appearance"));

    QGroupBox_2 = new QGroupBox(this, "NoName");
    QGroupBox_2->setGeometry(10, 140, 480, 120);
    QGroupBox_2->setTitle(tr("Second Image"));

    QLabel_4 = new QLabel(this, "NoName");
    QLabel_4->setGeometry(30, 170, 40, 30);
    QLabel_4->setText(tr("File"));

    FileEdit_2 = new QLineEdit(this, "NoName");
    FileEdit_2->setGeometry(80, 170, 280, 30);

    BrowseButton_2 = new QPushButton(this, "NoName");
    BrowseButton_2->setGeometry(370, 170, 100, 30);
    BrowseButton_2->setText(tr("Browse"));

    QLabel_5 = new QLabel(this, "NoName");
    QLabel_5->setGeometry(30, 210, 40, 30);
    QLabel_5->setText(tr("Target"));

    TargetCombo_2 = new QComboBox(this, "NoName");
    TargetCombo_2->setGeometry(80, 210, 150, 30);
    TargetCombo_2->insertItem(tr("Front, left side"));
    TargetCombo_2->insertItem(tr("Front, right side"));
    TargetCombo_2->insertItem(tr("Front, both sides"));
    TargetCombo_2->insertItem(tr("Back, without sides"));
    TargetCombo_2->insertItem(tr("Back, with sides"));

    AppearanceCombo_2 = new QComboBox(this, "NoName");
    AppearanceCombo_2->setGeometry(320, 210, 150, 30);
    AppearanceCombo_2->insertItem(tr("Centered"));
    AppearanceCombo_2->insertItem(tr("Tiled"));
    AppearanceCombo_2->insertItem(tr("Stretched"));

    QLabel_6 = new QLabel(this, "NoName");
    QLabel_6->setGeometry(240, 210, 70, 30);
    QLabel_6->setText(tr("Appearance"));

    QGroupBox_3 = new QGroupBox(this, "NoName");
    QGroupBox_3->setGeometry(10, 270, 480, 120);
    QGroupBox_3->setTitle(tr("Third Image"));

    QLabel_7 = new QLabel(this, "NoName");
    QLabel_7->setGeometry(30, 300, 40, 30);
    QLabel_7->setText(tr("File"));

    FileEdit_3 = new QLineEdit(this, "NoName");
    FileEdit_3->setGeometry(80, 300, 280, 30);

    BrowseButton_3 = new QPushButton(this, "NoName");
    BrowseButton_3->setGeometry(370, 300, 100, 30);
    BrowseButton_3->setText(tr("Browse"));

    QLabel_8 = new QLabel(this, "NoName");
    QLabel_8->setGeometry(30, 340, 40, 30);
    QLabel_8->setText(tr("Target"));

    TargetCombo_3 = new QComboBox(this, "NoName");
    TargetCombo_3->setGeometry(80, 340, 150, 30);
    TargetCombo_3->insertItem(tr("Front, left side"));
    TargetCombo_3->insertItem(tr("Front, right side"));
    TargetCombo_3->insertItem(tr("Front, both sides"));
    TargetCombo_3->insertItem(tr("Back, without sides"));
    TargetCombo_3->insertItem(tr("Back, with sides"));

    AppearanceCombo_3 = new QComboBox(this, "NoName");
    AppearanceCombo_3->setGeometry(320, 340, 150, 30);
    AppearanceCombo_3->insertItem(tr("Centered"));
    AppearanceCombo_3->insertItem(tr("Tiled"));
    AppearanceCombo_3->insertItem(tr("Stretched"));

    QLabel_9 = new QLabel(this, "NoName");
    QLabel_9->setGeometry(240, 340, 70, 30);
    QLabel_9->setText(tr("Appearance"));

    OKButton = new QPushButton(this, "NoName");
    OKButton->setGeometry(140, 400, 100, 30);
    OKButton->setText(tr("OK"));
    OKButton->setDefault(true);

    CancelButton = new QPushButton(this, "NoName");
    CancelButton->setGeometry(260, 400, 100, 30);
    CancelButton->setText(tr("Cancel"));
}
