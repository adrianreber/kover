/**  hey emacs! please try this -*- adrian-c -*-
	 kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
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
	 
	 File: PreferencesDialog.cc
	 
	 Description: Contains the implementation of the preferences dialog.
	 
	 Created: 26 Dec 2000
	 
	 Changes:
	 
	 26 Dec 2000: First day of existence...
	 25 Jan 2001: Added entries for cddb, cdrom and cddb_file page
	 27 Jan 2001: Added misc page
	 12 Jun 2001: Added slim case thingy
	 16 Oct 2001: Added 'inlet only' thingy
	 13 Mar 2002: Standard font page
*/

/* $Id: PreferencesDialog.cc,v 1.17 2003/06/14 23:46:31 adrian Exp $ */

#ifndef lint
static char vcid[] =
    "$Id: PreferencesDialog.cc,v 1.17 2003/06/14 23:46:31 adrian Exp $";
#endif /* lint */

#include "PreferencesDialog.moc"
#include "PreferencesDialog.h"
#include "server_dialog.h"

#include <qbuttongroup.h>
#include <klocale.h>
#include <kiconloader.h>
#include <qlayout.h>
#include <kmessagebox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qfontdialog.h>
#include <kfontdialog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

PreferencesDialog::PreferencesDialog(QWidget * parent,
    const QString & caption, bool changed):KDialogBase(KJanusWidget::IconList,
    caption, Ok | Cancel, Ok, parent)
{
    this->parent = parent;
    setupCDDBPage();
    setupCDROMPage();
    setup_cddb_files_page();
    setup_cover_page();
    setup_font_page();
    setup_misc_page();
    this->changed = changed;
}

PreferencesDialog::~PreferencesDialog()
{
}

void PreferencesDialog::setupCDDBPage(void)
{
    QFrame *page = addPage(i18n("CDDB"), i18n("CDDB options"),
        BarIcon("network", KIcon::SizeMedium));
    QVBoxLayout *topLayout = new QVBoxLayout(page, 0, spacingHint());

    QGroupBox *group = new QGroupBox(i18n("&CDDB server"), page);

    topLayout->addWidget(group);
    QVBoxLayout *vlay = new QVBoxLayout(group, spacingHint());

    vlay->addSpacing(fontMetrics().lineSpacing());

    QGridLayout *gbox = new QGridLayout(5, 3);

    vlay->addLayout(gbox);

    QString text;
    QLabel *label = new QLabel(i18n("CDDB server:"), group, "cddblabel");

    gbox->addWidget(label, 0, 0);

    cddb_widgets.cddb_server = new QLineEdit(group, "server");
    cddb_widgets.cddb_server->setMinimumWidth(fontMetrics().maxWidth() * 10);
    gbox->addWidget(cddb_widgets.cddb_server, 0, 1);

    QPushButton *browse = new QPushButton(i18n("Browse"), group, "browse");

    connect(browse, SIGNAL(clicked()), SLOT(browsing()));
    gbox->addWidget(browse, 0, 2);

    label = new QLabel(i18n("CGI path:"), group, "cgilabel");
    gbox->addWidget(label, 1, 0);
    cddb_widgets.cgi_path = new QLineEdit(group, "cgi");
    cddb_widgets.cgi_path->setMinimumWidth(fontMetrics().maxWidth() * 10);
    gbox->addWidget(cddb_widgets.cgi_path, 1, 1);

    group = new QGroupBox(i18n("&Proxy configuration"), page);
    topLayout->addWidget(group);
    vlay = new QVBoxLayout(group, spacingHint());
    vlay->addSpacing(fontMetrics().lineSpacing());

    gbox = new QGridLayout(5, 5);
    vlay->addLayout(gbox);
    text = i18n("Use proxy for CDDB lookups");
    cddb_widgets.use_proxy = new QCheckBox(text, group, "use_proxy");
    connect(cddb_widgets.use_proxy, SIGNAL(toggled(bool)), this,
        SLOT(use_proxy(bool)));
    gbox->addMultiCellWidget(cddb_widgets.use_proxy, 0, 0, 0, 5);
    text = i18n("Use 'http_proxy' environment variable");
    cddb_widgets.proxy_from_env =
        new QCheckBox(text, group, "proxy_from_env");
    connect(cddb_widgets.proxy_from_env, SIGNAL(toggled(bool)), this,
        SLOT(use_proxy_env(bool)));
    gbox->addMultiCellWidget(cddb_widgets.proxy_from_env, 1, 1, 0, 5);
    label = new QLabel(i18n("Proxy server:"), group, "proxylabel");
    gbox->addWidget(label, 2, 0);
    cddb_widgets.proxy_server = new QLineEdit(group, "proxy");
    cddb_widgets.proxy_server->setMinimumWidth(fontMetrics().maxWidth() * 10);

    gbox->addMultiCellWidget(cddb_widgets.proxy_server, 2, 2, 1, 5);
    label = new QLabel(i18n("Proxy server port:"), group, "proxyportlabel");
    gbox->addWidget(label, 3, 0);
    cddb_widgets.proxy_port = new QLineEdit(group, "proxyport");
    cddb_widgets.proxy_port->setMaxLength(5);

    gbox->addMultiCellWidget(cddb_widgets.proxy_port, 3, 3, 1, 5);
    set_cddb();
    topLayout->addStretch(10);
}

