/** -*- adrian-c -*-
	 kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
	 Copyright (C) 1999, 2000 by Denis Oliver Kropp
	 Copyright (C) 2000, 2001 by Adrian Reber 
	 
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
	 
	 File: CDView.h
	 
	 Description: N/A
	 
	 Changes:
*/

/* $Id: cdview.h,v 1.1 2001/12/04 16:23:50 adrian Exp $ */

#ifndef _CDVIEW_H
#define _CDVIEW_H

#include <qframe.h>
#include <qpixmap.h>
#include <kprinter.h>
#include "koverfile.h"

#include "kover.h"

/**
 * CDView draws the cover in mini form, print preview or direct to the printer.
 * @author Denis Oliver Kropp (dok@fischlustig.de)
 */
class CDView : public QFrame {
	 Q_OBJECT
public:
	 /**
	  * CDView constructor.
	  * @param parent Optional pointer to the parent widget.
	  * @param name Optional pointer to the name.
	  */
	 CDView( KoverFile* _kover_file, QWidget* parent = NULL, const char* name = NULL );
  
	 /**
	  * Opens a printer configuration dialog and prints or not.
	  */
	 void printKover();
	
	 /**
	  * Normally CDView draws the cover with a scale of 2/5.<br>
	  * If you set the preview mode then there´s no scale.
	  * @param preview true=print preview; false=normal 2/5 scale
	  */
	 void showPreview( bool preview = true );

public slots:
/**
 * Tells CDView that the data within KoverFile changed.
 */
void dataChanged(bool image);

	 signals:
/**
 * This signal is emitted when the user clicks into the CDView during preview.
 */
void stopPreview();
	 void actualSize();

protected:
	 void paintEvent( QPaintEvent * );
	 void mousePressEvent( QMouseEvent * );
	 void drawBooklet( QPainter *p, int X, int Y );
	 void drawInlet( QPainter *p, int X, int Y );
	
	
private:
	 QPixmap		images[3];
	 KoverFile*	kover_file;
	 bool			previewMode;
	 KPrinter*		printer;
	 QPainter *paint1;
};

#endif /* _CDVIEW_H */
