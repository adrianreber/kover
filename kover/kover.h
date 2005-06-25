/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 2000-2005 by Adrian Reber 
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
 * $Id: kover.h,v 1.16 2005/06/25 19:43:24 adrian Exp $ */

#ifndef KOVER_H
#define KOVER_H

#include "../config.h"

#ifdef ENABLE_DEBUG_OUTPUT
#define _DEBUG_ if(_debug_)
#else
#define _DEBUG_ if(0)
#endif

#include <string>
#include <list>

#include "kover_global.h"

using std::string;

extern int _debug_;

void eject_cdrom();

#endif
