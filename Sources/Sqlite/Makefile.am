lib_LTLIBRARIES = libclanSqlite.la

libclanSqlite_la_SOURCES = \
precomp.cpp \
sqlite3.c \
sqlite_command_provider.cpp \
sqlite_connection.cpp \
sqlite_connection_provider.cpp \
sqlite_reader_provider.cpp \
sqlite_transaction_provider.cpp

libclanSqlite_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE)

# EOF #
