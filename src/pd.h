/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
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

/* This is the Preferences Dialog (pd) */

#ifndef PD_H
#define PD_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QTabWidget>

using namespace std;

class pd:public QDialog
{
      Q_OBJECT public:

	pd(QWidget *p, bool changed = false);
	~pd();

      private:

	void setup_cdrom();
	void set_cdrom();
	void apply_settings();
	void setup_cddb();
	void set_cddb();
	void setup_cddb_files();
	void save_cddb_files();
	void set_cddb_files();
	void setup_cover();
	void output_changed(int type);
	void set_cover();
	void save_cover();
	void setup_font();
	void font_dialog(QFont *);
	void setup_misc();
	void set_misc();
	void save_misc();
	int check();

	QFont content;
	QFont title;
	QFont inlet_title;

	struct cddb_widgets {
		QLineEdit *cddb_server;
		QLineEdit *cgi_path;
		QCheckBox *use_proxy;
		QCheckBox *proxy_from_env;
		QLineEdit *proxy_server;
		QLineEdit *proxy_port;
		QComboBox *cddb_protocol;
	} cddb_widgets;

	struct cdrom_widgets {
		QLineEdit *cdrom_device;
		QCheckBox *eject_cdrom;
	} cdrom_widgets;

	struct cddb_files_widgets {
		QCheckBox *use_cache;
		QLineEdit *cddb_path;
	} cddb_files_widgets;

	struct misc_widgets {
		QCheckBox *save_position;
		QCheckBox *disable_animation;
		QCheckBox *trigger_actual_size;
	} misc_widgets;

	struct cover_widgets {
		QCheckBox *display_track_duration;
		QRadioButton *its_a_slim_case;
		QRadioButton *inlet_only;
		QRadioButton *its_normal;
		QRadioButton *one_page;
		QLabel *inlet;
		QLabel *booklet;
	} cover_widgets;

	struct font_widgets {
		QPushButton *change_content_font;
		QPushButton *change_title_font;
		QPushButton *change_inlet_title_font;
	} font_widgets;

	bool changed;
	QTabWidget *tabWidget;

      private Q_SLOTS:

	void use_proxy(bool status);
	void use_proxy_env(bool status);
	void protocol_changed(int);
	void output_changed_1();
	void output_changed_2();
	void output_changed_3();
	void output_changed_4();
	void browsing();
	void content_font_dialog();
	void title_font_dialog();
	void inlet_title_font_dialog();
	void accept() override;
};

#endif /* PD_H */
