/*
 * getcover - getcover is command-line tool to download covers
 * Copyright (C) 2007 by Qball Cow <Qball@Sarine.nl>
 * Copyright (C) 2007, 2008 by Adrian Reber
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
 * This code is largely based on the gmpc coveramazon plugin which
 * is written by Qball Cow <Qball@Sarine.nl>.
 *
 * There are only minimal modifications from me (Adrian) to make it
 * run as standalone command-line tool.
 */

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <curl/curl.h>
#include <getopt.h>

#define AMAZONKEY "14TC04B24356BPHXW1R2"
#define CURL_TIMEOUT 10
#define ENDPOINTS 6

static char *endpoints[ENDPOINTS][2] = {
	{"com", "United States"},
	{"co.uk", "United Kingdom"},
	{"jp", "Japan"},
	{"fr", "France"},
	{"ca", "Canada"},
	{"de", "Germany"}
};

typedef enum {
	META_DATA_AVAILABLE,
	META_DATA_UNAVAILABLE,
	META_DATA_FETCHING
} MetaDataResult;

typedef enum {
	META_ALBUM_ART = 1,	/* Album Cover art      */
	META_ALBUM_TXT = 4	/* Album story          */
} MetaDataType;

static char *host =
    "http://ecs.amazonaws.%s/onca/xml?Service=" "AWSECommerceService&Operation=ItemSearch&SearchIndex="
    "Music&ResponseGroup=Images,EditorialReview&" "SubscriptionId=%s&Artist=%s&%s=%s";

typedef struct amazon_song_info {
	char *image_big;
	char *image_medium;
	char *image_small;
	char *album_info;
} amazon_song_info;

typedef struct download {
	char *data;
	int size;
	int max_size;
} download;

static void usage(int) __attribute__ ((noreturn));

static size_t
write_data(void *buffer, size_t size, size_t nmemb, download * dld)
{
	if (!size || !nmemb)
		return 0;
	if (dld->data == NULL)
		dld->size = 0;

	dld->data = g_realloc(dld->data, (gulong) (size * nmemb + dld->size) + 1);

	memset(&(dld->data)[dld->size], '\0', (size * nmemb) + 1);
	memcpy(&(dld->data)[dld->size], buffer, size * nmemb);

	dld->size += size * nmemb;
	if (dld->size >= dld->max_size && dld->max_size > 0)
		return 0;

	return size * nmemb;
}

static void
download_clean(download * dld)
{
	if (dld->data)
		g_free(dld->data);
	dld->data = NULL;
	dld->size = 0;
}

static int
easy_download(const char *url, download * dld)
{
	int timeout = 0;
	int running = 0;
	int msgs_left = 0;
	int success = FALSE;
	CURL *curl = NULL;
	CURLM *curlm = NULL;
	CURLMsg *msg = NULL;
	/*int res; */
	if (!dld)
		return 0;
	if (url == NULL)
		return 0;
	if (url[0] == '\0')
		return 0;
	/**
	 * Make sure it's clean
	 */
	download_clean(dld);
	/* initialize curl */
	curl = curl_easy_init();
	if (!curl)
		return 0;
	curlm = curl_multi_init();
	if (!curlm)
		return 0;

	/* set uri */
	curl_easy_setopt(curl, CURLOPT_URL, url);
	/* set callback data */
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, dld);
	/* set callback function */
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	/* set timeout */
	//timeout = cfg_get_single_value_as_int_with_default(config, "Network Settings", "Connection Timeout", CURL_TIMEOUT);
	timeout = CURL_TIMEOUT;
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);
	/* set redirect */
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1);
	/* set NO SIGNAL */
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, TRUE);

	//if(cfg_get_single_value_as_int_with_default(config, "Network Settings", "Use Proxy", FALSE))
	if (FALSE) {
		//char *value = cfg_get_single_value_as_string(config, "Network Settings", "Proxy Address");
		char *value = g_strdup("localhost");
		//gint port =  cfg_get_single_value_as_int_with_default(config, "Network Settings", "Proxy Port",8080);
		gint port = 8080;
		if (value) {
			gchar *ppath = g_strdup_printf("http://%s:%i", value, port);
			printf("Setting proxy: %s:%i\n", value, port);
			/* hack to make stuff work */
			curl_easy_setopt(curl, CURLOPT_PROXY, ppath);
			/*                      curl_easy_setopt(curl, CURLOPT_PROXY, value);
			   curl_easy_setopt(curl, CURLOPT_PROXYPORT, port);
			 */
			g_free(ppath);
			ppath = NULL;
			//cfg_free_string(value);
		} else {
			printf("Proxy enabled, but no proxy defined");
		}
	}

	curl_multi_add_handle(curlm, curl);
	do {
		curl_multi_perform(curlm, &running);
		g_usleep(100000);
		while ((msg = curl_multi_info_read(curlm, &msgs_left))) {
			if (msg->msg == CURLMSG_DONE) {
				if ((!msg->data.result || msg->data.result == 23)) {
					success = TRUE;
				} else {
					/* don't print the can't resolve.. */
					if (msg->data.result != 108) {
						printf("Error: %i '%s' url: %s",
						       msg->data.result,
						       curl_easy_strerror(msg->data.result), url);
					}
				}
			}
		}
	} while (running);
	/**
	 * remove handler
	 */
	curl_multi_remove_handle(curlm, curl);
	/* cleanup */
	curl_easy_cleanup(curl);
	curl_multi_cleanup(curlm);
	printf("Downloaded: %i\n", dld->size);
	if (success)
		return 1;
	if (dld->data)
		g_free(dld->data);
	dld->data = NULL;
	return 0;
}

