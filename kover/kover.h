/** -*- adrian-c -*-
	 kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
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
	 
	 File: kover.h
	 
	 Description: Header to all files
*/

#ifndef KOVER_H
#define KOVER_H

#include "../config.h"

#include <ktoolbarbutton.h>
#include <kaboutapplication.h>
#include <kaboutdata.h>
#include <kmenubar.h>
#include <kaction.h>
#include <kstdaction.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kmainwindow.h>
#include <kedittoolbar.h>
#include <kdialogbase.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <ktmainwindow.h>
#include <kfiledialog.h>
#include <kfontdialog.h>
#include <kcolordialog.h>

#include <qframe.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qtimer.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qmultilinedit.h>
#include <qregexp.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qobject.h>
#include <qstring.h>
#include <qlist.h>

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/ioctl.h>

#include <string>

#include "kover_global.h"

#ifdef HAVE_LINUX_CDROM_H
#include <linux/cdrom.h>
#endif

#ifdef HAVE_LINUX_UCDROM_H
#include <linux/ucdrom.h>
#endif
using std::string;

#define MAIN_TOOLBAR 1
#define CD_TOOLBAR 2
#define SEM_CNT 2
#define SEMKEY  5

#define MODE    0600

#endif
