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

#include "kovertop.h"
#include "without_cd.h"
#include <pd.h>
#include <id.h>

#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QKeySequence>
#include <QApplication>
#include <QMenuBar>

#include <KXmlGuiWindow>
#include <KConfigGroup>
#include <KActionCollection>
#include <KMessageBox>
#include <QFontDialog>
#include <QColorDialog>
#include <KStandardShortcut>
#include <KStandardAction>
#include <KStandardGuiItem>
#include <KShortcutsDialog>
#include <KSharedConfig>
#include <KRecentFilesAction>
#include <KToolBar>

#define PREV_WIDTH 695
#define PREV_HEIGHT 684

#include <KLocalizedString>

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


	setWindowTitle(i18n("[New Document]"));

	status_bar = statusBar();
	status_bar->showMessage(QStringLiteral("Kover " K_VERSION " - https://github.com/adrianreber/kover"));

	make_menu();
	make_main_frame();
	make_option_frame();
	make_more_frame();

	connect(&kover_file, &KoverFile::dataChanged, this, &KoverTop::dataChanged);

	cddbfill = new cddb_fill(&kover_file, this);

	altered_data = false;
	more = false;
	actual = false;

	if (globals.save_position)
		move(globals.xpos, globals.ypos);

	recent->loadEntries(KSharedConfig::openConfig()->group(QStringLiteral("RecentFiles")));
	setCentralWidget(centralWidget);
	orig_width = width();
	orig_height = height();

	setupGUI();

	setAutoSaveSettings();

	/* fill with useful default values */
	orig_width = width();
	orig_height = height();
}

KoverTop::~KoverTop()
{
	stopPreview();
	if (globals.save_position) {
		globals.xpos = x();
		globals.ypos = y();
	}

	delete cddbfill;
	delete cdview;
	kprintf("~KoverTop()\n");
}

