/**  emacs, I suggest you use: -*- adrian-c -*-
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
	 
	 File: cdview.cc
	 
	 Description: Implements CDView Widget
	 
	 Changes: 
	 
	 14 Dec 1998: Initial release
	 18 Oct 2001: Added routines for 'inlet only' option
	 29 Oct 2001: Change size of the inlet title font
*/

/* $Id: cdview.cc,v 1.11 2004/04/22 16:03:25 adrian Exp $ */

#include "cdview.moc"
#include "koverfile.h"
#include "cdview.h"

#include <qimage.h>
#include <qregexp.h>
#include <qprinter.h>
#include <kprinter.h>
#include <qpainter.h>
#include <qsemimodal.h>
#include <kmessagebox.h>
#include <math.h>
#include <stdio.h>

#define FRONT_H	343
#define FRONT_V	338
#define BACK_HI	391
#define BACK_HS	17
#define BACK_V 334

CDView::CDView(KoverFile * _kover_file, QWidget * parent, const char *name)
:QFrame(parent, name)
{
    kover_file = _kover_file;
    connect(kover_file, SIGNAL(dataChanged(bool)), SLOT(dataChanged(bool)));

    setBackgroundColor(white);
    previewMode = false;
    setFrameRect(QRect(0, 0, 0, 0));
    setFrameStyle(WinPanel | Sunken);

    printer = new KPrinter(true, QPrinter::Compatible);
    printer->setOrientation(KPrinter::Landscape);
    printer->setMinMax(1, 2);
    printer->setFromTo(1, 2);
}

void CDView::paintEvent(QPaintEvent *)
{
    QPainter paint(this);

    drawFrame(&paint);

    if (previewMode) {
        drawBooklet(&paint, 4, 4);
        drawInlet(&paint, 150, 4 * 2 + FRONT_V);
        paint.setWorldMatrix(QWMatrix());
        paint.setFont(QFont("helvetica", 14));
        paint.setPen(black);
        paint.drawText(20, 400, tr("Click to close"));
    } else {
        paint.scale(0.4f, 0.4f);
        drawBooklet(&paint, 20, 15);
        drawInlet(&paint, 140, 15 * 2 + FRONT_V);
    }
}

void CDView::showPreview(bool preview)
{
    previewMode = preview;
    update();
}

void CDView::printKover()
{
    if (globals.one_page) {
        printer->setOrientation(KPrinter::Portrait);
        printer->setMinMax(1, 1);
        printer->setFromTo(1, 1);
    }
    if (printer->setup(this)) {
        QPainter *paint = new QPainter(printer);

        previewMode = true;     // hack

        //this is getting ugly
        if (!globals.one_page) {
            if (!globals.inlet_only) {
                if (printer->fromPage() == 1)
                    drawBooklet(paint, 20, 20);
            }
            if (!globals.its_a_slim_case) {
                if (!globals.inlet_only)
                    printer->newPage();
                if (printer->toPage() == 2)
                    drawInlet(paint, 20, 20);
            }
        } else {
            printer->setOrientation(KPrinter::Portrait);
            printer->setMinMax(1, 1);
            printer->setFromTo(1, 1);
            drawBooklet(paint, 20, 15);
            drawInlet(paint, 20, 370);
            printer->setOrientation(KPrinter::Landscape);
            printer->setMinMax(1, 2);
            printer->setFromTo(1, 2);
        }

        previewMode = false;
        paint->end();
        delete(paint);
    }
    return;
}

