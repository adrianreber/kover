
#KDE_CXXFLAGS = -ansi

bin_PROGRAMS = kover cddb-id 

kover_SOURCES = imagedlg.cc cddb_fill.cc koverfile.cc \
		kovertop.cc cdview.cc main.cc \
		PreferencesDialog.cc cdrom.cc config.cc \
		cddb_211_item.cc inexact_dialog.cc proxy_auth.cc \
		without_cd.cc categories.cc direntry.cc sites.cc \
		net.cc cddb.cc server.cc server_dialog.cc filemode.cc \
		directory.cc

#kover_LDADD   = $(LIB_KSYCOCA) $(LIB_KFILE) $(LIBSOCKET) -lkdeprint
kover_LDADD   = $(LIBSOCKET) -lkdeprint ./kostore/libkostore.la

# the library search path. 
kover_LDFLAGS = $(all_libraries) $(KDE_RPATH)

cddb_id_SOURCES = cdrom.cc cddb-id.cc cdrom_cddb.cc

SUBDIRS = kostore pics .

rcdir = $(kde_datadir)/kover
rc_DATA = koverui.rc

appsdir = $(kde_appsdir)/Multimedia
apps_DATA = kover.desktop

mimedir = $(kde_mimedir)/application
mime_DATA = x-kover.desktop

KDE_ICON = AUTO

EXTRA_DIST = kover.desktop koverui.rc lo16-app-kover.png lo32-app-kover.png hi32-app-kover.png hi48-app-kover.png x-kover.desktop

noinst_HEADERS = kovertop.h cddb_fill.h kover.h \
		 cdview.h PreferencesDialog.h imagedlg.h koverfile.h \
		 kover_global.h cdrom.h config.h cdrom_cddb.h \
		 cddb_211_item.h inexact_dialog.h proxy_auth.h \
		 categories.h without_cd.h direntry.h sites.h net.h \
		 cddb.h server.h server_dialog.h filemode.h directory.h

INCLUDES= $(all_includes)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO 

# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(kover_METASOURCES)

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	$(XGETTEXT) -C -ki18n -ktr \
	-x ../../src/kde-i18n-de/messages/kdelibs.po \
	$(kover_SOURCES) && mv messages.po ../po/kover.pot

