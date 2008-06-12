/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 2001, 2008 by Adrian Reber
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

#include <koverconfig.h>
#include <globals.h>
#include <kover_old.h>
#include <config.h>

#include <ksharedconfig.h>
#include <kglobal.h>
#include <kconfiggroup.h>

#include <ctype.h>
#include <stdio.h>
#include <cddb/cddb.h>

void config_class::load_globals()
{
	QString *string = new QString();

	KConfigGroup config = KGlobal::config()->group("CDDB");

	if ((config.readEntry("cddb_server")).isEmpty())
		globals.cddb_server = strdup("freedb.freedb.org");
	else
		globals.cddb_server =
		    strdup((config.readEntry("cddb_server")).toUtf8());

	if ((config.readEntry("cgi_path")).isEmpty())
		globals.cgi_path = strdup("~cddb/cddb.cgi");
	else
		globals.cgi_path =
		    strdup((config.readEntry("cgi_path")).toUtf8());

	if ((config.readEntry("proxy_server")).isEmpty())
		globals.proxy_server = NULL;
	else
		globals.proxy_server =
		    strdup((config.readEntry("proxy_server")).toUtf8());

	*string = config.readEntry("proxy_port");
	if (string->isEmpty())
		globals.proxy_port = 3128;
	else
		globals.proxy_port = string->toInt();

	kprintf("proxy port: %d\n", globals.proxy_port);

	*string = config.readEntry("use_proxy");
	if (string->isNull())
		globals.use_proxy = 0;
	else
		globals.use_proxy = string->toInt();

	*string = config.readEntry("use_cddbp");
	if (string->isNull())
		globals.use_cddbp = 0;
	else
		globals.use_cddbp = string->toInt();

	*string = config.readEntry("proxy_from_env");
	if (string->isNull())
		globals.proxy_from_env = 0;
	else
		globals.proxy_from_env = string->toInt();

	config = KGlobal::config()->group("CDROM");

	if ((config.readEntry("cdrom_device")).isEmpty())
		globals.cdrom_device = strdup("/dev/cdrom");
	else
		globals.cdrom_device =
		    strdup((config.readEntry("cdrom_device")).toUtf8());

	*string = config.readEntry("eject_cdrom");
	if (string->isNull())
		globals.eject_cdrom = 0;
	else
		globals.eject_cdrom = string->toInt();

	config = KGlobal::config()->group("CDDB_files");

	*string = config.readEntry("use_cache");
	if (string->isNull())
		globals.use_cache = 0;
	else
		globals.use_cache = string->toInt();
	if ((config.readEntry("cddb_path")).isEmpty()) {
		globals.cddb_path = check_cddb_dir();
	} else {
		globals.cddb_path =
		    strdup((config.readEntry("cddb_path")).toUtf8());
	}

	config = KGlobal::config()->group("misc");
	*string = config.readEntry("trigger_actual_size");
	if (string->isNull())
		globals.trigger_actual_size = 1;
	else
		globals.trigger_actual_size = string->toInt();

	*string = config.readEntry("xpos");
	if (string->isNull())
		globals.xpos = 0;
	else
		globals.xpos = string->toInt();

	*string = config.readEntry("ypos");
	if (string->isNull())
		globals.ypos = 0;
	else
		globals.ypos = string->toInt();

	*string = config.readEntry("save_position");
	if (string->isNull())
		globals.save_position = 1;
	else
		globals.save_position = string->toInt();

	*string = config.readEntry("disable_animation");
	if (string->isNull())
		globals.disable_animation = 0;
	else
		globals.disable_animation = string->toInt();

	*string = config.readEntry("display_track_duration");
	if (string->isNull())
		globals.display_track_duration = 1;
	else
		globals.display_track_duration = string->toInt();

	config = KGlobal::config()->group("cover");
	*string = config.readEntry("its_a_slim_case");
	if (string->isNull())
		globals.its_a_slim_case = 0;
	else
		globals.its_a_slim_case = string->toInt();

	*string = config.readEntry("one_page");
	if (string->isNull())
		globals.one_page = 0;
	else
		globals.one_page = string->toInt();

	*string = config.readEntry("inlet_only");
	if (string->isNull())
		globals.inlet_only = 0;
	else
		globals.inlet_only = string->toInt();

	config = KGlobal::config()->group("fonts");
	if ((config.readEntry("content_font_settings")).isEmpty())
		globals.content_font = new QFont("helvetica", 16);
	else {
		QFont *bla = new QFont();

		bla->fromString(config.
				readEntry("content_font_settings"));
		globals.content_font = new QFont(*bla);
		delete(bla);
		kprintf("font loaded: %s\n", ((globals.content_font)->family()).toUtf8().constData());
	}

	if ((config.readEntry("title_font_settings")).isEmpty())
		globals.title_font = new QFont("helvetica", 32);
	else {
		QFont *bla = new QFont();

		bla->fromString(config.readEntry("title_font_settings"));
		globals.title_font = new QFont(*bla);
		delete(bla);
		kprintf("font loaded: %s\n", ((globals.title_font)->family()).toUtf8().constData());
	}

	if ((config.readEntry("inlet_title_font_settings")).isEmpty())
		globals.inlet_title_font = new QFont("helvetica", 10);
	else {
		QFont *bla = new QFont();

		bla->fromString(config.
				readEntry("inlet_title_font_settings"));
		globals.inlet_title_font = new QFont(*bla);
		delete(bla);
		kprintf("font loaded: %s\n%s\n", ((globals.inlet_title_font)->family()).toUtf8().constData(),
				config.readEntry("inlet_title_font").toUtf8().constData());
	}

	delete(string);
	globals.username = NULL;
	globals.password = NULL;
}

