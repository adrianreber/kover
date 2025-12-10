/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 1999, 2000 by Denis Oliver Kropp
 * Copyright (C) 2000, 2025 by Adrian Reber
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

#include <config.h>
#include <kovertop.h>
#include <kover.h>
#include <kover_config.h>

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QUrl>
#include <QPointer>
#include <QIcon>
#include <KAboutData>
#include <KLocalizedString>

#include <signal.h>
#include <cdio/cdio.h>

kover_global globals;

int verbose = 0;

kover_config *config = NULL;
static QPointer<KoverTop> kovertop = NULL;

void
k_printf(const char *fn, int line, const char *format, ...)
{
	char tmp[1024];
	va_list arglist;

	va_start(arglist, format);
	vsnprintf(tmp, 1024, format, arglist);
	fprintf(stderr, "  %s(%s):%s:%d: %s", K_PACKAGE, K_VERSION, fn, line, tmp);
	va_end(arglist);
}

void
eject_cdrom()
{
	CdIo_t *cdio;
	char *device = NULL;

	if (!globals.cdrom_device) {
		device = cdio_get_default_device(NULL);
		if (!device) {
			fprintf(stderr, "%s: Unable to get default CD device.",
				K_PACKAGE);
			return;
		}
	} else
		device = strdup(globals.cdrom_device);
	cdio = cdio_open(device, DRIVER_UNKNOWN);
	cdio_eject_media(&cdio);
	cdio_destroy(cdio);
	free(device);
	return;
}

void
cleanup()
{
	free(globals.cddb_server);
	free(globals.cgi_path);
	free(globals.proxy_server);
	free(globals.cdrom_device);
	free(globals.cddb_path);
}

static void
the_end()
{
	if (globals.eject_cdrom) {
		eject_cdrom();
	}

	config->store_globals();
	config->sync();

	cleanup();
	fprintf(stderr, "In Double Vision where drunk.\n");
}

void
sighandler(int i)
{
	if (i == 2) {
		kprintf("SIGINT received...\n");
	} else {
		kprintf("SIGTERM received...\n");
	}
	kprintf("cleaning up...\n");

	if (kovertop) {
		/* if the preview is currently running; stop it
		 * so that not the values of the preview window
		 * are stored */
		kovertop->stopPreview();

		if (globals.save_position) {
			globals.xpos = kovertop->x();
			globals.ypos = kovertop->y();
		}

		delete kovertop;
	}

	the_end();
	exit(0);
}

int
main(int argc, char **argv)
{
	config = new kover_config();
	signal(SIGTERM, sighandler);
	signal(SIGINT, sighandler);

	// Set the translation domain before any i18n() calls
	KLocalizedString::setApplicationDomain(K_PACKAGE);

	KAboutData about(QStringLiteral(K_PACKAGE),
			 i18n(K_PACKAGE),
			 QStringLiteral(K_VERSION),
			 i18n("Kover is an easy to use WYSIWYG CD cover"
			      " printer with CDDB support."),
			 KAboutLicense::GPL_V2,
			 i18n("(C) 1998, 2000 Denis Oliver Kropp\n(C)"
			      " 2000, 2025 Adrian Reber"),
			 QString(),
			 QStringLiteral(""),
			 QStringLiteral("adrian@lisas.de"));

	about.addAuthor(i18n("Adrian Reber"),
			QString(),
			QStringLiteral("adrian@lisas.de"));

	about.addAuthor(i18n("Denis Oliver Kropp"),
			QString(),
			QStringLiteral("dok@fischlustig.de"));

	// Disable native dialogs to avoid GTK3 theme crashes in minimal environments
	QApplication::setAttribute(Qt::AA_DontUseNativeDialogs);

	QApplication app(argc, argv);
	app.setDesktopFileName(QStringLiteral("kover"));
	app.setWindowIcon(QIcon::fromTheme(QStringLiteral("kover")));
	KAboutData::setApplicationData(about);

	QCommandLineParser parser;
	parser.addPositionalArgument(QStringLiteral("URL"),
				     i18n("Document to open"),
				     QStringLiteral("[URL]"));
	parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("advise"),
					    i18n("Help me now!")));
	parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("debug"),
					    i18n("Enable debug output")));

	about.setupCommandLine(&parser);
	parser.process(app);
	about.processCommandLine(&parser);

	if (parser.isSet(QStringLiteral("advise"))) {
		fprintf(stderr, "Don't Panic!\n");
		exit(42);
	}

	if (parser.isSet(QStringLiteral("debug"))) {
		verbose = 1;
		fprintf(stderr, "\n");
		kprintf("debug output enabled\n");
	}

	config->load_globals();

	kovertop = new KoverTop();

	const QStringList args = parser.positionalArguments();
	if (!args.isEmpty())
		kovertop->fileOpen(QUrl::fromUserInput(args.at(0)));

	kovertop->show();

	int i = app.exec();

	if (kovertop) {
		delete kovertop;
	}

	the_end();
	return i;
}
