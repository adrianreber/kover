/**
	kover -

	
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
	
	File: PreferencesDialog.h
	
	Description: Header to PreferencesDialog.cpp
*/

#ifndef PREFERENCES_DIALOG_H
#define PREFERENCES_DIALOG_H

#include "kover.h"

class PreferencesDialog : public KDialogBase
{
  Q_OBJECT

public:
  PreferencesDialog(QWidget *parent, const QString &caption);
  //~PreferencesDialog();

private:


};

#endif
