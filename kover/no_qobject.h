/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 2005 by Adrian Reber 
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 * $Id: no_qobject.h,v 1.2 2005/06/25 19:01:41 adrian Exp $ */

#ifndef _NO_QOBJECT_H
#define _NO_QOBJECT_H

class no_qobject {
      public:
	virtual ~ no_qobject() {};
	virtual void update_id(unsigned long) = 0;
	virtual void set_status_text(const char *) = 0;
};
#endif
