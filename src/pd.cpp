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

#include "pd.moc"
#include "pd.h"
#include "sd.h"
#include <globals.h>
#include <QGroupBox>
#include <QVBoxLayout>
#include <kover_old.h>
#include <klocalizedstring.h>
#include <kfontdialog.h>
#include <kmessagebox.h>

pd::pd(QWidget *p, KConfigSkeleton *cs, bool changed):
KConfigDialog(p, "configure", cs)
{
	setButtons(Ok | Cancel | Help);
	setup_cddb();
	setup_cdrom();
	setup_cddb_files();
	setup_cover();
	setup_font();
	//setup_misc_page();
	this->changed = changed;
	content = *globals.content_font;
	title = *globals.title_font;
	inlet_title = *globals.inlet_title_font;
}

pd::~pd()
{
}

void
pd::setup_cdrom(void)
{
	QWidget *page = new QWidget;
	QVBoxLayout *topLayout = new QVBoxLayout(page);

	QGroupBox *group = new QGroupBox(i18n("&CDROM"), page);

	topLayout->addWidget(group);
	QVBoxLayout *vlay = new QVBoxLayout(group);

	vlay->addSpacing(fontMetrics().lineSpacing());
	QGridLayout *gbox = new QGridLayout();
	gbox->setMargin(2);
	gbox->setSpacing(4);

	vlay->addLayout(gbox);

	QString text;
	QLabel *label = new QLabel(i18n("CDROM device:"), group);

	gbox->addWidget(label, 0, 0, 1, 1);
	cdrom_widgets.cdrom_device = new QLineEdit(group);
	cdrom_widgets.cdrom_device->setMinimumWidth(fontMetrics().maxWidth() * 10);
	gbox->addWidget(cdrom_widgets.cdrom_device, 0, 1, 1, 1);

	text = i18n("Eject CD on exit");
	cdrom_widgets.eject_cdrom = new QCheckBox(text, group);
	gbox->addWidget(cdrom_widgets.eject_cdrom, 1, 0, 1, -1);

	set_cdrom();

	topLayout->addStretch(10);
	addPage(page, i18n("CDROM"));
}

void
pd::set_cdrom()
{
	cdrom_widgets.cdrom_device->setText(globals.cdrom_device);
	if (globals.eject_cdrom)
		cdrom_widgets.eject_cdrom->setChecked(true);
	else
		cdrom_widgets.eject_cdrom->setChecked(false);
}


void
pd::slotButtonClicked(int button)
{
	if (button == KDialog::Ok) {
		kprintf("OK\n");
		apply_settings();
		accept();
	} else
		KDialog::slotButtonClicked(button);
}


void
pd::apply_settings()
{
	if (!((cddb_widgets.proxy_port)->text()).isEmpty()) {
		globals.proxy_port = ((cddb_widgets.proxy_port)->text()).toInt();
	}

	if (!((cddb_widgets.cddb_server)->text()).isEmpty()) {
		kprintf("old value :%s\n", globals.cddb_server);
		kprintf("new value: %s\n", ((cddb_widgets.cddb_server)->text()).toUtf8().constData());
		free(globals.cddb_server);
		globals.cddb_server = strdup(((cddb_widgets.cddb_server)->text()).toUtf8());
	}

	if (!((cddb_widgets.cgi_path)->text()).isEmpty()) {
		kprintf("old value :%s\n", globals.cgi_path);
		kprintf("new value: %s\n", ((cddb_widgets.cgi_path)->text()).toUtf8().constData());
		free(globals.cgi_path);
		globals.cgi_path = strdup(((cddb_widgets.cgi_path)->text()).toUtf8());
	}

	if (!((cddb_widgets.proxy_server)->text()).isEmpty()) {
		if (globals.proxy_server)
			free(globals.proxy_server);
		globals.proxy_server = strdup(((cddb_widgets.proxy_server)->text()).toUtf8());
	} else {
		if (globals.proxy_server)
			free(globals.proxy_server);
		globals.proxy_server = NULL;
	}

	globals.use_cddbp = cddb_widgets.cddb_protocol->currentIndex();
	globals.use_proxy = ((cddb_widgets.use_proxy)->isChecked())? 1 : 0;
	globals.proxy_from_env = ((cddb_widgets.proxy_from_env)->isChecked())? 1 : 0;

	globals.eject_cdrom = ((cdrom_widgets.eject_cdrom)->isChecked())? 1 : 0;

	if (!((cdrom_widgets.cdrom_device)->text()).isEmpty()) {
		if (globals.cdrom_device)
			free(globals.cdrom_device);
		globals.cdrom_device = strdup(((cdrom_widgets.cdrom_device)->text()).toUtf8());
	} else {
		if (globals.cdrom_device)
			free(globals.cdrom_device);
		globals.cdrom_device = NULL;
	}
	*globals.content_font = content;
	*globals.title_font = title;
	*globals.inlet_title_font = inlet_title;

	save_cddb_files();
	save_cover();
}

