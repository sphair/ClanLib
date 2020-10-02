lib_LTLIBRARIES = libclan41Network.la

libclan41Network_la_SOURCES = \
precomp.cpp \
NetGame/connection_impl.cpp \
NetGame/event_value.cpp \
NetGame/server.cpp \
NetGame/network_data.cpp \
NetGame/event.cpp \
NetGame/connection.cpp \
NetGame/client.cpp \
Socket/tcp_listen.cpp \
Socket/network_condition_variable.cpp \
Socket/socket_error.cpp \
Socket/udp_socket.cpp \
Socket/tcp_connection.cpp \
Socket/socket_name.cpp

if WIN32
libclan41Network_la_SOURCES += \
setupnetwork_win32.cpp
else
libclan41Network_la_SOURCES += \
setupnetwork_unix.cpp
endif

libclan41Network_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_Network)

libclan41Network_la_CXXFLAGS=$(Network_CXXFLAGS) $(extra_CFLAGS_Network)

# EOF #
