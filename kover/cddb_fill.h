/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 1999-2000 by Denis Oliver Kropp
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
 * $Id: cddb_fill.h,v 1.27 2005/06/25 19:14:41 adrian Exp $ */


#ifndef CDDB_FILL_H
#define CDDB_FILL_H

#include "kover.h"
#include "koverfile.h"
#include "no_qobject.h"
#include <cddb/cddb.h>
#include <stdio.h>
#include <errno.h>
#include <vector>

using namespace std;

class trackinfo {
public:
	trackinfo() {};
	string name;
	unsigned long length;
	unsigned long start;
	unsigned short track;
};

class cdinfo {
public:
	string artist;
	string cdname;
	string category;
	int ntracks;
	int length;
	vector <trackinfo *> tracks;
	unsigned long cddb_id;
};

class cddb_fill {
public:
	cddb_fill(KoverFile *, no_qobject *bla);
	bool execute();
	bool execute_without_cd(const char *, int);
	void cdInfo();
	void setTitleAndContents();
	bool read_cdtext();

private:
	KoverFile * kover_file;
	cdinfo cd_info;
	no_qobject *blub;

	bool readTOC();
	bool cddb_query();
	bool cddb_read(unsigned long, string);
	bool set_connection_params(cddb_conn_t *);
	bool do_proxy_stuff(cddb_conn_t *);
	bool check_for_auth(cddb_conn_t *);
	bool reading_proxy_env_failed();
};

#endif				/* CDDB_FILL_H */
