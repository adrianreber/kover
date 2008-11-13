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

using namespace std;

#include "koverfile.moc"

#include <kover.h>
#include <iostream>
#include <qdom.h>
#include <qfile.h>
#include <qregexp.h>
#include <qfileinfo.h>
#include <kurl.h>
#include <ktemporaryfile.h>
#include <kio/netaccess.h>
#include <kconfiggroup.h>

KoverFile::KoverFile()
{
	reset();
}

KoverFile::~KoverFile()
{
}

void
KoverFile::reset()
{
	cd_title = "";
	cd_title_font = QFont(*globals.title_font);
	cd_contents = "";
	cd_contents_font = QFont(*globals.content_font);
	cd_inlet_title_font = QFont(*globals.inlet_title_font);
	cd_title_color = Qt::black;
	cd_contents_color = Qt::black;
	cd_back_color = Qt::white;
	cd_number = 0;
	cd_cddb_id = "";
	cd_image_file[0] = "";
	cd_image_mode[0] = IMG_CENTER;
	cd_image_target[0] = IMG_FRONT_LEFT;
	cd_image_file[1] = "";
	cd_image_mode[1] = IMG_CENTER;
	cd_image_target[1] = IMG_FRONT_LEFT;
	cd_image_file[2] = "";
	cd_image_mode[2] = IMG_CENTER;
	cd_image_target[2] = IMG_FRONT_LEFT;
	cd_display_title = false;
	cd_spine_text = false;
	cd_the_spine_text = "";

	emit dataChanged();
}

bool
KoverFile::empty()
{
	if (cd_title == "" && cd_contents == "" && cd_the_spine_text == "")
		return true;
	else
		return false;
}

void
KoverFile::setTitle(const QString & _title)
{
	if (cd_title != _title) {
		cd_title = _title;
		cd_title.replace(QRegExp("\r"), "\n");
		emit dataChanged();
	}
}

void
KoverFile::setContents(const QString & _contents)
{
	if (cd_contents != _contents) {
		cd_contents = _contents;
		cd_contents.replace(QRegExp("\r"), "\n");
		emit dataChanged();
	}
}

void
KoverFile::setTitleFont(const QFont & _title_font)
{
	if (cd_title_font != _title_font) {
		cd_title_font = _title_font;
		emit dataChanged();
	}
}

void
KoverFile::setTitleColor(const QColor & _title_color)
{
	if (cd_title_color != _title_color) {
		cd_title_color = _title_color;
		emit dataChanged();
	}
}

void
KoverFile::setContentsFont(const QFont & _contents_font)
{
	if (cd_contents_font != _contents_font) {
		cd_contents_font = _contents_font;
		emit dataChanged();
	}
}

void
KoverFile::set_inlet_title_font(const QFont & _inlet_title_font)
{
	if (cd_inlet_title_font != _inlet_title_font) {
		cd_inlet_title_font = _inlet_title_font;
		emit dataChanged();
	}
}

void
KoverFile::setContentsColor(const QColor & _contents_color)
{
	if (cd_contents_color != _contents_color) {
		cd_contents_color = _contents_color;
		emit dataChanged();
	}
}

void
KoverFile::setBackColor(const QColor & _back_color)
{
	if (cd_back_color != _back_color) {
		cd_back_color = _back_color;
		emit dataChanged();
	}
}

void
KoverFile::setNumber(const int _number)
{
	if (cd_number != _number) {
		cd_number = _number;
		emit dataChanged();
	}
}

void
KoverFile::setImageFile(const int _nr, const QString & _image_file)
{
	if (cd_image_file[_nr] != _image_file) {
		cd_image_file[_nr] = _image_file;
		emit dataChanged(true);
	}
}

void
KoverFile::setImageMode(const int _nr, const int _image_mode)
{
	if (cd_image_mode[_nr] != _image_mode) {
		cd_image_mode[_nr] = _image_mode;
		emit dataChanged(true);
	}
}

void
KoverFile::setImageTarget(const int _nr, const int _image_target)
{
	if (cd_image_target[_nr] != _image_target) {
		cd_image_target[_nr] = _image_target;
		emit dataChanged(true);
	}
}

void
KoverFile::set_display_title(bool title)
{
	cd_display_title = title;
	emit dataChanged();
}

bool
KoverFile::display_title() const
{
	return cd_display_title;
}

QString
KoverFile::title() const
{
	return cd_title;
}

QString
KoverFile::contents() const
{
	return cd_contents;
}

QFont
KoverFile::titleFont() const
{
	return cd_title_font;
}

QColor
KoverFile::titleColor() const
{
	return cd_title_color;
}

