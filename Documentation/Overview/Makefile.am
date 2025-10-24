pkgdatadir  = $(datadir)/doc/@PACKAGE@/Overview

XML_FILES  = $(wildcard $(srcdir)/*.xml)
HTML_FILES = $(patsubst %.xml,%.html, $(notdir $(XML_FILES)))

EXTRA_DIST   = $(XML_FILES)
pkgdata_DATA = $(HTML_FILES)

CLEANFILES  = $(HTML_FILES)

%.html : %.xml
	../Utilities/webbuilder.pl $(srcdir)/../documentation.theme $<

install-data-hook: $(HTML_FILES)
	$(INSTALL) -d $(DESTDIR)$(pkgdatadir)/Images
	$(INSTALL) -m 0644 *.html $(DESTDIR)$(pkgdatadir)
	$(INSTALL) -m 0644 $(srcdir)/Images/*.png $(DESTDIR)$(pkgdatadir)/Images

# EOF #
