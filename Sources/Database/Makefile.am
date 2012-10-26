lib_LTLIBRARIES = libclan30Database.la

libclan30Database_la_SOURCES = \
precomp.cpp \
db_command.cpp \
db_connection.cpp \
db_reader.cpp \
db_value.cpp \
db_transaction.cpp

libclan30Database_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanDatabase)

libclan30Database_la_CXXFLAGS=$(clanDatabase_CXXFLAGS) $(extra_CFLAGS_clanDatabase)

# EOF #
