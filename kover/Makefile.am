####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = kover cddb-id 

kover_SOURCES = imagedlgdata.cpp imagedlg.cpp cddb_fill.cpp koverfile.cpp \
		KoverTop.cpp CDView.cpp main.cpp \
		PreferencesDialog.cpp CDDB.cpp cdrom.cc config.cc \
		cddb_211_item.cc

kover_LDADD   = $(LIB_KSYCOCA) $(LIB_KFILE) $(LIBSOCKET)

# the library search path. 
kover_LDFLAGS = $(all_libraries) $(KDE_RPATH)

cddb_id_SOURCES = cdrom.cc cddb-id.cc cdrom_cddb.cc

SUBDIRS =  

rcdir = $(kde_datadir)/kover
rc_DATA = koverui.rc

appsdir = $(kde_appsdir)/Multimedia
apps_DATA = kover.desktop

KDE_ICON = AUTO

EXTRA_DIST = kover.desktop koverui.rc lo16-app-kover.png lo32-app-kover.png hi32-app-kover.png hi48-app-kover.png

noinst_HEADERS = CDDB.h KoverTop.h cddb_fill.h kover.h \
		 CDView.h PreferencesDialog.h imagedlg.h koverfile.h \
		 kover_global.h cdrom.h config.h cdrom_cddb.h \
		 cddb_211_item.h

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)
# claim, which subdirectories you want to install
# you can add here more. This one gets installed 

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO 

# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(kover_METASOURCES)

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	$(XGETTEXT) -C -ki18n -x ../../../kdelibs/po/kde.pot $(kover_SOURCES) && mv messages.po ../po/kover.pot