QFont
KoverFile::contentsFont() const
{
	return cd_contents_font;
}

QFont
KoverFile::inlet_title_font() const
{
	return cd_inlet_title_font;
}

QColor
KoverFile::contentsColor() const
{
	return cd_contents_color;
}

QColor
KoverFile::backColor() const
{
	return cd_back_color;
}

int
KoverFile::number() const
{
	return cd_number;
}

QString
KoverFile::imageFile(const int _nr) const
{
	kprintf("KoverFile::imageFile():%s\n",
		cd_image_file[_nr].toUtf8().constData());
	return cd_image_file[_nr];
}

int
KoverFile::imageMode(const int _nr) const
{
	return cd_image_mode[_nr];
}

int
KoverFile::imageTarget(const int _nr) const
{
	return cd_image_target[_nr];
}

void
KoverFile::set_spine_text(bool bla)
{
	cd_spine_text = bla;
	emit dataChanged();
}

bool
KoverFile::spine_text() const
{
	return cd_spine_text;
}

QString
KoverFile::the_spine_text() const
{
	return cd_the_spine_text;
}

void
KoverFile::set_the_spine_text(const QString & text)
{
	if (cd_the_spine_text != text) {
		cd_the_spine_text = text;
		emit dataChanged();
	}
}

QString
KoverFile::cddb_id() const
{
	return cd_cddb_id;
}

void
KoverFile::set_cddb_id(const QString & text)
{
	if (cd_cddb_id != text) {
		cd_cddb_id = text;
		emit dataChanged();
	}
}

bool
KoverFile::checkForECD(QString & filename)
{
	QFile ecd_file(filename);

	if (!ecd_file.open(QIODevice::ReadOnly))
		return false;

	if ((ecd_file.read(8) == "DCEi20RP")) {
		ecd_file.close();
		return true;
	}

	ecd_file.close();
	return false;
}

bool
KoverFile::openECD(QString & filename)
{
	QFileInfo fi(filename);

	if (!fi.exists())
		return false;
	if (!fi.isReadable())
		return false;
	if (fi.isDir())
		return false;

	char c;
	long i, length;
	QFile ecd_file(filename);

	if (!ecd_file.open(QIODevice::ReadOnly))
		return false;

	/* ID */
	ecd_file.seek(8);

	/* Title */
	cd_title = "";
	length = (long)ecd_file.read(1).constData();
	ecd_file.seek(1);
	for (i = 0; i < length; i++) {
		c = (long)ecd_file.read(1).constData();
		ecd_file.seek(1);
		if ((c != 10) && (c != 13))
			cd_title += c;
		else
			cd_title += 32;
	}

	/* Background image */
	length = (long)ecd_file.read(1).constData();
	ecd_file.seek(1);
	ecd_file.seek(ecd_file.pos() + length);

	/* Foreground image */
	length = (long)ecd_file.read(1).constData();
	ecd_file.seek(1);
	ecd_file.seek(ecd_file.pos() + length);

	/* Font 1, Data */
	ecd_file.seek(ecd_file.pos() + 28);

	/* Font 1, Name */
	length = (long)ecd_file.read(1).constData();
	ecd_file.seek(1);
	ecd_file.seek(ecd_file.pos() + length);

	/* Font 2, Data */
	ecd_file.seek(ecd_file.pos() + 28);

	/* Font 2, Name */
	length = (long)ecd_file.read(1).constData();
	ecd_file.seek(1);
	ecd_file.seek(ecd_file.pos() + length);

	/* Unknown */
	ecd_file.seek(ecd_file.pos() + 22);

	/* Contents */
	cd_contents = "";
	length = (long)ecd_file.read(1).constData();
	ecd_file.seek(1);
	if (length == 0xff) {
		length = (long)ecd_file.read(1).constData();
		ecd_file.seek(1);
		length += (long)ecd_file.read(1).constData() << 8;
		ecd_file.seek(1);
	}

	for (i = 0; i < length; i++) {
		c = (long)ecd_file.read(1).constData();
		ecd_file.seek(1);
		if (c != 13)
			cd_contents += c;
	}

	ecd_file.close();

	emit dataChanged();

	return true;
}

