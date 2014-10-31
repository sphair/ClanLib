
#pragma once

#include "Models/XMLSettings/xml_settings.h"
#include "Models/IRCSession/irc_session.h"
#include "Models/IdentServer/ident_server.h"
#include "Models/DCCChat/dcc_chat_connection.h"
#include "settings.h"
#include <vector>

class IRCSession;
class IdentServer;
#if defined(XMPP_SUPPORT)
class XMPPSession;
#endif

class AppModel
{
public:
	AppModel();
	~AppModel();

	Settings settings;

	IRCSession *create_irc_session(const std::string &connection_name);
	void destroy_irc_session(IRCSession *session);

#if defined(XMPP_SUPPORT)
	XMPPSession *create_xmpp_session(const std::string &connection_name);
	void destroy_xmpp_session(XMPPSession *session);
#endif

	DCCChatConnection *create_dcc_chat_connection(const std::string &local_nick, const std::string &remote_nick, clan::SocketName socket_name, bool is_server);
	void destroy_dcc_chat_connection(DCCChatConnection *connection);

	clan::Signal<void(IRCSession *)> cb_irc_session_created;
	clan::Signal<void(IRCSession *)> cb_irc_session_destroyed;
#if defined(XMPP_SUPPORT)
	clan::Signal<void(XMPPSession *)> cb_xmpp_session_created;
	clan::Signal<void(XMPPSession *)> cb_xmpp_session_destroyed;
#endif
	clan::Signal<void(DCCChatConnection *)> cb_dcc_chat_connection_created;
	clan::Signal<void(DCCChatConnection *)> cb_dcc_chat_connection_destroyed;

private:
	std::vector<IRCSession *> irc_sessions;
	std::auto_ptr<IdentServer> ident_server;
#if defined(XMPP_SUPPORT)
	std::vector<XMPPSession *> xmpp_sessions;
#endif
	std::vector<DCCChatConnection *> dcc_chat_connections;
};
