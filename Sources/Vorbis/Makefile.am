lib_LTLIBRARIES = libclanVorbis.la

libclanVorbis_la_SOURCES = \
	setupvorbis.cpp \
	soundprovider_vorbis.cpp \
	soundprovider_vorbis_session.cpp

libclanVorbis_la_LIBADD = -logg -lvorbis

libclanVorbis_la_LDFLAGS = \
  -release $(LT_RELEASE) \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

# EOF #
