/*
 * kover - Kover is an easy to use WYSIWYG CD cover printer with CDDB support.
 * Copyright (C) 1999, 2000 by Denis Oliver Kropp
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

#include <globals.h>
#include "cddb_fill.h"
#include <string>
#include <klocale.h>
#include <cdio/cdda.h>
#include <cdio/cdtext.h>
#include "cddb_211_item.h"
#include "inexact_dialog.h"
#include "pa.h"
#include "categories.h"
#include <kover_old.h>

cddb_fill::cddb_fill(KoverFile *_kover_file, no_qobject *bla)
{
	kover_file = _kover_file;
	blub = bla;
}

bool
cddb_fill::read_cdtext()
{
	CdIo_t *cdio;
	char *device = NULL;
	const char *status = "Unable to get default CD device.";
	const char *unable = "unable to open CD device";

	cd_info.artist = "Artist";
	cd_info.cdname = "Title";
	cd_info.length = 0;
	cd_info.ntracks = 0;
	cd_info.tracks.clear();
	cd_info.cddb_id = 0;

	if (!globals.cdrom_device) {
		device = cdio_get_default_device(NULL);
		if (!device) {
			blub->set_status_text(i18n(status).toUtf8());
			return false;
		}

	} else
		device = globals.cdrom_device;

	kprintf("CD-ROM device: %s\n", device);

	cdio = cdio_open(device, DRIVER_UNKNOWN);
	cd_info.ntracks = cdio_get_num_tracks(cdio);
	kprintf("CDIO_INVALID_TRACK %d\n", CDIO_INVALID_TRACK);

	if (!cdio) {
		blub->set_status_text(unable);
		kprintf("%s\n", unable);
		return false;
	}

	/* Get the track count for the CD. */
	cd_info.ntracks = cdio_get_num_tracks(cdio);

	kprintf("cdio_get_num_tracks: %d\n", cd_info.ntracks);
	kprintf("CDIO_INVALID_TRACK %d\n", CDIO_INVALID_TRACK);

	if (cd_info.ntracks == CDIO_INVALID_TRACK) {
		blub->set_status_text(unable);
		kprintf("%s\n", unable);
		cdio_destroy(cdio);
		return false;
	}

	kprintf("device %p\n", cdio);

	/* get disc artist and title */
	cdtext_t *cdtext = cdio_get_cdtext(cdio, 0);
	if (cdtext) {
		if (cdtext->field[CDTEXT_PERFORMER])
			cd_info.artist = cdtext->field[CDTEXT_PERFORMER];
		if (cdtext->field[CDTEXT_TITLE])
			cd_info.cdname = cdtext->field[CDTEXT_TITLE];
		if (cdtext->field[CDTEXT_DISCID])
			cd_info.cddb_id =
				strtoul(cdtext->field[CDTEXT_DISCID], NULL, 16);

	}

	for (int i = 1; i <= cd_info.ntracks; i++) {
		cdtext_t *cdtext = cdio_get_cdtext(cdio, i);
		trackinfo *blub = new trackinfo();
		blub->track = i;
		if (cdtext->field[CDTEXT_TITLE])
			blub->name = cdtext->field[CDTEXT_TITLE];
		cd_info.tracks.push_back(blub);

	}

	cdio_destroy(cdio);
	return true;
}

bool
cddb_fill::execute()
{
	if (!readTOC())
		return false;

	if (!cddb_query())
		return false;

	return true;
}

bool
cddb_fill::execute_without_cd(const char *id, int cat)
{
	bool without = true;
	categories *category = new categories();

	cd_info.cddb_id = strtoul(id, NULL, 16);
	cd_info.category = category->get_category(cat);

	cd_info.artist = "Artist";
	cd_info.cdname = "Title";
	cd_info.length = 0;
	cd_info.ntracks = 0;
	cd_info.tracks.clear();

	without = cddb_read(cd_info.cddb_id, cd_info.category);

	if (category) {
		delete (category);
		category = NULL;
	}

	return without;
}

