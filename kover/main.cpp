#include "kover.h"
#include "KoverTop.h"
#include <kimageio.h>
 
int main(int argc, char* argv[]) 
{ 

  
  fprintf(stderr, "%s Release %s - \tCopyright (c) 1998-1999 by Denis Oliver Kropp\n",PACKAGE,VERSION);
  fprintf(stderr, "\t\t\tCopyright (c) 2000 by Adrian Reber\n");
  fprintf(stderr, "kover comes with ABSOLUTELY NO WARRANTY - for details read the license.\n");
	//KImageIO::registerFormats();


  //  KAboutData("kover","Kover",VERSION,"lalalaabout");
  KAboutData aboutData( "kover", I18N_NOOP("kover"),
								VERSION, "A sophisticated super genius's work", KAboutData::License_GPL_V2,
								"(c) 1999-1999, Denis Oliver Kropp\n(c) 2000, Adrian Reber");
        aboutData.addAuthor("Denis Oliver Kropp",0, "dok@fischlustig.de");
		  aboutData.addAuthor("Adrian Reber",0,"adrian@lisas.de");

		  KCmdLineArgs::init( argc, argv, &aboutData );
		  //        KCmdLineArgs::addCmdLineOptions( options );


  KApplication kover;  
  
  KoverTop* kovertop = new KoverTop(&kover);
  
  kovertop->show();  
  return kover.exec();
}