static int
shrink_string(gchar * string, int start, int end)
{
	int i;

	for (i = start; i < end; i++)
		string[i] = string[i + 1];

	end--;

	return end;
}

/* Convert string to the wonderful % notation for url*/
static char *
cover_art_process_string(const gchar * string)
{
#define ACCEPTABLE(a) (((a) >= 'a' && (a) <= 'z') || ((a) >= 'A' && (a) <= 'Z') || ((a) >= '0' && (a) <= '9'))

	const gchar hex[16] = "0123456789ABCDEF";
	const gchar *p;
	gchar *q;
	gchar *result;
	int c;
	gint unacceptable = 0;
	const gchar *tmp_p;
	gchar *new_string;
	int depth = 0;
	int len;
	int i = 0;

	len = strlen(string);

	new_string = g_malloc(len + 1);

	/*      Get count of chars that will need to be converted to %
	   and remove ([{}]) and everything between */
	for (p = string; *p != '\0'; p++) {
		c = (guchar) * p;

		if (c == '(' || c == '[' || c == '{') {
			depth++;
		} else if (c == ')' || c == ']' || c == '}') {
			depth--;
			if (depth < 0)
				depth = 0;
		} else if (depth == 0) {
			if (!ACCEPTABLE(c))
				unacceptable++;

			new_string[i] = c;

			i++;
		}
	}

	new_string[i] = '\0';

	len = strlen(new_string);

	/* remove double spaces from the string because removing ([{}])
	   tends to create those */
	for (p = new_string + 1; *p != '\0'; p++) {
		c = (guchar) * p;
		if (c == ' ') {
			tmp_p = p - 1;
			if (*tmp_p == ' ') {
				len = shrink_string(new_string, p - new_string, len);
				p--;
			}
		}
	}

	/* make sure first char isn't a space */
	if (new_string[0] == ' ')
		len = shrink_string(new_string, 0, len);

	/* make sure there isn't a trailing space */
	if (new_string[len - 1] == ' ')
		len--;

	new_string[len] = '\0';

	result = g_malloc(len + unacceptable * 2 + 1);

	/*time to create the escaped string */
	for (q = result, p = new_string; *p != '\0'; p++) {
		c = (guchar) * p;

		if (!ACCEPTABLE(c)) {
			*q++ = '%';	/* means hex coming */
			*q++ = hex[c >> 4];
			*q++ = hex[c & 15];
		} else
			*q++ = *p;
	}

	*q = '\0';

	g_free(new_string);

	return result;
}

static void
init()
{
	char *file = g_strdup("covers");
	if (!g_file_test(file, G_FILE_TEST_EXISTS))
		g_mkdir(file, 0755);

	g_free(file);
}

