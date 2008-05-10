/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 1999, 2000 by Denis Oliver Kropp
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

#include "kovertop.moc"
#include "imagedlg.h"
#include "without_cd.h"
#include <pd.h>

#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>

#include <kxmlguiwindow.h>
#include <kconfigskeleton.h>
#include <kactioncollection.h>
#include <kaction.h>
#include <kmenubar.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kfontdialog.h>
#include <kcolordialog.h>
#include <kstandardshortcut.h>
#include <kapplication.h>
#include <ktoolbar.h>
#include <kstatusbar.h>
#include <kshortcutsdialog.h>

#define PREV_WIDTH 695
#define PREV_HEIGHT 684

#define i18n(x) x

#define CDVIEW_WIDTH    291
#define CDVIEW_HEIGHT 310

KoverTop::KoverTop() : KXmlGuiWindow()
{
	centralWidget = new QWidget(this);
	QVBoxLayout *vlayout = new QVBoxLayout();
	centralWidget->setLayout(vlayout);

	main_frame = new QWidget(centralWidget);
	vlayout->addWidget(main_frame);

	option_frame = new QGroupBox(centralWidget);
	vlayout->addWidget(option_frame);

	dead_space = new QFrame(centralWidget);
	vlayout->addWidget(dead_space);
	more_frame = new QFrame(centralWidget);
	vlayout->addWidget(more_frame);
	option_frame->setTitle(tr("All these options are not global"));


	setCaption(i18n("[New Document]"), false);

	status_bar = statusBar();
	status_bar->insertItem("Kover " VERSION " - http://lisas.de/kover/", 1);

	make_menu();
	make_main_frame();
	make_option_frame();
	make_more_frame();

	connect(&kover_file, SIGNAL(dataChanged(bool)), SLOT(dataChanged(bool)));

	cddbfill = new cddb_fill(&kover_file, this);

	altered_data = false;
	more = false;

	if (globals.save_position)
		move(globals.xpos, globals.ypos);

	recent->loadEntries(KGlobal::config()->group("RecentFiles"));
	setCentralWidget(centralWidget);
	orig_width = width();
	orig_height = height();

	createGUI();

	setAutoSaveSettings();
}

KoverTop::~KoverTop()
{
	recent->saveEntries(KGlobal::config()->group("RecentFiles"));
	delete status_bar;
	delete cddbfill;
	delete cdview;
	kprintf("~KoverTop()\n");
}

