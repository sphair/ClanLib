lib_LTLIBRARIES = libclan21MikMod.la

libclan21MikMod_la_SOURCES =  \
 module_reader.cpp          \
 resourcetype_module.h      \
 module_reader.h	    \
 setupmikmod.cpp            \
 soundprovider_mikmod.cpp   \
 soundprovider_mikmod_session.cpp

libclan21MikMod_la_LIBADD = -lmikmod

libclan21MikMod_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE)

# EOF #
