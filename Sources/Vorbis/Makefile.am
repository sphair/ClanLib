lib_LTLIBRARIES = libclan23Vorbis.la

libclan23Vorbis_la_SOURCES = \
	setupvorbis.cpp \
	soundprovider_vorbis.cpp \
	soundprovider_vorbis_session.cpp

libclan23Vorbis_la_LIBADD = -logg -lvorbis

libclan23Vorbis_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_Vorbis)

libclan23Sound_la_CXXFLAGS=$(Vorbis_CXXFLAGS) $(extra_CFLAGS_Vorbis)

# EOF #