void
KoverTop::make_menu()
{
	KActionCollection *ac = actionCollection();

	KStandardAction::openNew(this, &KoverTop::fileNew, ac);
	KStandardAction::open(this, static_cast<void (KoverTop::*)()>(&KoverTop::fileOpen), ac);
	KStandardAction::save(this, &KoverTop::fileSave, ac);
	KStandardAction::saveAs(this, &KoverTop::fileSaveAs, ac);
	KStandardAction::print(this, &KoverTop::filePrint, ac);
	KStandardAction::quit(this, &KoverTop::close, ac);
	KStandardAction::cut(this, &KoverTop::cut, ac);
	KStandardAction::copy(this, &KoverTop::copy, ac);
	KStandardAction::paste(this, &KoverTop::paste, ac);
	KStandardAction::preferences(this, &KoverTop::preferences, ac);
	KStandardAction::keyBindings(this, &KoverTop::config_keys, ac);
	recent = KStandardAction::openRecent(this, static_cast<void (KoverTop::*)(const QUrl&)>(&KoverTop::fileOpen), ac);

	QAction *act = ac->addAction(QStringLiteral("cddb"));
	act->setIcon(QIcon::fromTheme(QStringLiteral("network-connect")));
	act->setText(i18n("&CDDB lookup"));
	connect(act, &QAction::triggered, this, &KoverTop::cddbFill);

	act = ac->addAction(QStringLiteral("eject_cdrom"));
	act->setIcon(QIcon::fromTheme(QStringLiteral("media-eject")));
	act->setText(i18n("Eject CD"));
	connect(act, &QAction::triggered, this, &KoverTop::cdrom_eject);

	act = ac->addAction(QStringLiteral("actual_size"));
	act->setIcon(QIcon::fromTheme(QStringLiteral("zoom-in")));
	act->setText(i18n("&Actual size"));
	ac->setDefaultShortcuts(act, KStandardShortcut::zoomIn());
	connect(act, &QAction::triggered, this, &KoverTop::actualSize);

	act = ac->addAction(QStringLiteral("stop_preview"));
	act->setIcon(QIcon::fromTheme(QStringLiteral("zoom-out")));
	act->setText(i18n("Preview size"));
	ac->setDefaultShortcuts(act, QList<QKeySequence>() << QKeySequence(Qt::Key_Escape) << KStandardShortcut::zoomOut());
	connect(act, &QAction::triggered, this, &KoverTop::stopPreview);

	act = ac->addAction(QStringLiteral("image_embedding"));
	act->setIcon(QIcon::fromTheme(QStringLiteral("image-loading")));
	act->setText(i18n("&Image Embedding..."));
	connect(act, &QAction::triggered, this, &KoverTop::imageEmbedding);

	act = ac->addAction(QStringLiteral("title_font"));
	act->setIcon(QIcon::fromTheme(QStringLiteral("preferences-desktop-font")));
	act->setText(i18n("Title Font..."));
	connect(act, &QAction::triggered, this, &KoverTop::titleFont);

	act = ac->addAction(QStringLiteral("contents_font"));
	act->setIcon(QIcon::fromTheme(QStringLiteral("preferences-desktop-font")));
	act->setText(i18n("Contents Font..."));
	connect(act, &QAction::triggered, this, &KoverTop::contentsFont);

	act = ac->addAction(QStringLiteral("inlet_title_font"));
	act->setIcon(QIcon::fromTheme(QStringLiteral("preferences-desktop-font")));
	act->setText(i18n("Spine Text Font..."));
	connect(act, &QAction::triggered, this, &KoverTop::inlet_title_font);

	act = ac->addAction(QStringLiteral("title_font_color"));
	act->setIcon(QIcon::fromTheme(QStringLiteral("color-picker")));
	act->setText(i18n("Title Fontcolor..."));
	connect(act, &QAction::triggered, this, &KoverTop::titleFontColor);

	act = ac->addAction(QStringLiteral("contents_font_color"));
	act->setIcon(QIcon::fromTheme(QStringLiteral("color-picker")));
	act->setText(i18n("Contents Fontcolor..."));
	connect(act, &QAction::triggered, this, &KoverTop::contentsFontColor);

	act = ac->addAction(QStringLiteral("background_color"));
	act->setIcon(QIcon::fromTheme(QStringLiteral("color-picker")));
	act->setText(i18n("Background Fontcolor..."));
	connect(act, &QAction::triggered, this, &KoverTop::backgroundColor);

	act = ac->addAction(QStringLiteral("cddb_without_cd"));
	act->setIcon(QIcon::fromTheme(QStringLiteral("network-connect")));
	act->setText(i18n("CDDB without CD"));
	connect(act, &QAction::triggered, this, &KoverTop::cddb_without_cd);

	act = ac->addAction(QStringLiteral("read_cd_text"));
	act->setIcon(QIcon::fromTheme(QStringLiteral("network-connect")));
	act->setText(i18n("Read CD-TEXT"));
	connect(act, &QAction::triggered, this, &KoverTop::read_cd_text);
}

void
KoverTop::config_keys()
{
	KShortcutsDialog dlg(KShortcutsEditor::AllActions, KShortcutsEditor::LetterShortcutsAllowed, this);
	dlg.addCollection(actionCollection());
	dlg.configure();
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
	connect(title_edit, &QTextEdit::textChanged, this, &KoverTop::titleBoxChanged);

	contents_label = new QLabel(i18n("Contents"), left_frame);
	layout->addWidget(contents_label);
	contents_edit = new QTextEdit(left_frame);
	layout->addWidget(contents_edit);
	connect(contents_edit, &QTextEdit::textChanged, this, &KoverTop::contentsBoxChanged);

	cdview = new CDView(&kover_file, main_frame);
	hlayout->addWidget(cdview);
	cdview->setFixedSize(CDVIEW_WIDTH, CDVIEW_HEIGHT);
	connect(cdview, &CDView::stopPreview, this, &KoverTop::stopPreview);
	connect(cdview, &CDView::actualSize, this, &KoverTop::actualSize);

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
	connect(display_title, &QCheckBox::clicked, this, &KoverTop::display_title_signal);
	kprintf("before display_title\n");
	gbox->addWidget(display_title, 0, 0, 1, 1);

	spine_text = new QCheckBox(i18n("Separate Spine Text"), option_frame);
	kprintf("spine_text display_title\n");
	gbox->addWidget(spine_text, 1, 0, 1, 1);
	connect(spine_text, &QCheckBox::clicked, this, &KoverTop::spine_text_method);

	the_spine_text = new QLineEdit(option_frame);
	kprintf("spine_text display_title\n");
	gbox->addWidget(the_spine_text, 2, 0, 1, 4);
	the_spine_text->setEnabled(false);
	connect(the_spine_text, &QLineEdit::textChanged, this, &KoverTop::spine_text_changed_method);

	number_check = new QCheckBox(i18n("CD Number"), option_frame);
	gbox->addWidget(number_check, 3, 0, 1, 1);
	connect(number_check, &QCheckBox::toggled, this, &KoverTop::numberChecked);

	number_spin = new QSpinBox(option_frame);
	number_spin->setEnabled(false);
	gbox->addWidget(number_spin, 3, 1, 1, 1);
	connect(number_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &KoverTop::numberChanged);

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
	connect(more_button, &QPushButton::clicked, this, &KoverTop::more_or_less);
	button_layout->addStretch(1);
	cddb_id = new QLabel(QString(), more_frame);
	button_layout->addWidget(cddb_id, 0);
}

