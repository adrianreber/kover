/**  hey emacs! try -*- adrian-c -*- mode
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
	 
	 File: KoverTop.cc
	 
	 Description: TopLevelWidget
	 
	 Changes:

	 05 Apr 2001: globals initialization methods

	 10 Jun 2001: bugfix for [modified] bug.

	 03 Jul 2001: now displaying CDDB id

	 29 Oct 2001: inlet title font stuff

	 11 Nov 2001: CDDB without CD
*/

/* $Id: kovertop.cc,v 1.24 2004/04/20 20:58:07 adrian Exp $ */

//moc file
#include "kovertop.moc"

//local includes
#include "kovertop.h"
#include "imagedlg.h"
#include "without_cd.h"
#include "filemode.h"
#include <directory.h>

//kde includes
#include <klocale.h>
#include <kmainwindow.h>
#include <kapp.h>
#include <kstdaction.h>
#include <kaction.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kfontdialog.h>
#include <kcolordialog.h>
#include <krecentdocument.h>
#include <kstdaccel.h>
#include <kurl.h>

//qt includes
#include <qlayout.h>
#include <qpoint.h>
#include <qgroupbox.h>
#include <kkeydialog.h>

#define NORM_WIDTH 520
#define NORM_HEIGHT 460
#define MORE_HEIGHT 515

#define MORE_FRAME_HEIGHT 40
#define MORE_FRAME_WIDTH 510

#define PREV_WIDTH 695
#define PREV_HEIGHT 684

#define CDVIEW_X 224
#define CDVIEW_Y 15
#define CDVIEW_WIDTH	291
#define CDVIEW_HEIGHT 310

KoverTop::KoverTop(const char *name):KMainWindow(0, name)
{
    main_frame = new QFrame(this);
    more_frame = new QFrame(this);
    option_frame = new QFrame(this);

    setCaption(i18n("[New Document]"), false);
    setFixedSize(NORM_WIDTH, NORM_HEIGHT);

    status_bar = statusBar();
    status_bar->insertItem("Kover " VERSION " - http://lisas.de/kover/", 1);

    make_menu();
    make_main_frame();
    make_more_frame();
    make_option_frame();

    connect(&kover_file, SIGNAL(dataChanged(bool)), SLOT(dataChanged(bool)));

    cddb_fill = new CDDB_Fill(&kover_file);
    connect(cddb_fill, SIGNAL(statusText(const char *)),
        SLOT(setStatusText(const char *)));
    connect(cddb_fill, SIGNAL(update_id(unsigned long)),
        SLOT(update_id(unsigned long)));

    altered_data = false;
    more = false;

    main_frame->adjustSize();

    if (globals.save_position)
        move(globals.xpos, globals.ypos);

    recent->loadEntries((KApplication::kApplication())->config());
}

KoverTop::~KoverTop()
{
    recent->saveEntries((KApplication::kApplication())->config());
    delete status_bar;
    delete cddb_fill;
    delete cdview;
    _DEBUG_ fprintf(stderr, "~KoverTop()\n");
}

