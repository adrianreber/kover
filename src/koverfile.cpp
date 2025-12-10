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

using namespace std;

#include "koverfile.h"

#include <kover.h>
#include <iostream>
#include <QDomDocument>
#include <QFile>
#include <QRegularExpression>
#include <QFileInfo>
#include <QUrl>
#include <QTemporaryFile>

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
	cd_title = QString();
	cd_title_font = QFont(*globals.title_font);
	cd_contents = QString();
	cd_contents_font = QFont(*globals.content_font);
	cd_inlet_title_font = QFont(*globals.inlet_title_font);
	cd_title_color = Qt::black;
	cd_contents_color = Qt::black;
	cd_back_color = Qt::white;
	cd_number = 0;
	cd_cddb_id = QString();
	cd_image_file[0] = QString();
	cd_image_mode[0] = IMG_CENTER;
	cd_image_target[0] = IMG_FRONT_LEFT;
	cd_image_file[1] = QString();
	cd_image_mode[1] = IMG_CENTER;
	cd_image_target[1] = IMG_FRONT_LEFT;
	cd_image_file[2] = QString();
	cd_image_mode[2] = IMG_CENTER;
	cd_image_target[2] = IMG_FRONT_LEFT;
	cd_display_title = false;
	cd_spine_text = false;
	cd_the_spine_text = QString();

	Q_EMIT dataChanged();
}

bool
KoverFile::empty()
{
	if (cd_title.isEmpty() && cd_contents.isEmpty() && cd_the_spine_text.isEmpty())
		return true;
	else
		return false;
}

void
KoverFile::setTitle(const QString & _title)
{
	if (cd_title != _title) {
		cd_title = _title;
		cd_title.replace(QRegularExpression(QStringLiteral("\r")), QStringLiteral("\n"));
		Q_EMIT dataChanged();
	}
}

void
KoverFile::setContents(const QString & _contents)
{
	if (cd_contents != _contents) {
		cd_contents = _contents;
		cd_contents.replace(QRegularExpression(QStringLiteral("\r")), QStringLiteral("\n"));
		Q_EMIT dataChanged();
	}
}

void
KoverFile::setTitleFont(const QFont & _title_font)
{
	if (cd_title_font != _title_font) {
		cd_title_font = _title_font;
		Q_EMIT dataChanged();
	}
}

void
KoverFile::setTitleColor(const QColor & _title_color)
{
	if (cd_title_color != _title_color) {
		cd_title_color = _title_color;
		Q_EMIT dataChanged();
	}
}

void
KoverFile::setContentsFont(const QFont & _contents_font)
{
	if (cd_contents_font != _contents_font) {
		cd_contents_font = _contents_font;
		Q_EMIT dataChanged();
	}
}

void
KoverFile::set_inlet_title_font(const QFont & _inlet_title_font)
{
	if (cd_inlet_title_font != _inlet_title_font) {
		cd_inlet_title_font = _inlet_title_font;
		Q_EMIT dataChanged();
	}
}

void
KoverFile::setContentsColor(const QColor & _contents_color)
{
	if (cd_contents_color != _contents_color) {
		cd_contents_color = _contents_color;
		Q_EMIT dataChanged();
	}
}

void
KoverFile::setBackColor(const QColor & _back_color)
{
	if (cd_back_color != _back_color) {
		cd_back_color = _back_color;
		Q_EMIT dataChanged();
	}
}

void
KoverFile::setNumber(const int _number)
{
	if (cd_number != _number) {
		cd_number = _number;
		Q_EMIT dataChanged();
	}
}

void
KoverFile::setImageFile(const int _nr, const QString & _image_file)
{
	if (cd_image_file[_nr] != _image_file) {
		cd_image_file[_nr] = _image_file;
		Q_EMIT dataChanged(true);
	}
}

void
KoverFile::setImageMode(const int _nr, const int _image_mode)
{
	if (cd_image_mode[_nr] != _image_mode) {
		cd_image_mode[_nr] = _image_mode;
		Q_EMIT dataChanged(true);
	}
}

void
KoverFile::setImageTarget(const int _nr, const int _image_target)
{
	if (cd_image_target[_nr] != _image_target) {
		cd_image_target[_nr] = _image_target;
		Q_EMIT dataChanged(true);
	}
}

