lib_LTLIBRARIES = libclanApp.la

if WIN32
libclanApp_la_SOURCES = Win32/clanapp.cpp
else
libclanApp_la_SOURCES = Unix/clanapp.cpp
endif

libclanApp_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE)

# EOF #