void KoverTop::make_menu()
{
    KStdAction::openNew(this, SLOT(fileNew()), actionCollection());
    KStdAction::open(this, SLOT(fileOpen()), actionCollection());
    KStdAction::save(this, SLOT(fileSave()), actionCollection());
    KStdAction::saveAs(this, SLOT(fileSaveAs()), actionCollection());
    KStdAction::print(this, SLOT(filePrint()), actionCollection());
    KStdAction::quit(this, SLOT(close()), actionCollection());
    KStdAction::cut(this, SLOT(cut()), actionCollection());
    KStdAction::copy(this, SLOT(copy()), actionCollection());
    KStdAction::paste(this, SLOT(paste()), actionCollection());
    recent =
        KStdAction::openRecent(this, SLOT(fileOpen(const KURL &)),
        actionCollection());
    (void) new KAction(i18n("&Actual size"), "viewmag",
        KStdAccel::shortcut(KStdAccel::ZoomIn), this, SLOT(actualSize()),
        actionCollection(), "actual_size");
    (void) new KAction(i18n("&Actual size"), "viewmag",
        KStdAccel::shortcut(KStdAccel::ZoomOut), this, SLOT(stopPreview()),
        actionCollection(), "stop_preview");
    (void) new KAction(i18n("&File mode"), "view_tree",
        0, this, SLOT(file_mode()), actionCollection(), "file_mode");
    (void) new KAction(i18n("&CDDB lookup"), "network", 0, this,
        SLOT(cddbFill()), actionCollection(), "cddb");
    KStdAction::preferences(this, SLOT(preferences()), actionCollection());
    (void) new KAction(i18n("&Image Embedding..."), "background", 0, this,
        SLOT(imageEmbedding()), actionCollection(), "image_embedding");
    (void) new KAction(i18n("Title Font..."), "fonts", 0, this,
        SLOT(titleFont()), actionCollection(), "title_font");
    (void) new KAction(i18n("Title Fontcolor..."), "colorize", 0, this,
        SLOT(titleFontColor()), actionCollection(), "title_font_color");
    (void) new KAction(i18n("Contents Font..."), "fonts", 0, this,
        SLOT(contentsFont()), actionCollection(), "contents_font");
    (void) new KAction(i18n("Contents Fontcolor..."), "colorize", 0, this,
        SLOT(contentsFontColor()), actionCollection(), "contents_font_color");
    (void) new KAction(i18n("Background Color..."), "colors", 0, this,
        SLOT(backgroundColor()), actionCollection(), "background_color");
    (void) new KAction(i18n("Spine Text Font..."), "fonts", 0, this,
        SLOT(inlet_title_font()), actionCollection(), "inlet_title_font");
    (void) new KAction(i18n("Eject CD"), "player_eject", 0, this,
        SLOT(cdrom_eject()), actionCollection(), "eject_cdrom");
    (void) new KAction(i18n("CDDB without CD"), "network", 0, this,
        SLOT(cddb_without_cd()), actionCollection(), "cddb_without_cd");

    KStdAction::keyBindings( this, SLOT( slotConfigureKeys() ), actionCollection() );

    createGUI();
}

void KoverTop::slotConfigureKeys()
{
  KKeyDialog::configure( actionCollection(), this );
}

void KoverTop::make_main_frame()
{
    title_label = new QLabel(i18n("Title"), main_frame, "title_label");
    title_label->move(5, 0);

    title_edit = new QMultiLineEdit(main_frame, "title_edit");
    title_edit->resize(215, 50);
    title_edit->move(5, 25);
    connect(title_edit, SIGNAL(textChanged()), SLOT(titleBoxChanged()));

    contents_label =
        new QLabel(i18n("Contents"), main_frame, "contents_label");
    contents_label->move(5, 88);

    contents_edit = new QMultiLineEdit(main_frame, "contents_edit");
    contents_edit->resize(215, 215);
    contents_edit->move(5, 110);
    connect(contents_edit, SIGNAL(textChanged()), SLOT(contentsBoxChanged()));

    cdview = new CDView(&kover_file, main_frame);
    cdview->resize(CDVIEW_WIDTH, CDVIEW_HEIGHT);
    cdview->move(CDVIEW_X, CDVIEW_Y);
    connect(cdview, SIGNAL(stopPreview()), SLOT(stopPreview()));
    connect(cdview, SIGNAL(actualSize()), SLOT(actualSize()));

    main_frame->move(0, 70);
    title_edit->setFocus();
}