void
KoverTop::make_menu()
{
	KActionCollection *ac = actionCollection();

	KStandardAction::openNew(this, SLOT(fileNew()), ac);
	KStandardAction::open(this, SLOT(fileOpen()), ac);
	KStandardAction::save(this, SLOT(fileSave()), ac);
	KStandardAction::saveAs(this, SLOT(fileSaveAs()), ac);
	KStandardAction::print(this, SLOT(filePrint()), ac);
	KStandardAction::quit(this, SLOT(close()), ac);
	KStandardAction::cut(this, SLOT(cut()), ac);
	KStandardAction::copy(this, SLOT(copy()), ac);
	KStandardAction::paste(this, SLOT(paste()), ac);
	KStandardAction::preferences(this, SLOT(preferences()), ac);
	KStandardAction::keyBindings(this, SLOT(config_keys()), ac);
	recent = KStandardAction::openRecent(this, SLOT(fileOpen(KUrl)), ac);

	KAction *act = new KAction(KIcon("network-connect"), i18n("&CDDB lookup"), ac);
	ac->addAction("cddb", act);
	connect(act, SIGNAL(triggered(bool)), SLOT(cddbFill()));

	act = new KAction(KIcon("media-eject"), i18n("Eject CD"), ac);
	ac->addAction("eject_cdrom", act);
	connect(act, SIGNAL(triggered(bool)), SLOT(cdrom_eject()));

	act = new KAction(KIcon("zoom-in"), i18n("&Actual size"), ac);
	ac->addAction("actual_size", act);
	act->setShortcut(KStandardShortcut::zoomIn());
	connect(act, SIGNAL(triggered(bool)), SLOT(actualSize()));

	act = new KAction(KIcon("zoom-out"), i18n("Preview size"), ac);
	ac->addAction("stop_preview", act);
	act->setShortcut(KStandardShortcut::zoomOut());
	connect(act, SIGNAL(triggered(bool)), SLOT(stopPreview()));

	act = new KAction(KIcon("image-loading"), i18n("&Image Embedding..."), ac);
	ac->addAction("image_embedding", act);
	connect(act, SIGNAL(triggered(bool)), SLOT(imageEmbedding()));

	act = new KAction(KIcon("fonts-package"), i18n("Title Font..."), ac);
	ac->addAction("title_font", act);
	connect(act, SIGNAL(triggered(bool)), SLOT(titleFont()));

	act = new KAction(KIcon("fonts-package"), i18n("Contents Font..."), ac);
	ac->addAction("contents_font", act);
	connect(act, SIGNAL(triggered(bool)), SLOT(contentsFont()));

	act = new KAction(KIcon("fonts-package"), i18n("Spine Text Font..."), ac);
	ac->addAction("inlet_title_font", act);
	connect(act, SIGNAL(triggered(bool)), SLOT(inlet_title_font()));

	act = new KAction(KIcon("color-picker"), i18n("Title Fontcolor..."), ac);
	ac->addAction("title_font_color", act);
	connect(act, SIGNAL(triggered(bool)), SLOT(titleFontColor()));

	act = new KAction(KIcon("color-picker"), i18n("Contents Fontcolor..."), ac);
	ac->addAction("contents_font_color", act);
	connect(act, SIGNAL(triggered(bool)), SLOT(contentsFontColor()));

	act = new KAction(KIcon("color-picker"), i18n("Background Fontcolor..."), ac);
	ac->addAction("background_color", act);
	connect(act, SIGNAL(triggered(bool)), SLOT(backgroundColor()));

	act = new KAction(KIcon("network-connect"), i18n("CDDB without CD"), ac);
	ac->addAction("cddb_without_cd", act);
	connect(act, SIGNAL(triggered(bool)), SLOT(cddb_without_cd()));

	act = new KAction(KIcon("network-connect"), i18n("Read CD-TEXT"), ac);
	ac->addAction("read_cd_text", act);
	connect(act, SIGNAL(triggered(bool)), SLOT(read_cd_text()));
}

void
KoverTop::config_keys()
{
	KShortcutsDialog::configure(actionCollection());
}

void
KoverTop::make_main_frame()
{
	QHBoxLayout *hlayout = new QHBoxLayout();
	main_frame->setLayout(hlayout);
	left_frame = new QWidget();
	hlayout->addWidget(left_frame);

	QVBoxLayout *layout = new QVBoxLayout();
	left_frame->setLayout(layout);


	title_label = new QLabel(i18n("Title"), left_frame);
	layout->addWidget(title_label);

	title_edit = new QTextEdit(left_frame);
	layout->addWidget(title_edit);
	connect(title_edit, SIGNAL(textChanged()), SLOT(titleBoxChanged()));

	contents_label = new QLabel(i18n("Contents"), left_frame);
	layout->addWidget(contents_label);
	contents_edit = new QTextEdit(left_frame);
	layout->addWidget(contents_edit);
	connect(contents_edit, SIGNAL(textChanged()), SLOT(contentsBoxChanged()));

	cdview = new CDView(&kover_file, main_frame);
	hlayout->addWidget(cdview);
	cdview->setFixedSize(CDVIEW_WIDTH, CDVIEW_HEIGHT);
	connect(cdview, SIGNAL(stopPreview()), SLOT(stopPreview()));
	connect(cdview, SIGNAL(actualSize()), SLOT(actualSize()));

	title_edit->setFocus();
}