void
cddb_fill::setTitleAndContents()
{
	QString tracks, contents, cddb_id;
	string artist = cd_info.artist + "\n" + cd_info.cdname;

	kover_file->setTitle(artist.c_str());
	for (int i = 0; i < cd_info.ntracks; i++) {
		if (globals.display_track_duration) {
			int m = 0;
			int n = 0;

			m = FRAMES_TO_SECONDS(cd_info.tracks[i]->length) / 60;
			n = FRAMES_TO_SECONDS(cd_info.tracks[i]->length) % 60;
			tracks.sprintf("(%.2d:%.2d)-%.2d. ", m, n, i + 1);
		} else
			tracks.sprintf("%.2d. ", i + 1);

		tracks.append(cd_info.tracks[i]->name.c_str());
		if (i != cd_info.ntracks - 1)
			tracks.append("\n");
		contents.append(tracks);
	}
	kover_file->setContents(contents);
	cddb_id.sprintf("0x%lx", cd_info.cddb_id);
	kover_file->set_cddb_id(cddb_id);
}

void
cddb_fill::get_info()
{
	const char *contains = "CD contains %d tracks, "
			       "total time is %d:%02d, "
			       "the magic number is 0x%lx";
	char str[256];

	sprintf(str, contains, cd_info.ntracks, cd_info.length / 60,
		cd_info.length % 60,
		cd_info.cddb_id);

	kprintf("%s\n", str);

	blub->set_status_text(str);
	blub->update_id(cd_info.cddb_id);
}

bool
cddb_fill::readTOC()
{
	CdIo_t *cdio = NULL;
	track_t cnt, t;
	lsn_t lsn;
	int i, pos;
	char *device = NULL;
	const char *status = "Unable to get default CD device.";

	cd_info.artist = "Artist";
	cd_info.cdname = "Title";
	cd_info.length = 0;
	cd_info.ntracks = 0;
	cd_info.tracks.clear();

	if (!globals.cdrom_device) {
		device = cdio_get_default_device(NULL);
		if (!device) {
			blub->set_status_text(i18n(status).toUtf8());
			return false;
		}

	}
	device = globals.cdrom_device;
	kprintf("CD-ROM device: %s\n", device);
	cdio = cdio_open(device, DRIVER_DEVICE);
	if (!cdio) {
		blub->set_status_text("unable to open CD device");
		kprintf("unable to open CD device\n");
		return false;
	}


	/* Get the track count for the CD. */
	cnt = cdio_get_num_tracks(cdio);

	kprintf("cdio_get_num_tracks: %d\n", cnt);
	kprintf("CDIO_INVALID_TRACK %d\n", CDIO_INVALID_TRACK);

	if (cnt == CDIO_INVALID_TRACK) {
		blub->set_status_text("unable to open CD device");
		kprintf("unable to open CD device\n");
		cdio_destroy(cdio);
		return false;
	}

	kprintf("device %p\n", cdio);

	if (cnt == 0) {
		blub->set_status_text("no audio tracks on CD");
	}
	fprintf(stderr, "CD contains %d track(s)\n", cnt);


	cd_info.ntracks = cnt;
	for (t = 1; t <= cnt; t++) {

		lsn = cdio_get_track_lsn(cdio, t);
		fprintf(stderr, "lsn: %d\n", lsn);
		if (lsn == CDIO_INVALID_LSN) {
			blub->
			set_status_text
			("track has invalid Logical Sector Number");
		}
		/* lsn +=150; */

		trackinfo *blub = new trackinfo();
		blub->track = t;
		blub->start = lsn + SECONDS_TO_FRAMES(2);


		cd_info.tracks.push_back(blub);

		/* track_info(i + 1, entry.cdte_addr.msf.minute, */
		/*            entry.cdte_addr.msf.second, entry.cdte_addr.msf.frame)); */

	}

	lsn = cdio_get_track_lsn(cdio, CDIO_CDROM_LEADOUT_TRACK);

	trackinfo *blub = new trackinfo();
	blub->track = t;
	blub->start = lsn + SECONDS_TO_FRAMES(2);
	cd_info.tracks.push_back(blub);

	cd_info.length = FRAMES_TO_SECONDS(lsn);
	fprintf(stderr, "cd_info.length %d\n", cd_info.length);

	pos = cd_info.tracks[0]->start;
	for (i = 0; i < cd_info.ntracks; i++) {
		fprintf(stderr, "pos: %d\n", pos);
		cd_info.tracks[i]->length = cd_info.tracks[i + 1]->start - pos;
		fprintf(stderr, "length: %ld\n",
			cd_info.tracks[i]->length);
		fprintf(stderr, "3: min %ld sec %ld\n",
			FRAMES_TO_SECONDS(cd_info.tracks[i]->length -
					  SECONDS_TO_FRAMES(2)) / 60,
			FRAMES_TO_SECONDS(cd_info.tracks[i]->length -
					  SECONDS_TO_FRAMES(2)) % 60);
		pos = cd_info.tracks[i + 1]->start;
	}

	fprintf(stderr, "Table of contents successfully read: %08lx\n",
		cd_info.cddb_id);
	fprintf(stderr, "Disc length: %d\n", cd_info.length);

	cdio_destroy(cdio);

	return true;
}