void
pd::setup_cddb(void)
{
	QWidget *page = new QWidget;
	QVBoxLayout *topLayout = new QVBoxLayout(page);

	QGroupBox *group = new QGroupBox(i18n("&CDDB server"));

	topLayout->addWidget(group);
	QVBoxLayout *vlay = new QVBoxLayout(group);

	vlay->addSpacing(fontMetrics().lineSpacing());

	QGridLayout *gbox = new QGridLayout();
	gbox->setMargin(2);
	gbox->setSpacing(4);

	vlay->addLayout(gbox);

	QString text;
	QLabel *label = new QLabel(i18n("CDDB server:"), group);

	gbox->addWidget(label, 0, 0);

	cddb_widgets.cddb_protocol = new QComboBox(group);
	cddb_widgets.cddb_protocol->insertItem(0, "CDDBP");
	cddb_widgets.cddb_protocol->insertItem(0, "HTTP");
	connect(cddb_widgets.cddb_protocol, SIGNAL(activated(int)), SLOT(protocol_changed(int)));
	gbox->addWidget(cddb_widgets.cddb_protocol, 0, 1);

	cddb_widgets.cddb_server = new QLineEdit(group);
	cddb_widgets.cddb_server->setMinimumWidth(fontMetrics().maxWidth() * 10);
	gbox->addWidget(cddb_widgets.cddb_server, 0, 2);

	QPushButton *browse = new QPushButton(i18n("Browse"), group);

	connect(browse, SIGNAL(clicked()), SLOT(browsing()));
	gbox->addWidget(browse, 0, 3);

	label = new QLabel(i18n("CGI path:"), group);
	gbox->addWidget(label, 1, 0);
	cddb_widgets.cgi_path = new QLineEdit(group);
	cddb_widgets.cgi_path->setMinimumWidth(fontMetrics().maxWidth() * 10);
	gbox->addWidget(cddb_widgets.cgi_path, 1, 2);

	group = new QGroupBox(i18n("&Proxy configuration"));
	topLayout->addWidget(group);
	vlay = new QVBoxLayout(group);
	vlay->addSpacing(fontMetrics().lineSpacing());

	gbox = new QGridLayout();
	vlay->addLayout(gbox);
	text = i18n("Use proxy for CDDB lookups");
	cddb_widgets.use_proxy = new QCheckBox(text, group);
	connect(cddb_widgets.use_proxy, SIGNAL(toggled(bool)), this, SLOT(use_proxy(bool)));
	gbox->addWidget(cddb_widgets.use_proxy, 0, 0, 1, 2);
	text = i18n("Use 'http_proxy' environment variable");
	cddb_widgets.proxy_from_env = new QCheckBox(text, group);
	connect(cddb_widgets.proxy_from_env, SIGNAL(toggled(bool)), this, SLOT(use_proxy_env(bool)));
	gbox->addWidget(cddb_widgets.proxy_from_env, 3, 0, 1, 2);

	label = new QLabel(i18n("Proxy server:"), group);
	gbox->addWidget(label, 1, 0);
	cddb_widgets.proxy_server = new QLineEdit(group);
	cddb_widgets.proxy_server->setMinimumWidth(fontMetrics().maxWidth() * 10);
	gbox->addWidget(cddb_widgets.proxy_server, 1, 1, 1, 1);

	label = new QLabel(i18n("Proxy server port:"), group);
	gbox->addWidget(label, 2, 0);
	cddb_widgets.proxy_port = new QLineEdit(group);
	cddb_widgets.proxy_port->setMaxLength(5);
	gbox->addWidget(cddb_widgets.proxy_port, 2, 1, 1, 1);
	set_cddb();
	topLayout->addStretch(10);
	addPage(page, i18n("CDDB"));
}

