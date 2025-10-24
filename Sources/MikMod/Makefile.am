lib_LTLIBRARIES = libclanMikMod.la

libclanMikMod_la_SOURCES =  \
 module_reader.cpp          \
 resourcetype_module.h      \
 module_reader.h	    \
 setupmikmod.cpp            \
 soundprovider_mikmod.cpp   \
 soundprovider_mikmod_session.cpp

libclanMikMod_la_LIBADD = -lmikmod

libclanMikMod_la_LDFLAGS = \
  -release $(LT_RELEASE) \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

# EOF #