void PreferencesDialog::setupCDROMPage(void)
{
    QFrame *page = addPage(i18n("CDROM"), i18n("CDROM options"),
        BarIcon("cdrom_unmount", KIcon::SizeMedium));
    QVBoxLayout *topLayout = new QVBoxLayout(page, 0, spacingHint());

    QGroupBox *group = new QGroupBox(i18n("&CDROM"), page);

    topLayout->addWidget(group);
    QVBoxLayout *vlay = new QVBoxLayout(group, spacingHint());

    vlay->addSpacing(fontMetrics().lineSpacing());
    QGridLayout *gbox = new QGridLayout(5, 5);

    vlay->addLayout(gbox);

    QString text;
    QLabel *label = new QLabel(i18n("CDROM device:"), group, "cdromlabel");

    gbox->addWidget(label, 0, 0);
    cdrom_widgets.cdrom_device = new QLineEdit(group, "device");
    cdrom_widgets.cdrom_device->setMinimumWidth(fontMetrics().maxWidth() *
        10);
    gbox->addMultiCellWidget(cdrom_widgets.cdrom_device, 0, 0, 1, 5);

    text = i18n("Eject CD on exit");
    cdrom_widgets.eject_cdrom = new QCheckBox(text, group, "eject_cdrom");
    gbox->addMultiCellWidget(cdrom_widgets.eject_cdrom, 1, 1, 0, 5);

    set_cdrom();

    topLayout->addStretch(10);
}

void PreferencesDialog::use_proxy(bool status)
{
    if (status) {
        cddb_widgets.proxy_from_env->setEnabled(true);
        if ((cddb_widgets.proxy_from_env)->isChecked()) {
            cddb_widgets.proxy_server->setEnabled(false);
            cddb_widgets.proxy_port->setEnabled(false);
        } else {
            cddb_widgets.proxy_server->setEnabled(true);
            cddb_widgets.proxy_port->setEnabled(true);
        }
    } else {
        cddb_widgets.proxy_from_env->setEnabled(false);
        cddb_widgets.proxy_server->setEnabled(false);
        cddb_widgets.proxy_port->setEnabled(false);
    }
}

void PreferencesDialog::use_proxy_env(bool status)
{
    cddb_widgets.proxy_server->setEnabled(!status);
    cddb_widgets.proxy_port->setEnabled(!status);
}