bool
cddb_fill::reading_proxy_env_failed()
{
	const char *f = "Reading http_proxy environment variable failed!";

	blub->set_status_text(i18n(f).toUtf8());
	return false;
}

bool
cddb_fill::do_proxy_stuff(cddb_conn_t *conn)
{
	char *proxy_server = NULL;
	int proxy_port = 0;
	char *tmp = NULL;
	char *s, *ss;


	if (!globals.use_proxy)
		return true;

	if (globals.password && globals.username) {
		cddb_set_http_proxy_credentials(conn, globals.username,
						globals.password);
	}
	if (!globals.proxy_from_env) {
		printf("not using http_env %s %d\n", globals.proxy_server,
		       globals.proxy_port);
		cddb_http_proxy_enable(conn);
		cddb_set_http_proxy_server_name(conn, globals.proxy_server);
		cddb_set_http_proxy_server_port(conn, globals.proxy_port);
		return true;
	}
	/* saving the proxy configuration to temporary variables */
	/* reading from environment */
	if (getenv("http_proxy"))
		tmp = strdup(getenv("http_proxy"));
	if (!tmp)
		return reading_proxy_env_failed();
	if (strncmp(tmp, "http://", 7))
		return reading_proxy_env_failed();
	/* finding proxy server and port */
	s = strchr(tmp + 7, 58);
	if (!s)
		return reading_proxy_env_failed();

	*s = 0;
	ss = strchr(s + 1, 47);
	if (!ss)
		return reading_proxy_env_failed();

	*ss = 0;
	/* now globals has the environment proxy information */
	proxy_server = strdup(tmp + 7);
	proxy_port = atoi(s + 1);

	cddb_http_proxy_enable(conn);
	cddb_set_http_proxy_server_name(conn, proxy_server);
	cddb_set_http_proxy_server_port(conn, proxy_port);
	free(proxy_server);

	return true;
}

bool
cddb_fill::set_connection_params(cddb_conn_t *conn)
{
	char *logname = NULL;
	char *hostname = NULL;

	if (!conn)
		return false;

	logname = getenv("LOGNAME");
	hostname = getenv("HOSTNAME");
	if (!logname)
		logname = strdup("Kover_User");
	if (!hostname)
		hostname = strdup("Kover_Host");
	char email[64];
	snprintf(email, 63, "%s@%s", logname, hostname);

	cddb_set_client(conn, PACKAGE, VERSION);

	cddb_set_email_address(conn, email);

	cddb_set_server_name(conn, globals.cddb_server);
	if (globals.use_cache)
		cddb_cache_enable(conn);
	else
		cddb_cache_disable(conn);

	cddb_cache_set_dir(conn, globals.cddb_path);

	if (!globals.use_cddbp) {
		cddb_http_enable(conn);         /* REQ */
		cddb_set_server_port(conn, 80); /* REQ */
		return do_proxy_stuff(conn);
	}
	return true;
}

