lib_LTLIBRARIES = libclan31App.la

if WIN32
libclan31App_la_SOURCES = Win32/clanapp.cpp
else
libclan31App_la_SOURCES = Unix/clanapp.cpp
endif

libclan31App_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanApp)

libclan31App_la_CXXFLAGS=$(clanApp_CXXFLAGS) $(extra_CFLAGS_clanApp)
# EOF #