void config_class::store_globals()
{
	kprintf("entering config_class::store_globals()\n");
	KConfigGroup config = KGlobal::config()->group("CDDB");
	QString *string = new QString();

#define writeEntryy(bla, blubber) writeEntry(QString(bla), QString(blubber))
	config.writeEntryy("cddb_server", globals.cddb_server);
	config.writeEntryy("cgi_path", globals.cgi_path);
	config.writeEntryy("proxy_server", globals.proxy_server);
	string->sprintf("%d", globals.proxy_port);
	config.writeEntry("proxy_port", *string);
	string->sprintf("%d", globals.use_proxy);
	config.writeEntry("use_proxy", *string);
	string->sprintf("%d", globals.use_cddbp);
	config.writeEntry("use_cddbp", *string);
	string->sprintf("%d", globals.proxy_from_env);
	config.writeEntry("proxy_from_env", *string);

	config = KGlobal::config()->group("CDROM");
	string->sprintf("%d", globals.eject_cdrom);
	config.writeEntry("eject_cdrom", *string);
	config.writeEntryy("cdrom_device", globals.cdrom_device);

	config = KGlobal::config()->group("CDDB_files");
	string->sprintf("%d", globals.use_cache);
	config.writeEntry("use_cache", *string);
	config.writeEntryy("cddb_path", globals.cddb_path);

	config = KGlobal::config()->group("misc");
	string->sprintf("%d", globals.trigger_actual_size);
	config.writeEntry("trigger_actual_size", *string);

	string->sprintf("%d", globals.display_track_duration);
	config.writeEntry("display_track_duration", *string);

	string->sprintf("%d", globals.xpos);
	config.writeEntry("xpos", *string);

	string->sprintf("%d", globals.ypos);
	config.writeEntry("ypos", *string);

	string->sprintf("%d", globals.save_position);
	config.writeEntry("save_position", *string);

	string->sprintf("%d", globals.disable_animation);
	config.writeEntry("disable_animation", *string);

	config = KGlobal::config()->group("cover");
	string->sprintf("%d", globals.its_a_slim_case);
	config.writeEntry("its_a_slim_case", *string);

	string->sprintf("%d", globals.one_page);
	config.writeEntry("one_page", *string);

	string->sprintf("%d", globals.inlet_only);
	config.writeEntry("inlet_only", *string);

	config = KGlobal::config()->group("fonts");
	config.writeEntry("content_font_settings",
			   ((globals.content_font)->toString()));
	config.writeEntry("title_font_settings",
			   ((globals.title_font)->toString()));
	config.writeEntry("inlet_title_font_settings",
			   ((globals.inlet_title_font)->toString()));

	delete(string);
	kprintf("leaving config_class::store_globals()\n");
}

char *config_class::check_cddb_dir()
{
	char *cddb_path = NULL;
#ifdef CDDB_PATH
	cddb_path = strdup(CDDB_PATH);
#else
	cddb_conn_t *conn = cddb_new();
	cddb_path = (char *)cddb_cache_get_dir(conn);
	cddb_path = strdup(cddb_path);
	cddb_destroy(conn);
#endif
	return cddb_path;
}

void config_class::sync()
{
	KGlobal::config()->sync();
}