static amazon_song_info *
amazon_song_info_new()
{
	amazon_song_info *asi = g_malloc(sizeof(amazon_song_info));
	asi->image_big = NULL;
	asi->image_medium = NULL;
	asi->image_small = NULL;
	asi->album_info = NULL;
	return asi;
}
static void
amazon_song_info_free(amazon_song_info * asi)
{
	if (asi->image_big != NULL)
		g_free(asi->image_big);
	if (asi->image_medium != NULL)
		g_free(asi->image_medium);
	if (asi->image_small != NULL)
		g_free(asi->image_small);
	if (asi->album_info != NULL)
		g_free(asi->album_info);
	g_free(asi);
	return;
}

static xmlNodePtr
get_first_node_by_name(xmlNodePtr xml, gchar * name)
{
	if (xml) {
		xmlNodePtr c = xml->xmlChildrenNode;
		for (; c; c = c->next) {
			if (xmlStrEqual(c->name, (xmlChar *) name))
				return c;
		}
	}
	return NULL;
}

static amazon_song_info *
cover_art_xml_get_image(char *data, int size)
{
	xmlDocPtr doc = xmlParseMemory(data, size);
	if (doc) {
		xmlNodePtr root = xmlDocGetRootElement(doc);
		xmlNodePtr cur = get_first_node_by_name(root, "Items");
		amazon_song_info *asi = NULL;
		if (cur) {
			cur = get_first_node_by_name(cur, "Item");
			if (cur) {
				xmlNodePtr child = NULL;
				asi = amazon_song_info_new();
				if ((child = get_first_node_by_name(cur, "LargeImage"))) {
					xmlChar *temp =
					    xmlNodeGetContent(get_first_node_by_name(child, "URL"));
					/* copy it, so we can free it, and don't need xmlFree */
					asi->image_big = g_strdup((char *) temp);
					xmlFree(temp);
				}
				if ((child = get_first_node_by_name(cur, "MediumImage"))) {
					xmlChar *temp =
					    xmlNodeGetContent(get_first_node_by_name(child, "URL"));
					asi->image_medium = g_strdup((char *) temp);
					xmlFree(temp);
				}
				if ((child = get_first_node_by_name(cur, "SmallImage"))) {
					xmlChar *temp =
					    xmlNodeGetContent(get_first_node_by_name(child, "URL"));
					asi->image_small = g_strdup((char *) temp);
					xmlFree(temp);
				}

				if ((child = get_first_node_by_name(cur, "EditorialReviews"))) {
					child = get_first_node_by_name(child, "EditorialReview");
					if (child) {
						xmlChar *temp = xmlNodeGetContent(get_first_node_by_name(child, "Content"));	/* ugy, lazy */
						asi->album_info = g_strdup((char *) temp);
						xmlFree(temp);
					}
				}
			}
		}
		xmlFreeDoc(doc);
		return asi;
	}
	xmlCleanupParser();
	return NULL;
}

static int
fetch_metadata_amazon(const char *stype, char *nartist, char *nalbum, int type, char **url, int ep)
{
	download data = { NULL, 0, -1 };
	int found = 0;
	char furl[1024];
	char *endp = endpoints[ep][0];
	gchar *artist;
	gchar *album;

	printf("search-type: %s\n", stype);
	artist = cover_art_process_string(nartist);
	album = cover_art_process_string(nalbum);
	snprintf(furl, 1024, host, endp, AMAZONKEY, artist, stype, album);
	if (easy_download(furl, &data)) {
		amazon_song_info *asi = cover_art_xml_get_image(data.data, data.size);
		download_clean(&data);
		if (asi) {
			if (type & META_ALBUM_ART) {
				printf("Trying to fetch album art");
				easy_download(asi->image_big, &data);
				if (data.size <= 900) {
					download_clean(&data);
					easy_download(asi->image_medium, &data);
					if (data.size <= 900) {
						download_clean(&data);
						easy_download(asi->image_small, &data);
						if (data.size <= 900)
							download_clean(&data);
					}
				}
				if (data.size) {
					FILE *fp = NULL;
					gchar *imgpath = NULL;
					gchar *filename = g_strdup_printf("%s-%s.jpg", nartist, nalbum);
					imgpath = g_strdup_printf("covers/%s", filename);
					g_free(filename);
					fp = fopen(imgpath, "wb");
					if (fp) {
						fwrite(data.data, sizeof(char), data.size, fp);
						*url = g_strdup(imgpath);
						found = 1;
						fclose(fp);
					}
					g_free(imgpath);
				}
				download_clean(&data);


			} else if (type & META_ALBUM_TXT) {
				printf("Trying to fetch album txt");
				if (asi->album_info) {
					FILE *fp;
					gchar *filename, *imgpath;
					filename = g_strdup_printf("%s-%s.albuminfo", nartist, nalbum);
					imgpath = g_strdup_printf("covers/%s", filename);
					g_free(filename);
					fp = fopen(imgpath, "w");
					if (fp) {
						int j = 0, depth = 0;;
						*url = g_strdup(imgpath);
						/**
						 * Quick 'n Dirty html stripper
						 */
						for (j = 0; j < strlen(asi->album_info); j++) {
							if ((asi->album_info)[j] == '<')
								depth++;
							else if ((asi->album_info)[j] == '>' && depth)
								depth--;
							else if (depth == 0)
								fputc((asi->album_info)[j], fp);
						}
						fclose(fp);
						found = 1;
					}

					g_free(imgpath);
				}
			}
			amazon_song_info_free(asi);
		}
	}

	g_free(artist);
	g_free(album);
	return found;
}

