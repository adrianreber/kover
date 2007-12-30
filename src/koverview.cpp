/*
 * koverview.cpp
 *
 * Copyright (C) 2007 Adrian Reber <adrian@lisas.de>
 */
#include "koverview.h"
#include "settings.h"

#include <klocale.h>
#include <QtGui/QLabel>

koverView::koverView(QWidget *)
{
    ui_koverview_base.setupUi(this);
    settingsChanged();
    setAutoFillBackground(true);
}

koverView::~koverView()
{

}

void koverView::switchColors()
{
    // switch the foreground/background colors of the label
    QColor color = Settings::col_background();
    Settings::setCol_background( Settings::col_foreground() );
    Settings::setCol_foreground( color );

    settingsChanged();
}

void koverView::settingsChanged()
{
    QPalette pal;
    pal.setColor( QPalette::Window, Settings::col_background());
    pal.setColor( QPalette::WindowText, Settings::col_foreground());
    ui_koverview_base.kcfg_sillyLabel->setPalette( pal );

    // i18n : internationalization
    ui_koverview_base.kcfg_sillyLabel->setText( i18n("This project is %1 days old",Settings::val_time()) );
    emit signalChangeStatusbar( i18n("Settings changed") );
}

#include "koverview.moc"
