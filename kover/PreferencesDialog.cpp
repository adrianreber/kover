/** -*- adrian-c -*-
	 kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
	 Copyright (C) 2000, 2001 by Adrian Reber
	 
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
	 
	 File: PreferencesDialog.cpp
	 
	 Description: Contains the implementation of the preferences dialog.
	 
	 Created: 26 Dec 2000
	 
	 Changes:
	 
	 26 Dec 2000: First day of existence...
	 25 Jan 2001: Added entries for cddb, cdrom and cddb_file page
	 27 Jan 2001: Added misc page
*/

#include "PreferencesDialog.h"
#include "KoverTop.h"

PreferencesDialog::PreferencesDialog(QWidget *parent, const QString &caption):KDialogBase(KJanusWidget::IconList,caption,Ok|Cancel,Ok,parent)
{
	 this->parent = parent;
	 setupCDDBPage();
	 setupCDROMPage();
	 setup_cddb_files_page();
	 setup_misc_page();
}

void PreferencesDialog::setupCDDBPage( void )
{
		  
	 QFrame *page = addPage( i18n("CDDB"), i18n("CDDB options"),
									 BarIcon("network", KIcon::SizeMedium ) );
	 QVBoxLayout *topLayout = new QVBoxLayout( page, 0, spacingHint() );

	 QGroupBox *group = new QGroupBox(i18n("&CDDB server"),page );
	 topLayout->addWidget( group );
	 QVBoxLayout *vlay = new QVBoxLayout( group, spacingHint() );
	 vlay->addSpacing( fontMetrics().lineSpacing() );

	 QGridLayout *gbox = new QGridLayout( 5, 2 );
	 vlay->addLayout( gbox );

	 QString text;

	 QLabel *label = new QLabel(i18n("CDDB server:"), group, "cddblabel" );
	 gbox->addWidget( label, 0, 0 );

	 cddb_widgets.cddb_server = new QLineEdit( group, "server" );
	 cddb_widgets.cddb_server->setMinimumWidth( fontMetrics().maxWidth()*10 );
	
	 gbox->addWidget( cddb_widgets.cddb_server, 0, 1 );
	 label = new QLabel(i18n("CGI path:"), group, "cgilabel" );
	 gbox->addWidget( label, 1, 0 );
	 cddb_widgets.cgi_path = new QLineEdit( group, "cgi" );
	 cddb_widgets.cgi_path->setMinimumWidth( fontMetrics().maxWidth()*10 );
	
	 gbox->addWidget( cddb_widgets.cgi_path, 1, 1 );

	 group = new QGroupBox(i18n("&Proxy configuration"),page );
	 topLayout->addWidget( group );
	 vlay = new QVBoxLayout( group, spacingHint() );
	 vlay->addSpacing( fontMetrics().lineSpacing() );

	 gbox = new QGridLayout( 5, 5 );
	 vlay->addLayout( gbox );
	 text = i18n("Use proxy for CDDB lookups");
	 cddb_widgets.use_proxy = new QCheckBox( text, group, "use_proxy" );
	 connect(cddb_widgets.use_proxy,SIGNAL(toggled(bool)),this,SLOT(use_proxy(bool)));
	 gbox->addMultiCellWidget( cddb_widgets.use_proxy, 0, 0 ,0,5);
	 text = i18n("Use 'http_proxy' environment variable");
	 cddb_widgets.proxy_from_env = new QCheckBox( text, group, "proxy_from_env" );
	 gbox->addMultiCellWidget( cddb_widgets.proxy_from_env, 1, 1,0,5 );
	 label = new QLabel(i18n("Proxy server:"), group, "proxylabel" );
	 gbox->addWidget( label, 2, 0 );
	 cddb_widgets.proxy_server = new QLineEdit( group, "proxy" );
	 cddb_widgets.proxy_server->setMinimumWidth( fontMetrics().maxWidth()*10 );
	
	 gbox->addMultiCellWidget( cddb_widgets.proxy_server, 2, 2, 1,5 );
	 label = new QLabel(i18n("Proxy server port:"), group, "proxyportlabel" );
	 gbox->addWidget( label, 3, 0 );
	 cddb_widgets.proxy_port =  new QLineEdit( group, "proxyport" );
	 cddb_widgets.proxy_port->setMaxLength(5);
	
	 gbox->addMultiCellWidget( cddb_widgets.proxy_port, 3, 3,1,5 );

	 set_cddb();

	 topLayout->addStretch(10);
}

