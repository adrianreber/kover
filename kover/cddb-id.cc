/* $Id: cddb-id.cc,v 1.1 2001/07/09 09:15:27 adrian Exp $ */

#include "cdrom_cddb.h"
#include <stdio.h>

int main() {
  
  cdrom_cddb *ref = new cdrom_cddb("/dev/cdrom");
  
  unsigned long cddb_id = 0;

  cddb_id = ref->get_cddb_id();
  
  printf("0x%08lx\n",cddb_id);

  return 0;
}