void CDView::drawBooklet(QPainter * p, int X, int Y)
{
    if (globals.inlet_only)
        return;
    const float scale = 0.4;

    if (globals.one_page)
        p->fillRect(X, Y, FRONT_H, FRONT_V, kover_file->backColor());
    else
        p->fillRect(X, Y, FRONT_H * 2, FRONT_V, kover_file->backColor());

    for (int i = 0; i < 3; i++) {
        if (!images[i].isNull()) {
            switch (kover_file->imageMode(i)) {
            case IMG_CENTER:
                switch (kover_file->imageTarget(i)) {
                case IMG_FRONT_LEFT:
                    if (previewMode)
                        p->setClipRect(X, Y, FRONT_H, FRONT_V);
                    else
                        p->setClipRect((int) (X * scale), (int) (Y * scale),
                            (int) (FRONT_H * scale), (int) (FRONT_V * scale));
                    p->drawPixmap(X + FRONT_H / 2 - images[i].width() / 2,
                        Y + FRONT_V / 2 - images[i].height() / 2, images[i]);
                    p->setClipping(false);
                    break;
                case IMG_FRONT_RIGHT:
                    if (globals.one_page)
                        break;
                    if (previewMode)
                        p->setClipRect(X + FRONT_H, Y, FRONT_H, FRONT_V);
                    else
                        p->setClipRect((int) ((X + FRONT_H) * scale),
                            (int) (Y * scale), (int) (FRONT_H * scale),
                            (int) (FRONT_V * scale));

                    p->drawPixmap(X + FRONT_H + FRONT_H / 2 -
                        images[i].width() / 2,
                        Y + FRONT_V / 2 - images[i].height() / 2, images[i]);
                    p->setClipping(false);
                    break;
                case IMG_FRONT_FULL:
                    if (previewMode) {
                        if (globals.one_page)
                            p->setClipRect(X, Y, FRONT_H, FRONT_V);
                        else
                            p->setClipRect(X, Y, FRONT_H * 2, FRONT_V);
                    } else {
                        if (globals.one_page)
                            p->setClipRect((int) (X * scale),
                                (int) (Y * scale), (int) (FRONT_H * scale),
                                (int) (FRONT_V * scale));
                        else
                            p->setClipRect((int) (X * scale),
                                (int) (Y * scale),
                                (int) (FRONT_H * 2 * scale),
                                (int) (FRONT_V * scale));
                    }
                    if (globals.one_page)
                        p->drawPixmap(X + FRONT_H / 2 - images[i].width() / 2,
                            Y + FRONT_V / 2 - images[i].height() / 2,
                            images[i]);
                    else
                        p->drawPixmap(X + FRONT_H - images[i].width() / 2,
                            Y + FRONT_V / 2 - images[i].height() / 2,
                            images[i]);
                    p->setClipping(false);
                    break;
                }
                break;
            case IMG_TILE:
                switch (kover_file->imageTarget(i)) {
                case IMG_FRONT_LEFT:
                    p->drawTiledPixmap(X, Y, FRONT_H, FRONT_V, images[i]);
                    break;
                case IMG_FRONT_RIGHT:
                    if (globals.one_page)
                        break;
                    p->drawTiledPixmap(X + FRONT_H, Y, FRONT_H, FRONT_V,
                        images[i]);
                    break;
                case IMG_FRONT_FULL:
                    if (globals.one_page)
                        p->drawTiledPixmap(X, Y, FRONT_H, FRONT_V, images[i]);
                    else
                        p->drawTiledPixmap(X, Y, FRONT_H * 2, FRONT_V,
                            images[i]);
                    break;
                }
                break;
            case IMG_STRETCH:
                switch (kover_file->imageTarget(i)) {
                case IMG_FRONT_FULL:
                case IMG_FRONT_LEFT:
                    p->drawPixmap(X, Y, images[i]);
                    break;
                case IMG_FRONT_RIGHT:
                    if (globals.one_page)
                        break;
                    p->drawPixmap(X + FRONT_H, Y, images[i]);
                    break;
                }
                break;
            }
        }
    }

    p->setFont(kover_file->titleFont());
    p->setPen(kover_file->titleColor());

    if (!kover_file->display_title()) {
        if (globals.one_page)
            p->drawText(X + 10, Y + 10, FRONT_H - 20, FRONT_V - 10,
                AlignHCenter, kover_file->title(),
                kover_file->title().length());
        else
            p->drawText(X + FRONT_H, Y + 10, FRONT_H, FRONT_V - 10,
                AlignHCenter, kover_file->title(),
                kover_file->title().length());
    }

    p->setPen(black);

    if (globals.one_page) {
        //top
        p->drawLine(X, Y, X + FRONT_H, Y);
        //bottom
        p->drawLine(X, Y + FRONT_V, X + FRONT_H, Y + FRONT_V);
        //right
    } else {
        //top
        p->drawLine(X, Y, X + FRONT_H * 2, Y);
        //bottom
        p->drawLine(X, Y + FRONT_V, X + FRONT_H * 2, Y + FRONT_V);
        //right
        p->drawLine(X + FRONT_H * 2, Y, X + FRONT_H * 2, Y + FRONT_V);
    }
    //left
    //p->drawLine(X, Y-10, X, Y -8);
    //p->drawLine(X, Y-6, X, Y -4);
    //p->drawLine(X, Y-2, X, Y);
    p->drawLine(X, Y, X, Y + FRONT_V);

    if (kover_file->backColor() == black)
        p->setPen(white);

    //middle
    p->drawLine(X + FRONT_H, Y, X + FRONT_H, Y + FRONT_V);

    if (globals.its_a_slim_case) {
        p->setFont(kover_file->contentsFont());
        p->setPen(kover_file->contentsColor());
        p->drawText(X + 10, Y + 10, FRONT_H - 20, FRONT_V - 10, AlignLeft,
            kover_file->contents(), kover_file->contents().length());
    }
}