void
KoverTop::make_option_frame()
{
	QVBoxLayout *vlay = new QVBoxLayout(option_frame);

	vlay->addSpacing(fontMetrics().lineSpacing());

	QGridLayout *gbox = new QGridLayout();
	vlay->addLayout(gbox);

	display_title = new QCheckBox(tr("No title on booklet"), option_frame);
	connect(display_title, SIGNAL(clicked()), SLOT(display_title_signal()));
	kprintf("before display_title\n");
	gbox->addWidget(display_title, 0, 0, 1, 1);

	spine_text = new QCheckBox(i18n("Separate Spine Text"), option_frame);
	kprintf("spine_text display_title\n");
	gbox->addWidget(spine_text, 1, 0, 1, 1);
	connect(spine_text, SIGNAL(clicked()), SLOT(spine_text_method()));

	the_spine_text = new QLineEdit(option_frame);
	kprintf("spine_text display_title\n");
	gbox->addWidget(the_spine_text, 2, 0, 1, 4);
	the_spine_text->setEnabled(false);
	connect(the_spine_text, SIGNAL(textChanged(const QString &)),
		SLOT(spine_text_changed_method(const QString &)));

	number_check = new QCheckBox(i18n("CD Number"), option_frame);
	gbox->addWidget(number_check, 3, 0, 1, 1);
	connect(number_check, SIGNAL(toggled(bool)), SLOT(numberChecked(bool)));

	number_spin = new QSpinBox(option_frame);
	number_spin->setEnabled(false);
	gbox->addWidget(number_spin, 3, 1, 1, 1);
	connect(number_spin, SIGNAL(valueChanged(int)), SLOT(numberChanged(int)));

	option_frame->hide();
}

void
KoverTop::make_more_frame()
{
	button_layout = new QBoxLayout(QBoxLayout::RightToLeft);
	button_layout->setAlignment(Qt::AlignTop);
	more_frame->setLayout(button_layout);
	more_button = new QPushButton(i18n("Options"), more_frame);
	button_layout->addWidget(more_button, 0);
	connect(more_button, SIGNAL(clicked()), SLOT(more_or_less()));
	button_layout->addStretch(1);
	cddb_id = new QLabel("", more_frame);
	button_layout->addWidget(cddb_id, 0);
}

void
KoverTop::dataChanged(bool image)
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

void
KoverTop::setStatusText(const char *_status_text)
{
	set_status_text(_status_text);
}

void
KoverTop::set_status_text(const char *_status_text)
{
	status_bar->changeItem(_status_text, 1);
	kapp->processEvents();
}

void
KoverTop::update_id(unsigned long id)
{
	QString string = "";

	if (id != 0)
		string.sprintf("CDDB id: 0x%08lx", id);
	cddb_id->setText(string);
}

void
KoverTop::update_id(QString id)
{
	QString string = "";

	if (!id.isEmpty())
		string = "CDDB id: " + id;
	cddb_id->setText(string);
}

void
KoverTop::titleBoxChanged()
{
	kover_file.setTitle(title_edit->toPlainText());
	if (!kover_file.spine_text())
		the_spine_text->setText(title_edit->toPlainText());
}

void
KoverTop::contentsBoxChanged()
{
	kover_file.setContents(contents_edit->toPlainText());
}

void
KoverTop::numberChanged(int value)
{
	kover_file.setNumber(value);
}

void
KoverTop::numberChecked(bool checked)
{
	number_spin->setEnabled(checked);

	if (checked)
		kover_file.setNumber(number_spin->value());
	else
		kover_file.setNumber(0);
}

void
KoverTop::stopPreview()
{
	cdview->setFixedSize(CDVIEW_WIDTH, CDVIEW_HEIGHT);
	cdview->showPreview(false);
	more_frame->show();
	left_frame->show();
	menuBar()->show();
	statusBar()->show();
	dead_space->show();
	toolBar("koverToolBar")->show();
	toolBar("mainToolBar")->show();
	adjustSize();
	resize(orig_width, orig_height);
}

bool
KoverTop::queryClose()
{
	if (altered_data) {
		if (how_about_saving())
			return false;
	}
	return true;
}

void
KoverTop::fileNew()
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
	m_url = KUrl();
	setCaption(i18n("[New Document]"), false);
	update_id();
}

void
KoverTop::fileOpen()
{
	if (altered_data) {
		if (how_about_saving())
			return;
	}

	KUrl url =
		KFileDialog::getOpenUrl(KUrl(),
					i18n(
						"*.kover|Kover files\n*|All files\n*.k3b|k3b files"));

	if (!url.isEmpty()) {
		fileOpen(url);
	}
}

void
KoverTop::fileOpen(const KUrl & url)
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
				the_spine_text->setText(title_edit->toPlainText());
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
			recent->addUrl(url);
		} else
			KMessageBox::error(this,
					   i18n(
						   "Error while opening/reading file!"));
	}
}

