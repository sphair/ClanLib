lib_LTLIBRARIES = libclan30Vorbis.la

libclan30Vorbis_la_SOURCES = \
	setupvorbis.cpp \
	soundprovider_vorbis.cpp \
	soundprovider_vorbis_session.cpp

libclan30Vorbis_la_LIBADD = -logg -lvorbis

libclan30Vorbis_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_Vorbis)

libclan30Sound_la_CXXFLAGS=$(Vorbis_CXXFLAGS) $(extra_CFLAGS_Vorbis)

# EOF #