void KoverTop::make_option_frame()
{
    QGroupBox *group_box = new QGroupBox(option_frame, "group");

    group_box->setGeometry(10, 10, 500, 220);
    group_box->setTitle(tr("All this options are not global"));

    QVBoxLayout *vlay = new QVBoxLayout(group_box, 4);

    vlay->addSpacing(fontMetrics().lineSpacing());

    QGridLayout *gbox = new QGridLayout(5, 4);

    vlay->addLayout(gbox);

    display_title =
        new QCheckBox(tr("No title on booklet"), group_box, "display_title");
    connect(display_title, SIGNAL(clicked()), SLOT(display_title_signal()));
    gbox->addMultiCellWidget(display_title, 0, 0, 0, 1);

    spine_text =
        new QCheckBox(i18n("Separate Spine Text"), group_box, "spine_text");
    gbox->addMultiCellWidget(spine_text, 1, 1, 0, 1);
    connect(spine_text, SIGNAL(clicked()), SLOT(spine_text_method()));

    the_spine_text = new QLineEdit(group_box, "the_spine_text");
    gbox->addMultiCellWidget(the_spine_text, 2, 2, 0, 4);
    the_spine_text->setEnabled(false);
    connect(the_spine_text, SIGNAL(textChanged(const QString &)),
        SLOT(spine_text_changed_method(const QString &)));

    number_check =
        new QCheckBox(i18n("CD Number"), group_box, "number_check");
    gbox->addMultiCellWidget(number_check, 3, 3, 0, 0);
    connect(number_check, SIGNAL(toggled(bool)), SLOT(numberChecked(bool)));

    number_spin = new QSpinBox(1, 999, 1, group_box, "numberspin");
    number_spin->setEnabled(false);
    gbox->addMultiCellWidget(number_spin, 3, 3, 1, 1);
    connect(number_spin, SIGNAL(valueChanged(int)), SLOT(numberChanged(int)));

    option_frame->adjustSize();
    option_frame->hide();
}

void KoverTop::make_more_frame()
{
    QVBoxLayout *top_layout = new QVBoxLayout(more_frame);

    button_layout = new QBoxLayout(top_layout, QBoxLayout::RightToLeft, -10);
    button_layout->setAlignment(Qt::AlignTop);
    button_layout->setMargin(7);
    more_frame->setMargin(0);
    more_button = new QPushButton(i18n("Options"), more_frame, "more");
    button_layout->addWidget(more_button, 0, AlignRight);
    connect(more_button, SIGNAL(clicked()), SLOT(more_or_less()));
    top_layout->addSpacing(5);
    cddb_id = new QLabel("", more_frame, "cddb_id");
    button_layout->addWidget(cddb_id, 0, AlignLeft);
    more_frame->move(5, 70 + 310 + 15);
    more_frame->resize(MORE_FRAME_WIDTH, MORE_FRAME_HEIGHT);
}

void KoverTop::dataChanged(bool image)
{
    bool bla = image;

    bla = true;

    setStatusText(i18n("Data changed"));
    if (m_url.isEmpty())
        setCaption(i18n("[New Document]"), true);
    else
        setCaption(i18n(m_url.url()), true);

    altered_data = true;
}

void KoverTop::setStatusText(const char *_status_text)
{
    status_bar->changeItem(_status_text, 1);
    kapp->processEvents();
}

void KoverTop::update_id(unsigned long id)
{
    QString string = "";

    if (id != 0)
        string.sprintf("CDDB id: 0x%08lx", id);
    cddb_id->setText(string);
    cddb_id->adjustSize();
}

void KoverTop::update_id(QString id)
{
    QString string = "";

    if (!id.isEmpty())
        string = "CDDB id: " + id;
    cddb_id->setText(string);
    cddb_id->adjustSize();
}

void KoverTop::titleBoxChanged()
{
    kover_file.setTitle(title_edit->text());
    if (!kover_file.spine_text())
        the_spine_text->setText(title_edit->text());
}

void KoverTop::contentsBoxChanged()
{
    kover_file.setContents(contents_edit->text());
}

void KoverTop::numberChanged(int value)
{
    kover_file.setNumber(value);
}

void KoverTop::numberChecked(bool checked)
{
    number_spin->setEnabled(checked);

    if (checked)
        kover_file.setNumber(number_spin->value());
    else
        kover_file.setNumber(0);
}

