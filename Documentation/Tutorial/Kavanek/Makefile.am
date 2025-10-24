HTML_FILES = @Kavanek_Tutorial_HTML@

%.html : $(srcdir)/%.xml $(srcdir)/default.xsl
	echo $@
	../../Utilities/xslt.sh  $<  $@  $(srcdir)/default.xsl
# xslt.sh "xml file" "output file" "xslt stylesheet"

all: $(HTML_FILES)
	@echo "Done"

clean:
	rm -f *.html

# EOF #