void CDView::drawInlet(QPainter * p, int X, int Y)
{
    if (globals.its_a_slim_case)
        return;

    const float scale = 0.4;
    QString title;

    if (kover_file->spine_text())
        title = kover_file->the_spine_text();
    else
        title = kover_file->title();

    title.replace(QRegExp("\n"), " - ");

    p->fillRect(X, Y, BACK_HI + BACK_HS * 2, BACK_V, kover_file->backColor());

    for (int i = 0; i < 3; i++) {
        if (!images[i].isNull()) {
            switch (kover_file->imageMode(i)) {
            case IMG_CENTER:
                switch (kover_file->imageTarget(i)) {
                case IMG_BACK_INNER:
                    if (previewMode)
                        p->setClipRect((X + BACK_HS), Y, BACK_HI, BACK_V);
                    else
                        p->setClipRect((int) ((X + BACK_HS) * scale),
                            (int) (Y * scale), (int) (BACK_HI * scale),
                            (int) (BACK_V * scale));
                    p->drawPixmap(X + BACK_HS + BACK_HI / 2 -
                        images[i].width() / 2,
                        Y + BACK_V / 2 - images[i].height() / 2, images[i]);
                    p->setClipping(false);
                    break;
                case IMG_BACK_FULL:
                    if (previewMode)
                        p->setClipRect(X, Y, (BACK_HI + BACK_HS * 2), BACK_V);
                    else
                        p->setClipRect((int) (X * scale), (int) (Y * scale),
                            (int) ((BACK_HI + BACK_HS * 2) * scale),
                            (int) (BACK_V * scale));
                    p->drawPixmap(X + BACK_HS + BACK_HI / 2 -
                        images[i].width() / 2,
                        Y + BACK_V / 2 - images[i].height() / 2, images[i]);
                    p->setClipping(false);
                    break;
                }
                break;
            case IMG_TILE:
                switch (kover_file->imageTarget(i)) {
                case IMG_BACK_INNER:
                    p->drawTiledPixmap(X + BACK_HS, Y, BACK_HI, BACK_V,
                        images[i]);
                    break;
                case IMG_BACK_FULL:
                    p->drawTiledPixmap(X, Y, BACK_HI + BACK_HS * 2, BACK_V,
                        images[i]);
                    break;
                }
                break;
            case IMG_STRETCH:
                switch (kover_file->imageTarget(i)) {
                case IMG_BACK_INNER:
                    p->drawPixmap(X + BACK_HS, Y, images[i]);
                    break;
                case IMG_BACK_FULL:
                    p->drawPixmap(X, Y, images[i]);
                    break;
                }
                break;
            }
        }
    }

    p->setPen(black);

    //help
    p->drawLine(X -20 , Y, X -15, Y);
    p->drawLine(X -10 , Y, X -5, Y);
    p->drawLine(X + BACK_HI + BACK_HS * 2 +20 , Y, X + BACK_HI + BACK_HS * 2 +15, Y);
    p->drawLine(X + BACK_HI + BACK_HS * 2 +10 , Y, X + BACK_HI + BACK_HS * 2 +5, Y);

    p->drawLine(X -20 , Y+ BACK_V, X -15, Y+ BACK_V);
    p->drawLine(X -10 , Y+ BACK_V, X -5, Y+ BACK_V);
    p->drawLine(X + BACK_HI + BACK_HS * 2 +20 , Y+ BACK_V, X + BACK_HI + BACK_HS * 2 +15, Y+ BACK_V);
    p->drawLine(X + BACK_HI + BACK_HS * 2 +10 , Y+ BACK_V, X + BACK_HI + BACK_HS * 2 +5, Y+ BACK_V);

    p->drawLine(X, Y, X + BACK_HI + BACK_HS * 2, Y);
    p->drawLine(X, Y + BACK_V, X + BACK_HI + BACK_HS * 2, Y + BACK_V);
    p->drawLine(X, Y, X, Y + BACK_V);
    p->drawLine(X + BACK_HI + BACK_HS * 2, Y, X + BACK_HI + BACK_HS * 2,
        Y + BACK_V);

    if (kover_file->backColor() == black)
        p->setPen(white);

    p->drawLine(X + BACK_HS, Y, X + BACK_HS, Y + BACK_V);
    p->drawLine(X + BACK_HS + BACK_HI, Y, X + BACK_HS + BACK_HI, Y + BACK_V);

    p->translate(X, Y + BACK_V);
    p->rotate(-90);

    if (kover_file->number()) {
        QString numberStr;

        numberStr.setNum(kover_file->number());
        p->setPen(red);
        p->setFont(QFont("helvetica", 12, QFont::Bold));
        p->drawText(10, 0, BACK_V - 10, BACK_HS, AlignLeft | AlignVCenter,
            numberStr, numberStr.length());
        p->setPen(kover_file->titleColor());
        p->setFont(kover_file->inlet_title_font());
        p->drawText(38, 0, BACK_V - 38, BACK_HS, AlignLeft | AlignVCenter,
            title, title.length());
    } else {
        p->setPen(kover_file->titleColor());
        p->setFont(kover_file->inlet_title_font());
        p->drawText(10, 0, BACK_V - 10, BACK_HS, AlignLeft | AlignVCenter,
            title, title.length());
    }

    p->translate(BACK_V, BACK_HS + BACK_HI);
    p->rotate(-180);

    if (kover_file->number()) {
        QString numberStr;

        numberStr.setNum(kover_file->number());
        p->setPen(red);
        p->setFont(QFont("helvetica", 12, QFont::Bold));
        p->drawText(10, -1, BACK_V - 10, -BACK_HS, AlignLeft | AlignVCenter,
            numberStr, numberStr.length());
        p->setPen(kover_file->titleColor());
        p->setFont(kover_file->inlet_title_font());
        p->drawText(38, -1, BACK_V - 38, -BACK_HS, AlignLeft | AlignVCenter,
            title, title.length());
    } else {
        p->setPen(kover_file->titleColor());
        p->setFont(kover_file->inlet_title_font());
        p->drawText(10, -1, BACK_V - 10, -BACK_HS, AlignLeft | AlignVCenter,
            title, title.length());
    }

    p->translate(0, BACK_HI);
    p->rotate(-90);

    if (!globals.inlet_only) {
        p->setFont(kover_file->contentsFont());
        p->setPen(kover_file->contentsColor());
        p->drawText(5, 7, BACK_HI - 5, BACK_V - 7, AlignLeft,
            kover_file->contents(), kover_file->contents().length());
    } else {
        p->setFont(kover_file->titleFont());
        p->setPen(kover_file->titleColor());
        p->drawText(5, 7, BACK_HI - 5, BACK_V - 7, AlignHCenter,
            kover_file->title(), kover_file->title().length());

        p->setFont(kover_file->contentsFont());
        p->setPen(kover_file->contentsColor());
        int lines = kover_file->title().contains(QRegExp("\n")) + 2;

        p->drawText(5, kover_file->titleFont().pointSize() * lines + 5,
            BACK_HI - 5, BACK_V - 7, AlignLeft, kover_file->contents(),
            kover_file->contents().length());
    }
}