void PreferencesDialog::setupCDROMPage( void )
{
		  
	 QFrame *page = addPage( i18n("CDROM"), i18n("CDROM options"),
									 BarIcon("cdrom_unmount", KIcon::SizeMedium ) );
	 QVBoxLayout *topLayout = new QVBoxLayout( page, 0, spacingHint() );

	 QGroupBox *group = new QGroupBox(i18n("&CDROM"),page );
	 topLayout->addWidget( group );
	 QVBoxLayout *vlay = new QVBoxLayout( group, spacingHint() );
	 vlay->addSpacing( fontMetrics().lineSpacing() );
	 QGridLayout *gbox = new QGridLayout( 5, 5 );
	 vlay->addLayout( gbox );

	 QString text;

	 QLabel *label = new QLabel(i18n("CDROM device:"), group, "cdromlabel" );
	 gbox->addWidget( label, 0, 0 );
	 cdrom_widgets.cdrom_device = new QLineEdit( group, "device" );
	 cdrom_widgets.cdrom_device->setMinimumWidth( fontMetrics().maxWidth()*10 );
	 gbox->addMultiCellWidget( cdrom_widgets.cdrom_device, 0, 0,1,5 );

	 text = i18n("Eject CD on exit");
	 cdrom_widgets.eject_cdrom = new QCheckBox( text, group, "eject_cdrom" );
	 gbox->addMultiCellWidget( cdrom_widgets.eject_cdrom, 1, 1,0,5 );

	 set_cdrom();

	 topLayout->addStretch(10);
}


void PreferencesDialog::use_proxy(bool status)
{
	 if (status)
	 {
		  cddb_widgets.proxy_from_env->setEnabled(false);
		  cddb_widgets.proxy_server->setEnabled(true);
		  cddb_widgets.proxy_port->setEnabled(true);
	 }
	 else
	 {
		  cddb_widgets.proxy_from_env->setEnabled(false);
		  cddb_widgets.proxy_server->setEnabled(false);
		  cddb_widgets.proxy_port->setEnabled(false);
	 }

}

void PreferencesDialog::slotOk()
{
	 static int i = 0;
	 if ((((cddb_widgets.proxy_server)->text()).isEmpty() || ((cddb_widgets.proxy_port)->text()).isEmpty()) && (cddb_widgets.use_proxy)->isChecked())
	 {
		  switch (i)
		  {
		  case 0:
				KMessageBox::information(this,"<NELSON>HAHA</NELSON>");
				i++;
				return;
		  case 1:
				KMessageBox::sorry(this,"Please reboot now.");
				i++;
				return;
		  case 2:
				KMessageBox::error(this,"Disable use of a proxy server or enter something!");
		  default:
				i=0;
				return;
		  }
	 }

	 if (((cdrom_widgets.cdrom_device)->text()).isEmpty())
	 {
		  KMessageBox::sorry(this,"Please enter a cdrom device.");
		  return;
	 }
					
	 apply_settings();
	 accept();
}

void PreferencesDialog::apply_settings()
{
	 save_cddb_files();
	 save_misc();

	 if (!((cddb_widgets.proxy_port)->text()).isEmpty())
	 {
		  globals.proxy_port = ((cddb_widgets.proxy_port)->text()).toInt();
	 }
		 
	 if (!((cddb_widgets.cddb_server)->text()).isEmpty())
	 {
					 
#ifdef ENABLE_DEBUG_OUTPUT					 
		  fprintf(stderr,"old value :%s\nnew value: %s\n",globals.cddb_server,((cddb_widgets.cddb_server)->text()).latin1());
#endif
		  free(globals.cddb_server);
		  globals.cddb_server = strdup(((cddb_widgets.cddb_server)->text()).latin1());
	 }
	
	 if (!((cddb_widgets.cgi_path)->text()).isEmpty())
	 {
					 
#ifdef ENABLE_DEBUG_OUTPUT					 
		  fprintf(stderr,"old value :%s\nnew value: %s\n",globals.cgi_path,((cddb_widgets.cgi_path)->text()).latin1());
#endif
		  free(globals.cgi_path);
		  globals.cgi_path = strdup(((cddb_widgets.cgi_path)->text()).latin1());
	 }
			
	 if (!((cddb_widgets.proxy_server)->text()).isEmpty())
	 {
		  if (globals.proxy_server)
				free(globals.proxy_server);
		  globals.proxy_server = strdup(((cddb_widgets.proxy_server)->text()).latin1());
	 }
	 else
	 {
		  if (globals.proxy_server)
				free(globals.proxy_server);
		  globals.proxy_server = NULL;
	 }
		  
		  
	 if ((cddb_widgets.use_proxy)->isChecked())
		  globals.use_proxy = 1;
	 else
		  globals.use_proxy = 0;
		  
	 if ((cddb_widgets.proxy_from_env)->isChecked())
		  globals.proxy_from_env = 1;
	 else
		  globals.proxy_from_env = 0;

	 if ((cdrom_widgets.eject_cdrom)->isChecked())
		  globals.eject_cdrom = 1;
	 else
		  globals.eject_cdrom = 0;
		  
	 if (!((cdrom_widgets.cdrom_device)->text()).isEmpty())
	 {
		  if (globals.cdrom_device)
				free(globals.cdrom_device);
		  globals.cdrom_device = strdup(((cdrom_widgets.cdrom_device)->text()).latin1());
	 }
	 else
	 {
		  if (globals.cdrom_device)
				free(globals.cdrom_device);
		  globals.cdrom_device = NULL;
	 }

}

