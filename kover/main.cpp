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
	
	 File: main.cpp
	
	 Description: File containing the main().

	 Changes:

	 22 Jan 2001: Great new commandline options
*/

#include "kover.h"
#include "KoverTop.h"

kover_global globals;

static const KCmdLineOptions options[] =
{
	 { "advise", I18N_NOOP("Help me now!"), 0 },
	 { 0, 0, 0}
};

void cleanup()
{
	 free (globals.cddb_server);
	 free (globals.cgi_path);
	 free (globals.proxy_server);
	 free (globals.cdrom_device);
	 free (globals.cddb_path);
}

void sighandler(int i)
{
	 if (i==2)
	 {
#ifdef ENABLE_DEBUG_OUTPUT
		  fprintf(stderr,"kover:SIGINT received...");
#endif
	 }
	 else
	 {
#ifdef ENABLE_DEBUG_OUTPUT
		  fprintf(stderr,"kover:SIGTERM received...");
#endif
	 }
#ifdef ENABLE_DEBUG_OUTPUT
	 fprintf(stderr,"cleaning up...\n");
#endif
}

int main(int argc, char* argv[]) 
{
	 int cdrom;

	 //signal(SIGTERM,sighandler);
	 //signal(SIGINT,sighandler);
 
	 fprintf(stderr, "%s Release %s - Copyright (C) 1999, 2000 by Denis Oliver Kropp\n",PACKAGE,VERSION);
	 fprintf(stderr, "                    Copyright (C) 2000, 2001 by Adrian Reber\n");
	 fprintf(stderr, "%s comes with ABSOLUTELY NO WARRANTY - for details read the license.\n",PACKAGE);

	 KAboutData aboutData(PACKAGE, I18N_NOOP(PACKAGE),VERSION, "Kover is an easy to use WYSIWYG CD cover printer with CDDB support.", KAboutData::License_GPL_V2,"(C) 1999, 2000 Denis Oliver Kropp\n(C) 2000, 2001 Adrian Reber");
	 aboutData.addAuthor("Denis Oliver Kropp",0, "dok@fischlustig.de");
	 aboutData.addAuthor("Adrian Reber",0,"adrian@lisas.de");

	 KCmdLineArgs::init(argc,argv,&aboutData);
	 KCmdLineArgs::addCmdLineOptions(options);

	 KApplication kover;  
  
	 KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
  
	 if (args->isSet("advise"))
	 {
		  fprintf(stderr,"Don't Panic!\n");
		  exit(42);
	 }
  
	 args->clear();

		  
	 KoverTop* kovertop = new KoverTop();
  
	 kovertop->load_globals();

	 kovertop->show();  
	 int i = kover.exec();
		  
	 kovertop->store_globals();
	 cleanup();

	 if (globals.eject_cdrom)
	 {
		  if ((cdrom = open(globals.cdrom_device, O_RDONLY | O_NONBLOCK)) >= 0)
				ioctl(cdrom,CDROMEJECT);
	 }

	 fprintf(stderr,"In Double Vision where drunk.\n");
	 return i;
}
