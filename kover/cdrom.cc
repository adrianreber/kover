/** -*- adrian-c -*-
	 kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
	 Copyright (C) 2001 by Adrian Reber 
	 
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
	 
	 File: cdrom.cc
	 
	 Description: class for all cdrom accesses
	 
	 Changes:

	 24 Apr 2001: Initial release

*/

#include "cdrom.h"

cdrom::cdrom(char *_path)
{
  path = strdup(_path);
  cdrom_fd = -42;
}

cdrom::~cdrom()
{
  free (path);
}

int cdrom::open()
{
  return 0;
}

int cdrom::eject()
{
  if ((cdrom_fd = ::open(path, O_RDONLY | O_NONBLOCK)) >= 0)
	 ioctl(cdrom_fd,CDROMEJECT);
  close(cdrom_fd);
  cdrom_fd = -42;

  return 0;
}
