/** -*- adrian-c -*-
	 kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
	 Copyright (C) 1999-2000 by Denis Oliver Kropp
	 Copyright (C) 2000-2002 by Adrian Reber
	 
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
	 
	 File: koverfile.h
	 
	 Description: storage of cover data
	 
	 Changes:

	 29 Oct 2001: variable inlet title font
	 
*/

/* $Id: koverfile.h,v 1.15 2003/03/10 22:44:06 adrian Exp $ */

#ifndef KOVERFILE_H
#define KOVERFILE_H

#include <qobject.h>
#include <qstring.h>
#include <qfont.h>
#include <qcolor.h>
#include <ksimpleconfig.h>
#include <qdom.h>

class KURL;

typedef enum {
    IMG_CENTER,
    IMG_TILE,
    IMG_STRETCH
} IMG_MODE;

typedef enum {
    IMG_FRONT_LEFT,
    IMG_FRONT_RIGHT,
    IMG_FRONT_FULL,
    IMG_BACK_INNER,
    IMG_BACK_FULL
} IMG_TARGET;

/**This class provides storage of cover data with file I/O
 *@author Denis Oliver Kropp
 */

class KoverFile:public QObject {
  Q_OBJECT public:
    KoverFile();
    ~KoverFile();

    void setTitle(const QString & _title);
    void setContents(const QString & _contents);
    void setTitleFont(const QFont & _title_font);
    void setTitleColor(const QColor & _title_color);
    void setContentsFont(const QFont & _contents_font);
    void set_inlet_title_font(const QFont & _inlet_title_font);
    void setContentsColor(const QColor & _contents_color);
    void setBackColor(const QColor & _back_color);
    void setNumber(const int _number);

    void setImageFile(const int _nr, const QString & _image_file);
    void setImageMode(const int _nr, const int _image_mode);
    void setImageTarget(const int _nr, const int _image_target);

    void set_display_title(bool title);
    void set_spine_text(bool bla);
    void set_the_spine_text(const QString & text);
    void set_cddb_id(const QString & text);

    QString title() const;
    QString contents() const;
    QFont titleFont() const;
    QColor titleColor() const;
    QFont contentsFont() const;
    QFont inlet_title_font() const;
    QColor contentsColor() const;
    QColor backColor() const;
    int number() const;
    QString imageFile(const int _nr) const;
    int imageMode(const int _nr) const;
    int imageTarget(const int _nr) const;
    bool display_title() const;
    bool spine_text() const;
    QString the_spine_text() const;
    QString cddb_id() const;

    bool openFile(const KURL & url);
    bool saveFile(const KURL & url);
    void reset();
    bool empty();

      signals:
         /**
	  * This signal is emitted when any data changed.
	  */
    void dataChanged(bool image = false);
  private:
      QString cd_title;
    QString cd_contents;
    QFont cd_title_font;
    QColor cd_title_color;
    QFont cd_contents_font;
    QFont cd_inlet_title_font;
    QColor cd_contents_color;
    QColor cd_back_color;
    int cd_number;
    QString cd_image_file[3];
    int cd_image_mode[3];
    int cd_image_target[3];
    bool cd_display_title;
    bool cd_spine_text;
    QString cd_the_spine_text;
    QString cd_cddb_id;
    bool save_as_XML(const QString & filename);
    bool open_XML(const QString & filename);
    bool open_XML(QDomDocument doc);
    bool old_save_method(const QString & filename);
    void old_open_method(KSimpleConfig & file);
    bool checkForECD(QString & filename);
    bool openECD(QString & filename);
    void open_k3b_audio_project(QDomDocument doc);
    void open_k3b_data_project(QDomDocument doc);
};

#endif /* KOVERFILE_H */