void
pd::set_cddb()
{
	QString text;

	cddb_widgets.cddb_server->setText(globals.cddb_server);
	cddb_widgets.cgi_path->setText(globals.cgi_path);

	if (globals.proxy_server)
		cddb_widgets.proxy_server->setText(globals.proxy_server);

	cddb_widgets.proxy_port->setText(text.sprintf("%d", globals.proxy_port));

	cddb_widgets.cddb_protocol->setCurrentIndex(globals.use_cddbp ? 1 : 0);
	cddb_widgets.proxy_from_env->setChecked(globals.proxy_from_env);
	cddb_widgets.use_proxy->setChecked(globals.use_proxy);

	protocol_changed(globals.use_cddbp ? 1 : 0);
}

/**
 * enable/disable the use proxy and proxy information input fields
 *
 * @param status  true if proxy input fields should be enabled
 */
void
pd::use_proxy(bool status)
{
	if ((cddb_widgets.proxy_from_env)->isChecked() && status)
		use_proxy_env(true);
	else
		use_proxy_env(!status);

	cddb_widgets.proxy_from_env->setEnabled(status);
}

/**
 * if status is true the proxy server and proxy port input
 * text area are disabled
 *
 * @param status  true if proxy should be read from environment variable
 *                this will disable the input text area for proxy information
 */
void
pd::use_proxy_env(bool status)
{
	cddb_widgets.proxy_server->setEnabled(!status);
	cddb_widgets.proxy_port->setEnabled(!status);
}

/**
 * protocol_changed is called everytime the user selects between the
 * protocols (HTTP/CDDBP)
 *
 * @param prot  0 = CDDBP
 *              1 = HTTP
 */
void
pd::protocol_changed(int prot)
{
	bool http = prot ? false : true;

	if (!cddb_widgets.use_proxy->isChecked() && http) {
		use_proxy_env(true);
		cddb_widgets.proxy_from_env->setEnabled(false);
	} else
		use_proxy(http);

	cddb_widgets.cgi_path->setEnabled(http);
	cddb_widgets.use_proxy->setEnabled(http);
}

void
pd::setup_cddb_files(void)
{
	QWidget *page = new QWidget;
	QVBoxLayout *topLayout = new QVBoxLayout(page);

	QGroupBox *group = new QGroupBox(i18n("&Local CDDB files"), page);

	topLayout->addWidget(group);
	QVBoxLayout *vlay = new QVBoxLayout(group);

	vlay->addSpacing(fontMetrics().lineSpacing());
	QGridLayout *gbox = new QGridLayout();

	vlay->addLayout(gbox);

	QString text;

	text = i18n("Enable caching of CDDB entries locally");
	cddb_files_widgets.use_cache = new QCheckBox(text, group);
	gbox->addWidget(cddb_files_widgets.use_cache, 0, 0, 1, -1);

	QLabel *label = new QLabel(i18n("CDDB path:"), group);

	gbox->addWidget(label, 1, 0);
	cddb_files_widgets.cddb_path = new QLineEdit(group);
	cddb_files_widgets.cddb_path->setMinimumWidth(fontMetrics().maxWidth() * 10);
	gbox->addWidget(cddb_files_widgets.cddb_path, 1, 1, 1, 1);

	set_cddb_files();
	topLayout->addStretch(10);
	addPage(page, i18n("CDDB files"));
}

void
pd::set_cddb_files()
{
	cddb_files_widgets.use_cache->setChecked(globals.use_cache);
	cddb_files_widgets.cddb_path->setText(globals.cddb_path);
}

void
pd::save_cddb_files()
{
	globals.use_cache = ((cddb_files_widgets.use_cache)->isChecked())? 1 : 0;

	if (globals.cddb_path)
		free(globals.cddb_path);
	if (!((cddb_files_widgets.cddb_path)->text()).isEmpty())
		globals.cddb_path = strdup(((cddb_files_widgets.cddb_path)->text()).toUtf8());
	else
		globals.cddb_path = NULL;
}