bool
KoverFile::openFile(const KUrl & url)
{
	QString filename;
	QString tempFile;
	bool retval = false;

	filename = url.path();

	QFileInfo fi(filename);

	if (!fi.exists() || !fi.isReadable() || fi.isDir()) {
		return false;
	}
	/* check for ECD */
	if (checkForECD(filename)) {
		kprintf("even older format; opening ECD file\n");
		retval = openECD(filename);
		emit dataChanged(true);

		return retval;
	}
	/* check for xml */
	QDomDocument doc("kover");
	QFile f(filename);

	if (!f.open(QIODevice::ReadOnly)) {
		return false;
	}
	/* this is for the k3b files */
	/* unfortunately they changed to zipped format */
	/* that's why i had to include this code and */
	/* the kostore classes from koffice */
#if 0
	KoStore *store = KoStore::createStore(filename, KoStore::Read);

	if (store) {
		if (!store->bad()) {
			if (store->open("maindata.xml")) {
				_DEBUG_ fprintf(
					stderr,
					"%s:this should be a kostore file\n",
					PACKAGE);
				bool answer = false;
				QIODevice *dev = store->device();

				dev->open(QIODevice::ReadOnly);
				if (doc.setContent(dev))
					answer = true;
				dev->close();
				if (answer)
					answer = open_XML(doc);
				else
					answer = false;
				store->close();
				delete store;
				emit dataChanged(true);
				return answer;
			}
		}
	}
	delete store;
#endif
	if (doc.setContent(&f)) {
		f.close();
		kprintf("must be one of the new XML files\n");
		retval = open_XML(filename);
		emit dataChanged(true);

		return retval;
	} else {
		f.close();
		return false;
	}

	emit dataChanged(true);

	if (!url.isLocalFile())
		KIO::NetAccess::removeTempFile(tempFile);

	return true;
}

bool
KoverFile::saveFile(const KUrl & url)
{
	QString filename;
	KTemporaryFile tempFile;

	tempFile.setAutoRemove(true);

	if (!url.isLocalFile())
		filename = tempFile.fileName();
	else
		filename = url.path();

	QFileInfo fi(filename);

	if (fi.exists() && !fi.isWritable())
		return false;
	if (fi.isDir())
		return false;

	if (fi.absolutePath() == "")
		return false;
	fi.setFile(fi.absoluteFilePath());
	if (fi.exists() && !fi.isWritable())
		return false;

	if (!save_as_XML(filename))
		return false;

	return true;
}

bool
KoverFile::save_as_XML(const QString & filename)
{
	QDomElement text;
	QDomElement img;
	QDomText the_text;
	QStringList list;

	QDomDocument doc("kover");

	QDomElement root = doc.documentElement();

	QDomElement kover = doc.createElement("kover");

	kover.setAttribute("name", PACKAGE);
	kover.setAttribute("version", VERSION);
	doc.appendChild(kover);

	/* comment */
	QDomComment comment = doc.createComment("This file was created by "
						PACKAGE " " VERSION);
	kover.appendChild(comment);

	/* title */
	QDomElement title = doc.createElement("title");

	title.setAttribute("color", cd_title_color.name());
	title.setAttribute("font", cd_title_font.family());
	title.setAttribute("size", cd_title_font.pointSize());
	title.setAttribute("weight", cd_title_font.weight());
	title.setAttribute("italic", cd_title_font.italic());
	title.setAttribute("display", cd_display_title);
	title.setAttribute("font_settings", cd_title_font.toString());
	kover.appendChild(title);

	/* replace carriage return with newline */
	cd_title.replace(QRegExp("\r"), "\n");
	cd_contents.replace(QRegExp("\r"), "\n");

	/* title text */
	list = cd_title.split("\n");

	for (QStringList::Iterator it = list.begin(); it != list.end(); ++it) {
		text = doc.createElement("text");
		title.appendChild(text);
		the_text = doc.createCDATASection((*it).toUtf8());
		text.appendChild(the_text);
	}

	for (int i = 0; i < 3; i++) {
		img = doc.createElement("img");
		img.setAttribute("src", cd_image_file[i]);
		img.setAttribute("mode", cd_image_mode[i]);
		img.setAttribute("target", cd_image_target[i]);
		kover.appendChild(img);
	}

	/* general */
	QDomElement general = doc.createElement("general");

	general.setAttribute("bgcolor", cd_back_color.name());
	general.setAttribute("number", cd_number);
	general.setAttribute("cddb_id", cd_cddb_id);
	kover.appendChild(general);

	/* inlet */
	QDomElement inlet = doc.createElement("inlet");

	inlet.setAttribute("font", cd_inlet_title_font.family());
	inlet.setAttribute("size", cd_inlet_title_font.pointSize());
	inlet.setAttribute("weight", cd_inlet_title_font.weight());
	inlet.setAttribute("italic", cd_inlet_title_font.italic());
	inlet.setAttribute("spine_text", cd_spine_text);
	inlet.setAttribute("font_settings", cd_inlet_title_font.toString());
	the_text = doc.createCDATASection(cd_the_spine_text.toUtf8());
	inlet.appendChild(the_text);
	kover.appendChild(inlet);

	/* content */
	QDomElement content = doc.createElement("content");

	content.setAttribute("color", cd_contents_color.name());
	content.setAttribute("font", cd_contents_font.family());
	content.setAttribute("size", cd_contents_font.pointSize());
	content.setAttribute("weight", cd_contents_font.weight());
	content.setAttribute("italic", cd_contents_font.italic());
	content.setAttribute("font_settings", cd_contents_font.toString());
	kover.appendChild(content);

	/* content text */
	list = cd_contents.split("\n");

	for (QStringList::Iterator it = list.begin(); it != list.end(); ++it) {
		text = doc.createElement("text");
		content.appendChild(text);
		the_text = doc.createCDATASection((*it).toUtf8());
		text.appendChild(the_text);
	}

	/* saving */
	QFile f(filename);

	if (!f.open(QIODevice::WriteOnly))
		return false;

	f.write(doc.toString().toUtf8());
	f.write("\n", 1);          /* newline */
	f.close();

	kprintf("%s\n", (const char *)(doc.toString()).toUtf8());

	return true;
}