void KoverTop::stopPreview()
{
    main_frame->move(0, 70);
    if (more)
        setFixedSize(NORM_WIDTH, MORE_HEIGHT);
    else
        setFixedSize(NORM_WIDTH, NORM_HEIGHT);
    cdview->resize(CDVIEW_WIDTH, CDVIEW_HEIGHT);
    cdview->move(CDVIEW_X, CDVIEW_Y);
    cdview->showPreview(false);
    menuBar()->show();
    statusBar()->show();
    toolBar("koverToolBar")->show();
    toolBar("mainToolBar")->show();
    more_frame->show();
}

bool KoverTop::queryClose()
{
    if (altered_data) {
        if (how_about_saving())
            return false;
    }
    return true;
}

void KoverTop::fileNew()
{
    if (altered_data) {
        if (how_about_saving())
            return;
    }

    title_edit->clear();
    contents_edit->clear();
    number_spin->setValue(1);
    number_spin->setEnabled(false);
    number_check->setChecked(false);
    title_edit->setFocus();
    kover_file.reset();
    display_title->setChecked(kover_file.display_title());
    the_spine_text->setEnabled(kover_file.spine_text());
    spine_text->setChecked(kover_file.spine_text());
    setStatusText("Chop!");
    altered_data = false;
    m_url = KURL();
    setCaption(i18n("[New Document]"), false);
    update_id();
}

void KoverTop::fileOpen()
{
    if (altered_data) {
        if (how_about_saving())
            return;
    }

    KURL url = KFileDialog::getOpenURL(":koverfile",
        i18n("*.kover|Kover files\n*|All files\n*.k3b|k3b files"));

    if (!url.isEmpty()) {
        fileOpen(url);
    }
}

void KoverTop::fileOpen(const KURL & url)
{
    if (!url.isEmpty()) {
        if (kover_file.openFile(url)) {
            m_url = url;

            setCaption(i18n(m_url.url()), false);
            disconnect(contents_edit, SIGNAL(textChanged()), this,
                SLOT(contentsBoxChanged()));
            title_edit->setText(kover_file.title());
            contents_edit->setText(kover_file.contents());
            connect(contents_edit, SIGNAL(textChanged()),
                SLOT(contentsBoxChanged()));

            update_id(kover_file.cddb_id());
            display_title->setChecked(kover_file.display_title());

            if (!kover_file.spine_text()) {
                the_spine_text->setText(title_edit->text());
                the_spine_text->setEnabled(kover_file.spine_text());
                spine_text->setChecked(kover_file.spine_text());
            } else {
                the_spine_text->setText(kover_file.the_spine_text());
                the_spine_text->setEnabled(kover_file.spine_text());
                spine_text->setChecked(kover_file.spine_text());
            }

            if (kover_file.number()) {
                number_spin->setValue(kover_file.number());
                number_spin->setEnabled(true);
                number_check->setChecked(true);
            } else {
                number_spin->setEnabled(false);
                number_check->setChecked(false);
            }

            setStatusText(i18n("File loaded"));
            altered_data = false;
            recent->addURL(url);
        } else
            KMessageBox::error(this,
                i18n("Error while opening/reading file!"));
    }
}

int KoverTop::how_about_saving()
{
    switch (KMessageBox::warningYesNoCancel(this,
            i18n
            ("Data changed since last saving!\nDo you want to save the changes?")))
    {
    case 3:                    //YES
        fileSave();
        if (altered_data)
            return -1;
        return 0;
    case 4:                    //NO
        return 0;
    case 2:                    //CANCEL
        return -1;
    }
    return -1;
}

void KoverTop::saveFile(const KURL & url)
{
    if (kover_file.saveFile(url)) {
        setCaption(i18n(url.url()), false);
        setStatusText(i18n("File saved"));
        altered_data = false;
        recent->addURL(url);
        m_url = url;
    } else
        KMessageBox::error(this, i18n("Error while opening/reading file!"));
}

void KoverTop::fileSave()
{
    if (m_url.isEmpty())
        fileSaveAs();
    else
        saveFile(m_url);
}

