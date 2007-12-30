/*
 * kover.h
 *
 * Copyright (C) 2007 Adrian Reber <adrian@lisas.de>
 */
#ifndef KOVER_H
#define KOVER_H


#include <kxmlguiwindow.h>

#include "ui_prefs_base.h"

class koverView;
class QPrinter;
class KToggleAction;
class KUrl;

/**
 * This class serves as the main window for kover.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author Adrian Reber <adrian@lisas.de>
 * @version 4
 */
class kover : public KXmlGuiWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    kover();

    /**
     * Default Destructor
     */
    virtual ~kover();

private slots:
    void fileNew();
    void optionsPreferences();

private:
    void setupActions();

private:
    Ui::prefs_base ui_prefs_base ;
    koverView *m_view;

    QPrinter   *m_printer;
    KToggleAction *m_toolbarAction;
    KToggleAction *m_statusbarAction;
};

#endif // _KOVER_H_
