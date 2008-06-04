/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 2001, 2008 by Adrian Reber
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
 * Description: dialog to execute a cddb query without a cd
 *
 * 11 Nov 2001: Initial release
 */

#ifndef WITHOUT_CD_H
#define WITHOUT_CD_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>

class without_cd : public QDialog {
	Q_OBJECT
 public:
	without_cd();
	~without_cd();
	int get_category();
	char *get_id();

 private:
	QLineEdit *cddb_id;
	QComboBox *category;
	void handle_input();

 private slots:
	void accept();
	void quit();

};

#endif /* WITHOUT_CD_H */
