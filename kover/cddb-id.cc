/* $Id: cddb-id.cc,v 1.3 2001/12/13 23:29:19 adrian Exp $ */

#include "cdrom_cddb.h"
#include <stdio.h>

char name[255];

int main(int argc, char* argv[]) {
  
  char *device=NULL;

  strncpy(argv[0],name,250);  
  
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
