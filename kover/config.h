/** -*- adrian-c -*-, ho!
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
	 
	 File: config.h
	 
	 Description: header for the config class
	 
	 Changes:
	 
	 13 Jun 2001: Initial creation 
*/

#ifndef CONFIG_H
#define CONFIG_H

#include "kover.h"
#include <kapp.h>

class config_class {

  public:
    config_class(KApplication * kover);
    void store_globals();
    void load_globals();
    void sync();
  private:
    void check_categories(char * check);
    char *check_cddb_dir();
    KApplication *kover;

};

#endif /* CONFIG_H */