void
pd::setup_cover()
{
	QWidget *page = new QWidget;
	QVBoxLayout *topLayout = new QVBoxLayout(page);

	QGroupBox *group = new QGroupBox(i18n("Cover"), page);

	topLayout->addWidget(group);
	QVBoxLayout *vlay = new QVBoxLayout(group);

	vlay->addSpacing(fontMetrics().lineSpacing());
	QGridLayout *gbox = new QGridLayout();

	vlay->addLayout(gbox);

	QString text;

	text = i18n("Display track duration after a CDDB request");
	cover_widgets.display_track_duration = new QCheckBox(text, group);
	gbox->addWidget(cover_widgets.display_track_duration, 0, 0, 1, -1);

	text = i18n("Print inlet and booklet");
	cover_widgets.its_normal = new QRadioButton(text, group);
	gbox->addWidget(cover_widgets.its_normal, 1, 0);
	connect(cover_widgets.its_normal, SIGNAL(clicked(bool)), SLOT(output_changed_1()));

	text = i18n("Print inlet on left side of booklet.\n(slim case option)");
	cover_widgets.its_a_slim_case = new QRadioButton(text, group);
	gbox->addWidget(cover_widgets.its_a_slim_case, 2, 0, 1, -1);
	connect(cover_widgets.its_a_slim_case, SIGNAL(clicked(bool)), SLOT(output_changed_2()));

	text = i18n("Don't print booklet.\n(inlet only option)");
	cover_widgets.inlet_only = new QRadioButton(text, group);
	gbox->addWidget(cover_widgets.inlet_only, 3, 0, 1, -1);
	connect(cover_widgets.inlet_only, SIGNAL(clicked(bool)), SLOT(output_changed_3()));

	text = i18n("Print all on one page");
	cover_widgets.one_page = new QRadioButton(text, group);
	gbox->addWidget(cover_widgets.one_page, 4, 0, 1, -1);
	connect(cover_widgets.one_page, SIGNAL(clicked(bool)), SLOT(output_changed_4()));

	cover_widgets.inlet = new QLabel(group);
	KIconLoader *pixmap = KIconLoader::global ();

	cover_widgets.inlet->setPixmap(pixmap->loadIcon("back_content", KIconLoader::NoGroup));
	gbox->addWidget(cover_widgets.inlet, 5, 0, 1, 1, Qt::AlignHCenter);

	cover_widgets.booklet = new QLabel(group);
	cover_widgets.booklet->setPixmap(pixmap->loadIcon("front_title_only", KIconLoader::NoGroup));
	gbox->addWidget(cover_widgets.booklet, 5, 1, 1, 1, Qt::AlignHCenter);

	set_cover();
	addPage(page, i18n("Cover"));
}

void
pd::output_changed_1()
{
	output_changed(1);
}

void
pd::output_changed_2()
{
	output_changed(2);
}

void
pd::output_changed_3()
{
	output_changed(3);
}

void
pd::output_changed_4()
{
	output_changed(4);
}

void
pd::output_changed(int type)
{
	KIconLoader *pixmap = KIconLoader::global ();

	if (type == 1) {
		cover_widgets.inlet->setPixmap(pixmap->loadIcon("back_content", KIconLoader::NoGroup));
		cover_widgets.booklet->setPixmap(pixmap->loadIcon("front_title_only", KIconLoader::NoGroup));
	}
	if (type == 2) {
		cover_widgets.inlet->setPixmap(NULL);
		cover_widgets.booklet->setPixmap(pixmap->
						 loadIcon("front_title-right_content-left",
							  KIconLoader::NoGroup));
	}
	if (type == 3) {
		cover_widgets.inlet->setPixmap(pixmap->loadIcon("back_title_content", KIconLoader::NoGroup));
		cover_widgets.booklet->setPixmap(NULL);
	}
	if (type == 4) {
		cover_widgets.inlet->setPixmap(pixmap->loadIcon("back_content", KIconLoader::NoGroup));
		cover_widgets.booklet->setPixmap(pixmap->loadIcon("one_page", KIconLoader::NoGroup));
	}
}

