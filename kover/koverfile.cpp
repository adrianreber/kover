/***************************************************************************
                          koverfile.cpcd_image_file[_nr]p  -  description                              
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

#include "kover.h"
#include "koverfile.h"
#include <qfile.h>
#include <qregexp.h>
#include <qfileinfo.h>
#include <ksimpleconfig.h>

KoverFile::KoverFile() {
	reset();
}

KoverFile::~KoverFile() {
}

void KoverFile::reset() {
	cd_title = "";
	cd_title_font = QFont("times", 32);
	cd_contents = "";
	cd_contents_font = QFont("helvetica", 16);
	cd_booklet_title_font = QFont("helvetica", 12);
	cd_title_color = black;
	cd_contents_color = black;
	cd_back_color = white;
	cd_number = 0;
	cd_image_file[0] = "";
	cd_image_mode[0] = IMG_CENTER;
	cd_image_target[0] = IMG_FRONT_LEFT;
	cd_image_file[1] = "";
	cd_image_mode[1] = IMG_CENTER;
	cd_image_target[1] = IMG_FRONT_LEFT;
	cd_image_file[2] = "";
	cd_image_mode[2] = IMG_CENTER;
	cd_image_target[2] = IMG_FRONT_LEFT;

	emit dataChanged();
}

void KoverFile::setTitle(const QString& _title) {
	if (cd_title != _title) {
		cd_title = _title;
		emit dataChanged();
	}
}

void KoverFile::setContents( const QString& _contents )
{
	if (cd_contents != _contents)
	{
		cd_contents = _contents;
		emit dataChanged();
	}
}

void KoverFile::setTitleFont( const QFont& _title_font )
{
	if (cd_title_font != _title_font)
	{
		cd_title_font = _title_font;
		emit dataChanged();
	}
}

void KoverFile::setTitleColor( const QColor& _title_color )
{
	if (cd_title_color != _title_color)
	{
		cd_title_color = _title_color;
		emit dataChanged();
	}
}

void KoverFile::setContentsFont( const QFont& _contents_font )
{
	if (cd_contents_font != _contents_font)
	{
		cd_contents_font = _contents_font;
		emit dataChanged();
	}
}

void KoverFile::set_booklet_title_font(const QFont& _booklet_title_font) {
  if (cd_booklet_title_font != _booklet_title_font) {
	 cd_booklet_title_font = _booklet_title_font;
	 	emit dataChanged();
	}
}

void KoverFile::setContentsColor( const QColor& _contents_color )
{
	if (cd_contents_color != _contents_color)
	{
		cd_contents_color = _contents_color;
		emit dataChanged();
	}
}

void KoverFile::setBackColor( const QColor& _back_color )
{
	if (cd_back_color != _back_color)
	{
		cd_back_color = _back_color;
		emit dataChanged();
	}
}

void KoverFile::setNumber( const int _number )
{
	if (cd_number != _number)
	{
		cd_number = _number;
		emit dataChanged();
	}
}

void KoverFile::setImageFile( const int _nr, const QString& _image_file )
{
	if (cd_image_file[_nr] != _image_file)
	{
		cd_image_file[_nr] = _image_file;
		emit dataChanged(true);
	}
}

void KoverFile::setImageMode( const int _nr, const int _image_mode )
{
	if (cd_image_mode[_nr] != _image_mode)
	{
		cd_image_mode[_nr] = _image_mode;
		emit dataChanged(true);
	}
}

void KoverFile::setImageTarget( const int _nr, const int _image_target )
{
	if (cd_image_target[_nr] != _image_target)
	{
		cd_image_target[_nr] = _image_target;
		emit dataChanged(true);
	}
}

QString KoverFile::title() const
{
	return cd_title;
}

QString KoverFile::contents() const
{
	return cd_contents;
}

QFont KoverFile::titleFont() const
{
	return cd_title_font;
}

QColor KoverFile::titleColor() const
{
	return cd_title_color;
}

QFont KoverFile::contentsFont() const
{
	return cd_contents_font;
}

QFont KoverFile::booklet_title_font() const {
  return cd_booklet_title_font;
}

QColor KoverFile::contentsColor() const
{
	return cd_contents_color;
}

QColor KoverFile::backColor() const
{
	return cd_back_color;
}

int KoverFile::number() const
{
	return cd_number;
}

QString KoverFile::imageFile(const int _nr) const {
  _DEBUG_ fprintf(stderr,"kover:KoverFile::imageFile():%s\n",cd_image_file[_nr].latin1());
  return cd_image_file[_nr];
}

int KoverFile::imageMode(const int _nr) const
{
	return cd_image_mode[_nr];
}

int KoverFile::imageTarget(const int _nr) const
{
	return cd_image_target[_nr];
}

bool KoverFile::checkForECD( QString& filename )
{
	QFile ecd_file(filename);
	
	if (!ecd_file.open( IO_ReadOnly ))
	{
		return false;
	}
	
	// Some kind off hacky, I just test for three of eight ID bytes: "DCE", whole id: "DCEi20RP"
	if ( (ecd_file.getch() == 68) && (ecd_file.getch() == 67) && (ecd_file.getch() == 69) )
	{
		ecd_file.close();
		return true;
	}
	
	ecd_file.close();
	return false;  
}

bool KoverFile::openECD( QString& filename )
{
	QFileInfo fi( filename );
	if ( !fi.exists() )
		return false;
//	if ( !fi.permission(QFileInfo::ReadUser) )
	if ( !fi.isReadable() )
		return false;
	if ( fi.isDir() )
		return false;

	char	c;
	int	i, length;
	QFile	ecd_file(filename);
	
	if (!ecd_file.open( IO_ReadOnly ))
	{
		return false;
	}
	
	// ID
	ecd_file.at(8);

	// Title
	cd_title	= "";
	length = ecd_file.getch();
	for (i=0; i<length; i++)
	{
		c = ecd_file.getch();
		if ( (c!=10) && (c!=13) )
		{
			cd_title += c;
		} else
		{
			cd_title += 32;
		}
	}

	// Background image
	length = ecd_file.getch();
	ecd_file.at( ecd_file.at() + length );
	
	// Foreground image
	length = ecd_file.getch();
	ecd_file.at( ecd_file.at() + length );
	
	// Font 1, Data
	ecd_file.at( ecd_file.at() + 28 );
	
	// Font 1, Name
	length = ecd_file.getch();
	ecd_file.at( ecd_file.at() + length );
	
	// Font 2, Data
	ecd_file.at( ecd_file.at() + 28 );
	
	// Font 2, Name
	length = ecd_file.getch();
	ecd_file.at( ecd_file.at() + length );
	
	// Unknown
	ecd_file.at( ecd_file.at() + 22 );
	
	// Contents
	cd_contents	= "";
	length = ecd_file.getch();
	if (length == 0xff)
	{
		length = ecd_file.getch();
		length += ecd_file.getch() << 8;
	}
	for (i=0; i<length; i++)
	{
		c = ecd_file.getch();
		if (c != 13)
		{
			cd_contents += c;
		}
	}
	
	ecd_file.close();

	emit dataChanged();
	
	return true;  
}

bool KoverFile::openFile( QString& filename )
{
	QFileInfo fi( filename );
	if ( !fi.exists() )
		return false;
	if ( !fi.isReadable() )
		return false;
	if ( fi.isDir() )
		return false;

	if ( checkForECD(filename) )
	{
		return openECD( filename );
	}
	
	KSimpleConfig	file( filename, true );

	file.setGroup( "Title" );
	cd_title	= file.readEntry( "Text", "" );
	cd_title_font	= file.readFontEntry( "Font", new QFont("times",32) );
	cd_title_color	= file.readColorEntry( "Color", new QColor(0,0,0) );

	file.setGroup( "Contents" );
	cd_contents		= file.readEntry( "Text", "" );
	cd_contents_font	= file.readFontEntry( "Font", new QFont("helvetica",16) );
	cd_contents_color	= file.readColorEntry( "Color", new QColor(0,0,0) );
	
	file.setGroup("Booklet");
	cd_booklet_title_font = file.readFontEntry("Font", new QFont("helvetica",12));
	

	file.setGroup( "Image1" );
	cd_image_file[0]	= file.readEntry( "Filename", "" );
	cd_image_mode[0]	= file.readNumEntry( "Mode", IMG_CENTER );
	cd_image_target[0]	= file.readNumEntry( "Target", IMG_FRONT_LEFT );
	
	file.setGroup( "Image2" );
	cd_image_file[1]	= file.readEntry( "Filename", "" );
	cd_image_mode[1]	= file.readNumEntry( "Mode", IMG_CENTER );
	cd_image_target[1]	= file.readNumEntry( "Target", IMG_FRONT_LEFT );
	
	file.setGroup( "Image3" );
	cd_image_file[2]	= file.readEntry( "Filename", "" );
	cd_image_mode[2]	= file.readNumEntry( "Mode", IMG_CENTER );
	cd_image_target[2]	= file.readNumEntry( "Target", IMG_FRONT_LEFT );
	
	file.setGroup( "General" );
	cd_number	= file.readNumEntry( "Number", 0 );
	cd_back_color	= file.readColorEntry( "BackgroundColor", new QColor(255,255,255) );
  
	cd_contents.replace( QRegExp(">|<"), "\n" );
	cd_title.replace( QRegExp(">|<"), "\n" );
	
	emit dataChanged(true);
	
	return true;  
}

bool KoverFile::saveFile( QString& filename )
{
// Help! I´m looking for a better way of error handling. KSimpleConfig doesn´t return anything.
	
	QFileInfo fi( filename );
	if ( fi.exists() && !fi.isWritable() )
		return false;
	if ( fi.isDir() )
		return false;
	
	if (fi.dirPath(true) == "")
		return false;
	fi.setFile( fi.dirPath(true) );
	if ( fi.exists() && !fi.isWritable() )
		return false;

	
	KSimpleConfig	file( filename );

	cd_contents.replace( QRegExp("\n"), ">|<" );
	cd_title.replace( QRegExp("\n"), ">|<" );
	
	file.setGroup( "Program" );
	file.writeEntry( "Name", "Kover" );
	file.writeEntry( "Version", VERSION );

	file.setGroup( "Title" );
	file.writeEntry( "Text", cd_title );
	file.writeEntry( "Font", cd_title_font );
	file.writeEntry( "Color", cd_title_color );

	file.setGroup( "Contents" );
	file.writeEntry( "Text", cd_contents );
	file.writeEntry( "Font", cd_contents_font );
	file.writeEntry( "Color", cd_contents_color );
	
	file.setGroup("Booklet");
	file.writeEntry("Font",cd_booklet_title_font);

	file.setGroup( "Image1" );
	file.writeEntry( "Filename", cd_image_file[0] );
	file.writeEntry( "Mode", cd_image_mode[0] );
	file.writeEntry( "Target", cd_image_target[0] );

	file.setGroup( "Image2" );
	file.writeEntry( "Filename", cd_image_file[1] );
	file.writeEntry( "Mode", cd_image_mode[1] );
	file.writeEntry( "Target", cd_image_target[1] );

	file.setGroup( "Image3" );
	file.writeEntry( "Filename", cd_image_file[2] );
	file.writeEntry( "Mode", cd_image_mode[2] );
	file.writeEntry( "Target", cd_image_target[2] );

	file.setGroup( "General" );
	file.writeEntry( "Number", cd_number );
	file.writeEntry( "BackgroundColor", cd_back_color );
	
	file.sync();
	
	cd_contents.replace( QRegExp(">|<"), "\n" );
	cd_title.replace( QRegExp(">|<"), "\n" );

	return true;
}

