lib_LTLIBRARIES = libclanApp.la

if WIN32
libclanApp_la_SOURCES = Win32/clanapp.cpp
else
libclanApp_la_SOURCES = Unix/clanapp.cpp
endif

libclanApp_la_LDFLAGS = \
  -release $(LT_RELEASE) \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

# EOF #
