/**
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 2004 Adrian Reber
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id: cd-text.cc,v 1.3 2004/09/20 14:56:03 adrian Exp $
 *
 * $Author: adrian $
 */

#include "cdtext.h"
#include <stdio.h>
#include <unistd.h>

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

	usleep(10);
	if(cd_text->read_cdtext())
		return 1;
	cd_text->dump();

	return 0;
}