bool
KoverFile::open_XML(const QString & filename)
{
	reset();
	QDomDocument doc;
	QFile f(filename);

	if (!f.open(QIODevice::ReadOnly))
		return false;
	if (!doc.setContent(&f)) {
		f.close();
		return false;
	}
	f.close();

	return open_XML(doc);
}

bool
KoverFile::open_XML(QDomDocument doc)
{

	QDomElement docElem = doc.documentElement();

	/* cerr << docElem.tagName() << endl; */

	if (docElem.tagName() == "k3b_audio_project") {
		open_k3b_audio_project(doc);
		return true;
	}

	if (docElem.tagName() == "k3b_data_project") {
		open_k3b_data_project(doc);
		return true;
	}

	QDomNamedNodeMap nmm = docElem.attributes();

	for (uint length = 0; length < nmm.length(); length++) {
		/* cerr << (nmm.item(length)).nodeName() << "..." << (nmm.  item(length)).nodeValue() << endl; */
	}

	QDomNode n = docElem.firstChild();
	int image = 0;

	while (!n.isNull()) {
		QDomElement e = n.toElement();  /* try to convert the node to an element. */

		if (!e.isNull()) {              /* the node was really an element. */
			/* cerr << e.tagName() << endl; */
			QDomNamedNodeMap nm = e.attributes();

			if (e.tagName() == "title") {
				cd_title_font = QFont((nm.namedItem(
							       "font")).nodeValue(),
						      (nm.namedItem(
							       "size")).nodeValue(
							      ).toInt(),
						      (nm.namedItem(
							       "weight")).
						      nodeValue().toInt(),
						      (nm.namedItem(
							       "italic")).
						      nodeValue().toInt());
				cd_title_color = QColor((nm.namedItem(
								 "color")).
							nodeValue());
				cd_display_title =
					nm.namedItem("display").nodeValue().toInt();
				kprintf("font:%s\n",
					cd_title_font.rawName().toUtf8().
					constData());
			}

			if (e.tagName() == "content") {
				cd_contents_font =
					QFont((nm.namedItem("font")).nodeValue(),
					      (nm.namedItem(
						       "size")).nodeValue().toInt(),
					      (nm.namedItem(
						       "weight")).nodeValue().
					      toInt(),
					      (nm.namedItem(
						       "italic")).nodeValue().
					      toInt());
				cd_contents_color =
					QColor((nm.namedItem("color")).nodeValue());
				kprintf("font:%s\n",
					cd_contents_font.rawName().toUtf8().
					constData());
			}

			if (e.tagName() == "inlet") {
				cd_inlet_title_font =
					QFont((nm.namedItem("font")).nodeValue(),
					      (nm.namedItem(
						       "size")).nodeValue().toInt(),
					      (nm.namedItem(
						       "weight")).nodeValue().
					      toInt(),
					      (nm.namedItem(
						       "italic")).nodeValue().
					      toInt());
				cd_spine_text =
					nm.namedItem("spine_text").nodeValue().
				toInt();
				cd_the_spine_text = e.text();
				kprintf("font:%s\n",
					cd_inlet_title_font.rawName().toUtf8().
					constData());
			}

			if (e.tagName() == "general") {
				cd_number =
					(nm.namedItem("number")).nodeValue().
				toInt();
				cd_back_color = QColor((nm.namedItem(
								"bgcolor")).
						       nodeValue());
				cd_cddb_id = (nm.namedItem("cddb_id")).nodeValue();
			}

			if (e.tagName() == "img") {
				cd_image_file[image] =
					(nm.namedItem("src")).nodeValue();
				cd_image_mode[image] =
					(nm.namedItem("mode")).nodeValue().toInt();
				cd_image_target[image++] =
					(nm.namedItem("target")).nodeValue().
				toInt();
			}

		}
		QDomNode m = n.firstChild();

		while (!m.isNull()) {
			QDomElement ee = m.toElement();

			if (!ee.isNull()) { /* the node was really an element. */
				/* cout << ee.tagName() << "...." << ee.text() << endl; */

				if (e.tagName() == "title" && ee.tagName() ==
				    "text") {
					cd_title += ee.text() + "\n";
				}
				if (e.tagName() == "content" && ee.tagName() ==
				    "text") {
					cd_contents += ee.text() + "\n";
				}

			}
			m = m.nextSibling();
		}

		n = n.nextSibling();
	}

	cd_title.truncate(cd_title.length() - 1);
	cd_contents.truncate(cd_contents.length() - 1);

	return true;
}

