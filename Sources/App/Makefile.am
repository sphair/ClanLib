lib_LTLIBRARIES = libclan40App.la

if WIN32
libclan40App_la_SOURCES = Win32/clanapp.cpp
else
libclan40App_la_SOURCES = Unix/clanapp.cpp
endif

libclan40App_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanApp)

libclan40App_la_CXXFLAGS=$(clanApp_CXXFLAGS) $(extra_CFLAGS_clanApp)
# EOF #
