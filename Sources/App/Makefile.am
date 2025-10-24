lib_LTLIBRARIES = libclan21App.la

if WIN32
libclan21App_la_SOURCES = Win32/clanapp.cpp
else
libclan21App_la_SOURCES = Unix/clanapp.cpp
endif

libclan21App_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE)
# EOF #