void
KoverTop::dataChanged(bool /* image */)
{

	setStatusText(i18n("Data changed"));
	if (m_url.isEmpty())
		setWindowTitle(i18n("[New Document]"));
	else
		setWindowTitle(m_url.toString());

	altered_data = true;
}

void
KoverTop::setStatusText(const QString &_status_text)
{
	set_status_text(_status_text);
}

void
KoverTop::set_status_text(const QString &_status_text)
{
	status_bar->showMessage(_status_text);
	QCoreApplication::processEvents();
}

void
KoverTop::update_id(unsigned long id)
{
	QString string;

	if (id != 0)
		string = QString::asprintf("CDDB id: 0x%08lx", id);
	cddb_id->setText(string);
}

void
KoverTop::update_id(QString id)
{
	QString string;

	if (!id.isEmpty())
		string = QStringLiteral("CDDB id: ") + id;
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
	kprintf("begin\n");
	if (more || !actual)
		return;
	cdview->setFixedSize(CDVIEW_WIDTH, CDVIEW_HEIGHT);
	cdview->showPreview(false);
	more_frame->show();
	left_frame->show();
	menuBar()->show();
	statusBar()->show();
	dead_space->show();
	toolBar(QStringLiteral("koverToolBar"))->show();
	toolBar(QStringLiteral("mainToolBar"))->show();
	adjustSize();
	resize(orig_width, orig_height);
	actual = false;
	kprintf("end\n");
}

bool
KoverTop::queryClose()
{
	kprintf("begin\n");

	if (actual) {
		stopPreview();
		return false;
	}

	if (altered_data)
		if (how_about_saving())
			return false;

	recent->saveEntries(KSharedConfig::openConfig()->group(QStringLiteral("RecentFiles")));

	kprintf("end\n");
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
	setStatusText(QStringLiteral("Chop!"));
	altered_data = false;
	m_url = QUrl();
	setWindowTitle(i18n("[New Document]"));
	update_id();
}

void
KoverTop::fileOpen()
{
	if (altered_data) {
		if (how_about_saving())
			return;
	}

	QUrl url = QFileDialog::getOpenFileUrl(this,
					i18n("Open File"),
					QUrl(),
					i18n("Kover files (*.kover);;All files (*)"));

	if (!url.isEmpty()) {
		fileOpen(url);
	}
}

