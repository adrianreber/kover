
dist-hook:
	perl admin/am_edit

AUTOMAKE_OPTIONS = foreign 1.4

SUBDIRS = $(TOPSUBDIRS)

$(top_srcdir)/subdirs:
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common subdirs
 
$(top_srcdir)/acinclude.m4: $(top_srcdir)/admin/acinclude.m4.in $(top_srcdir)/admin/libtool.m4.in
	@cd $(top_srcdir) && cat admin/acinclude.m4.in admin/libtool.m4.in > acinclude.m4
 
MAINTAINERCLEANFILES = subdirs configure.in acinclude.m4 SUBDIRS kover.spec.in

EXTRA_DIST = admin ABOUT-NLS kover.spec

man_MANS = kover.1 cddb-id.1