void
pd::set_cover()
{
	cover_widgets.display_track_duration->setChecked(globals.display_track_duration);

	/* no comment */
	if (!globals.inlet_only && !globals.its_a_slim_case && !globals.one_page) {
		cover_widgets.its_normal->setChecked(true);
		output_changed(1);
	}

	cover_widgets.its_a_slim_case->setChecked(globals.its_a_slim_case);
	cover_widgets.inlet_only->setChecked(globals.inlet_only);
	cover_widgets.one_page->setChecked(globals.one_page);
	if (globals.its_a_slim_case)
		output_changed(2);

	if (globals.inlet_only)
		output_changed(3);

	if (globals.one_page)
		output_changed(4);
}

void
pd::save_cover()
{
	globals.display_track_duration = (cover_widgets.display_track_duration)->isChecked()? 1 : 0;
	globals.its_a_slim_case = (cover_widgets.its_a_slim_case)->isChecked()? 1 : 0;
	globals.inlet_only = (cover_widgets.inlet_only)->isChecked()? 1 : 0;
	globals.one_page = (cover_widgets.one_page)->isChecked()? 1 : 0;
}

void
pd::browsing()
{
	sd *dialog;
	int aber = 0;

	dialog = new sd();
	aber = dialog->exec();
	kprintf("sd returns: %d %s\n", aber, (dialog->get(aber)).c_str());
	if (aber >= 0)
		cddb_widgets.cddb_server->setText((dialog->get(aber)).c_str());
	delete(dialog);
}

void
pd::setup_font()
{
	QWidget *page = new QWidget;
	QVBoxLayout *topLayout = new QVBoxLayout(page);

	QGroupBox *group = new QGroupBox(i18n("&Standard Fonts"), page);

	topLayout->addWidget(group);
	QVBoxLayout *vlay = new QVBoxLayout(group);

	vlay->addSpacing(fontMetrics().lineSpacing());
	QGridLayout *gbox = new QGridLayout();

	vlay->addLayout(gbox);

	QLabel *label = new QLabel(i18n
		       ("<qt>Changes to any of these fonts are global. "
			"This means, that changes will only be available "
			"for the next new cover. "
			"Except that the current cover is empty. "
			"Then changes are applied to the current cover.</qt>"),
		       group);
	label->setWordWrap(true);
	gbox->addWidget(label, 0, 0, 1, -1);

	label = new QLabel(i18n("Content Font: "), group);

	gbox->addWidget(label, 1, 0);

	font_widgets.change_content_font = new QPushButton(i18n("Change"), group);
	gbox->addWidget(font_widgets.change_content_font, 1, 1);
	connect(font_widgets.change_content_font, SIGNAL(clicked()), SLOT(content_font_dialog()));

	label = new QLabel(i18n("Title Font: "), group);
	gbox->addWidget(label, 2, 0);

	font_widgets.change_title_font = new QPushButton(i18n("Change"), group);
	gbox->addWidget(font_widgets.change_title_font, 2, 1);
	connect(font_widgets.change_title_font, SIGNAL(clicked()), this, SLOT(title_font_dialog()));

	label = new QLabel(i18n("Spine Text Font: "), group);
	gbox->addWidget(label, 3, 0);

	font_widgets.change_inlet_title_font = new QPushButton(i18n("Change"), group);
	gbox->addWidget(font_widgets.change_inlet_title_font, 3, 1);
	connect(font_widgets.change_inlet_title_font, SIGNAL(clicked()), this, SLOT(inlet_title_font_dialog()));

	topLayout->addStretch();
	addPage(page, i18n("Fonts"));
}

void
pd::font_dialog(QFont *f)
{
	KFontDialog kf;
	kprintf("font name before: %s\n", f->family().toUtf8().constData());

	kf.setFont(*f);
	if (kf.getFont(*f) && changed) {
		KMessageBox::information(this, i18n ("Changes to the fonts" " will not be applied to" " the current cover," " but for the next new cover."));
		changed = false;
	}
	kprintf("font name after: %s\n", f->family().toUtf8().constData());
}

