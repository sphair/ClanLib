lib_LTLIBRARIES = libclan42App.la

if WIN32
libclan42App_la_SOURCES = Win32/clanapp.cpp
else
libclan42App_la_SOURCES = Unix/clanapp.cpp
endif

libclan42App_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanApp)

libclan42App_la_CXXFLAGS=$(clanApp_CXXFLAGS) $(extra_CFLAGS_clanApp)
# EOF #
