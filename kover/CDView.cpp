/***************************************************************************
                          CDView.cpp - Implements CDView Widget
                             -------------------                                         

    version              :                                   
    begin                : Mon Dez 14 19:01:57 CET 1998
                                           
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
#include "CDView.h"

#include <qimage.h>
#include <qregexp.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qsemimodal.h>
#include <kmessagebox.h>
#include <math.h>
#include <stdio.h>
#include "koverfile.h"

#define FRONT_H	343
#define FRONT_V	338

#define BACK_HI	391
#define BACK_HS	17
#define BACK_V		334

CDView::CDView( KoverFile* _kover_file, QWidget* parent = NULL, const char* name = NULL ) : QFrame( parent, name )
{
  kover_file = _kover_file;
  connect( kover_file, SIGNAL(dataChanged(bool)), SLOT(dataChanged(bool)) );
  
  setBackgroundColor( white );
  previewMode = false;
  setFrameRect( QRect(0,0,0,0) );
  setFrameStyle( WinPanel | Sunken );

  printer = new QPrinter;
  printer->setOrientation( QPrinter::Landscape );
  printer->setMinMax( 1, 2 );
  printer->setFromTo( 1, 2 );
  print_thread = 0;
}

void CDView::paintEvent( QPaintEvent * )
{
  QPainter paint( this );
  drawFrame(&paint);

  if (previewMode)
  {
    drawBooklet( &paint, 4, 4 );
    drawInlet( &paint, 140, 4*2 + 338 );
    
    paint.setWorldMatrix( QWMatrix() );
    paint.setFont( QFont("helvetica",14) );
    paint.setPen( black );
    paint.drawText( 20, 400, "Click to close" );
  } else
  {
    paint.scale( 0.4f, 0.4f );
    drawBooklet( &paint, 20, 15 );
    drawInlet( &paint, 150, 15*2 + 338 );
  }
}





void CDView::showPreview( bool preview )
{
  previewMode = preview;
  update();
}

void CDView::printKover()
{
  if ( printer->setup(this) ) 
    {
		QPainter paint( printer );

		//paint1 = new QPainter(printer);
      
       previewMode = true; // hack
      
      if (printer->fromPage() == 1)
		  {
			 //pthread_create(&print_thread, NULL,printThreadPage1,(void *)this);
			 drawBooklet(&paint, 20, 15);
 		  }
      
      printer->newPage();
      
       if (printer->toPage() == 2)
 		  {
 			 drawInlet( &paint, 20, 15 );
 		  }

		previewMode = false;
		
    }
}

void *printThreadPage1(void *parm)
{
  CDView *cd_view = (CDView *) parm;

  QPainter paint( cd_view->printer );

#ifdef ENABLE_DEBUG_OUTPUT
  fprintf(stderr, "[printThreadPage1()] print thread up, PID: %i\n", getpid());
#endif
  
  cd_view->drawBooklet(&paint, 20, 15);
  
  pthread_exit(NULL);
}


void CDView::drawBooklet( QPainter *p, int X, int Y )
{
  
  float sc;
  sc = previewMode ? 1 : 0.4f; // hack

  p->fillRect( X, Y, FRONT_H*2, FRONT_V, kover_file->backColor() );
	
  for (int i=0; i<3; i++)
	 {
		if (!images[i].isNull())
		  {
			 switch (kover_file->imageMode(i))
				{
				case IMG_CENTER:
				  switch (kover_file->imageTarget(i))
					 {
					 case IMG_FRONT_LEFT:
						p->setClipRect( X*sc, Y*sc, FRONT_H*sc, FRONT_V*sc );
						p->drawPixmap( X + FRONT_H/2 - images[i].width()/2, Y + FRONT_V/2 - images[i].height()/2, images[i] );
						p->setClipping( false );
						break;
					 case IMG_FRONT_RIGHT:
						p->setClipRect( (X+FRONT_H)*sc, Y*sc, FRONT_H*sc, FRONT_V*sc );
						p->drawPixmap( X+FRONT_H + FRONT_H/2 - images[i].width()/2, Y + FRONT_V/2 - images[i].height()/2, images[i] );
						p->setClipping( false );
						break;
					 case IMG_FRONT_FULL:
						p->setClipRect( X*sc, Y*sc, FRONT_H*2*sc, FRONT_V*sc );
						p->drawPixmap( X + FRONT_H - images[i].width()/2, Y + FRONT_V/2 - images[i].height()/2, images[i] );
						p->setClipping( false );
						break;
					 }
				  break;
				case IMG_TILE:
				  switch (kover_file->imageTarget(i))
					 {
					 case IMG_FRONT_LEFT:
						p->drawTiledPixmap( X, Y, FRONT_H, FRONT_V, images[i] );
						break;
					 case IMG_FRONT_RIGHT:
						p->drawTiledPixmap( X+FRONT_H, Y, FRONT_H, FRONT_V, images[i] );
						break;
					 case IMG_FRONT_FULL:
						p->drawTiledPixmap( X, Y, FRONT_H*2, FRONT_V, images[i] );
						break;
					 }
				  break;
				case IMG_STRETCH:
				  switch (kover_file->imageTarget(i))
					 {
					 case IMG_FRONT_LEFT:
					 case IMG_FRONT_FULL:
						p->drawPixmap( X, Y, images[i] );
						break;
					 case IMG_FRONT_RIGHT:
						p->drawPixmap( X+FRONT_H, Y, images[i] );
						break;
					 }
				  break;
				}
		  }
	 }
	
  p->setFont( kover_file->titleFont() );
  p->setPen( kover_file->titleColor() );
  p->drawText( X+FRONT_H, Y+10, FRONT_H, FRONT_V-10, AlignHCenter, kover_file->title(), kover_file->title().length() );
	
  p->setPen( black );

  p->drawLine( X, Y, X+FRONT_H*2, Y );
  p->drawLine( X, Y+FRONT_V, X+FRONT_H*2, Y+FRONT_V );

  p->drawLine( X, Y, X, Y+FRONT_V );
  p->drawLine( X+FRONT_H*2, Y, X+FRONT_H*2, Y+FRONT_V );
  
  if (kover_file->backColor() == black)
	 {
		p->setPen( white );
	 }
  p->drawLine( X+FRONT_H, Y, X+FRONT_H, Y+FRONT_V );
}

void CDView::drawInlet( QPainter *p, int X, int Y )
{
	float sc;
	sc = previewMode ? 1 : 0.402f; // hack

	QString title = kover_file->title();
	title.replace( QRegExp("\n"), " - " );

	p->fillRect( X, Y, BACK_HI+BACK_HS*2, BACK_V, kover_file->backColor() );
	
	for (int i=0; i<3; i++)
	{
		if (!images[i].isNull())
		{
			switch (kover_file->imageMode(i))
			{
				case IMG_CENTER:
					switch (kover_file->imageTarget(i))
					{
						case IMG_BACK_INNER:
							p->setClipRect( (X+BACK_HS)*sc, Y*sc, BACK_HI*sc, BACK_V*sc );
							p->drawPixmap( X+BACK_HS + BACK_HI/2 - images[i].width()/2, Y + BACK_V/2 - images[i].height()/2, images[i] );
							p->setClipping( false );
						break;
						case IMG_BACK_FULL:
							p->setClipRect( X*sc, Y*sc, (BACK_HI+BACK_HS*2)*sc, BACK_V*sc );
							p->drawPixmap( X+BACK_HS + BACK_HI/2 - images[i].width()/2, Y + BACK_V/2 - images[i].height()/2, images[i] );
							p->setClipping( false );
						break;
					}
				break;
				case IMG_TILE:
					switch (kover_file->imageTarget(i))
					{
						case IMG_BACK_INNER:
							p->drawTiledPixmap( X+BACK_HS, Y, BACK_HI, BACK_V, images[i] );
						break;
						case IMG_BACK_FULL:
							p->drawTiledPixmap( X, Y, BACK_HI+BACK_HS*2, BACK_V, images[i] );
						break;
					}
				break;
				case IMG_STRETCH:
					switch (kover_file->imageTarget(i))
					{
						case IMG_BACK_INNER:
							p->drawPixmap( X+BACK_HS, Y, images[i] );
						break;
						case IMG_BACK_FULL:
							p->drawPixmap( X, Y, images[i] );
						break;
					}
				break;
			}
		}
	}
	
	p->setPen( black );

	p->drawLine( X, Y, X+BACK_HI+BACK_HS*2, Y );
	p->drawLine( X, Y+BACK_V, X+BACK_HI+BACK_HS*2, Y+BACK_V );
	p->drawLine( X, Y, X, Y+BACK_V );
	p->drawLine( X+BACK_HI+BACK_HS*2, Y, X+BACK_HI+BACK_HS*2, Y+BACK_V );
  
	if (kover_file->backColor() == black)
	{
		p->setPen( white );
	}
	
	p->drawLine( X+BACK_HS, Y, X+BACK_HS, Y+BACK_V );
	p->drawLine( X+BACK_HS+BACK_HI, Y, X+BACK_HS+BACK_HI, Y+BACK_V );


	p->translate( X, Y+BACK_V );
	p->rotate( -90 );
  
	if (kover_file->number())
	{
		QString numberStr;
		numberStr.setNum( kover_file->number() );
		p->setPen( red );
		p->setFont( QFont("helvetica", 12, QFont::Bold) );
		p->drawText( 10, 0, BACK_V-10, BACK_HS, AlignLeft | AlignVCenter, numberStr, numberStr.length() );
		p->setPen( kover_file->titleColor() );
		p->setFont( QFont("helvetica", 12) );
		p->drawText( 38, 0, BACK_V-38, BACK_HS, AlignLeft | AlignVCenter, title, title.length() );
	} else
	{
		p->setPen( kover_file->titleColor() );
		p->setFont( QFont("helvetica", 12) );
		p->drawText( 10, 0, BACK_V-10, BACK_HS, AlignLeft | AlignVCenter, title, title.length() );
	}
  
	p->translate( BACK_V, BACK_HS+BACK_HI );
	p->rotate( -180 );

	if (kover_file->number())
	{
		QString numberStr;
		numberStr.setNum( kover_file->number() );
		p->setPen( red );
		p->setFont( QFont("helvetica", 12, QFont::Bold) );
		p->drawText( 10, -1, BACK_V-10, -BACK_HS, AlignLeft | AlignVCenter, numberStr, numberStr.length() );
		p->setPen( kover_file->titleColor() );
		p->setFont( QFont("helvetica", 12) );
		p->drawText( 38, -1, BACK_V-38, -BACK_HS, AlignLeft | AlignVCenter, title, title.length() );
	} else
	{
		p->setPen( kover_file->titleColor() );
		p->setFont( QFont("helvetica", 12) );
		p->drawText( 10, -1, BACK_V-10, -BACK_HS, AlignLeft | AlignVCenter, title, title.length() );
	}


	p->translate( 0, BACK_HI );
	p->rotate( -90 );
  
	p->setFont( kover_file->contentsFont() );
	p->setPen( kover_file->contentsColor() );
	p->drawText( 5, 7, BACK_HI-5, BACK_V-7, AlignLeft, kover_file->contents(), kover_file->contents().length() );
}

void CDView::mousePressEvent( QMouseEvent* evt )
{
  if (previewMode)
	 emit stopPreview();
  else
	 {
		printf("emitting actualSize()\n");
		emit actualSize();
	 }
}

void CDView::dataChanged(bool image)
{
	if (image)
	{
		for (int i=0; i<3; i++)
		{
			QImage load( kover_file->imageFile(i) );
			if (load.isNull())
			{
				images[i].resize(0,0);
			} else
			{
				switch (kover_file->imageMode(i))
				{
					case IMG_TILE:
					case IMG_CENTER:
						images[i].convertFromImage( load );
					break;
					case IMG_STRETCH:
						switch (kover_file->imageTarget(i))
						{
							case IMG_FRONT_LEFT:
							case IMG_FRONT_RIGHT:
								images[i].convertFromImage( load.smoothScale( FRONT_H, FRONT_V ) );
							break;
							case IMG_FRONT_FULL:
								images[i].convertFromImage( load.smoothScale( FRONT_H*2, FRONT_V ) );
							break;
							case IMG_BACK_INNER:
								images[i].convertFromImage( load.smoothScale( BACK_HI, BACK_V ) );
							break;
							case IMG_BACK_FULL:
								images[i].convertFromImage( load.smoothScale( BACK_HI+BACK_HS*2, BACK_V ) );
							break;
						}
					break;
				}
			}
		}
	}

	repaint(false);
}