void CDView::mousePressEvent(QMouseEvent * evt)
{
    int i = evt->x();

    i++;
    if (previewMode)
        emit stopPreview();

    else {
        if (!globals.trigger_actual_size)
            return;
        emit actualSize();
    }
}

void CDView::dataChanged(bool image)
{
    int i = 3;
    _DEBUG_ fprintf(stderr, "kover:CDView::dataChanged() %d\n",
        globals.one_page);

    while (i--) {
        if (kover_file->imageFile(i).isEmpty())
            images[i].resize(0, 0);
    }
    if (image) {
        for (i = 0; i < 3; i++) {
            QImage load(kover_file->imageFile(i));

            if (load.isNull()) {
                images[i].resize(0, 0);
            } else {
                switch (kover_file->imageMode(i)) {
                case IMG_TILE:
                case IMG_CENTER:
                    images[i].convertFromImage(load);
                    break;
                case IMG_STRETCH:
                    switch (kover_file->imageTarget(i)) {
                    case IMG_FRONT_LEFT:
                    case IMG_FRONT_RIGHT:
                        images[i].convertFromImage(load.smoothScale(FRONT_H,
                                FRONT_V));
                        break;
                    case IMG_FRONT_FULL:
                        if (globals.one_page)
                            images[i].convertFromImage(load.
                                smoothScale(FRONT_H, FRONT_V));
                        else
                            images[i].convertFromImage(load.
                                smoothScale(FRONT_H * 2, FRONT_V));
                        break;
                    case IMG_BACK_INNER:
                        images[i].convertFromImage(load.smoothScale(BACK_HI,
                                BACK_V));
                        break;
                    case IMG_BACK_FULL:
                        images[i].convertFromImage(load.
                            smoothScale(BACK_HI + BACK_HS * 2, BACK_V));
                        break;
                    }
                    break;
                }
            }
        }
    }
    repaint(false);
}