void
KoverTop::fileOpen(const QUrl & url)
{
	if (!url.isEmpty()) {
		if (kover_file.openFile(url)) {
			m_url = url;

			setWindowTitle(m_url.toString());
			disconnect(contents_edit, &QTextEdit::textChanged, this,
				   &KoverTop::contentsBoxChanged);
			title_edit->setText(kover_file.title());
			contents_edit->setText(kover_file.contents());
			connect(contents_edit, &QTextEdit::textChanged,
				this, &KoverTop::contentsBoxChanged);

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
	switch (KMessageBox::warningTwoActionsCancel(this,
						i18n("Data changed since last saving!\nDo you want to save the changes?"),
						QString(),
						KStandardGuiItem::save(),
						KStandardGuiItem::discard()))
	{
	case KMessageBox::PrimaryAction:         /* YES/Save */
		fileSave();
		if (altered_data)
			return -1;
		return 0;
	case KMessageBox::SecondaryAction:         /* NO/Discard */
		return 0;
	case KMessageBox::Cancel:         /* CANCEL */
	default:
		return -1;
	}
}

void
KoverTop::saveFile(const QUrl & url)
{
	if (kover_file.saveFile(url)) {
		setWindowTitle(url.toString());
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
	QUrl url = QFileDialog::getSaveFileUrl(this,
					i18n("Save File"),
					QUrl(),
					i18n("Kover files (*.kover);;All files (*)"));

	if (!url.isEmpty())
		saveFile(url);
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
	if (more || actual)
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
	toolBar(QStringLiteral("mainToolBar"))->hide();
	toolBar(QStringLiteral("koverToolBar"))->hide();
	actual = true;
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
		disconnect(contents_edit, &QTextEdit::textChanged, this,
			   &KoverTop::contentsBoxChanged);
		title_edit->setText(kover_file.title());
		contents_edit->setText(kover_file.contents());
		connect(contents_edit, &QTextEdit::textChanged,
			this, &KoverTop::contentsBoxChanged);
		altered_data = false;
		cddbfill->get_info();
	}
}

void
KoverTop::preferences()
{
	pd *dialog = NULL;

	if (kover_file.empty())
		dialog = new pd(this);
	else
		dialog = new pd(this, true);

	if (dialog->exec())
		cdview->dataChanged(true);
	delete dialog;

	if (!altered_data) {
		if (kover_file.empty()) {
			kover_file.reset();
			altered_data = false;
			setWindowTitle(i18n("[New Document]"));
		}
	}
}

QFont *
KoverTop::font_dialog(QFont *f)
{
	kprintf("font name before: %s\n", f->family().toUtf8().constData());

	// Ensure font has valid point size before opening dialog
	if (f->pointSize() <= 0 && f->pointSizeF() <= 0) {
		f->setPointSize(12);  // Use reasonable default
	}

	QFontDialog dialog;
	dialog.setOption(QFontDialog::DontUseNativeDialog);
	dialog.setCurrentFont(*f);
	if (dialog.exec() == QDialog::Accepted) {
		*f = dialog.selectedFont();
		return f;
	}

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
	id *dialog = new id(&kover_file);

	if (dialog->exec())
		cdview->dataChanged(true);
	delete dialog;
}

void
KoverTop::titleFontColor()
{
	QColorDialog dialog;
	dialog.setOption(QColorDialog::DontUseNativeDialog);
	dialog.setCurrentColor(kover_file.titleColor());
	if (dialog.exec() == QDialog::Accepted)
		kover_file.setTitleColor(dialog.selectedColor());
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
	QColorDialog dialog;
	dialog.setOption(QColorDialog::DontUseNativeDialog);
	dialog.setCurrentColor(kover_file.contentsColor());
	if (dialog.exec() == QDialog::Accepted)
		kover_file.setContentsColor(dialog.selectedColor());
}

void
KoverTop::backgroundColor()
{
	QColorDialog dialog;
	dialog.setOption(QColorDialog::DontUseNativeDialog);
	dialog.setCurrentColor(kover_file.backColor());
	if (dialog.exec() == QDialog::Accepted)
		kover_file.setBackColor(dialog.selectedColor());
}

void
KoverTop::cdrom_eject()
{
	eject_cdrom();
}

void
KoverTop::cddb_without_cd()
{
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
		disconnect(contents_edit, &QTextEdit::textChanged, this,
			   &KoverTop::contentsBoxChanged);
		title_edit->setText(kover_file.title());
		contents_edit->setText(kover_file.contents());
		connect(contents_edit, &QTextEdit::textChanged,
			this, &KoverTop::contentsBoxChanged);
		altered_data = false;
		cddbfill->get_info();
	}
	delete (without);
	free(id);
	globals.display_track_duration = display_track_duration;
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
		main_frame->hide();
		option_frame->show();
		dead_space->resize(0, height());
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

	bla = QStringLiteral("kk");
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
		/* directory *child = tmp->get_childs(i); */

		/* if (child == NULL) */
		/*    continue; */

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
		disconnect(contents_edit, &QTextEdit::textChanged, this,
			   &KoverTop::contentsBoxChanged);
		title_edit->setText(kover_file.title());
		contents_edit->setText(kover_file.contents());
		connect(contents_edit, &QTextEdit::textChanged,
			this, &KoverTop::contentsBoxChanged);
		altered_data = false;
		globals.display_track_duration = i;
		setStatusText(i18n("Trying to read CD-TEXT succeeded!"));
		update_id(QString());
	}
}