bool
cddb_fill::check_for_auth(cddb_conn_t *conn)
{

	int blubber = -2;

	if (!conn)
		return false;

	blub->set_status_text(cddb_error_str(cddb_errno(conn)));
	pa *pad =
		new pa(globals.proxy_server, globals.proxy_port);
	blubber = pad->exec();
	if (blubber) {
		blub->set_status_text(i18n("Operation aborted.").toUtf8());
		/* canceled */
		return false;
	}
	if (globals.username)
		free(globals.username);
	if (globals.password)
		free(globals.password);
	globals.username = pad->get_username();
	globals.password = pad->get_password();
	if (!set_connection_params(conn))
		return false;
	delete pad;

	return true;
}

/* Sends query to server -- this is the first thing to be done */
bool
cddb_fill::cddb_query()
{
	int i;

	list < cddb_211_item * >inexact_list;
	inexact_dialog *inexact;
	cddb_211_item *ref_211;
	int aber = -2;
	cddb_disc_t *disc = NULL;
	cddb_track_t *track = NULL;
	cddb_conn_t *conn = NULL;
	int matches;
	string inexact_string = "";

	blub->set_status_text(i18n("Querying database...").toUtf8());

	disc = cddb_disc_new();
	if (disc == NULL) {
		fprintf(stderr, "out of memory, unable to create disc");
		exit(-1);
	}

	conn = cddb_new();
	if (conn == NULL) {
		fprintf(stderr,
			"out of memory, "
			"unable to create connection structure");
	}

	if (!set_connection_params(conn))
		return false;

	cddb_disc_set_length(disc, cd_info.length);
	for (i = 0; i < cd_info.ntracks; i++) {
		track = cddb_track_new();
		if (track == NULL) {
			fprintf(stderr,
				"out of memory, unable to create track");
			exit(-1);
		}
		cddb_track_set_frame_offset(track, cd_info.tracks[i]->start);
		cddb_disc_add_track(disc, track);
	}

	while (aber) {

		matches = ::cddb_query(conn, disc);
		if (cddb_errno(conn) == CDDB_ERR_PROXY_AUTH) {
			if (check_for_auth(conn))
				continue;
			else
				return false;
		}
		if (matches == -1) {
			blub->set_status_text(cddb_error_str(cddb_errno(conn)));
			return false;
		} else
			aber = 0;

	}

	i = matches;

	if (i == 0) {
		blub->set_status_text(i18n("No match found.").toUtf8());
		return false;
	}
	if (i == 1) {
		matches = 0;
		ref_211 = new cddb_211_item();
		ref_211->set_category(string(cddb_disc_get_category_str(disc)));
		ref_211->set_id(cddb_disc_get_discid(disc));
		inexact_list.push_back(ref_211);
	}

	while (matches > 0) {
		ref_211 = new cddb_211_item();
		ref_211->set_title(string(cddb_disc_get_title(disc)));
		ref_211->set_artist(string(cddb_disc_get_artist(disc)));
		ref_211->set_category(string(cddb_disc_get_category_str(disc)));
		ref_211->set_id(cddb_disc_get_discid(disc));
		inexact_list.push_back(ref_211);

		matches--;
		if (matches > 0) {
			if (!cddb_query_next(conn, disc)) {
				fprintf(stderr, "query index out of bounds");
				exit(-1);
			}
		}
	}

	cddb_destroy(conn);

	if (inexact_list.empty()) {
		blub->set_status_text(i18n("No match found.").toUtf8());
		return false;
	}
	inexact = new inexact_dialog(inexact_list);
	if (inexact_list.size() > 1) {
		/* dialog to choose one of the matches */
		aber = inexact->exec();
	}

	if (aber == -1) {
		blub->set_status_text(i18n("Query aborted...").toUtf8());
		return false;
	}
	ref_211 = inexact->get_object(aber);

	fprintf(stderr, "Using disc: %08lX:%s:%s:%s\n",
		ref_211->get_id(), ref_211->get_artist().c_str(),
		ref_211->get_title().c_str(),
		ref_211->get_category().c_str());

	return cddb_read(ref_211->get_id(), ref_211->get_category());
}

