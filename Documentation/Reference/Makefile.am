EXTRA_DIST = $(wildcart images/*.png)

HTML_PREFIX = $(datadir)/doc/@PACKAGE@-@LT_RELEASE@

all-local:

html:
	if [ -d doxyoutput ]; then rm -rf doxyoutput; fi
	if [ -d reftest ]; then rm -rf reftest; fi
	mkdir doxyoutput;
	mkdir reftest
	doxygen ./clanlib.doxygen
	make -C ../Utilities/ReferenceDocs
	../Utilities/ReferenceDocs/ReferenceDocs

install-html:
	$(INSTALL) -d $(DESTDIR)$(HTML_PREFIX)
	$(INSTALL) -d $(DESTDIR)$(HTML_PREFIX)/Reference
	$(INSTALL) -d $(DESTDIR)$(HTML_PREFIX)/Reference/images
	$(INSTALL) -d $(DESTDIR)$(HTML_PREFIX)/Reference/html
	$(INSTALL) -m 0644 $(srcdir)/images/*.png $(DESTDIR)$(HTML_PREFIX)/Reference/images

	find reftest/ -name "*.html" -exec $(INSTALL) -m 0644 {} $(DESTDIR)$(HTML_PREFIX)/Reference/html \;

clean-local:
	rm -rf doxyoutput
	rm -rf reftest

# EOF #

