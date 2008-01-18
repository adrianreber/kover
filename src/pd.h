/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
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
 */

/* This is the Preferences Dialog (pd) */

#ifndef PD_H
#define PD_H

#include <kconfigdialog.h>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>

using namespace std;

class pd:public KConfigDialog
{
      Q_OBJECT public:
	  pd(QWidget *p, KConfigSkeleton *cs, bool changed = false);
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
#if 0
	void setupCDDBPage();
	void setup_cddb_files_page();
	void setup_misc_page();
	void setup_font_page();
	void setup_cover_page();

	void apply_settings();

	void set_cddb();
	void set_cdrom();
	void set_cddb_files();
	void set_misc();
	void set_cover();

	void save_cddb_files();
	void save_misc();
	void save_cover();
	void show_font_warning();
#endif
	struct cddb_widgets
	{
		QLineEdit *cddb_server;
		QLineEdit *cgi_path;
		QCheckBox *use_proxy;
		QCheckBox *proxy_from_env;
		QLineEdit *proxy_server;
		QLineEdit *proxy_port;
		QComboBox *cddb_protocol;
	} cddb_widgets;

	enum Page
	{
		page_cddb = 0,
		page_cdrom,
		page_cddb_files,
		page_cover,
		page_font,
		page_misc,
		page_max
	};

	enum output
	{
		normal = 0,
		slim_case,
		inlet_only,
		one_page
	};

	struct cdrom_widgets
	{
		QLineEdit *cdrom_device;
		QCheckBox *eject_cdrom;
	} cdrom_widgets;

	struct cddb_files_widgets
	{
		QCheckBox *use_cache;
		QLineEdit *cddb_path;
	} cddb_files_widgets;

	struct misc_widgets
	{
		QCheckBox *save_position;
		QCheckBox *disable_animation;
		QCheckBox *trigger_actual_size;
	} misc_widgets;

	struct cover_widgets
	{
		QCheckBox *display_track_duration;
		QRadioButton *its_a_slim_case;
		QRadioButton *inlet_only;
		QRadioButton *its_normal;
		QRadioButton *one_page;
		QLabel *inlet;
		QLabel *booklet;
	} cover_widgets;

	//struct font_widgets {
	//QButton *change_content_font;
	//QButton *change_title_font;
	//QButton *change_inlet_title_font;
	//} font_widgets;

	bool changed;
#if 0
	private slots:void use_proxy(bool status);
	void use_proxy_env(bool status);
	virtual void slotOk(void);
	virtual void slotDefault();
	void content_font_dialog();
	void title_font_dialog();
	void inlet_title_font_dialog();
	void output_changed(int type);
	void browsing();
#endif

      private slots:

	void use_proxy(bool status);
	void use_proxy_env(bool status);
	void protocol_changed(int);

      protected Q_SLOTS:

	virtual void slotButtonClicked(int button);
};

#endif /* PD_H */
