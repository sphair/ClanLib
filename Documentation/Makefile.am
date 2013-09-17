EXTRA_DIST = $(wildcart images/*.png)

HTML_PREFIX = $(datadir)/doc/@PACKAGE@-@LT_RELEASE@

all-local:

html:
	if [ -d doxyoutput ]; then rm -rf doxyoutput; fi
	mkdir doxyoutput;
	doxygen ./clanlib.doxygen

install-html:
	$(INSTALL) -d $(DESTDIR)$(HTML_PREFIX)
	$(INSTALL) -d $(DESTDIR)$(HTML_PREFIX)/Reference
	$(INSTALL) -d $(DESTDIR)$(HTML_PREFIX)/img
	$(INSTALL) -d $(DESTDIR)$(HTML_PREFIX)/css
	$(INSTALL) -d $(DESTDIR)$(HTML_PREFIX)/Reference/html
	$(INSTALL) -m 0644 $(srcdir)/img/*.png $(DESTDIR)$(HTML_PREFIX)/img
	$(INSTALL) -m 0644 $(srcdir)/css/*.css $(DESTDIR)$(HTML_PREFIX)/css

	find doxyoutput/html/ -name "*.html" -exec $(INSTALL) -m 0644 {} $(DESTDIR)$(HTML_PREFIX)/Reference/html \;
	find doxyoutput/html/ -name "*.png" -exec $(INSTALL) -m 0644 {} $(DESTDIR)$(HTML_PREFIX)/Reference/html \;
	find doxyoutput/html/ -name "*.css" -exec $(INSTALL) -m 0644 {} $(DESTDIR)$(HTML_PREFIX)/Reference/html \;
	find doxyoutput/html/ -name "*.js" -exec $(INSTALL) -m 0644 {} $(DESTDIR)$(HTML_PREFIX)/Reference/html \;

clean-local:
	rm -rf doxyoutput

# EOF #

