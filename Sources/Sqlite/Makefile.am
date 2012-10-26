lib_LTLIBRARIES = libclan30Sqlite.la

libclan30Sqlite_la_SOURCES = \
precomp.cpp \
sqlite_command_provider.cpp \
sqlite_connection.cpp \
sqlite_connection_provider.cpp \
sqlite_reader_provider.cpp \
sqlite_transaction_provider.cpp

libclan30Sqlite_la_LIBADD = -lsqlite3

libclan30Sqlite_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_Sqlite)

libclan30Sound_la_CXXFLAGS=$(Sqlite_CXXFLAGS) $(extra_CFLAGS_Sqlite)

# EOF #
