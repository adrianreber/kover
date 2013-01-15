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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * 14 Dec 1998: Initial release
 */

/* this is the image dialog (id) */

#include "id.moc"
#include "id.h"
#include <kfiledialog.h>
#include <QLabel>

id::id(KoverFile *_kover_file)
	:  QDialog()
{
	kover_file = _kover_file;

	init();

	connect(browse1, SIGNAL(clicked()), SLOT(browse_1()));
	connect(browse2, SIGNAL(clicked()), SLOT(browse_2()));
	connect(browse3, SIGNAL(clicked()), SLOT(browse_3()));

	edit1->setText(kover_file->imageFile(0));
	edit2->setText(kover_file->imageFile(1));
	edit3->setText(kover_file->imageFile(2));

	appear1->setCurrentIndex(kover_file->imageMode(0));
	appear2->setCurrentIndex(kover_file->imageMode(1));
	appear3->setCurrentIndex(kover_file->imageMode(2));

	target1->setCurrentIndex(kover_file->imageTarget(0));
	target2->setCurrentIndex(kover_file->imageTarget(1));
	target3->setCurrentIndex(kover_file->imageTarget(2));
}

id::~id()
{
}

void
id::ok()
{
	kover_file->setImageFile(0, edit1->text());
	kover_file->setImageFile(1, edit2->text());
	kover_file->setImageFile(2, edit3->text());

	kover_file->setImageMode(0, appear1->currentIndex());
	kover_file->setImageMode(1, appear2->currentIndex());
	kover_file->setImageMode(2, appear3->currentIndex());

	kover_file->setImageTarget(0, target1->currentIndex());
	kover_file->setImageTarget(1, target2->currentIndex());
	kover_file->setImageTarget(2, target3->currentIndex());

	accept();
}

void
id::cancel()
{
	reject();
}

void
id::browse_1()
{
	QString filename = KFileDialog::getOpenFileName();

	if (!filename.isEmpty())
		edit1->setText(filename);
}

void
id::browse_2()
{
	QString filename = KFileDialog::getOpenFileName();

	if (!filename.isEmpty())
		edit2->setText(filename);
}

void
id::browse_3()
{
	QString filename = KFileDialog::getOpenFileName();

	if (!filename.isEmpty())
		edit3->setText(filename);
}

void
id::add_targets(QComboBox *box)
{
	box->insertItem(IMG_FRONT_LEFT, tr("Front, left side"));
	box->insertItem(IMG_FRONT_RIGHT, tr("Front, right side"));
	box->insertItem(IMG_FRONT_FULL, tr("Front, both sides"));
	box->insertItem(IMG_BACK_INNER, tr("Back, without sides"));
	box->insertItem(IMG_BACK_FULL, tr("Back, with sides"));
}

void
id::add_appear(QComboBox *box)
{
	box->insertItem(IMG_CENTER, tr("Centered"));
	box->insertItem(IMG_TILE, tr("Tiled"));
	box->insertItem(IMG_STRETCH, tr("Stretched"));
}

void
id::init()
{
	QVBoxLayout *top_layout = new QVBoxLayout(this);

	top_layout->setMargin(12);
	top_layout->addSpacing(5);

	group1 = new QGroupBox(this);
	group1->setTitle(tr("First Image"));
	top_layout->addWidget(group1);
	QGridLayout *gbox = new QGridLayout(group1);

	add_labels(gbox);

	edit1 = new QLineEdit(group1);
	gbox->addWidget(edit1, 0, 1, 1, 3);

	browse1 = new QPushButton(group1);
	browse1->setText(tr("Browse"));
	gbox->addWidget(browse1, 0, 5, 1, 1);

	target1 = new QComboBox(group1);
	add_targets(target1);
	gbox->addWidget(target1, 1, 1, 1, 2);

	appear1 = new QComboBox(group1);
	add_appear(appear1);
	gbox->addWidget(appear1, 1, 4, 1, 2);


	top_layout->addSpacing(15);
	group2 = new QGroupBox(this);
	group2->setTitle(tr("Second Image"));
	top_layout->addWidget(group2);
	gbox = new QGridLayout(group2);

	add_labels(gbox);

	edit2 = new QLineEdit(group2);
	gbox->addWidget(edit2, 0, 1, 1, 3);

	browse2 = new QPushButton(group2);
	browse2->setText(tr("Browse"));
	gbox->addWidget(browse2, 0, 5, 1, 1);

	target2 = new QComboBox(group2);
	add_targets(target2);
	gbox->addWidget(target2, 1, 1, 1, 2);

	appear2 = new QComboBox(group2);
	add_appear(appear2);
	gbox->addWidget(appear2, 1, 4, 1, 2);

	top_layout->addSpacing(15);
	group3 = new QGroupBox(this);
	group3->setTitle(tr("Third Image"));
	top_layout->addWidget(group3);
	gbox = new QGridLayout(group3);

	add_labels(gbox);

	edit3 = new QLineEdit(group3);
	gbox->addWidget(edit3, 0, 1, 1, 3);

	browse3 = new QPushButton(group3);
	browse3->setText(tr("Browse"));
	gbox->addWidget(browse3, 0, 5, 1, 1);

	target3 = new QComboBox(group3);
	add_targets(target3);
	gbox->addWidget(target3, 1, 1, 1, 2);

	appear3 = new QComboBox(group3);
	add_appear(appear3);
	gbox->addWidget(appear3, 1, 4, 1, 2);
	top_layout->addSpacing(15);

	buttons(top_layout);
}

void
id::buttons(QVBoxLayout *top)
{
	QBoxLayout *button_layout = new QBoxLayout(QBoxLayout::RightToLeft);

	top->addLayout(button_layout);

	QPushButton *ok = new QPushButton(this);
	ok->setText(tr("OK"));
	ok->setDefault(true);
	button_layout->addWidget(ok, 0, Qt::AlignRight);

	QPushButton *cancel = new QPushButton(this);
	cancel->setText(tr("Cancel"));
	button_layout->addWidget(cancel, 0, Qt::AlignRight);

	connect(ok, SIGNAL(clicked()), SLOT(ok()));
	connect(cancel, SIGNAL(clicked()), SLOT(cancel()));
}

void
id::add_labels(QGridLayout *gbox)
{
	QLabel *label = new QLabel(this);

	label->setText(tr("File"));
	gbox->addWidget(label, 0, 0, 1, 1);

	label = new QLabel(this);
	label->setText(tr("Target"));
	gbox->addWidget(label, 1, 0, 1, 1);

	label = new QLabel(this);
	label->setText(tr("Appearance"));
	gbox->addWidget(label, 1, 3, 1, 1);
}
