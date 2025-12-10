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

#include <kover_config.h>
#include <kover.h>

#include <KSharedConfig>
#include <KConfigGroup>

#include <ctype.h>
#include <stdio.h>
#include <cddb/cddb.h>

void kover_config::load_globals()
{
	QString *string = new QString();

	KConfigGroup config = KSharedConfig::openConfig()->group(QStringLiteral("CDDB"));

	if ((config.readEntry(QStringLiteral("cddb_server"))).isEmpty())
		globals.cddb_server = strdup("freedb.freedb.org");
	else
		globals.cddb_server =
		    strdup((config.readEntry(QStringLiteral("cddb_server"))).toUtf8().constData());

	if ((config.readEntry(QStringLiteral("cgi_path"))).isEmpty())
		globals.cgi_path = strdup("~cddb/cddb.cgi");
	else
		globals.cgi_path =
		    strdup((config.readEntry(QStringLiteral("cgi_path"))).toUtf8().constData());

	if ((config.readEntry(QStringLiteral("proxy_server"))).isEmpty())
		globals.proxy_server = NULL;
	else
		globals.proxy_server =
		    strdup((config.readEntry(QStringLiteral("proxy_server"))).toUtf8().constData());

	*string = config.readEntry(QStringLiteral("proxy_port"));
	if (string->isEmpty())
		globals.proxy_port = 3128;
	else
		globals.proxy_port = string->toInt();

	kprintf("proxy port: %d\n", globals.proxy_port);

	*string = config.readEntry(QStringLiteral("use_proxy"));
	if (string->isNull())
		globals.use_proxy = 0;
	else
		globals.use_proxy = string->toInt();

	*string = config.readEntry(QStringLiteral("use_cddbp"));
	if (string->isNull())
		globals.use_cddbp = 0;
	else
		globals.use_cddbp = string->toInt();

	*string = config.readEntry(QStringLiteral("proxy_from_env"));
	if (string->isNull())
		globals.proxy_from_env = 0;
	else
		globals.proxy_from_env = string->toInt();

	config = KSharedConfig::openConfig()->group(QStringLiteral("CDROM"));

	if ((config.readEntry(QStringLiteral("cdrom_device"))).isEmpty())
		globals.cdrom_device = strdup("/dev/cdrom");
	else
		globals.cdrom_device =
		    strdup((config.readEntry(QStringLiteral("cdrom_device"))).toUtf8().constData());

	*string = config.readEntry(QStringLiteral("eject_cdrom"));
	if (string->isNull())
		globals.eject_cdrom = 0;
	else
		globals.eject_cdrom = string->toInt();

	config = KSharedConfig::openConfig()->group(QStringLiteral("CDDB_files"));

	*string = config.readEntry(QStringLiteral("use_cache"));
	if (string->isNull())
		globals.use_cache = 0;
	else
		globals.use_cache = string->toInt();
	if ((config.readEntry(QStringLiteral("cddb_path"))).isEmpty()) {
		globals.cddb_path = check_cddb_dir();
	} else {
		globals.cddb_path =
		    strdup((config.readEntry(QStringLiteral("cddb_path"))).toUtf8().constData());
	}

	config = KSharedConfig::openConfig()->group(QStringLiteral("misc"));
	*string = config.readEntry(QStringLiteral("trigger_actual_size"));
	if (string->isNull())
		globals.trigger_actual_size = 1;
	else
		globals.trigger_actual_size = string->toInt();

	*string = config.readEntry(QStringLiteral("xpos"));
	if (string->isNull())
		globals.xpos = 0;
	else
		globals.xpos = string->toInt();

	*string = config.readEntry(QStringLiteral("ypos"));
	if (string->isNull())
		globals.ypos = 0;
	else
		globals.ypos = string->toInt();

	*string = config.readEntry(QStringLiteral("save_position"));
	if (string->isNull())
		globals.save_position = 1;
	else
		globals.save_position = string->toInt();

	*string = config.readEntry(QStringLiteral("disable_animation"));
	if (string->isNull())
		globals.disable_animation = 0;
	else
		globals.disable_animation = string->toInt();

	*string = config.readEntry(QStringLiteral("display_track_duration"));
	if (string->isNull())
		globals.display_track_duration = 1;
	else
		globals.display_track_duration = string->toInt();

	config = KSharedConfig::openConfig()->group(QStringLiteral("cover"));
	*string = config.readEntry(QStringLiteral("its_a_slim_case"));
	if (string->isNull())
		globals.its_a_slim_case = 0;
	else
		globals.its_a_slim_case = string->toInt();

	*string = config.readEntry(QStringLiteral("one_page"));
	if (string->isNull())
		globals.one_page = 0;
	else
		globals.one_page = string->toInt();

	*string = config.readEntry(QStringLiteral("inlet_only"));
	if (string->isNull())
		globals.inlet_only = 0;
	else
		globals.inlet_only = string->toInt();

	config = KSharedConfig::openConfig()->group(QStringLiteral("fonts"));
	if ((config.readEntry(QStringLiteral("content_font_settings"))).isEmpty())
		globals.content_font = new QFont(QStringLiteral("helvetica"), 16);
	else {
		QFont *loadedFont = new QFont();

		loadedFont->fromString(config.
				readEntry(QStringLiteral("content_font_settings")));
		if (loadedFont->pointSize() <= 0)
			loadedFont->setPointSize(16);
		globals.content_font = new QFont(*loadedFont);
		delete(loadedFont);
		kprintf("font loaded: %s\n", ((globals.content_font)->family()).toUtf8().constData());
	}

	if ((config.readEntry(QStringLiteral("title_font_settings"))).isEmpty())
		globals.title_font = new QFont(QStringLiteral("helvetica"), 32);
	else {
		QFont *loadedFont = new QFont();

		loadedFont->fromString(config.readEntry(QStringLiteral("title_font_settings")));
		if (loadedFont->pointSize() <= 0)
			loadedFont->setPointSize(32);
		globals.title_font = new QFont(*loadedFont);
		delete(loadedFont);
		kprintf("font loaded: %s\n", ((globals.title_font)->family()).toUtf8().constData());
	}

	if ((config.readEntry(QStringLiteral("inlet_title_font_settings"))).isEmpty())
		globals.inlet_title_font = new QFont(QStringLiteral("helvetica"), 10);
	else {
		QFont *loadedFont = new QFont();

		loadedFont->fromString(config.
				readEntry(QStringLiteral("inlet_title_font_settings")));
		if (loadedFont->pointSize() <= 0)
			loadedFont->setPointSize(10);
		globals.inlet_title_font = new QFont(*loadedFont);
		delete(loadedFont);
		kprintf("font loaded: %s\n%s\n", ((globals.inlet_title_font)->family()).toUtf8().constData(),
				config.readEntry(QStringLiteral("inlet_title_font")).toUtf8().constData());
	}

	delete(string);
	globals.username = NULL;
	globals.password = NULL;
}

