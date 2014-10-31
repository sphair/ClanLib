
#include "precomp.h"
#include "app_model.h"
#include "IRCSession/irc_session.h"
#if defined(XMPP_SUPPORT)
#include "XMPPSession/xmpp_session.h"
#endif
#include "IdentServer/ident_server.h"
#include <algorithm>

AppModel::AppModel()
{
}

AppModel::~AppModel()
{
	for (size_t i = 0; i < irc_sessions.size(); i++)
		delete irc_sessions[i];
	irc_sessions.clear();

#if defined(XMPP_SUPPORT)
	for (size_t i = 0; i < xmpp_sessions.size(); i++)
		delete xmpp_sessions[i];
	xmpp_sessions.clear();
#endif

	for (size_t i = 0; i < dcc_chat_connections.size(); i++)
		delete dcc_chat_connections[i];
	dcc_chat_connections.clear();
}

IRCSession *AppModel::create_irc_session(const std::string &connection_name)
{
	if (ident_server.get() == 0)
	{
		ident_server.reset(new IdentServer());
		ident_server->start();
	}

	IRCSession *session = new IRCSession;
	session->set_connection_name(connection_name);

	irc_sessions.push_back(session);

	cb_irc_session_created(session);

	return session;
}

void AppModel::destroy_irc_session(IRCSession *session)
{
	if (session)
	{
		std::vector<IRCSession*>::iterator it = std::find(irc_sessions.begin(), irc_sessions.end(), session);
		if (it == irc_sessions.end())
			throw clan::Exception("IRCSession object is already destroyed or does not belong to this document");

		cb_irc_session_destroyed(session);

		irc_sessions.erase(it);
		delete session;
	}

	if (irc_sessions.empty())
		ident_server.reset();
}

#if defined(XMPP_SUPPORT)
XMPPSession *AppModel::create_xmpp_session(const std::string &connection_name)
{
	if (ident_server.get() == 0)
	{
		ident_server.reset(new IdentServer());
		ident_server->start();
	}

	XMPPSession *session = new XMPPSession;
	session->set_connection_name(connection_name);

	xmpp_sessions.push_back(session);

	cb_xmpp_session_created(session);

	return session;
}

void AppModel::destroy_xmpp_session(XMPPSession *session)
{
	if (session)
	{
		std::vector<XMPPSession*>::iterator it = std::find(xmpp_sessions.begin(), xmpp_sessions.end(), session);
		if (it == xmpp_sessions.end())
			throw clan::Exception("XMPPSession object is already destroyed or does not belong to this document");

		cb_xmpp_session_destroyed(session);

		xmpp_sessions.erase(it);
		delete session;
	}
}
#endif

DCCChatConnection *AppModel::create_dcc_chat_connection(const std::string &local_nick, const std::string &remote_nick, clan::SocketName socket_name, bool is_server)
{
	DCCChatConnection *connection = new DCCChatConnection(local_nick, remote_nick, socket_name);
	if (is_server)
		connection->offer();
	else
		connection->connect();
	dcc_chat_connections.push_back(connection);
	cb_dcc_chat_connection_created(connection);
	return connection;
}

void AppModel::destroy_dcc_chat_connection(DCCChatConnection *connection)
{
	if (connection)
	{
		std::vector<DCCChatConnection*>::iterator it = std::find(dcc_chat_connections.begin(), dcc_chat_connections.end(), connection);
		if (it == dcc_chat_connections.end())
			throw clan::Exception("DCCChatConnection object is already destroyed or does not belong to this document");

		cb_dcc_chat_connection_destroyed(connection);

		dcc_chat_connections.erase(it);
		delete connection;
	}
}