void KoverTop::fileSaveAs()
{
    KURL url = KFileDialog::getSaveURL(":koverfile",
        i18n("*.kover|Kover files\n*|All files"));

    if (!url.isEmpty()) {
        if (url.fileName().find('.') == -1)
            url.setFileName(url.fileName() + ".kover");
        saveFile(url);
    }
}

void KoverTop::filePrint()
{
    cdview->printKover();
    _DEBUG_ fprintf(stderr, "Printing done\n");
}

void KoverTop::cut()
{
    if (title_edit->hasFocus())
        title_edit->cut();
    if (contents_edit->hasFocus())
        contents_edit->cut();
}

void KoverTop::copy()
{
    if (title_edit->hasFocus())
        title_edit->copy();
    if (contents_edit->hasFocus())
        contents_edit->copy();
}

void KoverTop::paste()
{
    if (title_edit->hasFocus())
        title_edit->paste();
    if (contents_edit->hasFocus())
        contents_edit->paste();
}

void KoverTop::actualSize()
{
    if (more)
        return;
    cdview->resize(PREV_WIDTH, PREV_HEIGHT);
    cdview->move(0, 0);
    main_frame->move(0, 0);
    setFixedSize(PREV_WIDTH, PREV_HEIGHT);
    cdview->showPreview();
    cdview->setFocus();
    more_frame->hide();
    option_frame->hide();
    menuBar()->hide();
    statusBar()->hide();
    toolBar("mainToolBar")->hide();
    toolBar("koverToolBar")->hide();
    main_frame->adjustSize();
}

void KoverTop::cddbFill()
{
    setStatusText(i18n("Initiating CDDB lookup!"));
    if (altered_data) {
        if (how_about_saving())
            return;
    }

    if (cddb_fill->execute()) {
        cddb_fill->setTitleAndContents();
        disconnect(contents_edit, SIGNAL(textChanged()), this,
            SLOT(contentsBoxChanged()));
        title_edit->setText(kover_file.title());
        contents_edit->setText(kover_file.contents());
        connect(contents_edit, SIGNAL(textChanged()),
            SLOT(contentsBoxChanged()));
        altered_data = false;
        cddb_fill->cdInfo();
    }
}

void KoverTop::preferences()
{
    PreferencesDialog *dialog = NULL;

    if (kover_file.empty())
        dialog = new PreferencesDialog(this, i18n("config me"));
    else
        dialog = new PreferencesDialog(this, i18n("config me"), true);
    if (dialog->exec())
        cdview->dataChanged(true);
    delete dialog;

    if (!altered_data) {
        if (kover_file.empty()) {
            kover_file.reset();
            altered_data = false;
            setCaption(i18n("[New Document]"), false);
        }
    }
}

void KoverTop::titleFont()
{
    QFont new_font;
    KFontDialog *kf = new KFontDialog(this, "kf", true);

    new_font = kover_file.titleFont();
    if (kf->getFont(new_font))
        kover_file.setTitleFont(new_font);
    delete kf;
}

void KoverTop::inlet_title_font()
{
    QFont new_font;
    KFontDialog *kf = new KFontDialog(this, "inlet_font", true);

    new_font = kover_file.inlet_title_font();
    if (kf->getFont(new_font))
        kover_file.set_inlet_title_font(new_font);
    delete kf;
}

void KoverTop::imageEmbedding()
{
    ImageDlg *imgdlg = new ImageDlg(this, &kover_file);

    if (imgdlg->exec())
        cdview->dataChanged(true);
    delete imgdlg;
}

void KoverTop::titleFontColor()
{
    QColor new_color;
    KColorDialog *kc = new KColorDialog(this, "kc", true);

    new_color = kover_file.titleColor();
    if (kc->getColor(new_color))
        kover_file.setTitleColor(new_color);
    delete kc;
}

void KoverTop::contentsFont()
{
    QFont new_font;
    KFontDialog *kf = new KFontDialog(this, "kf", true);

    new_font = kover_file.contentsFont();
    if (kf->getFont(new_font))
        kover_file.setContentsFont(new_font);
    delete kf;
}

