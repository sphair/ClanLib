pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = $(subst .pc,-$(LT_RELEASE).pc,@ClanLib_pkgconfig@)
CLEANFILES = $(pkgconfig_DATA)

%-$(LT_RELEASE).pc : %.pc
	@cp $< $@

# EOF #