int
KoverTop::how_about_saving()
{
	switch (KMessageBox::warningYesNoCancel(this,
						i18n
						(
							"Data changed since last saving!\nDo you want to save the changes?")))
	{
	case 3:         //YES
		fileSave();
		if (altered_data)
			return -1;
		return 0;
	case 4:         //NO
		return 0;
	case 2:         //CANCEL
		return -1;
	}
	return -1;
}

void
KoverTop::saveFile(const KUrl & url)
{
	if (kover_file.saveFile(url)) {
		setCaption(i18n(url.url()), false);
		setStatusText(i18n("File saved"));
		altered_data = false;
		recent->addUrl(url);
		m_url = url;
	} else
		KMessageBox::error(this, i18n("Error while opening/reading file!"));
}

void
KoverTop::fileSave()
{
	if (m_url.isEmpty())
		fileSaveAs();
	else
		saveFile(m_url);
}

void
KoverTop::fileSaveAs()
{
	KUrl url =
		KFileDialog::getSaveUrl(KUrl(),
					i18n("*.kover|Kover files\n*|All files"));

	if (!url.isEmpty()) {
		if (url.fileName().contains('.'))
			url.setFileName(url.fileName() + ".kover");
		saveFile(url);
	}
}

void
KoverTop::filePrint()
{
	cdview->printKover();
	kprintf("Printing done\n");
}

void
KoverTop::cut()
{
	if (title_edit->hasFocus())
		title_edit->cut();
	if (contents_edit->hasFocus())
		contents_edit->cut();
}

void
KoverTop::copy()
{
	if (title_edit->hasFocus())
		title_edit->copy();
	if (contents_edit->hasFocus())
		contents_edit->copy();
}

void
KoverTop::paste()
{
	if (title_edit->hasFocus())
		title_edit->paste();
	if (contents_edit->hasFocus())
		contents_edit->paste();
}

void
KoverTop::actualSize()
{
	if (more)
		return;
	orig_width = width();
	orig_height = height();
	cdview->setFixedSize(PREV_WIDTH, PREV_HEIGHT);
	cdview->showPreview();
	cdview->setFocus();
	more_frame->hide();
	left_frame->hide();
	menuBar()->hide();
	statusBar()->hide();
	dead_space->hide();
	toolBar("mainToolBar")->hide();
	toolBar("koverToolBar")->hide();
}

void
KoverTop::cddbFill()
{
	setStatusText(i18n("Initiating CDDB lookup!"));
	if (altered_data) {
		if (how_about_saving())
			return;
	}

	if (cddbfill->execute()) {
		cddbfill->setTitleAndContents();
		disconnect(contents_edit, SIGNAL(textChanged()), this,
			   SLOT(contentsBoxChanged()));
		title_edit->setText(kover_file.title());
		contents_edit->setText(kover_file.contents());
		connect(contents_edit, SIGNAL(textChanged()),
			SLOT(contentsBoxChanged()));
		altered_data = false;
		cddbfill->get_info();
	}
}

