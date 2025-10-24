EXTRA_DIST = $(wildcart images/*.png) \
 pce2.txt pce2.in \
 class.plg  cross.plg  desc.plg	init.plg  input.plg  namespace.plg  output.plg

HTML_PREFIX = $(datadir)/doc/@PACKAGE@

HEADERS_API = $(shell find @top_srcdir@/Sources/API -name '*.h')
HEADERS_ALL = $(shell find @top_srcdir@/Sources -name '*.h')

all-local: html/

PLGS = class.plg cross.plg desc.plg init.plg input.plg namespace.plg output.plg


dev_ref: $(PLGS) $(HEADERS_ALL)
	if [ ! -d html ]; then mkdir html; fi
	chmod +x ./pce2
	./pce2 --plgdir $(srcdir) $(HEADERS_ALL)

html/: $(PLGS) $(HEADERS_API)
	if [ ! -d html ]; then mkdir html; fi
	chmod +x ./pce2
	./pce2 --plgdir $(srcdir) $(HEADERS_API)

$(PLGS):
	$(LN_S) $(srcdir)/$@ $@

install-data-hook: html/
	$(INSTALL) -d $(DESTDIR)$(HTML_PREFIX)
	$(INSTALL) -d $(DESTDIR)$(HTML_PREFIX)/Reference
	$(INSTALL) -d $(DESTDIR)$(HTML_PREFIX)/Reference/images
	$(INSTALL) -d $(DESTDIR)$(HTML_PREFIX)/Reference/html
	$(INSTALL) -m 0644 $(srcdir)/images/*.png $(DESTDIR)$(HTML_PREFIX)/Reference/images
	find html/ -name "*.html" -exec $(INSTALL) -m 0644 {} $(DESTDIR)$(HTML_PREFIX)/Reference/html \;

clean-local:
	rm -rf html

# EOF #
