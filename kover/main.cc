/** -*- adrian-c -*-
	 kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
	 Copyright (C) 1999, 2000 by Denis Oliver Kropp
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
	
	 File: main.cc
	
	 Description: File containing the main().

	 Changes:

	 22 Jan 2001: Great new commandline options

	 20 Jun 2001: Lot of undocumented changes

	 21 May 2002: Manual Connection to DCOP server
*/

/* $Id: main.cc,v 1.11 2004/04/29 06:56:31 adrian Exp $ */

#include "kover.h"
#include "kovertop.h"
#include "config.h"

#include <klocale.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <dcopclient.h>
#include <signal.h>

kover_global globals;

KoverTop *kovertop = NULL;

config_class *config = NULL;

int _debug_ = 0;

static const KCmdLineOptions options[] = {
    {"advise", I18N_NOOP("Help me now!"), 0},
    {"debug", I18N_NOOP("Enable debug output."), 0},
    {"+file", I18N_NOOP("File to load"), 0},
    {0, 0, 0}
};

void cleanup()
{
    free(globals.cddb_server);
    free(globals.cgi_path);
    free(globals.proxy_server);
    free(globals.cdrom_device);
    free(globals.cddb_path);
}

void the_end()
{
    if (globals.eject_cdrom) {
        cdrom *cdrom_class = new cdrom(globals.cdrom_device);

        cdrom_class->eject();
        delete cdrom_class;
    }
    if (globals.save_position) {
        globals.xpos = kovertop->x();
        globals.ypos = kovertop->y();
    }
    config->store_globals();
    config->sync();
    cleanup();
    fprintf(stderr, "In Double Vision where drunk.\n");
}

void sighandler(int i)
{
    if (i == 2) {
        _DEBUG_ fprintf(stderr, "kover:SIGINT received...");
    } else {
        _DEBUG_ fprintf(stderr, "kover:SIGTERM received...");
    }
    _DEBUG_ fprintf(stderr, "cleaning up...\n");

    the_end();
    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGTERM, sighandler);
    signal(SIGINT, sighandler);

    fprintf(stderr,
        "%s Release %s - Copyright (C) 1998-2000 by Denis Oliver Kropp\n",
        PACKAGE, VERSION);
    fprintf(stderr,
        "                      Copyright (C) 2000-2004 by Adrian Reber\n");
    fprintf(stderr,
        "%s comes with ABSOLUTELY NO WARRANTY - for details read the license.\n",
        PACKAGE);
    fprintf(stderr,"kover \"now-with-release-name\" release or\n");
    fprintf(stderr,"kover \"yes-i-am-still-alive\" release\n");

    KAboutData aboutData(PACKAGE, I18N_NOOP(PACKAGE), VERSION,
        "Kover is an easy to use WYSIWYG CD cover printer with CDDB support.",
        KAboutData::License_GPL_V2,
        "(C) 1998-2000 Denis Oliver Kropp\n(C) 2000-2004 Adrian Reber");
    aboutData.addAuthor("Denis Oliver Kropp", 0, "dok@fischlustig.de");
    aboutData.addAuthor("Adrian Reber", 0, "adrian@lisas.de");

    KCmdLineArgs::init(argc, argv, &aboutData);
    KCmdLineArgs::addCmdLineOptions(options);

    KApplication kover;

    DCOPClient *client = kover.dcopClient();

    client->attach();

    config = new config_class(&kover);

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    if (args->isSet("advise")) {
        fprintf(stderr, "Don't Panic!\n");
        exit(42);
    }

    if (args->isSet("debug")) {
        _debug_ = 1;
        _DEBUG_ {
            fprintf(stderr, "kover:debug output enabled\n");
        }
        else {
            fprintf(stderr,
                "kover:not with debug support compiled - no debug output\n");
        }
    }

    _DEBUG_ fprintf(stderr, "This is %s %s compiled with %s\n", PACKAGE,
        VERSION, __VERSION__);

    config->load_globals();

    kovertop = new KoverTop();

    if (args->count() > 0) {
        kovertop->fileOpen(args->url(0));
    }

    args->clear();

    kovertop->show();
    int i = kover.exec();

    the_end();

    return i;
}
