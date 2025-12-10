/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 2001, 2025 by Adrian Reber
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef CDDB_211_ITEM_H
#define CDDB_211_ITEM_H

#include <string>
#include <config.h>

using namespace std;

class cddb_211_item {
 public:
	cddb_211_item();
	cddb_211_item(char *);
	cddb_211_item(string);
	~cddb_211_item();
	char *get();
	void set_title(string);
	void set_artist(string);
	void set_category(string);
	void set_id(unsigned long);
	string get_title();
	string get_artist();
	string get_category();
	unsigned long get_id();
 private:
	char *cddb_data;
	string title;
	string artist;
	string category;
	unsigned long id;
};

#endif /* CDDB_211_ITEM_H */