void
KoverFile::open_k3b_audio_project(QDomDocument doc)
{
	QString title, artist;
	QDomElement docElem = doc.documentElement();

	QDomNamedNodeMap nmm = docElem.attributes();

	QDomNode n = docElem.firstChild();

	title = "";
	artist = "";

	while (!n.isNull()) {
		QDomElement e = n.toElement();

		if (e.tagName() == "cd-text") {
			QDomNode m = n.firstChild();

			while (!m.isNull()) {
				QDomElement ee = m.toElement();

				if (ee.tagName() == "title")
					title = ee.text();
				if (ee.tagName() == "artist")
					artist = ee.text();
				if (ee.tagName() == "disc_id")
					cd_cddb_id = ee.text();

				m = m.nextSibling();
			}

			if (title.isEmpty())
				cd_title = artist;
			else if (artist.isEmpty())
				cd_title = title;
			else
				cd_title = title + "\n" + artist;
		}

		title = "";
		artist = "";

		if (e.tagName() == "contents") {
			QDomNode m = n.firstChild();

			while (!m.isNull()) {
				QDomElement ee = m.toElement();

				if (ee.tagName() == "track") {
					QDomNode mm = m.firstChild();

					while (!mm.isNull()) {
						QDomNode mmm = mm.firstChild();
						QDomElement eee = mm.toElement();

						if (eee.tagName() == "cd-text") {
							while (!mmm.isNull()) {
								QDomElement eeee
								= mmm.toElement();

								if (eeee.tagName()
								    == "title")
									title =
										eeee
										.
										text();
								if (eeee.tagName()
								    == "artist")
									artist =
										eeee
										.
										text();

								mmm =
									mmm.
									nextSibling();
							}
							if (title.isEmpty())
								cd_contents +=
									artist +
									"\n";
							else if (artist.isEmpty())
								cd_contents +=
									title +
									"\n";
							else if (!title.isEmpty()
								 && !artist.
								 isEmpty())
								cd_contents +=
									title +
									" - " +
									artist +
									"\n";
						}
						mm = mm.nextSibling();
					}
				}
				m = m.nextSibling();
			}
		}
		n = n.nextSibling();
	}

	cd_contents.truncate(cd_contents.length() - 1);
}

void
KoverFile::open_k3b_data_project(QDomDocument doc)
{
	/* QDomElement docElem = doc.documentElement(); */
	QDomNode n = (doc.documentElement()).firstChild();

	while (!n.isNull()) {
		QDomElement e = n.toElement();

		if (e.tagName() == "header") {
			QDomNode m = n.firstChild();

			while (!m.isNull()) {
				QDomElement ee = m.toElement();

				if (ee.tagName() == "volume_id")
					cd_title = ee.text();
				m = m.nextSibling();
			}
		}
		if (e.tagName() == "files") {
			QDomNode m = n.firstChild();

			while (!m.isNull()) {
				QDomElement ee = m.toElement();
				QDomNamedNodeMap nm = ee.attributes();

				if (ee.tagName() == "directory")
					cd_contents +=
						(nm.namedItem("name")).nodeValue()
					+ "\n";
				if (ee.tagName() == "file")
					cd_contents +=
						(nm.namedItem("name")).nodeValue()
					+ "\n";

				m = m.nextSibling();
			}
		}
		n = n.nextSibling();
	}
	cd_contents.truncate(cd_contents.length() - 1);
}