void PreferencesDialog::slotDefault()
{
	 ((KoverTop *)parent)->load_globals();
	 switch( activePageIndex() )
	 {
	 case page_cddb:
		  set_cddb();
		  break;
					  
	 case page_cdrom:
		  set_cdrom();
		  break;

	 case page_cddb_files:
		  set_cddb_files();
		  break;

	 case page_misc:
		  set_misc();
		  break;

	 default:
		  fprintf(stderr,"kover:%s:%d: this should not happen",__FILE__,__LINE__);
		  return;
	 }
}

void PreferencesDialog::set_cddb()
{
	 QString text;
		  
	 cddb_widgets.cddb_server->setText(globals.cddb_server);
	
	 cddb_widgets.cgi_path->setText(globals.cgi_path);
	
	 if (globals.proxy_server)
		  cddb_widgets.proxy_server->setText(globals.proxy_server);
		  
	 cddb_widgets.proxy_port->setText(text.sprintf("%d",globals.proxy_port));
		  
	 if (globals.use_proxy)
	 {
		  use_proxy(true);
		  cddb_widgets.use_proxy->setChecked(true);
	 }
	 else
	 {
		  use_proxy(false);
		  cddb_widgets.use_proxy->setChecked(false);
	 }
		  
	 if (globals.proxy_from_env)
		  cddb_widgets.proxy_from_env->setChecked(true);
	 else
		  cddb_widgets.proxy_from_env->setChecked(false);

}

void PreferencesDialog::set_cdrom()
{
	 cdrom_widgets.cdrom_device->setText(globals.cdrom_device);
	 if (globals.eject_cdrom)
		  cdrom_widgets.eject_cdrom->setChecked(true);
	 else
		  cdrom_widgets.eject_cdrom->setChecked(false);
}

void PreferencesDialog::set_cddb_files()
{
	 if (globals.read_local_cddb)
		  cddb_files_widgets.read_local_cddb->setChecked(true);
	 else
		  cddb_files_widgets.read_local_cddb->setChecked(false);

	 if (globals.write_local_cddb)
		  cddb_files_widgets.write_local_cddb->setChecked(true);
	 else
		  cddb_files_widgets.write_local_cddb->setChecked(false);

	 cddb_files_widgets.cddb_path->setText(globals.cddb_path);

}

void PreferencesDialog::setup_cddb_files_page(void)
{
		  
	 QFrame *page = addPage( i18n("CDBB files"), i18n("Local CDDB files"),
									 BarIcon("folder_blue", KIcon::SizeMedium ) );
	 QVBoxLayout *topLayout = new QVBoxLayout( page, 0, spacingHint() );

	 QGroupBox *group = new QGroupBox(i18n("&CDROM"),page );
	 topLayout->addWidget( group );
	 QVBoxLayout *vlay = new QVBoxLayout( group, spacingHint() );
	 vlay->addSpacing( fontMetrics().lineSpacing() );
	 QGridLayout *gbox = new QGridLayout( 5, 5 );
	 vlay->addLayout( gbox );

	 QString text;

	 text = i18n("Read CDDB data from file if available");
	 cddb_files_widgets.read_local_cddb = new QCheckBox( text, group, "read_cddb" );
	 gbox->addMultiCellWidget( cddb_files_widgets.read_local_cddb, 0, 0,0,5 );

	 text = i18n("Write CDDB data to file");
	 cddb_files_widgets.write_local_cddb = new QCheckBox( text, group, "write_cddb" );
	 gbox->addMultiCellWidget( cddb_files_widgets.write_local_cddb, 1, 1,0,5 );
		  

	 QLabel *label = new QLabel(i18n("CDDB path:"), group, "pathlabel" );
	 gbox->addWidget( label, 2, 0 );
	 cddb_files_widgets.cddb_path = new QLineEdit( group, "path" );
	 cddb_files_widgets.cddb_path->setMinimumWidth( fontMetrics().maxWidth()*10 );
	 gbox->addMultiCellWidget(cddb_files_widgets.cddb_path, 2, 2,1,5 );

	 set_cddb_files();

	 topLayout->addStretch(10);
}

