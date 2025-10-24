pkgdatadir = $(datadir)/doc/@PACKAGE@

SUBDIRS = Tutorial @ClanLib_add_docs@

EXTRA_DIST = \
Images/clanlib_logo_small.png \
FAQ.xml \
FAQ.xsl \
Overview/2dapi.xml \
Overview/engine_design.xml \
Overview/font_creation_new.xml \
Overview/font_creation_old.xml \
Overview/font_overview.xml \
Overview/getting_started.xml \
Overview/gui_overview.xml \
Overview/gui_theme.xml \
Overview/index.xml \
Overview/loading_graphics.xml \
Overview/network_overview.xml \
Overview/opengl_overview.xml \
Overview/resource_overview.xml \
Overview/signals.xml \
Overview/sprites_overview.xml \
Overview/tipsandtricks.xml \
Overview/warnings.xml \
Overview/Images/font0.png \
Overview/Images/font1.png \
Overview/Images/font2.png \
Overview/Images/font3.png \
Overview/Images/font4.png \
Overview/Images/font5.png \
Overview/Images/font6.png \
Overview/Images/font_old0.png \
index.xml \
documentation.theme \
default.css \
default.xls

pkgdata_DATA = FAQ.html index.html

FAQ.html: $(srcdir)/FAQ.xml $(srcdir)/default.xsl
	echo $@
	Utilities/xslt.sh  $<  $@  $(srcdir)/default.xsl
# xslt.sh "xml file" "output file" "xslt stylesheet"

index.html: $(srcdir)/index.xml $(srcdir)/documentation.theme
	Utilities/webbuilder.pl $(srcdir)/documentation.theme $(srcdir)/index.xml

publish-docs : all
	$(srcdir)/Utilities/transform.sh Reference/html
	$(srcdir)/Utilities/transform.sh Overview/
	mv Reference/html Reference/php

# CLEANFILES = Utilities/webbuilder.pl Reference/php Overview/*.php

# EOF #