bool
cddb_fill::cddb_read(unsigned long disc_id, string category)
{

	cddb_disc_t *disc = NULL;
	cddb_track_t *track = NULL;
	cddb_conn_t *conn = NULL;
	int i = -2;

	conn = cddb_new();
	if (conn == NULL) {
		fprintf(stderr,
			"out of memory, "
			"unable to create connection structure");
	}

	if (!set_connection_params(conn))
		return false;
	disc = cddb_disc_new();
	if (disc == NULL) {
		fprintf(stderr, "out of memory, unable to create disc");
		exit(-1);
	}

	cddb_disc_set_category_str(disc, category.c_str());
	cddb_disc_set_discid(disc, disc_id);

	while (i) {

		int success = ::cddb_read(conn, disc);
		if (cddb_errno(conn) == CDDB_ERR_PROXY_AUTH) {
			if (check_for_auth(conn))
				continue;
			else
				return false;
		}
		if (!success) {
			blub->set_status_text(cddb_error_str(cddb_errno(conn)));
			return false;
		} else
			i = 0;

	}

	cd_info.length = cddb_disc_get_length(disc);

	if (cd_info.length == 0) {

		blub->
		set_status_text(i18n
				(
					"Disc length == 0; this can't be right. Aborting.")
				.toUtf8());
		return false;
	}
	cd_info.artist = cddb_disc_get_artist(disc);
	cd_info.category = cddb_disc_get_category_str(disc);
	cd_info.cddb_id = cddb_disc_get_discid(disc);
	cd_info.cdname = cddb_disc_get_title(disc);

	i = 0;
	cd_info.ntracks = cddb_disc_get_track_count(disc);
	while ((signed int)cd_info.tracks.size() < cd_info.ntracks)
		cd_info.tracks.push_back(new trackinfo());


	track = cddb_disc_get_track_first(disc);
	i = 0;
	while (track) {
		cd_info.tracks[i++]->name = cddb_track_get_title(track);

		track = cddb_disc_get_track_next(disc);
	}
	cddb_destroy(conn);

	return true;
}

bool
cddb_fill::sites(list < server * >&server_list)
{

	cddb_conn_t *conn = NULL;

	conn = cddb_new();
	if (conn == NULL) {
		fprintf(stderr,
			"out of memory, "
			"unable to create connection structure");
		return false;
	}

	if (!set_connection_params(conn))
		return false;
	cddb_sites(conn);

	const cddb_site_t *site = cddb_first_site(conn);
	while (site) {
		const char *address;
		unsigned int port;
		cddb_site_get_address(site, &address, &port);
		cddb_protocol_t proto = cddb_site_get_protocol(site);
		string protocol = "";
		switch (proto) {
		case PROTO_CDDBP:
			protocol = "cddbp";
			break;
		case PROTO_HTTP:
			protocol = "http";
			break;
		default:
			protocol = "unknown";
		}
		const char *path;
		cddb_site_get_query_path(site, &path);
		float latitude;
		float longitude;
		cddb_site_get_location(site, &latitude, &longitude);
		char blubber[64];
		snprintf(blubber, 63, "%f", latitude);
		string lat = blubber;
		snprintf(blubber, 63, "%f", longitude);
		string lon = blubber;
		const char *desc;
		cddb_site_get_description(site, &desc);
		cddb_site_print(site);
		server_list.
		push_back(new
			  server(address, protocol, port, path, lat, lon,
				 desc));
		site = cddb_next_site(conn);
	}
	cddb_destroy(conn);

	return false;
}
