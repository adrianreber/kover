/** -*- adrian-c -*-
	 kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
	 Copyright (C) 2001-2003 by Adrian Reber 
	 
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
	 
	 File: proxy_auth.cc
	 
	 Description: proxy authentification dialog
	 
	 Changes:

	 30 Sep 2001: Initial release

*/

/* $Id: proxy_auth.cc,v 1.6 2003/02/07 16:44:40 adrian Exp $ */

#include "proxy_auth.moc"
#include "proxy_auth.h"

#include <qpushbutton.h>
#include <qstring.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <stdlib.h>
#include <stdio.h>

proxy_auth::proxy_auth(char *host, int port):QDialog(0, 0, TRUE, 0)
{
    base64encoded = NULL;
    QString greeting;
    QString tmp;
    QVBoxLayout *top_layout = new QVBoxLayout(this);

    top_layout->setMargin(7);
    top_layout->addSpacing(10);
    greeting = tr("Enter username and password for proxy at ");
    tmp.sprintf("%s:%d", host, port);
    greeting += tmp;
    QLabel *label = new QLabel(greeting, this);

    top_layout->addWidget(label);
    top_layout->addSpacing(5);

    user = new QLineEdit(this);
    user->setFocus();
    pw = new QLineEdit(this);
    pw->setEchoMode(QLineEdit::Password);

    top_layout->addWidget(new QLabel(tr("User Name:"), this));
    top_layout->addSpacing(5);
    top_layout->addWidget(user);
    top_layout->addSpacing(5);
    top_layout->addWidget(new QLabel(tr("Password:"), this));
    top_layout->addSpacing(5);
    top_layout->addWidget(pw);
    top_layout->addSpacing(10);
    QBoxLayout *button_layout =
        new QBoxLayout(top_layout, QBoxLayout::RightToLeft, -10);
    QPushButton *ok = new QPushButton(tr("Ok"), this, "ok");

    ok->setDefault(TRUE);
    ok->setMaximumWidth(70);
    connect(ok, SIGNAL(clicked()), SLOT(accept()));
    button_layout->addWidget(ok, 0, AlignRight);
    button_layout->addSpacing(10);
    QPushButton *quit = new QPushButton(tr("Cancel"), this, "quit");

    connect(quit, SIGNAL(clicked()), SLOT(quit()));
    quit->setMaximumWidth(70);
    button_layout->addWidget(quit, 0, AlignRight);
    button_layout->addStretch(20);
    adjustSize();
}

proxy_auth::~proxy_auth()
{
    if (base64encoded)
        free(base64encoded);
}

void proxy_auth::accept()
{
    handle_input();
    QDialog::done(0);
}

void proxy_auth::quit()
{
    handle_input();
    QDialog::done(-1);
}

/** proxy_auth::handle_input()
 *  gets the entered username and password and creates the base64
 *  encoded string needed for the proxy authentification
 *  
 *  I have taken this code from somewhere on the web
 *  no author or copyright information was available
 *
 */
void proxy_auth::handle_input()
{
    QString tmp;
    char lArray[65];
    unsigned char lIn[3];
    char lHold;
    int lDone = 0, lCount = 0;
    char *pwd = NULL;
    char base64[1000];
    int i = 0, bla = 0;

    strcpy(lArray,
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");

    tmp = user->text();
    tmp += ":";
    tmp += pw->text();
    //tmp += "\n";

    pwd = strdup(tmp.latin1());

    bla = strlen(pwd);
    base64[0] = 0;

    while (!lDone) {
        int lRead = 0;

        if (i < bla) {
            lIn[0] = pwd[i++];
            if (i < bla) {
                lIn[1] = pwd[i++];
                if (i < bla) {
                    lIn[2] = pwd[i++];
                    lRead++;
                }
                lRead++;
            }
            lRead++;
        }
        /*
         * Every 19 blocks of three bytes needs to have a line break
         */
        if (lCount == 19 * 3) {
            sprintf(base64, "%c", '\n');
            lCount = 0;
        }
        lCount += lRead;

        /*
         * lRead refers to the number of characters that have been read in.
         * This will always be three until the end of the file.  At the end
         * of a file that isn't a multiple of three in length, lRead will
         * equal 1 or 2.  These special cases are taken care of below.
         */
        switch (lRead) {
        case 0:
            lDone = 1;
            break;
        case 1:
            lHold = lIn[0] >> 2;
            sprintf(base64, "%s%c", base64, lArray[lHold]);
            lHold = (lIn[0] << 4) & 63;
            sprintf(base64, "%s%c", base64, lArray[lHold]);
            /* Add padding */
            strcat(base64, "==");
            lDone = 1;
            break;
        case 2:
            lHold = lIn[0] >> 2;
            sprintf(base64, "%s%c", base64, lArray[lHold]);
            lHold = ((lIn[0] << 4) & 63) + (lIn[1] >> 4);
            sprintf(base64, "%s%c", base64, lArray[lHold]);
            lHold = (lIn[1] << 2) & 63;
            sprintf(base64, "%s%c", base64, lArray[lHold]);
            /* Add padding */
            strcat(base64, "=");
            lDone = 1;
            break;
        case 3:
            lHold = lIn[0] >> 2;
            sprintf(base64, "%s%c", base64, lArray[lHold]);
            lHold = (((lIn[0] << 4) & 63) + (lIn[1] >> 4));
            sprintf(base64, "%s%c", base64, lArray[lHold]);
            lHold = (((lIn[1] << 2) & 63) + (lIn[2] >> 6));
            sprintf(base64, "%s%c", base64, lArray[lHold]);
            lHold = lIn[2] & 63;
            sprintf(base64, "%s%c", base64, lArray[lHold]);
            break;
        default:
            base64[0] = 0;
            fprintf(stderr, "kover:%s:%d: this should not happen\n", __FILE__,
                __LINE__);
            break;
        }
    }

    if (base64[0] == 0)
        base64encoded = NULL;
    else {
        base64encoded = (char *) malloc(strlen(base64) + 1);
        strncpy(base64encoded, base64, strlen(base64) + 1);
    }

    if (pwd)
        free(pwd);
}

char *proxy_auth::get_authentification()
{
    if (base64encoded)
        return strdup(base64encoded);
    return NULL;
}
