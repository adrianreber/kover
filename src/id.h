/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 1998, 2000 by Denis Oliver Kropp
 * Copyright (C) 2000, 2025 by Adrian Reber
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

#ifndef ID_H
#define ID_H

#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QVBoxLayout>

#include "koverfile.h"

class id : public QDialog {
	Q_OBJECT public:
	id (KoverFile *_kover_file);
	~id ();

 protected Q_SLOTS:
	void ok();
	void cancel();
	void browse_1();
	void browse_2();
	void browse_3();

 private:
	void init();
	void add_labels(QGridLayout *);
	void buttons(QVBoxLayout *);
	void add_targets(QComboBox *);
	void add_appear(QComboBox *);

	QGroupBox *group1;
	QLineEdit *edit1;
	QPushButton *browse1;
	QComboBox *target1;
	QComboBox *appear1;

	QGroupBox *group2;
	QLineEdit *edit2;
	QPushButton *browse2;
	QComboBox *target2;
	QComboBox *appear2;

	QGroupBox *group3;
	QLineEdit *edit3;
	QPushButton *browse3;
	QComboBox *target3;
	QComboBox *appear3;

	KoverFile *kover_file;
};

#endif /* ID_H */
