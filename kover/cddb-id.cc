/* $Id: cddb-id.cc,v 1.4 2002/05/12 17:02:00 adrian Exp $ */

#include "cdrom_cddb.h"
#include <stdio.h>

char name[255];

int main(int argc, char* argv[]) {
  
  char *device=NULL;

  strncpy(name,argv[0],250);  
  
  if (argc!=2)
		device = strdup("/dev/cdrom");
  else
			 device = argv[1];
 
  if (!strcmp(device,"-h")) {
			 fprintf(stderr,"Usage: %s [device]\n",argv[0]);
			 fprintf(stderr,"Use it to display the CDDB id of CD in [device]\n");
			 exit(42);
  }
		  
  cdrom_cddb *ref = new cdrom_cddb(device);
  
  unsigned long cddb_id = 0;

  cddb_id = ref->get_cddb_id();
  
  printf("0x%08lx\n",cddb_id);

  return 0;
}