void
KoverTop::preferences()
{
	KConfigSkeleton* cs = new KConfigSkeleton();
	pd *dialog = NULL;

	if (kover_file.empty())
		dialog = new pd(this, cs);
	else
		dialog = new pd(this, cs, true);

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

QFont *
KoverTop::font_dialog(QFont *f)
{
	KFontDialog kf;

	kprintf("font name before: %s\n", f->family().toUtf8().constData());

	kf.setFont(*f);
	if (kf.getFont(*f))
		return f;

	return NULL;
}

void
KoverTop::titleFont()
{
	QFont *font2 = NULL;
	QFont font = kover_file.titleFont();

	font2 = font_dialog(&font);
	if (font2)
		kover_file.setTitleFont(*font2);
}

void
KoverTop::inlet_title_font()
{
	QFont *font2 = NULL;
	QFont font = kover_file.inlet_title_font();

	font2 = font_dialog(&font);
	if (font2)
		kover_file.set_inlet_title_font(*font2);
}

void
KoverTop::imageEmbedding()
{
	ImageDlg *imgdlg = new ImageDlg(&kover_file);

	if (imgdlg->exec())
		cdview->dataChanged(true);
	delete imgdlg;
}

void
KoverTop::titleFontColor()
{
	QColor new_color;
	KColorDialog *kc = new KColorDialog(this, true);

	new_color = kover_file.titleColor();
	if (kc->getColor(new_color))
		kover_file.setTitleColor(new_color);
	delete kc;
}

void
KoverTop::contentsFont()
{
	QFont *font2 = NULL;
	QFont font = kover_file.contentsFont();

	font2 = font_dialog(&font);
	if (font2)
		kover_file.setContentsFont(*font2);
}

void
KoverTop::contentsFontColor()
{
	QColor new_color;
	KColorDialog *kc = new KColorDialog(this, true);

	new_color = kover_file.contentsColor();
	if (kc->getColor(new_color))
		kover_file.setContentsColor(new_color);
	delete kc;
}

void
KoverTop::backgroundColor()
{
	QColor new_color;
	KColorDialog *kc = new KColorDialog(this, true);

	new_color = kover_file.backColor();
	if (kc->getColor(new_color))
		kover_file.setBackColor(new_color);
	delete kc;
}

void
KoverTop::cdrom_eject()
{
	eject_cdrom();
}

void
KoverTop::cddb_without_cd()
{
#if 0
	int display_track_duration = globals.display_track_duration;

	setStatusText(i18n("Initiating CDDB lookup!"));
	if (altered_data) {
		if (how_about_saving())
			return;
	}

	globals.display_track_duration = 0;
	without_cd *without = new without_cd();

	if (without->exec() == -1) {
		delete (without);
		return;
	}

	int category = without->get_category();
	char *id = without->get_id();

	if (cddbfill->execute_without_cd(id, category)) {
		cddbfill->setTitleAndContents();
		disconnect(contents_edit, SIGNAL(textChanged()), this,
			   SLOT(contentsBoxChanged()));
		title_edit->setText(kover_file.title());
		contents_edit->setText(kover_file.contents());
		connect(contents_edit, SIGNAL(textChanged()),
			SLOT(contentsBoxChanged()));
		altered_data = false;
		cddbfill->get_info();
	}
	delete (without);
	free(id);
	globals.display_track_duration = display_track_duration;
#endif
}

void
KoverTop::more_or_less()
{
	more_frame->setMaximumHeight(more_frame->height());
	if (more) {
		more = false;
		dead_space->resize(0, 0);
		option_frame->hide();
		main_frame->show();
	} else {
		more = true;
		option_frame->show();
		dead_space->resize(0, height());
		main_frame->hide();
	}
}

void
KoverTop::display_title_signal()
{
	kover_file.set_display_title(display_title->isChecked());
}

void
KoverTop::spine_text_method()
{
	the_spine_text->setEnabled(spine_text->isChecked());
	kover_file.set_spine_text(spine_text->isChecked());
	if (!kover_file.spine_text())
		the_spine_text->setText(title_edit->toPlainText());
	else {
		kover_file.set_the_spine_text(the_spine_text->text());
	}
}

void
KoverTop::spine_text_changed_method(const QString & s)
{
	QString bla = s;

	bla = "kk";
	if (kover_file.spine_text())
		kover_file.set_the_spine_text(the_spine_text->text());
}

void
KoverTop::file_mode()
{
#if 0
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
			add += tmp->get_file(i) + "\n";
		}
	}
	delete tmp;
	contents_edit->setText(add.c_str());
#endif
}

void
KoverTop::read_cd_text()
{
	setStatusText(i18n("Trying to read CD-TEXT!"));
	if (altered_data) {
		if (how_about_saving())
			return;
	}

	if (cddbfill->read_cdtext()) {
		int i = globals.display_track_duration;
		globals.display_track_duration = 0;
		cddbfill->setTitleAndContents();
		disconnect(contents_edit, SIGNAL(textChanged()), this,
			   SLOT(contentsBoxChanged()));
		title_edit->setText(kover_file.title());
		contents_edit->setText(kover_file.contents());
		connect(contents_edit, SIGNAL(textChanged()),
			SLOT(contentsBoxChanged()));
		altered_data = false;
		globals.display_track_duration = i;
		setStatusText(i18n("Trying to read CD-TEXT succeeded!"));
		update_id("");
	}
}
