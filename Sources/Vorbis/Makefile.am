lib_LTLIBRARIES = libclan22Vorbis.la

libclan22Vorbis_la_SOURCES = \
	setupvorbis.cpp \
	soundprovider_vorbis.cpp \
	soundprovider_vorbis_session.cpp

libclan22Vorbis_la_LIBADD = -logg -lvorbis

libclan22Vorbis_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_Vorbis)

libclan22Sound_la_CXXFLAGS=$(Vorbis_CXXFLAGS) $(extra_CFLAGS_Vorbis)

# EOF #
