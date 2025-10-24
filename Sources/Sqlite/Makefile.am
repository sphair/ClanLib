lib_LTLIBRARIES = libclan23Sqlite.la

libclan23Sqlite_la_SOURCES = \
precomp.cpp \
sqlite3.c \
sqlite_command_provider.cpp \
sqlite_connection.cpp \
sqlite_connection_provider.cpp \
sqlite_reader_provider.cpp \
sqlite_transaction_provider.cpp

libclan23Sqlite_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_Sqlite)

libclan23Sound_la_CXXFLAGS=$(Sqlite_CXXFLAGS) $(extra_CFLAGS_Sqlite)

# EOF #
