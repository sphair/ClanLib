lib_LTLIBRARIES = libclan21Network.la

libclan21Network_la_SOURCES = \
precomp.cpp \
IRC/dcc_download.cpp \
IRC/dcc_download_generic.cpp \
IRC/irc_connection.cpp \
IRC/irc_connection_generic.cpp \
NetGame/client.cpp \
NetGame/connection.cpp \
NetGame/connection_impl.cpp \
NetGame/event.cpp \
NetGame/event_value.cpp \
NetGame/network_data.cpp \
NetGame/server.cpp \
Web/http_client_connection.cpp \
Web/http_request_handler.cpp \
Web/http_request_handler_impl.cpp \
Web/http_server_connection.cpp \
Web/http_server_connection_impl.cpp \
Web/http_server.cpp \
Web/http_server_impl.cpp \
Web/soap_help.cpp \
Web/url_help.cpp \
Web/webservice_client.cpp \
Web/webservice_message.cpp \
Web/webservice_part.cpp \
Socket/dns_packet.cpp \
Socket/dns_resolver.cpp \
Socket/dns_resolver_impl.cpp \
Socket/dns_resource_record.cpp \
Socket/iodevice_provider_tcp_connection.cpp \
Socket/socket_error.cpp \
Socket/socket_name.cpp \
Socket/tcp_connection.cpp \
Socket/tcp_listen.cpp \
Socket/tcp_listen_impl.cpp \
Socket/udp_socket.cpp \
Socket/udp_socket_impl.cpp \
Socket/unix_socket.cpp

if WIN32
libclan21Network_la_SOURCES += \
Socket/event_provider_win32socket.cpp \
Socket/win32_socket.cpp \
setupnetwork_win32.cpp
else
libclan21Network_la_SOURCES += \
setupnetwork_unix.cpp
endif

libclan21Network_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE)

# EOF #