void kover_config::store_globals()
{
	kprintf("entering kover_config::store_globals()\n");
	KConfigGroup config = KSharedConfig::openConfig()->group(QStringLiteral("CDDB"));
	QString *string = new QString();

#define writeEntryy(bla, blubber) writeEntry(QStringLiteral(bla), QString::fromUtf8(blubber))
	config.writeEntryy("cddb_server", globals.cddb_server);
	config.writeEntryy("cgi_path", globals.cgi_path);
	config.writeEntryy("proxy_server", globals.proxy_server);
	*string = QString::number(globals.proxy_port);
	config.writeEntry(QStringLiteral("proxy_port"), *string);
	*string = QString::number(globals.use_proxy);
	config.writeEntry(QStringLiteral("use_proxy"), *string);
	*string = QString::number(globals.use_cddbp);
	config.writeEntry(QStringLiteral("use_cddbp"), *string);
	*string = QString::number(globals.proxy_from_env);
	config.writeEntry(QStringLiteral("proxy_from_env"), *string);

	config = KSharedConfig::openConfig()->group(QStringLiteral("CDROM"));
	*string = QString::number(globals.eject_cdrom);
	config.writeEntry(QStringLiteral("eject_cdrom"), *string);
	config.writeEntryy("cdrom_device", globals.cdrom_device);

	config = KSharedConfig::openConfig()->group(QStringLiteral("CDDB_files"));
	*string = QString::number(globals.use_cache);
	config.writeEntry(QStringLiteral("use_cache"), *string);
	config.writeEntryy("cddb_path", globals.cddb_path);

	config = KSharedConfig::openConfig()->group(QStringLiteral("misc"));
	*string = QString::number(globals.trigger_actual_size);
	config.writeEntry(QStringLiteral("trigger_actual_size"), *string);

	*string = QString::number(globals.display_track_duration);
	config.writeEntry(QStringLiteral("display_track_duration"), *string);

	*string = QString::number(globals.xpos);
	config.writeEntry(QStringLiteral("xpos"), *string);

	*string = QString::number(globals.ypos);
	config.writeEntry(QStringLiteral("ypos"), *string);

	*string = QString::number(globals.save_position);
	config.writeEntry(QStringLiteral("save_position"), *string);

	*string = QString::number(globals.disable_animation);
	config.writeEntry(QStringLiteral("disable_animation"), *string);

	config = KSharedConfig::openConfig()->group(QStringLiteral("cover"));
	*string = QString::number(globals.its_a_slim_case);
	config.writeEntry(QStringLiteral("its_a_slim_case"), *string);

	*string = QString::number(globals.one_page);
	config.writeEntry(QStringLiteral("one_page"), *string);

	*string = QString::number(globals.inlet_only);
	config.writeEntry(QStringLiteral("inlet_only"), *string);

	config = KSharedConfig::openConfig()->group(QStringLiteral("fonts"));
	config.writeEntry(QStringLiteral("content_font_settings"),
			   ((globals.content_font)->toString()));
	config.writeEntry(QStringLiteral("title_font_settings"),
			   ((globals.title_font)->toString()));
	config.writeEntry(QStringLiteral("inlet_title_font_settings"),
			   ((globals.inlet_title_font)->toString()));

	delete(string);
	kprintf("leaving kover_config::store_globals()\n");
}

char *kover_config::check_cddb_dir()
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

void kover_config::sync()
{
	KSharedConfig::openConfig()->sync();
}
