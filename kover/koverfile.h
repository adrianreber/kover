/***************************************************************************
                          koverfile.h  -  description                              
                             -------------------                                         

    version              :                                   
    begin                : Tue Dec 22 1998                                           
    copyright            : (C) 1998 by Denis Oliver Kropp                         
    email                : dok@fischlustig.de                                     
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

// alter mach kaffe

#ifndef KOVERFILE_H
#define KOVERFILE_H

#include <qobject.h>
#include <qstring.h>
#include <qfont.h>
#include <qcolor.h>

typedef enum
{
	IMG_CENTER,
	IMG_TILE,
	IMG_STRETCH
} IMG_MODE;

typedef enum
{
	IMG_FRONT_LEFT,
	IMG_FRONT_RIGHT,
	IMG_FRONT_FULL,
	IMG_BACK_INNER,
	IMG_BACK_FULL
} IMG_TARGET;

/**This class provides storage of cover data with file I/O
  *@author Denis Oliver Kropp
  */

class KoverFile : public QObject
{
	Q_OBJECT
public: 
	KoverFile();
	~KoverFile();
	
	void setTitle( const QString& _title );
	void setContents( const QString& _contents );
	void setTitleFont( const QFont& _title_font );
	void setTitleColor( const QColor& _title_color );
	void setContentsFont( const QFont& _contents_font );
	void set_booklet_title_font(const QFont& _booklet_title_font);
	void setContentsColor( const QColor& _contents_color );
	void setBackColor( const QColor& _back_color );
	void setNumber( const int _number );
	
	void setImageFile( const int _nr, const QString& _image_file );
	void setImageMode( const int _nr, const int _image_mode );
	void setImageTarget( const int _nr, const int _image_target );

	QString	title() const;
	QString	contents() const;
	QFont	titleFont() const;
	QColor	titleColor() const;
	QFont	contentsFont() const;
	QFont booklet_title_font() const;
	QColor	contentsColor() const;
	QColor	backColor() const;
	int		number() const;
	QString	imageFile(const int _nr) const;
	int		imageMode(const int _nr) const;
	int		imageTarget(const int _nr) const;
	
	bool checkForECD( QString& filename );
	bool openECD( QString& filename );
	bool openFile( QString& filename );
	bool saveFile( QString& filename );

	void reset();
signals:
	/**
	  * This signal is emitted when any data changed.
	  */
	void dataChanged(bool image=false);
private:
	QString	cd_title;
	QString	cd_contents;
	QFont	cd_title_font;
	QColor	cd_title_color;
	QFont	cd_contents_font;
	QFont cd_booklet_title_font;
	QColor	cd_contents_color;
	QColor	cd_back_color;
	int		cd_number;
	QString	cd_image_file[3];
	int		cd_image_mode[3];
	int		cd_image_target[3];
};

#endif

