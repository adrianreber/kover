####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = kover
kover_SOURCES = imagedlgdata.cpp imagedlg.cpp cddb_fill.cpp koverfile.cpp KoverTop.cpp CDView.cpp main.cpp 
kover_LDADD   = $(LIB_KIMGIO) -lkfile -lkfm -lkdeui -lkdecore -lqt -lXext -lX11 $(LIBSOCKET)

SUBDIRS = docs xpm 

EXTRA_DIST = main.cpp kover.kdelnk kover.xpm mini-kover.xpm CDView.h CDView.cpp KoverTop.cpp KoverTop.h koverfile.cpp koverfile.h cddb_fill.cpp cddb_fill.h image_dlg.kdevdlg imagedlg.h imagedlg.cpp imagedlgdata.cpp 

install-data-local:
	$(mkinstalldirs) $(kde_appsdir)/Utilities/
	$(INSTALL_DATA) kover.kdelnk $(kde_appsdir)/Utilities/kover.kdelnk
	$(mkinstalldirs) $(kde_icondir)/
	$(INSTALL_DATA) kover.xpm $(kde_icondir)/kover.xpm
	$(mkinstalldirs) $(kde_minidir)/
	$(INSTALL_DATA) mini-kover.xpm $(kde_minidir)/mini-kover.xpm

uninstall-local:
	-rm -f $(kde_appsdir)/Utilities/kover.kdelnk
	-rm -f $(kde_icondir)/kover.xpm
	-rm -f $(kde_minidir)/mini-kover.xpm

####### kdevelop will overwrite this part!!! (end)############
# this 10 paths are KDE specific. Use them:
# kde_htmldir       Where your docs should go to. (contains lang subdirs)
# kde_appsdir       Where your application file (.kdelnk) should go to. 
# kde_icondir       Where your icon should go to.
# kde_minidir       Where your mini icon should go to.
# kde_datadir       Where you install application data. (Use a subdir)
# kde_locale        Where translation files should go to.(contains lang subdirs)
# kde_cgidir        Where cgi-bin executables should go to.
# kde_confdir       Where config files should go to.
# kde_mimedir       Where mimetypes should go to.
# kde_toolbardir    Where general toolbar icons should go to.
# kde_wallpaperdir  Where general wallpapers should go to.

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)
# claim, which subdirectories you want to install
# you can add here more. This one gets installed 
bin_PROGRAMS = kover 

kover_METASOURCES = USE_AUTOMOC

# the library search path. 
kover_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(kover_METASOURCES)

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	$(XGETTEXT) -C -ki18n -x ../../../kdelibs/po/kde.pot $(kover_SOURCES) && mv messages.po ../po/kover.pot