void
pd::content_font_dialog()
{
	font_dialog(&content);
}

void
pd::title_font_dialog()
{
	font_dialog(&title);
}

void
pd::inlet_title_font_dialog()
{
	font_dialog(&inlet_title);
}

#if 0
void
PreferencesDialog::slotOk()
{
	static int i = 0;

	if ((((cddb_widgets.proxy_server)->text()).isEmpty()
	     || ((cddb_widgets.proxy_port)->text()).isEmpty())
	    && (cddb_widgets.use_proxy)->isChecked() && !(cddb_widgets.proxy_from_env)->isChecked()) {
		switch (i) {
		case 0:
			KMessageBox::information(this, "Come for the third, Laertes. You do but dally.");
			i++;
			return;
		case 1:
			KMessageBox::sorry(this, "I pray you, pass with your best violence.");
			i++;
			return;
		case 2:
			KMessageBox::error(this, "I am afeard you make a wanton of me.");
		default:
			i = 0;
			return;
		}
	}

	if (((cdrom_widgets.cdrom_device)->text()).isEmpty()) {
		KMessageBox::sorry(this, i18n("Please enter a cdrom device."));
		return;
	}

	apply_settings();
	accept();
}

void
PreferencesDialog::slotDefault()
{
	switch (activePageIndex()) {
	case page_cddb:
		set_cddb();
		break;

	case page_cdrom:
		set_cdrom();
		break;

	case page_cddb_files:
		set_cddb_files();
		break;

	case page_misc:
		set_misc();
		break;

	case page_font:

		break;

	case page_cover:
		set_cover();
		break;

	default:
		fprintf(stderr, "kover:%s:%d: this should not happen\n", __FILE__, __LINE__);
		return;
	}
}



void
PreferencesDialog::setup_misc_page(void)
{
	Q3Frame *page = addPage(i18n("Miscellaneous"),
				i18n("Various properties"),
				BarIcon("misc", KIcon::SizeMedium));
	Q3VBoxLayout *topLayout = new Q3VBoxLayout(page, 0, spacingHint());

	Q3GroupBox *group = new Q3GroupBox(i18n("&Stuff"), page);

	topLayout->addWidget(group);
	Q3VBoxLayout *vlay = new Q3VBoxLayout(group, spacingHint());

	vlay->addSpacing(fontMetrics().lineSpacing());
	Q3GridLayout *gbox = new Q3GridLayout(5, 5);

	vlay->addLayout(gbox);

	QString text;

	text = i18n("Save window position");
	misc_widgets.save_position = new QCheckBox(text, group, "save_position");
	gbox->addWidget(misc_widgets.save_position, 0, 0);

	text = i18n("Disable unnecessary animation");
	misc_widgets.disable_animation = new QCheckBox(text, group, "disable_animation");
	gbox->addWidget(misc_widgets.disable_animation, 1, 0);

	text = i18n("Mouse click on kover preview triggers actual size");
	misc_widgets.trigger_actual_size = new QCheckBox(text, group, "trigger_actual_size");
	gbox->addWidget(misc_widgets.trigger_actual_size, 2, 0);

	set_misc();
	topLayout->addStretch(10);
}

void
PreferencesDialog::save_misc()
{
	if ((misc_widgets.save_position)->isChecked())
		globals.save_position = 1;
	else
		globals.save_position = 0;

	if ((misc_widgets.disable_animation)->isChecked())
		globals.disable_animation = 1;
	else
		globals.disable_animation = 0;

	if ((misc_widgets.trigger_actual_size)->isChecked())
		globals.trigger_actual_size = 1;
	else
		globals.trigger_actual_size = 0;
}

void
PreferencesDialog::set_misc()
{
	if (globals.save_position)
		misc_widgets.save_position->setChecked(true);
	else
		misc_widgets.save_position->setChecked(false);

	if (globals.disable_animation)
		misc_widgets.disable_animation->setChecked(true);
	else
		misc_widgets.disable_animation->setChecked(false);

	if (globals.trigger_actual_size)
		misc_widgets.trigger_actual_size->setChecked(true);
	else
		misc_widgets.trigger_actual_size->setChecked(false);
}

#endif
