lib_LTLIBRARIES = libclan24App.la

if WIN32
libclan24App_la_SOURCES = Win32/clanapp.cpp
else
libclan24App_la_SOURCES = Unix/clanapp.cpp
endif

libclan24App_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanApp)

libclan24App_la_CXXFLAGS=$(clanApp_CXXFLAGS) $(extra_CFLAGS_clanApp)
# EOF #
