lib_LTLIBRARIES = libclanNetwork.la

libclanNetwork_la_SOURCES = \
IRC/irc_connection.cpp \
IRC/irc_connection_generic.cpp \
IRC/irc_connection_generic.h \
IRC/dcc_download.cpp \
IRC/dcc_download_generic.cpp \
IRC/dcc_download_generic.h \
Browse/browse_server_generic.h \
NetSession/inputsource_netpacket.cpp \
NetSession/netsession_generic.cpp \
NetSession/netstream.cpp \
NetSession/outputsource_netstream.cpp \
NetSession/netpacket_generic.cpp \
NetSession/netpacket.cpp \
NetSession/outputsource_netpacket.cpp \
NetSession/inputsource_netpacket_generic.cpp \
NetSession/netpacket_generic.h \
NetSession/netstream_generic.h \
NetSession/netstream_generic.cpp \
NetSession/netcomputer_generic.cpp \
NetSession/netcomputer_generic.h \
NetSession/outputsource_netpacket_generic.cpp \
NetSession/netcomputer.cpp \
NetSession/inputsource_netpacket_generic.h \
NetSession/inputsource_netstream.cpp \
NetSession/netsession_generic.h \
NetSession/outputsource_netpacket_generic.h \
NetSession/netsession.cpp \
NetSession/netgroup.cpp \
Socket/event_trigger_socket.cpp \
Socket/socket_generic.h \
Socket/outputsource_socket.cpp \
Socket/ip_address_getaddr.cpp \
Socket/socket_select.cpp \
Socket/event_trigger_socket.h \
Socket/ip_address.cpp \
Socket/inputsource_socket.cpp \
Socket/socket.cpp \
Socket/ip_address_getaddr.h \
Socket/ip_address_getsock.cpp \
Socket/socket_generic.cpp \
Socket/ip_address_generic.h \
Socket/socket_select.h \
Socket/ip_address_getsock.h \
NetObjects/netobject_controller_generic.cpp \
NetObjects/netobject_client_generic.cpp \
NetObjects/netobject_server_generic.cpp \
NetObjects/netobject_controller_generic.h \
NetObjects/netobject_server_generic.h \
NetObjects/netobject_controller.cpp \
NetObjects/netobject_server.cpp \
NetObjects/netobject_client.cpp \
NetObjects/netobject_client_generic.h \
NetVariables/netvariables.cpp \
NetVariables/netvariables_generic.h

if WIN32
libclanNetwork_la_SOURCES += setupnetwork_win32.cpp
else
libclanNetwork_la_SOURCES += setupnetwork_unix.cpp
endif

libclanNetwork_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE)

# EOF #