void PreferencesDialog::save_cddb_files()
{
	 struct stat stat_struct;
		  
	 if ((cddb_files_widgets.read_local_cddb)->isChecked())
		  globals.read_local_cddb = 1;
	 else
		  globals.read_local_cddb = 0;
		  
	 if ((cddb_files_widgets.write_local_cddb)->isChecked())
		  globals.write_local_cddb = 1;
	 else
		  globals.write_local_cddb = 0;
		  
	 if (!((cddb_files_widgets.cddb_path)->text()).isEmpty())
	 {
		  if (globals.cddb_path)
				free(globals.cddb_path);
		  globals.cddb_path = strdup(((cddb_files_widgets.cddb_path)->text()).latin1());
		  /* checking for "/" at the end */
		  if (globals.cddb_path[strlen(globals.cddb_path)-1]!='/')
		  {
				char *dummy = strdup(globals.cddb_path);
				free(globals.cddb_path);
				globals.cddb_path = (char *) malloc(strlen(dummy)+1);
				strcpy(globals.cddb_path,dummy);
				strcat(globals.cddb_path,"/");
				free(dummy);
		  }
	 }
	 else
	 {
		  if (globals.cddb_path)
				free(globals.cddb_path);
		  globals.cddb_path = NULL;
	 }
		  
	 if (globals.cddb_path)
	 {
		  /* does the directory exist */
		  if (stat(globals.cddb_path,&stat_struct)==-1)
				/* no it doesn't... let's create one */
				if(mkdir(globals.cddb_path,0777)==-1)
				{
					 /* failed */
					 free(globals.cddb_path);
					 globals.cddb_path = NULL;
				}
	 }
}

void PreferencesDialog::setup_misc_page()
{
	 QFrame *page = addPage( i18n("Miscellaneous"), i18n("Various properties"),
									 BarIcon("misc", KIcon::SizeMedium ) );
	 QVBoxLayout *topLayout = new QVBoxLayout( page, 0, spacingHint() );

	 QGroupBox *group = new QGroupBox(i18n("&Stuff"),page );
	 topLayout->addWidget( group );
	 QVBoxLayout *vlay = new QVBoxLayout( group, spacingHint() );
	 vlay->addSpacing( fontMetrics().lineSpacing() );
	 QGridLayout *gbox = new QGridLayout( 5, 5 );
	 vlay->addLayout( gbox );

	 QString text;

	 text = i18n("Mouse click on kover preview triggers actual size");
	 misc_widgets.trigger_actual_size = new QCheckBox( text, group, "trigger_actual_size" );
	 gbox->addMultiCellWidget( misc_widgets.trigger_actual_size, 0, 0,0,5 );

	 text = i18n("Display track duration after a CDDB request.");
	 misc_widgets.display_track_duration = new QCheckBox( text, group, "display_track_duration" );
	 gbox->addMultiCellWidget( misc_widgets.display_track_duration, 1, 1,0,5 );

	 set_misc();
}

void PreferencesDialog::set_misc()
{
	 if (globals.trigger_actual_size)
		  misc_widgets.trigger_actual_size->setChecked(true);
	 else
		  misc_widgets.trigger_actual_size->setChecked(false);

	 if (globals.display_track_duration)
		  misc_widgets.display_track_duration->setChecked(true);
	 else
		  misc_widgets.display_track_duration->setChecked(false);
		  
}

void PreferencesDialog::save_misc()
{
	 if ((misc_widgets.trigger_actual_size)->isChecked())
		  globals.trigger_actual_size = 1;
	 else
		  globals.trigger_actual_size = 0;

	 if ((misc_widgets.display_track_duration)->isChecked())
		  globals.display_track_duration = 1;
	 else
		  globals.display_track_duration = 0;
}
