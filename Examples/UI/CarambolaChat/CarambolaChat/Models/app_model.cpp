
#include "precomp.h"
#include "app_model.h"
#include "IRCSession/irc_session.h"
#if defined(XMPP_SUPPORT)
#include "XMPPSession/xmpp_session.h"
#endif
#include "IdentServer/ident_server.h"
#include "XMLSettings/xml_settings_list.h"
#include <algorithm>

AppModel *AppModel::instance_ptr = nullptr;

AppModel::AppModel()
{
	instance_ptr = this;
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

	instance_ptr = nullptr;
}

AppModel *AppModel::instance()
{
	if (instance_ptr == nullptr)
		throw clan::Exception("No AppModel instance!");
	return instance_ptr;
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

void AppModel::auto_connect_to_servers()
{
	XMLSettingsList connections = settings.xml_settings.get_list("connections");
	int connection_count = connections.get_count();
	for (int i = 0; i < connection_count; ++i)
	{
		XMLSettings connection = connections.get(i);

		if (connection.get_bool("autoconnect"))
		{
			connect_to_server(connection);
		}
	}
}

void AppModel::connect_to_server(XMLSettings &connection)
{
	std::string connection_name = connection.get_string("connectionname");
	std::string connection_type = connection.get_string("connectiontype", "irc");
	if (connection_type == "irc")
	{
		std::string nick = connection.get_string("nick", "CTalk2015");
		std::string alt_nick = connection.get_string("altnick", nick + "_");
		std::string username = connection.get_string("username", "carambola");
		std::string name = connection.get_string("name", "Anonymous ClanLib Carambola User");
		std::string server_name = connection.get_string("server");

		std::vector<std::string> perform;
		XMLSettingsList performlist = connection.get_list("performlist");
		int perform_count = performlist.get_count();
		for (int i = 0; i<perform_count; i++)
		{
			XMLSettings item = performlist.get(i);
			perform.push_back(item.get_string("command"));
		}

		IRCSession *session = create_irc_session(connection_name);
		session->set_perform_list(perform);
		session->connect(server_name, "6667", nick, alt_nick, username, name);
	}
#if defined(XMPP_SUPPORT)
	else if (connection_type == "xmpp")
	{
		std::string server = connection.get_string("server");
		std::string port = connection.get_string("port");
		std::string username = connection.get_string("username");
		std::string password = connection.get_string("password");
		std::string domain = connection.get_string("domain");
		std::string name = connection.get_string("name");

		XMPPSession *session = document->create_xmpp_session(connection_name);
		session->set_nick_name(name);
		session->connect(server, port, username, password, domain);
	}
#endif
}