void KoverTop::contentsFontColor()
{
    QColor new_color;
    KColorDialog *kc = new KColorDialog(this, "kc", true);

    new_color = kover_file.contentsColor();
    if (kc->getColor(new_color))
        kover_file.setContentsColor(new_color);
    delete kc;
}

void KoverTop::backgroundColor()
{
    QColor new_color;
    KColorDialog *kc = new KColorDialog(this, "kc", true);

    new_color = kover_file.backColor();
    if (kc->getColor(new_color))
        kover_file.setBackColor(new_color);
    delete kc;
}

void KoverTop::cdrom_eject()
{
    cdrom *cdrom_class = new cdrom(globals.cdrom_device);

    cdrom_class->eject();
    delete cdrom_class;
}

void KoverTop::cddb_without_cd()
{
    int display_track_duration = globals.display_track_duration;

    setStatusText(i18n("Initiating CDDB lookup!"));
    if (altered_data) {
        if (how_about_saving())
            return;
    }

    globals.display_track_duration = 0;
    without_cd *without = new without_cd();

	    if (without->exec()==-1) {
		       delete(without);
		       return;
		    }

    int category = without->get_category();
    char *id = without->get_id();

    if (cddb_fill->execute_without_cd(id, category)) {
        cddb_fill->setTitleAndContents();
        disconnect(contents_edit, SIGNAL(textChanged()), this,
            SLOT(contentsBoxChanged()));
        title_edit->setText(kover_file.title());
        contents_edit->setText(kover_file.contents());
        connect(contents_edit, SIGNAL(textChanged()),
            SLOT(contentsBoxChanged()));
        altered_data = false;
        cddb_fill->cdInfo();
    }
    delete(without);
    free(id);
    globals.display_track_duration = display_track_duration;
}

void KoverTop::more_or_less()
{
    if (more) {
        more = false;
        if (globals.disable_animation)
            main_frame->move(0, 70);
        else {
            for (int i = NORM_WIDTH; i > 0; i--) {
                main_frame->move(i, 70);
                option_frame->move(i - NORM_WIDTH, 70);
                if (i % 10 == 0)
                    kapp->processEvents();
            }
        }
        option_frame->hide();
    } else {
        more = true;
        option_frame->move(-NORM_WIDTH, 70);
        option_frame->show();
        if (globals.disable_animation) {
            option_frame->move(0, 70);
            main_frame->move(NORM_WIDTH, 70);
        } else {
            for (int i = 0; i < NORM_WIDTH; i++) {
                option_frame->move(-NORM_WIDTH + i, 70);
                main_frame->move(i, 70);
                if (i % 10 == 0)
                    kapp->processEvents();
            }
        }
    }
}

void KoverTop::display_title_signal()
{
    kover_file.set_display_title(display_title->isChecked());
}

void KoverTop::spine_text_method()
{
    the_spine_text->setEnabled(spine_text->isChecked());
    kover_file.set_spine_text(spine_text->isChecked());
    if (!kover_file.spine_text())
        the_spine_text->setText(title_edit->text());
    else {
        kover_file.set_the_spine_text(the_spine_text->text());
    }
}

void KoverTop::spine_text_changed_method(const QString & s)
{
    QString bla = s;

    bla = "kk";
    if (kover_file.spine_text())
        kover_file.set_the_spine_text(the_spine_text->text());
}

void KoverTop::file_mode()
{
    filemode *file_mode = new filemode();

    file_mode->exec();
    fprintf(stderr, "%s:%s\n", PACKAGE, file_mode->get(0));
    directory *tmp = new directory("/tmp");

    tmp->make_childs();
    delete file_mode;

    string add = "";

    for (int i = 0; i <= tmp->how_many_files(); i++) {
        //directory *child = tmp->get_childs(i);

        //if (child == NULL)
        //    continue;

        if (!tmp->get_file(i).empty()) {
            add +=
                tmp->get_file(i) + "\n";
        }
    }
    delete tmp;
    contents_edit->setText(add.c_str());

}
