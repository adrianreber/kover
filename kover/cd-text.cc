/* $Id: cd-text.cc,v 1.1 2004/09/17 19:06:18 adrian Exp $ */

#include "cdtext.h"
#include <stdio.h>

int main(int argc, char *argv[])
{

	char *device = NULL;

	if (argc != 2)
		device = strdup("/dev/cdrom");
	else
		device = argv[1];

	if (!strcmp(device, "-h")) {
		fprintf(stderr, "Usage: %s [device]\n", argv[0]);
		fprintf(stderr,
		    "Use it to display the CD-TEXT of the CD in [device]\n");
		exit(42);
	}

	cdtext *cd_text = new cdtext(device);

	cd_text->read_cdtext();
	cd_text->dump();

	return 0;
}
