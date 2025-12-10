/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 1999, 2000 by Denis Oliver Kropp
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
 */

#ifndef KOVER_TOP_H
#define KOVER_TOP_H

#include <kover.h>
#include "cdview.h"
#include "koverfile.h"
#include "cddb_fill.h"
#include "no_qobject.h"
#include "pd.h"

#include <QObject>
#include <QTextEdit>
#include <QGroupBox>
#include <QBoxLayout>
#include <QSpinBox>
#include <QUrl>
#include <QStatusBar>
#include <QCheckBox>
#include <QLineEdit>
#include <QFrame>
#include <QPushButton>
#include <QLabel>

#include <KRecentFilesAction>
#include <KXmlGuiWindow>

/** KoverTop is the TopLevelWidget. */

class KoverTop : public KXmlGuiWindow, no_qobject {
	Q_OBJECT
 public:
	KoverTop();
	~KoverTop();

 public Q_SLOTS:
	void contentsBoxChanged();
	void titleBoxChanged();
	void stopPreview();
	void numberChecked(bool checked);
	void numberChanged(int number);
	void dataChanged(bool image);
	void set_status_text(const QString &) override;
	void setStatusText(const QString &_status_text);
	void actualSize();
	void update_id(unsigned long = 0) override;
	void fileOpen(const QUrl &);

 private:
	void saveFile(const QUrl &);

 private Q_SLOTS:
	void fileNew();
	void fileOpen();
	void fileSave();
	void fileSaveAs();
	void filePrint();
	void cut();

	void copy();
	void paste();
	void cddbFill();
	void preferences();
	void imageEmbedding();
	void titleFont();
	void titleFontColor();
	void contentsFont();
	void contentsFontColor();
	void backgroundColor();
	void cdrom_eject();
	void inlet_title_font();
	void cddb_without_cd();
	void more_or_less();
	void display_title_signal();
	void spine_text_method();
	void spine_text_changed_method(const QString & s);
	void file_mode();
	void config_keys();
	void read_cd_text();

 private:

	bool queryClose() override;
	int how_about_saving();

	void make_menu();
	void make_main_frame();
	void make_more_frame();
	void make_option_frame();
	void update_id(QString id);

	QGroupBox *option_frame;
	QFrame *dead_space;
	QCheckBox *display_title;
	QLineEdit *the_spine_text;
	QCheckBox *spine_text;

	KRecentFilesAction *recent;
	QCheckBox *slim_case;

	QWidget *centralWidget;
	QWidget *main_frame;
	QWidget *left_frame;

	QFont *font_dialog(QFont *f);

	KoverFile kover_file;
	cddb_fill *cddbfill;

	QUrl m_url;
	bool altered_data;

	QLabel *title_label;
	QTextEdit *title_edit;

	QLabel *contents_label;
	QTextEdit *contents_edit;

	QCheckBox *number_check;
	QSpinBox *number_spin;

	QStatusBar *status_bar;

	QLabel *cddb_id;
	CDView *cdview;

	QBoxLayout *button_layout;
	QFrame *more_frame;
	QPushButton *more_button;
	bool more;
	int orig_width;
	int orig_height;
	bool actual;
};

#endif                          /* KOVER_TOP_H */