void
KoverFile::set_display_title(bool title)
{
	cd_display_title = title;
	Q_EMIT dataChanged();
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
	Q_EMIT dataChanged();
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
		Q_EMIT dataChanged();
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
		Q_EMIT dataChanged();
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
	cd_title = QString();
	length = (long)ecd_file.read(1).constData();
	ecd_file.seek(1);
	for (i = 0; i < length; i++) {
		c = (long)ecd_file.read(1).constData();
		ecd_file.seek(1);
		if ((c != 10) && (c != 13))
			cd_title += QChar(static_cast<ushort>(c));
		else
			cd_title += QChar(32);
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
	cd_contents = QString();
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
			cd_contents += QChar(static_cast<ushort>(c));
	}

	ecd_file.close();

	Q_EMIT dataChanged();

	return true;
}

bool
KoverFile::openFile(const QUrl & url)
{
	QString filename;
	bool retval = false;

	if (url.isLocalFile())
		filename = url.toLocalFile();
	else
		filename = url.path();

	QFileInfo fi(filename);

	if (!fi.exists() || !fi.isReadable() || fi.isDir()) {
		return false;
	}
	/* check for ECD */
	if (checkForECD(filename)) {
		kprintf("even older format; opening ECD file\n");
		retval = openECD(filename);
		Q_EMIT dataChanged(true);

		return retval;
	}
	/* check for xml */
	QDomDocument doc(QStringLiteral("kover"));
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
				Q_EMIT dataChanged(true);
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
		Q_EMIT dataChanged(true);

		return retval;
	} else {
		f.close();
		return false;
	}
}

