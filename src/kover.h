/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 2000, 2008 by Adrian Reber
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
 */

#ifndef KOVER_OLD_H
#define KOVER_OLD_H

#include <config.h>

#define kprintf(format, ARGS...)	\
	{ if (verbose) \
		k_printf(__PRETTY_FUNCTION__, __LINE__, format, ##ARGS); }

#include <string>
#include <list>

#include <globals.h>

using std::string;

extern int verbose;

void eject_cdrom();
void k_printf(const char *, int, const char *, ...);

#endif
