#include "kover.h"
#include "KoverTop.h"
#include <kimageio.h>
 
int main(int argc, char* argv[]) 
{ 
  char *home_dir = NULL;
  char *cddb_file = NULL;
  struct stat stat_struct;
 



  fprintf(stderr, "%s Release %s - Copyright (C) 1998-1999 by Denis Oliver Kropp\n",PACKAGE,VERSION);
  fprintf(stderr, "                    Copyright (C) 2000 by Adrian Reber\n");
  fprintf(stderr, "%s comes with ABSOLUTELY NO WARRANTY - for details read the license.\n",PACKAGE);


  home_dir = getenv("HOME");
  if (home_dir)
	 {
		if (home_dir[strlen(home_dir)-1]!='/')
		  {
			 cddb_file = (char *) malloc(strlen(home_dir)+7+8);
			 strcpy(cddb_file,home_dir);
			 strcat(cddb_file, "/.cddb/");
		  }
		else
		  {
			 cddb_file = (char *) malloc(strlen(home_dir)+6+8);
			 strcpy(cddb_file,home_dir);
			 strcat(cddb_file, ".cddb/");
		  }
		if (stat(cddb_file,&stat_struct)==-1)
		  mkdir(cddb_file,0777);

		free(cddb_file);
	 }
  
 
	//KImageIO::registerFormats();

  

  //  KAboutData("kover","Kover",VERSION,"lalalaabout");
  KAboutData aboutData(PACKAGE, I18N_NOOP(PACKAGE),VERSION, "Kover is an easy to use WYSIWYG CD cover printer with CDDB support.", KAboutData::License_GPL_V2,"(C) 1999-1999, Denis Oliver Kropp\n(C) 2000, Adrian Reber");
  aboutData.addAuthor("Denis Oliver Kropp",0, "dok@fischlustig.de");
  aboutData.addAuthor("Adrian Reber",0,"adrian@lisas.de");

  KCmdLineArgs::init( argc, argv, &aboutData );
  //        KCmdLineArgs::addCmdLineOptions( options );


  KApplication kover;  
  
  KoverTop* kovertop = new KoverTop(&kover);
  
  kovertop->show();  
  return kover.exec();

 
}
