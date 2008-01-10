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

#include <string.h>
#include <libxml/parser.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <curl/curl.h>
#include <getopt.h>
#include <config.h>
#include <stdarg.h>

#define AMAZONKEY "14TC04B24356BPHXW1R2"
#define ENDPOINTS 6

#define dprintf(format, ARGS...)	\
	{ if (verbose) \
		d_printf(__PRETTY_FUNCTION__, __LINE__, format, ##ARGS); }

static const char *endpoints[ENDPOINTS][2] = {
	{"com", "United States"},
	{"co.uk", "United Kingdom"},
	{"jp", "Japan"},
	{"fr", "France"},
	{"ca", "Canada"},
	{"de", "Germany"}
};

typedef enum {
	META_ALBUM_ART = 1,	/* Album Cover art      */
	META_ALBUM_TXT = 4	/* Album story          */
} meta_data_type;

static const char *host =
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

typedef struct extra_params {
	int ep;
	int port;
	char *proxy;
	char *dir;
	char *artist;
	char *album;
	char *stype;
	char *url;
	int type;
} ep;

static int verbose = 0;

static void usage(int) __attribute__ ((noreturn));

static void
d_printf(const char *fn, int line, const char *format, ...)
{
	char *tmp;
	va_list arglist;
	va_start(arglist, format);
	tmp = g_strdup_vprintf(format, arglist);
	fprintf(stderr, "  %s():%d: %s", fn, line, tmp);
	g_free(tmp);
	va_end(arglist);
}

static size_t
write_data(void *buffer, size_t size, size_t nmemb, download *dld)
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
download_clean(download *dld)
{
	if (dld->data)
		g_free(dld->data);
	dld->data = NULL;
	dld->size = 0;
}

static int
easy_download(const char *url, download *dld, ep *ep)
{
	int running = 0;
	int msgs_left = 0;
	int success = FALSE;
	CURL *curl = NULL;
	CURLM *curlm = NULL;
	CURLMsg *msg = NULL;
	char *proxy = NULL;
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
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
	/* set redirect */
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1);
	/* set NO SIGNAL */
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, TRUE);

	if (ep->proxy)
		proxy = g_strdup_printf("http://%s:%i/", ep->proxy, ep->port);

	if (getenv("http_proxy") && !proxy)
		proxy = g_strdup(getenv("http_proxy"));

	if (proxy) {
		dprintf("setting proxy: %s\n", proxy);
		curl_easy_setopt(curl, CURLOPT_PROXY, proxy);
		g_free(proxy);
		proxy = NULL;
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
	dprintf("downloaded: %i\n", dld->size);
	if (success)
		return 1;
	if (dld->data)
		g_free(dld->data);
	dld->data = NULL;
	return 0;
}

static int
shrink_string(char *string, int start, int end)
{
	int i;

	for (i = start; i < end; i++)
		string[i] = string[i + 1];

	end--;

	return end;
}

/* Convert string to the wonderful % notation for url*/
static char *
cover_art_process_string(const char *string)
{
#define ACCEPTABLE(a) (((a) >= 'a' && (a) <= 'z') || ((a) >= 'A' && (a) <= 'Z') || ((a) >= '0' && (a) <= '9'))

	const char hex[16] = "0123456789ABCDEF";
	const char *p;
	char *q;
	char *result;
	int c;
	gint unacceptable = 0;
	const char *tmp_p;
	char *new_string;
	int depth = 0;
	int len;
	int i = 0;

	len = strlen(string);

	new_string = g_malloc(len + 1);

	/*      Get count of chars that will need to be converted to %
	   and remove ([{}]) and everything between */
	for (p = string; *p != '\0'; p++) {
		c = (unsigned char) *p;

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
		c = (unsigned char) *p;
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
		c = (unsigned char) *p;

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
amazon_song_info_free(amazon_song_info *asi)
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
get_first_node_by_name(xmlNodePtr xml, char *name)
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
	amazon_song_info *asi = NULL;
	xmlDocPtr doc = xmlParseMemory(data, size);
	if (!doc)
		goto no_doc;

	xmlNodePtr root = xmlDocGetRootElement(doc);
	xmlNodePtr cur = get_first_node_by_name(root, "Items");
	if (!cur)
		goto no_items;

	cur = get_first_node_by_name(cur, "Item");
	if (!cur)
		goto no_items;

	xmlNodePtr child = NULL;
	asi = amazon_song_info_new();

	if ((child = get_first_node_by_name(cur, "LargeImage"))) {
		xmlChar *temp = xmlNodeGetContent(get_first_node_by_name(child, "URL"));
		/* copy it, so we can free it, and don't need xmlFree */
		asi->image_big = g_strdup((char *) temp);
		xmlFree(temp);
	}
	if ((child = get_first_node_by_name(cur, "MediumImage"))) {
		xmlChar *temp = xmlNodeGetContent(get_first_node_by_name(child, "URL"));
		asi->image_medium = g_strdup((char *) temp);
		xmlFree(temp);
	}
	if ((child = get_first_node_by_name(cur, "SmallImage"))) {
		xmlChar *temp = xmlNodeGetContent(get_first_node_by_name(child, "URL"));
		asi->image_small = g_strdup((char *) temp);
		xmlFree(temp);
	}

	if ((child = get_first_node_by_name(cur, "EditorialReviews"))) {
		child = get_first_node_by_name(child, "EditorialReview");
		if (child) {
			/* ugy, lazy */
			xmlChar *temp = xmlNodeGetContent(get_first_node_by_name(child, "Content"));
			asi->album_info = g_strdup((char *) temp);
			xmlFree(temp);
		}
	}

no_items:
	xmlFreeDoc(doc);
no_doc:
	xmlCleanupParser();
	return asi;
}

static FILE *
file_open(ep *ep)
{
	char *tmp;
	char *mode;
	tmp = g_strdup_printf("%s/%s-%s.", ep->dir, ep->artist, ep->album);
	if (ep->type & META_ALBUM_TXT) {
		ep->url = g_strdup_printf("%s%s", tmp, "albuminfo");
		mode = "w";
	} else {
		ep->url = g_strdup_printf("%s%s", tmp, "jpg");
		mode = "wb";
	}
	g_free(tmp);
	dprintf("destination %s\n", ep->url);
	return fopen(ep->url, mode);
}

static int
fetch_metadata_amazon(ep *ep)
{
	download data = { NULL, 0, -1 };
	char furl[1024];
	const char *endp = endpoints[ep->ep][0];
	char *artist;
	char *album;
	FILE *fp = NULL;

	dprintf("search-type: %s\n", ep->stype);
	dprintf("using endpoint %s (%s)\n", endp, endpoints[ep->ep][1]);
	dprintf("artist %s\n", ep->artist);
	dprintf("album %s\n", ep->album);
	artist = cover_art_process_string(ep->artist);
	album = cover_art_process_string(ep->album);
	snprintf(furl, 1024, host, endp, AMAZONKEY, artist, ep->stype, album);
	if (easy_download(furl, &data, ep)) {
		amazon_song_info *asi = cover_art_xml_get_image(data.data, data.size);
		download_clean(&data);
		if (asi) {
			if (ep->type & META_ALBUM_ART) {
				dprintf("trying to fetch album art\n");
				easy_download(asi->image_big, &data, ep);
				if (data.size <= 900) {
					download_clean(&data);
					easy_download(asi->image_medium, &data, ep);
					if (data.size <= 900) {
						download_clean(&data);
						easy_download(asi->image_small, &data, ep);
						if (data.size <= 900)
							download_clean(&data);
					}
				}
				if (data.size) {
					fp = file_open(ep);
					if (fp)
						fwrite(data.data, sizeof(char), data.size, fp);
				}
				download_clean(&data);

			} else if (ep->type & META_ALBUM_TXT) {
				dprintf("trying to fetch album txt\n");
				if (asi->album_info) {
					fp = file_open(ep);
					if (fp) {
						int j = 0, depth = 0;;
						/* Quick 'n Dirty html stripper */
						for (j = 0; j < strlen(asi->album_info); j++) {
							if ((asi->album_info)[j] == '<')
								depth++;
							else if ((asi->album_info)[j] == '>' && depth)
								depth--;
							else if (depth == 0)
								fputc((asi->album_info)[j], fp);
						}
					}
				}
			}
			amazon_song_info_free(asi);
		}
	}

	g_free(artist);
	g_free(album);

	if (fp) {
		fclose(fp);
		return 0;
	}

	return -1;
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
	fprintf(stderr, "  -h, --help       print out this information\n");
	fprintf(stderr, "  -a, --artist     specify the artist whose cover should be retrieved\n");
	fprintf(stderr, "                   (required)\n");
	fprintf(stderr, "  -l, --album      specify the album\n");
	fprintf(stderr, "                   (required)\n");
	fprintf(stderr, "  -t, --title      search type: Title (default)\n");
	fprintf(stderr, "  -k, --keyword    search type: Keyword\n");
	fprintf(stderr, "  -c, --cover      download cover (default)\n");
	fprintf(stderr, "  -i, --info       download artist information\n");
	fprintf(stderr, "  -e, --endpoint   select endpoint (specify \"list\" to see all options)\n");
	fprintf(stderr, "  -o, --host       specify proxy host\n");
	fprintf(stderr, "  -p, --port       specify proxy port\n");
	fprintf(stderr, "                   if the environment variable http_proxy is set\n");
	fprintf(stderr, "                   that value will be used. to disable the use of\n");
	fprintf(stderr, "                   proxy in that case the environment variable has to be unset.\n");
	fprintf(stderr, "  -d, --directory  destination directory (defaults to current directory)\n");
	fprintf(stderr, "  -v, --verbose    verbose output\n");
	exit(rc);
}

int
main(int argc, char *argv[])
{
	int next_option;
	char *searchtype[] = { "Title", "Keyword" };
	ep ep = { 0, -1, NULL, NULL, NULL, NULL, NULL, NULL, META_ALBUM_ART };
	int proxy = 0;
	int result;

	const char *short_options = "hcitka:l:e:p:o:d:v";

	const struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"artist", required_argument, NULL, 'a'},
		{"album", required_argument, NULL, 'l'},
		{"title", no_argument, NULL, 't'},
		{"keyword", no_argument, NULL, 'k'},
		{"cover", no_argument, NULL, 'c'},
		{"info", no_argument, NULL, 'i'},
		{"endpoint", required_argument, NULL, 'e'},
		{"host", required_argument, NULL, 'o'},
		{"port", required_argument, NULL, 'p'},
		{"directory", required_argument, NULL, 'd'},
		{"verbose", no_argument, NULL, 'v'},
		{0, 0, 0, 0}
	};

	fprintf(stderr, "%s %s, Copyright (C) 2008 by Adrian Reber <adrian@lisas.de>\n", PACKAGE, VERSION);
	fprintf(stderr, "%s comes with ABSOLUTELY NO WARRANTY - for details read the license.\n\n", PACKAGE);

	if (argc < 3)
		usage(-2);

	/* default is "Title" search */
	ep.stype = searchtype[0];

	while (1) {
		next_option = getopt_long(argc, argv, short_options, long_options, NULL);
		if (next_option == -1)
			break;
		switch (next_option) {
		case 'a':
			ep.artist = g_strdup(optarg);
			break;
		case 'l':
			ep.album = g_strdup(optarg);
			break;
		case 't':
			ep.stype = searchtype[0];
			break;
		case 'k':
			ep.stype = searchtype[1];
			break;
		case 'i':
			ep.type = META_ALBUM_TXT;
			dprintf("trying to download album text\n");
			break;
		case 'c':
			ep.type = META_ALBUM_ART;
			dprintf("trying to download album art\n");
			break;
		case 'e':
			if (!strncmp("list", optarg, 4))
				dump_endpoints();
			ep.ep = atoi(optarg);
			if (ep.ep > ENDPOINTS || ep.ep < 0)
				dump_endpoints();
			break;
		case 'p':
			proxy++;
			ep.port = atoi(optarg);
			break;
		case 'o':
			proxy++;
			ep.proxy = g_strdup(optarg);
			break;
		case 'd':
			ep.dir = g_strdup(optarg);
			break;
		case 'v':
			verbose++;
			break;
		case 'h':
			usage(0);
		default:
			usage(-3);
		}
	}

	/* let's do a few sanity checks to see if all parameters are used
	 * like they are supposed to
	 *
	 * we need at least an artist and an album name */
	if (!ep.artist || !ep.album)
		usage(-4);

	/* only if a hostname and a port a specfied
	 * for the proxy it makes sense */
	if ((ep.port < 1 || !ep.proxy) && (proxy >= 1))
		usage(-5);

	if (!ep.dir)
		ep.dir = g_strdup(".");
	else if (!g_file_test(ep.dir, G_FILE_TEST_EXISTS))
		g_mkdir(ep.dir, 0755);

	result = fetch_metadata_amazon(&ep);
	if (!result)
		printf("\nDownloaded %s\n", ep.url);
	else
		printf("\nDownload failed\n");
	/* freeing the memory so close to the end of the application
	 * makes not much sense; but hey... it should be done */
	dprintf("freeing memory\n");
	if (ep.url)
		g_free(ep.url);
	if (ep.proxy)
		g_free(ep.proxy);
	if (ep.dir)
		g_free(ep.dir);
	if (ep.artist)
		g_free(ep.artist);
	if (ep.album)
		g_free(ep.album);
	return result;
}