bool
KoverFile::saveFile(const QUrl & url)
{
	QString filename;

	if (url.isLocalFile())
		filename = url.toLocalFile();
	else
		filename = url.path();

	QFileInfo fi(filename);

	if (fi.exists() && !fi.isWritable())
		return false;
	if (fi.isDir())
		return false;

	if (fi.absolutePath().isEmpty())
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

	QDomDocument doc(QStringLiteral("kover"));

	QDomElement root = doc.documentElement();

	QDomElement kover = doc.createElement(QStringLiteral("kover"));

	kover.setAttribute(QStringLiteral("name"), QString::fromUtf8(K_PACKAGE));
	kover.setAttribute(QStringLiteral("version"), QString::fromUtf8(K_VERSION));
	doc.appendChild(kover);

	/* comment */
	QDomComment comment = doc.createComment(QString::fromUtf8("This file was created by "
						K_PACKAGE " " K_VERSION));
	kover.appendChild(comment);

	/* title */
	QDomElement title = doc.createElement(QStringLiteral("title"));

	title.setAttribute(QStringLiteral("color"), cd_title_color.name());
	title.setAttribute(QStringLiteral("font"), cd_title_font.family());
	title.setAttribute(QStringLiteral("size"), cd_title_font.pointSize());
	title.setAttribute(QStringLiteral("weight"), cd_title_font.weight());
	title.setAttribute(QStringLiteral("italic"), cd_title_font.italic());
	title.setAttribute(QStringLiteral("display"), cd_display_title);
	title.setAttribute(QStringLiteral("font_settings"), cd_title_font.toString());
	kover.appendChild(title);

	/* replace carriage return with newline */
	cd_title.replace(QRegularExpression(QStringLiteral("\r")), QStringLiteral("\n"));
	cd_contents.replace(QRegularExpression(QStringLiteral("\r")), QStringLiteral("\n"));

	/* title text */
	list = cd_title.split(QStringLiteral("\n"));

	for (QStringList::Iterator it = list.begin(); it != list.end(); ++it) {
		text = doc.createElement(QStringLiteral("text"));
		title.appendChild(text);
		the_text = doc.createCDATASection(*it);
		text.appendChild(the_text);
	}

	for (int i = 0; i < 3; i++) {
		img = doc.createElement(QStringLiteral("img"));
		img.setAttribute(QStringLiteral("src"), cd_image_file[i]);
		img.setAttribute(QStringLiteral("mode"), cd_image_mode[i]);
		img.setAttribute(QStringLiteral("target"), cd_image_target[i]);
		kover.appendChild(img);
	}

	/* general */
	QDomElement general = doc.createElement(QStringLiteral("general"));

	general.setAttribute(QStringLiteral("bgcolor"), cd_back_color.name());
	general.setAttribute(QStringLiteral("number"), cd_number);
	general.setAttribute(QStringLiteral("cddb_id"), cd_cddb_id);
	kover.appendChild(general);

	/* inlet */
	QDomElement inlet = doc.createElement(QStringLiteral("inlet"));

	inlet.setAttribute(QStringLiteral("font"), cd_inlet_title_font.family());
	inlet.setAttribute(QStringLiteral("size"), cd_inlet_title_font.pointSize());
	inlet.setAttribute(QStringLiteral("weight"), cd_inlet_title_font.weight());
	inlet.setAttribute(QStringLiteral("italic"), cd_inlet_title_font.italic());
	inlet.setAttribute(QStringLiteral("spine_text"), cd_spine_text);
	inlet.setAttribute(QStringLiteral("font_settings"), cd_inlet_title_font.toString());
	the_text = doc.createCDATASection(cd_the_spine_text);
	inlet.appendChild(the_text);
	kover.appendChild(inlet);

	/* content */
	QDomElement content = doc.createElement(QStringLiteral("content"));

	content.setAttribute(QStringLiteral("color"), cd_contents_color.name());
	content.setAttribute(QStringLiteral("font"), cd_contents_font.family());
	content.setAttribute(QStringLiteral("size"), cd_contents_font.pointSize());
	content.setAttribute(QStringLiteral("weight"), cd_contents_font.weight());
	content.setAttribute(QStringLiteral("italic"), cd_contents_font.italic());
	content.setAttribute(QStringLiteral("font_settings"), cd_contents_font.toString());
	kover.appendChild(content);

	/* content text */
	list = cd_contents.split(QStringLiteral("\n"));

	for (QStringList::Iterator it = list.begin(); it != list.end(); ++it) {
		text = doc.createElement(QStringLiteral("text"));
		content.appendChild(text);
		the_text = doc.createCDATASection(*it);
		text.appendChild(the_text);
	}

	/* saving */
	QFile f(filename);

	if (!f.open(QIODevice::WriteOnly))
		return false;

	f.write(doc.toString().toUtf8());
	f.write("\n", 1);          /* newline */
	f.close();

	kprintf("%s\n", (doc.toString()).toUtf8().constData());

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

	if (docElem.tagName() == QStringLiteral("k3b_audio_project")) {
		open_k3b_audio_project(doc);
		return true;
	}

	if (docElem.tagName() == QStringLiteral("k3b_data_project")) {
		open_k3b_data_project(doc);
		return true;
	}

	QDomNamedNodeMap nmm = docElem.attributes();

	for (int length = 0; length < nmm.length(); length++) {
		/* cerr << (nmm.item(length)).nodeName() << "..." << (nmm.  item(length)).nodeValue() << endl; */
	}

	QDomNode n = docElem.firstChild();
	int image = 0;

	while (!n.isNull()) {
		QDomElement e = n.toElement();  /* try to convert the node to an element. */

		if (!e.isNull()) {              /* the node was really an element. */
			/* cerr << e.tagName() << endl; */
			QDomNamedNodeMap nm = e.attributes();

			if (e.tagName() == QStringLiteral("title")) {
				cd_title_font = QFont(nm.namedItem(QStringLiteral("font")).nodeValue(),
						      nm.namedItem(QStringLiteral("size")).nodeValue().toInt(),
					      nm.namedItem(QStringLiteral("weight")).nodeValue().toInt(),
					      nm.namedItem(QStringLiteral("italic")).nodeValue().toInt());
			cd_title_color = QColor(nm.namedItem(QStringLiteral("color")).nodeValue());
				cd_display_title =
					nm.namedItem(QStringLiteral("display")).nodeValue().toInt();
				kprintf("font:%s\n",
					cd_title_font.family().toUtf8().
					constData());
			}

			if (e.tagName() == QStringLiteral("content")) {
				cd_contents_font =
					QFont(nm.namedItem(QStringLiteral("font")).nodeValue(),
					      nm.namedItem(QStringLiteral("size")).nodeValue().toInt(),
					      nm.namedItem(QStringLiteral("weight")).nodeValue().
					      toInt(),
					      nm.namedItem(QStringLiteral("italic")).nodeValue().
					      toInt());
				cd_contents_color =
					QColor(nm.namedItem(QStringLiteral("color")).nodeValue());
				kprintf("font:%s\n",
					cd_contents_font.family().toUtf8().
					constData());
			}

			if (e.tagName() == QStringLiteral("inlet")) {
				cd_inlet_title_font =
					QFont(nm.namedItem(QStringLiteral("font")).nodeValue(),
					      nm.namedItem(QStringLiteral("size")).nodeValue().toInt(),
					      nm.namedItem(QStringLiteral("weight")).nodeValue().
					      toInt(),
					      nm.namedItem(QStringLiteral("italic")).nodeValue().
					      toInt());
				cd_spine_text =
				nm.namedItem(QStringLiteral("spine_text")).nodeValue().toInt();
				cd_the_spine_text = e.text();
				kprintf("font:%s\n",
					cd_inlet_title_font.family().toUtf8().
					constData());
			}

			if (e.tagName() == QStringLiteral("general")) {
				cd_number =
				nm.namedItem(QStringLiteral("number")).nodeValue().toInt();
			cd_back_color = QColor(nm.namedItem(QStringLiteral("bgcolor")).nodeValue());
				cd_cddb_id = nm.namedItem(QStringLiteral("cddb_id")).nodeValue();
			}

			if (e.tagName() == QStringLiteral("img")) {
				cd_image_file[image] =
					nm.namedItem(QStringLiteral("src")).nodeValue();
				cd_image_mode[image] =
					nm.namedItem(QStringLiteral("mode")).nodeValue().toInt();
				cd_image_target[image++] =
				nm.namedItem(QStringLiteral("target")).nodeValue().toInt();
			}

		}
		QDomNode m = n.firstChild();

		while (!m.isNull()) {
			QDomElement ee = m.toElement();

			if (!ee.isNull()) { /* the node was really an element. */
				/* cout << ee.tagName() << "...." << ee.text() << endl; */

				if (e.tagName() == QStringLiteral("title") && ee.tagName() ==
				    QStringLiteral("text")) {
					cd_title += ee.text() + QStringLiteral("\n");
				}
				if (e.tagName() == QStringLiteral("content") && ee.tagName() ==
				    QStringLiteral("text")) {
					cd_contents += ee.text() + QStringLiteral("\n");
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

	title = QString();
	artist = QString();

	while (!n.isNull()) {
		QDomElement e = n.toElement();

		if (e.tagName() == QStringLiteral("cd-text")) {
			QDomNode m = n.firstChild();

			while (!m.isNull()) {
				QDomElement ee = m.toElement();

				if (ee.tagName() == QStringLiteral("title"))
					title = ee.text();
				if (ee.tagName() == QStringLiteral("artist"))
					artist = ee.text();
				if (ee.tagName() == QStringLiteral("disc_id"))
					cd_cddb_id = ee.text();

				m = m.nextSibling();
			}

			if (title.isEmpty())
				cd_title = artist;
			else if (artist.isEmpty())
				cd_title = title;
			else
				cd_title = title + QStringLiteral("\n") + artist;
		}

		title = QString();
		artist = QString();

		if (e.tagName() == QStringLiteral("contents")) {
			QDomNode m = n.firstChild();

			while (!m.isNull()) {
				QDomElement ee = m.toElement();

				if (ee.tagName() == QStringLiteral("track")) {
					QDomNode mm = m.firstChild();

					while (!mm.isNull()) {
						QDomNode mmm = mm.firstChild();
						QDomElement eee = mm.toElement();

						if (eee.tagName() == QStringLiteral("cd-text")) {
							while (!mmm.isNull()) {
								QDomElement eeee
								= mmm.toElement();

								if (eeee.tagName()
								    == QStringLiteral("title"))
									title =
										eeee
										.
										text();
								if (eeee.tagName()
								    == QStringLiteral("artist"))
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
									QStringLiteral("\n");
							else if (artist.isEmpty())
								cd_contents +=
									title +
									QStringLiteral("\n");
							else if (!title.isEmpty()
								 && !artist.
								 isEmpty())
								cd_contents +=
									title +
									QStringLiteral(" - ") +
									artist +
									QStringLiteral("\n");
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

		if (e.tagName() == QStringLiteral("header")) {
			QDomNode m = n.firstChild();

			while (!m.isNull()) {
				QDomElement ee = m.toElement();

				if (ee.tagName() == QStringLiteral("volume_id"))
					cd_title = ee.text();
				m = m.nextSibling();
			}
		}
		if (e.tagName() == QStringLiteral("files")) {
			QDomNode m = n.firstChild();

			while (!m.isNull()) {
				QDomElement ee = m.toElement();
				QDomNamedNodeMap nm = ee.attributes();

				if (ee.tagName() == QStringLiteral("directory"))
					cd_contents +=
						(nm.namedItem(QStringLiteral("name"))).nodeValue()
					+ QStringLiteral("\n");
				if (ee.tagName() == QStringLiteral("file"))
					cd_contents +=
						(nm.namedItem(QStringLiteral("name"))).nodeValue()
					+ QStringLiteral("\n");

				m = m.nextSibling();
			}
		}
		n = n.nextSibling();
	}
	cd_contents.truncate(cd_contents.length() - 1);
}
#include "moc_koverfile.cpp"
