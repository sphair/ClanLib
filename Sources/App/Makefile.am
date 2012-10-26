lib_LTLIBRARIES = libclan30App.la

if WIN32
libclan30App_la_SOURCES = Win32/clanapp.cpp
else
libclan30App_la_SOURCES = Unix/clanapp.cpp
endif

libclan30App_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanApp)

libclan30App_la_CXXFLAGS=$(clanApp_CXXFLAGS) $(extra_CFLAGS_clanApp)
# EOF #