void PreferencesDialog::slotOk()
{
    static int i = 0;

    if ((((cddb_widgets.proxy_server)->text()).isEmpty()
            || ((cddb_widgets.proxy_port)->text()).isEmpty())
        && (cddb_widgets.use_proxy)->isChecked()
        && !(cddb_widgets.proxy_from_env)->isChecked()) {
        switch (i) {
        case 0:
            KMessageBox::information(this,
                "Come for the third, Laertes. You do but dally.");
            i++;
            return;
        case 1:
            KMessageBox::sorry(this,
                "I pray you, pass with your best violence.");
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

void PreferencesDialog::apply_settings()
{
    save_cddb_files();
    save_misc();
    save_cover();

    if (!((cddb_widgets.proxy_port)->text()).isEmpty()) {
        globals.proxy_port = ((cddb_widgets.proxy_port)->text()).toInt();
    }

    if (!((cddb_widgets.cddb_server)->text()).isEmpty()) {
        _DEBUG_ fprintf(stderr, "kover:old value :%s\nkover:new value: %s\n",
            globals.cddb_server,
            ((cddb_widgets.cddb_server)->text()).latin1());
        free(globals.cddb_server);
        globals.cddb_server =
            strdup(((cddb_widgets.cddb_server)->text()).latin1());
    }

    if (!((cddb_widgets.cgi_path)->text()).isEmpty()) {
        _DEBUG_ fprintf(stderr, "kover:old value :%s\nkover:new value: %s\n",
            globals.cgi_path, ((cddb_widgets.cgi_path)->text()).latin1());
        free(globals.cgi_path);
        globals.cgi_path = strdup(((cddb_widgets.cgi_path)->text()).latin1());
    }

    if (!((cddb_widgets.proxy_server)->text()).isEmpty()) {
        if (globals.proxy_server)
            free(globals.proxy_server);
        globals.proxy_server =
            strdup(((cddb_widgets.proxy_server)->text()).latin1());
    } else {
        if (globals.proxy_server)
            free(globals.proxy_server);
        globals.proxy_server = NULL;
    }

    globals.use_proxy = ((cddb_widgets.use_proxy)->isChecked())? 1 : 0;
    globals.proxy_from_env =
        ((cddb_widgets.proxy_from_env)->isChecked())? 1 : 0;
    globals.eject_cdrom = ((cdrom_widgets.eject_cdrom)->isChecked())? 1 : 0;

    if (!((cdrom_widgets.cdrom_device)->text()).isEmpty()) {
        if (globals.cdrom_device)
            free(globals.cdrom_device);
        globals.cdrom_device =
            strdup(((cdrom_widgets.cdrom_device)->text()).latin1());
    } else {
        if (globals.cdrom_device)
            free(globals.cdrom_device);
        globals.cdrom_device = NULL;
    }

}

void PreferencesDialog::slotDefault()
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
        fprintf(stderr, "kover:%s:%d: this should not happen\n", __FILE__,
            __LINE__);
        return;
    }
}

void PreferencesDialog::set_cddb()
{
    QString text;

    cddb_widgets.cddb_server->setText(globals.cddb_server);
    cddb_widgets.cgi_path->setText(globals.cgi_path);

    if (globals.proxy_server)
        cddb_widgets.proxy_server->setText(globals.proxy_server);

    cddb_widgets.proxy_port->setText(text.sprintf("%d", globals.proxy_port));

    if (globals.use_proxy) {
        use_proxy(true);
        cddb_widgets.use_proxy->setChecked(true);
    } else {
        use_proxy(false);
        cddb_widgets.use_proxy->setChecked(false);
    }

    if (globals.proxy_from_env) {
        cddb_widgets.proxy_from_env->setChecked(true);
        use_proxy_env(true);
    } else {
        cddb_widgets.proxy_from_env->setChecked(false);
        use_proxy_env(false);
    }
}

void PreferencesDialog::set_cdrom()
{
    cdrom_widgets.cdrom_device->setText(globals.cdrom_device);
    if (globals.eject_cdrom)
        cdrom_widgets.eject_cdrom->setChecked(true);
    else
        cdrom_widgets.eject_cdrom->setChecked(false);
}

void PreferencesDialog::set_cddb_files()
{
    if (globals.read_local_cddb)
        cddb_files_widgets.read_local_cddb->setChecked(true);
    else
        cddb_files_widgets.read_local_cddb->setChecked(false);

    if (globals.write_local_cddb)
        cddb_files_widgets.write_local_cddb->setChecked(true);
    else
        cddb_files_widgets.write_local_cddb->setChecked(false);

    cddb_files_widgets.cddb_path->setText(globals.cddb_path);
}

void PreferencesDialog::setup_cddb_files_page(void)
{
    QFrame *page = addPage(i18n("CDDB files"), i18n("Local CDDB files"),
        BarIcon("folder_blue", KIcon::SizeMedium));
    QVBoxLayout *topLayout = new QVBoxLayout(page, 0, spacingHint());

    QGroupBox *group = new QGroupBox(i18n("&Local CDDB files"), page);

    topLayout->addWidget(group);
    QVBoxLayout *vlay = new QVBoxLayout(group, spacingHint());

    vlay->addSpacing(fontMetrics().lineSpacing());
    QGridLayout *gbox = new QGridLayout(5, 5);

    vlay->addLayout(gbox);

    QString text;

    text = i18n("Read CDDB data from file if available");
    cddb_files_widgets.read_local_cddb =
        new QCheckBox(text, group, "read_cddb");
    gbox->addMultiCellWidget(cddb_files_widgets.read_local_cddb, 0, 0, 0, 5);

    text = i18n("Write CDDB data to file");
    cddb_files_widgets.write_local_cddb =
        new QCheckBox(text, group, "write_cddb");
    gbox->addMultiCellWidget(cddb_files_widgets.write_local_cddb, 1, 1, 0, 5);

    QLabel *label = new QLabel(i18n("CDDB path:"), group, "pathlabel");

    gbox->addWidget(label, 2, 0);
    cddb_files_widgets.cddb_path = new QLineEdit(group, "path");
    cddb_files_widgets.cddb_path->setMinimumWidth(fontMetrics().
        maxWidth() * 10);
    gbox->addMultiCellWidget(cddb_files_widgets.cddb_path, 2, 2, 1, 5);

    set_cddb_files();
    topLayout->addStretch(10);
}

void PreferencesDialog::save_cddb_files()
{
    struct stat stat_struct;

    globals.read_local_cddb =
        ((cddb_files_widgets.read_local_cddb)->isChecked())? 1 : 0;

    globals.write_local_cddb =
        ((cddb_files_widgets.write_local_cddb)->isChecked())? 1 : 0;

    if (!((cddb_files_widgets.cddb_path)->text()).isEmpty()) {
        if (globals.cddb_path)
            free(globals.cddb_path);
        globals.cddb_path =
            strdup(((cddb_files_widgets.cddb_path)->text()).latin1());
        /* checking for "/" at the end */
        if (globals.cddb_path[strlen(globals.cddb_path) - 1] != '/') {
            char *dummy = strdup(globals.cddb_path);

            free(globals.cddb_path);
            globals.cddb_path = (char *) malloc(strlen(dummy) + 1);
            strcpy(globals.cddb_path, dummy);
            strcat(globals.cddb_path, "/");
            free(dummy);
        }
    } else {
        if (globals.cddb_path)
            free(globals.cddb_path);
        globals.cddb_path = NULL;
    }

    if (globals.cddb_path) {
        /* does the directory exist */
        if (stat(globals.cddb_path, &stat_struct) == -1)
            /* no it doesn't... let's create one */
            if (mkdir(globals.cddb_path, 0777) == -1) {
                /* failed */
                free(globals.cddb_path);
                globals.cddb_path = NULL;
            }
    }
}

void PreferencesDialog::setup_cover_page()
{
    QFrame *page = addPage(i18n("Cover"), i18n("Cover"),
        BarIcon("kover", KIcon::SizeMedium));
    QVBoxLayout *topLayout = new QVBoxLayout(page, 0, spacingHint());

    QButtonGroup *group = new QButtonGroup(i18n("Cover"), page);

    topLayout->addWidget(group);
    QVBoxLayout *vlay = new QVBoxLayout(group, spacingHint());

    vlay->addSpacing(fontMetrics().lineSpacing());
    QGridLayout *gbox = new QGridLayout(6, 5);

    vlay->addLayout(gbox);

    QString text;

    text = i18n("Display track duration after a CDDB request");
    cover_widgets.display_track_duration =
        new QCheckBox(text, group, "display_track_duration");
    gbox->addMultiCellWidget(cover_widgets.display_track_duration, 0, 0, 0,
        5);

    text = i18n("Print inlet and booklet");
    cover_widgets.its_normal = new QRadioButton(text, group, "its_normal");
    gbox->addMultiCellWidget(cover_widgets.its_normal, 1, 1, 0, 3);

    text = i18n("Print inlet on left side of booklet.\n(slim case option)");
    cover_widgets.its_a_slim_case =
        new QRadioButton(text, group, "its_a_slim_case");
    gbox->addMultiCellWidget(cover_widgets.its_a_slim_case, 2, 2, 0, 3);

    text = i18n("Don't print booklet.\n(inlet only option)");
    cover_widgets.inlet_only = new QRadioButton(text, group, "inlet_only");
    gbox->addMultiCellWidget(cover_widgets.inlet_only, 3, 3, 0, 3);

    text = i18n("Print all on one page");
    cover_widgets.one_page = new QRadioButton(text, group, "one_page");
    gbox->addMultiCellWidget(cover_widgets.one_page, 4, 4, 0, 3);
    connect(group, SIGNAL(clicked(int)), SLOT(output_changed(int)));

    cover_widgets.inlet = new QLabel(group);
    KIconLoader pixmap = KIconLoader();

    cover_widgets.inlet->setPixmap(pixmap.loadIcon("back_content",
            KIcon::NoGroup));
    gbox->addMultiCellWidget(cover_widgets.inlet, 5, 5, 1, 2);

    cover_widgets.booklet = new QLabel(group);
    cover_widgets.booklet->setPixmap(pixmap.loadIcon("front_title_only",
            KIcon::NoGroup));
    gbox->addMultiCellWidget(cover_widgets.booklet, 5, 5, 3, 4);

    set_cover();
}

void PreferencesDialog::set_cover()
{
    if (globals.display_track_duration)
        cover_widgets.display_track_duration->setChecked(true);
    else
        cover_widgets.display_track_duration->setChecked(false);

    /* no comment */
    if (globals.its_a_slim_case) {
        cover_widgets.its_a_slim_case->setChecked(true);
        output_changed(2);
    } else
        cover_widgets.its_a_slim_case->setChecked(false);

    if (globals.inlet_only) {
        cover_widgets.inlet_only->setChecked(true);
        output_changed(3);
    } else
        cover_widgets.inlet_only->setChecked(false);

    if (!globals.inlet_only && !globals.its_a_slim_case && !globals.one_page) {
        cover_widgets.its_normal->setChecked(true);
        output_changed(1);
    }

    if (globals.one_page) {
        cover_widgets.one_page->setChecked(true);
        output_changed(4);
    } else
        cover_widgets.one_page->setChecked(false);
}

void PreferencesDialog::save_cover()
{
    if ((cover_widgets.display_track_duration)->isChecked())
        globals.display_track_duration = 1;
    else
        globals.display_track_duration = 0;

    if ((cover_widgets.its_a_slim_case)->isChecked())
        globals.its_a_slim_case = 1;
    else
        globals.its_a_slim_case = 0;

    if ((cover_widgets.inlet_only)->isChecked())
        globals.inlet_only = 1;
    else
        globals.inlet_only = 0;

    if ((cover_widgets.one_page)->isChecked())
        globals.one_page = 1;
    else
        globals.one_page = 0;
}

void PreferencesDialog::setup_font_page()
{
    QFrame *page = addPage(i18n("Fonts"), i18n("Standard Fonts"),
        BarIcon("fonts", KIcon::SizeMedium));
    QVBoxLayout *topLayout = new QVBoxLayout(page, 0, spacingHint());

    QGroupBox *group = new QGroupBox(i18n("&Standard Fonts"), page);

    topLayout->addWidget(group);
    QVBoxLayout *vlay = new QVBoxLayout(group, spacingHint());

    vlay->addSpacing(fontMetrics().lineSpacing());
    QGridLayout *gbox = new QGridLayout(4, 1);

    vlay->addLayout(gbox);

    QLabel *label =
        new
        QLabel(i18n("<qt>Changes to any of these fonts are global. "
            "This means, that changes will only be available "
            "for the next new cover. "
            "Except that the current cover is empty. "
            "Then changes are applied to the current cover.</qt>"),
        group, "font_info");

    gbox->addMultiCellWidget(label, 0, 0, 0, 1);

    label = new QLabel(i18n("Content Font: "), group, "content_font");

    gbox->addWidget(label, 1, 0);

    font_widgets.change_content_font =
        new QPushButton(i18n("Change"), group, "change_content_font");
    gbox->addWidget(font_widgets.change_content_font, 1, 1);
    connect(font_widgets.change_content_font, SIGNAL(clicked()),
        SLOT(content_font_dialog()));

    label = new QLabel(i18n("Title Font: "), group, "title_font");
    gbox->addWidget(label, 2, 0);

    font_widgets.change_title_font =
        new QPushButton(i18n("Change"), group, "change_title_font");
    gbox->addWidget(font_widgets.change_title_font, 2, 1);
    connect(font_widgets.change_title_font, SIGNAL(clicked()), this,
        SLOT(title_font_dialog()));

    label = new QLabel(i18n("Spine Text Font: "), group, "inlet_title_font");
    gbox->addWidget(label, 3, 0);

    font_widgets.change_inlet_title_font =
        new QPushButton(i18n("Change"), group, "change_inlet_title_font");
    gbox->addWidget(font_widgets.change_inlet_title_font, 3, 1);
    connect(font_widgets.change_inlet_title_font, SIGNAL(clicked()), this,
        SLOT(inlet_title_font_dialog()));

    topLayout->addStretch();
}

void PreferencesDialog::content_font_dialog()
{
    _DEBUG_ fprintf(stderr, "%s:font name before: %s\n", PACKAGE,
        ((globals.content_font)->rawName()).latin1());
    KFontDialog *kf = new KFontDialog(this, "kf", true);

    kf->getFont(*globals.content_font);
    _DEBUG_ fprintf(stderr, "%s:font name after: %s\n", PACKAGE,
        ((globals.content_font)->rawName()).latin1());
    if (changed)
        show_font_warning();
}

void PreferencesDialog::title_font_dialog()
{
    KFontDialog *kf = new KFontDialog(this, "kf", true);

    kf->getFont(*globals.title_font);
    if (changed)
        show_font_warning();
}

void PreferencesDialog::inlet_title_font_dialog()
{
    KFontDialog *kf = new KFontDialog(this, "kf", true);

    kf->getFont(*globals.inlet_title_font);
    if (changed)
        show_font_warning();
}

void PreferencesDialog::output_changed(int type)
{
    KIconLoader pixmap = KIconLoader();

    if (type == 1) {
        cover_widgets.inlet->setPixmap(pixmap.loadIcon("back_content",
                KIcon::NoGroup));
        cover_widgets.booklet->setPixmap(pixmap.loadIcon("front_title_only",
                KIcon::NoGroup));
    }
    if (type == 2) {
        cover_widgets.inlet->setPixmap(NULL);
        cover_widgets.booklet->setPixmap(pixmap.
            loadIcon("front_title-right_content-left", KIcon::NoGroup));
    }
    if (type == 3) {
        cover_widgets.inlet->setPixmap(pixmap.loadIcon("back_title_content",
                KIcon::NoGroup));
        cover_widgets.booklet->setPixmap(NULL);
    }
    if (type == 4) {
        cover_widgets.inlet->setPixmap(pixmap.loadIcon("back_content",
                KIcon::NoGroup));
        cover_widgets.booklet->setPixmap(pixmap.loadIcon("one_page",
                KIcon::NoGroup));
    }
}

void PreferencesDialog::browsing()
{
    server_dialog *dialog;
    int aber = 0;

    dialog = new server_dialog();
    aber = dialog->exec();
    _DEBUG_ fprintf(stderr, "kover:server_dialog returns: %d %s\n", aber,
        (dialog->get(aber)).c_str());

    cddb_widgets.cddb_server->setText((dialog->get(aber)).c_str());
    delete(dialog);
}

void PreferencesDialog::setup_misc_page(void)
{
    QFrame *page = addPage(i18n("Miscellaneous"), i18n("Various properties"),
        BarIcon("misc", KIcon::SizeMedium));
    QVBoxLayout *topLayout = new QVBoxLayout(page, 0, spacingHint());

    QGroupBox *group = new QGroupBox(i18n("&Stuff"), page);

    topLayout->addWidget(group);
    QVBoxLayout *vlay = new QVBoxLayout(group, spacingHint());

    vlay->addSpacing(fontMetrics().lineSpacing());
    QGridLayout *gbox = new QGridLayout(5, 5);

    vlay->addLayout(gbox);

    QString text;

    text = i18n("Save window position");
    misc_widgets.save_position = new QCheckBox(text, group, "save_position");
    gbox->addWidget(misc_widgets.save_position, 0, 0);

    text = i18n("Disable unnecessary animation");
    misc_widgets.disable_animation =
        new QCheckBox(text, group, "disable_animation");
    gbox->addWidget(misc_widgets.disable_animation, 1, 0);

    text = i18n("Mouse click on kover preview triggers actual size");
    misc_widgets.trigger_actual_size =
        new QCheckBox(text, group, "trigger_actual_size");
    gbox->addWidget(misc_widgets.trigger_actual_size, 2, 0);

    set_misc();
    topLayout->addStretch(10);
}

void PreferencesDialog::save_misc()
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

void PreferencesDialog::set_misc()
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

void PreferencesDialog::show_font_warning()
{
    KMessageBox::information(this,
        tr
        ("Changes to the fonts will not be applied to the current cover, but for the next new cover."));
    changed = false;
}
