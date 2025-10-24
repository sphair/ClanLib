lib_LTLIBRARIES = libclan22MikMod.la

libclan22MikMod_la_SOURCES =  \
 module_reader.cpp          \
 resourcetype_module.h      \
 module_reader.h	    \
 setupmikmod.cpp            \
 soundprovider_mikmod.cpp   \
 soundprovider_mikmod_session.cpp

libclan22MikMod_la_LIBADD = -lmikmod

libclan22MikMod_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_MikMod)

libclan22Sound_la_CXXFLAGS=$(MikMod_CXXFLAGS) $(extra_CFLAGS_MikMod)

# EOF #
