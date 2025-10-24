lib_LTLIBRARIES = libclan23Database.la

libclan23Database_la_SOURCES = \
precomp.cpp \
db_command.cpp \
db_connection.cpp \
db_reader.cpp \
db_value.cpp \
db_transaction.cpp

libclan23Database_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanDatabase)

libclan23Database_la_CXXFLAGS=$(clanDatabase_CXXFLAGS) $(extra_CFLAGS_clanDatabase)

# EOF #