static void
dump_endpoints()
{
	int i;
	fprintf(stderr, "\nSelect an endpoint by specifying the corresponding number:\n\n");
	for (i = 0; i < ENDPOINTS; i++) {
		fprintf(stderr, "  %d: %s (%s)\n", i, endpoints[i][1], endpoints[i][0]);
	}
	exit(0);
}

static void
usage(int rc)
{
	fprintf(stderr, "Usage: getcover [options]\n\n");
	fprintf(stderr, "getcover is command-line tool to download covers\n\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  -h, --help      print out this information\n");
	fprintf(stderr, "  -a, --artist    specify the artist whose cover should be retrieved\n");
	fprintf(stderr, "                  (required)\n");
	fprintf(stderr, "  -l, --album     specify the album\n");
	fprintf(stderr, "                  (required)\n");
	fprintf(stderr, "  -t, --title     search type: Title (default)\n");
	fprintf(stderr, "  -k, --keyword   search type: Keyword\n");
	fprintf(stderr, "  -c, --cover     download cover (default)\n");
	fprintf(stderr, "  -i, --info      download artist information\n");
	fprintf(stderr, "  -e, --endpoint  select endpoint (specify \"list\" to see all options)\n");
	exit(rc);
}

int
main(int argc, char *argv[])
{
	gchar *url = NULL;
	int next_option;
	char album[256];
	char artist[256];
	const char *searchtype[] = { "Title", "Keyword" };
	int stype = 0;
	int mtype = META_ALBUM_ART;
	int ep = 0;

	const char *short_options = "hcitka:l:e:";

	struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"artist", required_argument, NULL, 'a'},
		{"album", required_argument, NULL, 'l'},
		{"title", no_argument, NULL, 't'},
		{"keyword", no_argument, NULL, 'k'},
		{"cover", no_argument, NULL, 'c'},
		{"info", no_argument, NULL, 'i'},
		{"endpoint", required_argument, NULL, 'e'},
		{0, 0, 0, 0}
	};

	fprintf(stderr, "getcover 1, Copyright (C) 2008 by Adrian Reber <adrian@lisas.de>\n");
	fprintf(stderr, "getcover comes with ABSOLUTELY NO WARRANTY - for details read the license.\n");

	while (1) {
		next_option = getopt_long(argc, argv, short_options, long_options, NULL);
		if (next_option == -1)
			break;
		switch (next_option) {
		case 'a':
			strncpy(artist, optarg, 256);
			break;
		case 'l':
			strncpy(album, optarg, 256);
			break;
		case 't':
			stype = 0;
			break;
		case 'k':
			stype = 1;
			break;
		case 'i':
			mtype = META_ALBUM_TXT;
			break;
		case 'c':
			mtype = META_ALBUM_ART;
			break;
		case 'e':
			if (!strncmp("list", optarg, 4))
				dump_endpoints();
			ep = atoi(optarg);
			if (ep > ENDPOINTS || ep < 0)
				dump_endpoints();
			break;
		case 'h':
			usage(0);
		default:
			usage(-1);
		}
	}

	init();
	fetch_metadata_amazon(searchtype[stype], artist, album, mtype, &url, ep);
	printf("url %s\n", url);
	return 0;
}
