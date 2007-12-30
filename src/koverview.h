/*
 * koverview.h
 *
 * Copyright (C) 2007 Adrian Reber <adrian@lisas.de>
 */
#ifndef KOVERVIEW_H
#define KOVERVIEW_H

#include <QtGui/QWidget>

#include "ui_koverview_base.h"

class QPainter;
class KUrl;

/**
 * This is the main view class for kover.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go
 * here.
 *
 * @short Main view
 * @author Adrian Reber <adrian@lisas.de>
 * @version 4
 */

class koverView : public QWidget, public Ui::koverview_base
{
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    koverView(QWidget *parent);

    /**
     * Destructor
     */
    virtual ~koverView();

private:
    Ui::koverview_base ui_koverview_base;

signals:
    /**
     * Use this signal to change the content of the statusbar
     */
    void signalChangeStatusbar(const QString& text);

    /**
     * Use this signal to change the content of the caption
     */
    void signalChangeCaption(const QString& text);

private slots:
    void switchColors();
    void settingsChanged();
};

#endif // koverVIEW_H
