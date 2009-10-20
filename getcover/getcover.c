/*
 * getcover - getcover is command-line tool to download covers
 * Copyright (C) 2007, 2009 by Qball Cow <Qball@Sarine.nl>
 * Copyright (C) 2007, 2009 by Adrian Reber
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
 * This code is largely based on the gmpc coveramazon plugin and
 * gmpc discogs plugin which are written by Qball Cow <Qball@Sarine.nl>.
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

#define DISCOGS_API_KEY "332020810c"
#define DISCOGS_API_ROOT "http://www.discogs.com/"

#define dprintf(format, ARGS...)	\
	{ if (verbose) \
		d_printf(__PRETTY_FUNCTION__, __LINE__, format, ##ARGS); }

typedef enum {
	META_ALBUM_ART = 1,	/* Album Cover art */
	META_ARTIST_ART = 4	/* Artist art */
} meta_data_type;

typedef struct download {
	char *data;
	int size;
	int max_size;
} download;

typedef struct extra_params {
	int port;
	char *proxy;
	char *dir;
	char *artist;
	char *album;
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

	/* set encoding (discogs want gzip) */
	curl_easy_setopt(curl, CURLOPT_ENCODING , "gzip");
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

static FILE *
file_open(ep *ep)
{
	char *tmp;
	char *mode;
	if (ep->type & META_ALBUM_ART)
		tmp = g_strdup_printf("%s/%s-%s.", ep->dir, ep->artist, ep->album);
	else
		tmp = g_strdup_printf("%s/%s.", ep->dir, ep->artist);
	ep->url = g_strdup_printf("%s%s", tmp, "jpg");
	mode = "wb";
	g_free(tmp);
	dprintf("destination %s\n", ep->url);
	return fopen(ep->url, mode);
}

static int check_result(download *dld)
{
    xmlDocPtr doc;
    if(dld->size < 4 || strncmp(dld->data, "<res",4)) {
        dprintf("invalid XML - good, probably an image\n");
	return 0;
	}

        doc = xmlParseMemory(dld->data,dld->size);
        if(doc)
	{
            xmlNodePtr root = xmlDocGetRootElement(doc);
            if(root)
            {
                /* loop through all albums */
                xmlNodePtr cur = get_first_node_by_name(root,"searchresults");
                if(cur)
                {
                            xmlChar *temp = xmlGetProp(cur, (xmlChar *)"numResults");
                            if(temp)
                                if(xmlStrEqual(temp, (xmlChar *)"0")) {
					dprintf("0 search results\n");
					return -1;
				}
                }
            }
            xmlFreeDoc(doc);
        }
    return 0;
}
static gchar * __query_album_get_uri(download *dld, ep *ep)
{
    char *retv = NULL;
    char *temp_b = g_utf8_casefold(ep->album,-1);
    xmlDocPtr doc;
    /**
     * Get artist name
     */
    if(dld->size < 4 || strncmp(dld->data, "<res",4)) {
        dprintf("Invalid XML\n");
	goto out;
    }
        doc = xmlParseMemory(dld->data,dld->size);
        if(doc)
	{
            xmlNodePtr root = xmlDocGetRootElement(doc);
            if(root)
            {
                /* loop through all albums */
                xmlNodePtr cur = get_first_node_by_name(root,"searchresults");
                if(cur)
                {
                    xmlNodePtr cur2 = get_first_node_by_name(cur,"result");
                    if(cur2) {
                        xmlNodePtr cur4 = get_first_node_by_name(cur2,"title");
                        if(cur4){
                            xmlChar *title = xmlNodeGetContent(cur4);

                            if(title)
                            {   
                                char *temp_a = g_utf8_casefold((gchar *)title,-1);
                                /** Todo make this check fuzzy */
                                if(strstr((char *)temp_a, temp_b))
                                {
                                    xmlNodePtr cur3 = get_first_node_by_name(cur2,"uri");
                                    if(cur3){
                                        xmlChar *xurl = xmlNodeGetContent(cur3);
                                        retv = g_strdup((char *)xurl);
                                        xmlFree(xurl);
                                    }
                                }
                                g_free(temp_a);
                            }
                            if(title)xmlFree(title);
                        }
                    }
                }
            }
            xmlFreeDoc(doc);
        }
out:
    g_free(temp_b);
    return retv;
}
static GList *__query_album_get_uri_list(download *dld, ep *ep)
{ 
    GList *retv = NULL;
    xmlDocPtr doc;
    if(dld->size < 4 || strncmp(dld->data, "<res",4))
    {
        dprintf("Invalid XML\n");
	goto out;
    }

        doc = xmlParseMemory(dld->data,dld->size);
        if(doc)
        {
            xmlNodePtr root = xmlDocGetRootElement(doc);
            if(root)
            {
                /* loop through all albums */
                xmlNodePtr cur = get_first_node_by_name(root,"release");
                if(cur)
                {
                    xmlNodePtr cur2 = get_first_node_by_name(cur,"images");
                    if(cur2) {
                        xmlNodePtr cur3 = get_first_node_by_name(cur2,"image");
                        while(cur3){
                            xmlChar *temp = xmlGetProp(cur3, (xmlChar *)"type");

                            if(temp){
                                if(xmlStrEqual(temp, (xmlChar *)"primary"))
                                {
                                    xmlChar *xurl = xmlGetProp(cur3, (xmlChar *)"uri");
                                    retv = g_list_prepend(retv,g_strdup((char *)xurl));
                                    if(xurl) xmlFree(xurl);
                                } else if(xmlStrEqual(temp, (xmlChar *)"secondary"))
                                {
                                    xmlChar *xurl = xmlGetProp(cur3, (xmlChar *)"uri");
                                    retv = g_list_append(retv,g_strdup((char *)xurl));
                                    if(xurl) xmlFree(xurl);
                                }

                                xmlFree(temp);
                            }
                            cur3 = cur3->next;
                        }
                    }
                }
            }
            xmlFreeDoc(doc);
        }
out:
    return retv;
}

static void __query_get_album_art_uris(download *dld, ep *ep)
{
        GList *list =  __query_album_get_uri_list(dld, ep);
	GList* node = NULL;

	ep->url = g_strdup(g_list_first(list)->data);
	for (node = g_list_first(list); node != NULL; node = g_list_next(node))
	{
		dprintf("found: %s\n", node->data);
		g_free(node->data);
	}
	g_list_free(list);
}
static void __query_get_album_art(download *dld, ep *ep)
{
        gchar *artist_uri = NULL;
        char furl[1024];
        int i=0;
        artist_uri = __query_album_get_uri(dld, ep);
        if(!artist_uri)
		return;
	/* Hack to fix bug in discogs api */
	for(i=strlen(artist_uri); artist_uri[i] != '/' && i > 0; i--);
	snprintf(furl,1024,DISCOGS_API_ROOT"release%s?f=xml&api_key=%s", &artist_uri[i],DISCOGS_API_KEY);
	download_clean(dld);
	easy_download(furl, dld, ep);
	__query_get_album_art_uris(dld, ep);
	download_clean(dld);
	g_free(artist_uri);
}

/** other */
static void discogs_fetch_cover_album_art(ep *ep, download *dld)
{
	char *artist = cover_art_process_string(ep->artist);
	char *album = cover_art_process_string(ep->album);
	char furl[1024];
	snprintf(furl,1024,DISCOGS_API_ROOT"search?type=all&f=xml&q=%s%%20%s&api_key=%s", artist,album,DISCOGS_API_KEY);
	easy_download(furl, dld, ep);
	__query_get_album_art(dld, ep);
	g_free(artist);
	g_free(album);
}
static gchar * __query_artist_get_uri(download *dld)
{
    char *retv = NULL;
    xmlDocPtr doc;
    /**
     * Get artist name
     */
    if(dld->size < 4 || strncmp(dld->data, "<res",4))
    {
        dprintf("Invalid XML\n");
	goto out;
    }
        doc = xmlParseMemory(dld->data,dld->size);
        if(doc)
        {
            xmlNodePtr root = xmlDocGetRootElement(doc);
            if(root)
            {
                /* loop through all albums */
                xmlNodePtr cur = get_first_node_by_name(root,"exactresults");
                if(cur)
                {
                    xmlNodePtr cur2 = get_first_node_by_name(cur,"result");
                    if(cur2) {
                        xmlNodePtr cur3 = get_first_node_by_name(cur2,"uri");
                        if(cur3){
                            xmlChar *xurl = xmlNodeGetContent(cur3);
                            retv = g_strdup((char *)xurl);
                            xmlFree(xurl);
                        }
                    }
                }
            }
            xmlFreeDoc(doc);
        }

out:
    return retv;
}

static GList *__query_artist_get_uri_list(download *dld, ep *ep)
{ 
    GList *retv = NULL;
    xmlDocPtr doc;
    if(dld->size < 4 || strncmp(dld->data, "<res",4))
    {
        dprintf("Invalid XML\n");
	goto out;
    }
        doc = xmlParseMemory(dld->data,dld->size);
        if(doc)
        {
            xmlNodePtr root = xmlDocGetRootElement(doc);
            if(root)
            {
                /* loop through all albums */
                xmlNodePtr cur = get_first_node_by_name(root,"artist");
                if(cur)
                {
                    xmlNodePtr cur2 = get_first_node_by_name(cur,"images");
                    if(cur2) {
                        xmlNodePtr cur3 = get_first_node_by_name(cur2,"image");
                        while(cur3 ){
                            xmlChar *temp = xmlGetProp(cur3, (xmlChar *)"type");
                            if(temp){
                                if(xmlStrEqual(temp, (xmlChar *)"primary"))
                                {
                                    xmlChar *xurl = xmlGetProp(cur3, (xmlChar *)"uri");
				retv = g_list_prepend(retv, g_strdup((char *)xurl));
                                    if(xurl) xmlFree(xurl);
                                } else if(xmlStrEqual(temp, (xmlChar *)"secondary"))
                                {
                                    xmlChar *xurl = xmlGetProp(cur3, (xmlChar *)"uri");
                                    retv = g_list_append(retv,g_strdup((char *)xurl));
                                    if(xurl) xmlFree(xurl);
                                }

                                xmlFree(temp);
                            }
                            cur3 = cur3->next;
                        }
                    }
                }
            }
            xmlFreeDoc(doc);
        }

out:
    return retv;
}

static void __query_get_artist_art_uris(download *dld, ep *ep)
{
        GList *list =  __query_artist_get_uri_list(dld, ep);
	GList* node = NULL;

	ep->url = g_strdup(g_list_first(list)->data);
	for (node = g_list_first(list); node != NULL; node = g_list_next(node))
	{
		dprintf("found: %s\n", node->data);
		g_free(node->data);
	}
	g_list_free(list);
}

static void __query_get_artist_art(download *dld, ep *ep)
{
        gchar *artist_uri = NULL;
        char furl[1024];
	char fix;
        artist_uri = __query_artist_get_uri(dld);
	dprintf("artist_uri: %s\n", artist_uri);
        if(!artist_uri)
		return;
	/* Hack to fix bug in discogs api */
	if(strstr(artist_uri, "?") != NULL)
		fix = '&';
	else
		fix = '?';
	snprintf(furl,1024,"%s%cf=xml&api_key=%s", artist_uri,fix,DISCOGS_API_KEY);

	download_clean(dld);
	easy_download(furl, dld, ep);
	__query_get_artist_art_uris(dld, ep);
	download_clean(dld);
	g_free(artist_uri);
	return;
}


static void
discogs_fetch_artist_art(ep *ep, download *dld)
{
	char *artist = cover_art_process_string(ep->artist);
	char furl[1024];
	snprintf(furl,1024,DISCOGS_API_ROOT"search?type=all&f=xml&q=%s&api_key=%s", artist,DISCOGS_API_KEY);
	easy_download(furl, dld, ep);
	__query_get_artist_art(dld, ep);
	g_free(artist);
	return ;
}

static int
fetch_metadata(ep *ep)
{
	download data = { NULL, 0, -1 };
	FILE *fp = NULL;

	dprintf("artist %s\n", ep->artist);
	dprintf("album %s\n", ep->album);

	if (ep->type & META_ALBUM_ART) {
		dprintf("trying to fetch album art\n");
        	discogs_fetch_cover_album_art(ep, &data);
	} else {
		dprintf("trying to fetch arist art\n");
        	discogs_fetch_artist_art(ep, &data);
	}

	easy_download(ep->url, &data, ep);

	if (check_result(&data) == -1)
		return -1;

	if (data.size) {
		fp = file_open(ep);
		if (fp)
			fwrite(data.data, sizeof(char), data.size, fp);
	}
	download_clean(&data);

	if (fp) {
		fclose(fp);
		return 0;
	}

	return -1;
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
	fprintf(stderr, "                   (required if searching for album cover art)\n");
	fprintf(stderr, "  -c, --cover      download cover (default)\n");
	fprintf(stderr, "  -i, --info       download artist art\n");
	fprintf(stderr, "  -o, --host       specify proxy host\n");
	fprintf(stderr, "  -p, --port       specify proxy port\n");
	fprintf(stderr, "                   if the environment variable http_proxy is set\n");
	fprintf(stderr, "                   that value will be used. to disable the use of a\n");
	fprintf(stderr, "                   proxy the environment variable has to be unset.\n");
	fprintf(stderr, "  -d, --directory  destination directory (defaults to current directory)\n");
	fprintf(stderr, "  -v, --verbose    verbose output\n");
	exit(rc);
}

int
main(int argc, char *argv[])
{
	int next_option;
	ep ep = { -1, NULL, NULL, NULL, NULL, NULL, META_ALBUM_ART };
	int proxy = 0;
	int result;

	const char *short_options = "hcita:l:p:o:d:v";

	const struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"artist", required_argument, NULL, 'a'},
		{"album", no_argument, NULL, 'l'},
		{"cover", no_argument, NULL, 'c'},
		{"info", no_argument, NULL, 'i'},
		{"host", required_argument, NULL, 'o'},
		{"port", required_argument, NULL, 'p'},
		{"directory", required_argument, NULL, 'd'},
		{"verbose", no_argument, NULL, 'v'},
		{0, 0, 0, 0}
	};

	fprintf(stderr, "%s %s, Copyright (C) 2008, 2009 by Adrian Reber <adrian@lisas.de>\n", PACKAGE, VERSION);
	fprintf(stderr, "%s comes with ABSOLUTELY NO WARRANTY - for details read the license.\n\n", PACKAGE);

	if (argc < 3)
		usage(-2);

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
		case 'i':
			ep.type = META_ARTIST_ART;
			dprintf("trying to download artist art\n");
			break;
		case 'c':
			ep.type = META_ALBUM_ART;
			dprintf("trying to download album art\n");
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
	 * we need at least an artist */
	if (!ep.artist)
		usage(-4);

	/* and an album name if cover art has been requested */
	if (ep.type == META_ALBUM_ART && !ep.album) {
		fprintf(stderr, "Artist name and album name required\n");
		usage(-5);
	}

	/* only if a hostname and a port a specfied
	 * for the proxy it makes sense */
	if ((ep.port < 1 || !ep.proxy) && (proxy >= 1))
		usage(-6);

	if (!ep.dir)
		ep.dir = g_strdup(".");
	else if (!g_file_test(ep.dir, G_FILE_TEST_EXISTS))
		g_mkdir(ep.dir, 0755);

	result = fetch_metadata(&ep);
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
