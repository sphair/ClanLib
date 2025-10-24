
#include "clanbot.h"

ClanBotConnection app;

int ClanBotConnection::main(int argc, char **argv)
{
	// Create a console window for text-output if not available
	CL_ConsoleWindow console("Console");
	console.redirect_stdio();

	CL_SetupCore setup_core;
	CL_SetupNetwork setup_network;

	CL_Slot slot_log = CL_Log::sig_log().connect(this, &ClanBotConnection::on_log);

	connected = false;
	server = "irc.freenode.net";

	try
	{
		connection = new CL_IRCConnection(server);
		connection->send_nick("ClanBot");
		connection->send_user("clanbot", "localhost", "localhost", "ClanLib Channel Bot");

		slots.connect(connection->sig_command_received(), this, &ClanBotConnection::on_connection_command_received);
		slots.connect(connection->sig_unknown_command_received(), this, &ClanBotConnection::on_connection_unknown_command_received);
		slots.connect(connection->sig_numeric_reply(), this, &ClanBotConnection::on_connection_numeric_reply);
		slots.connect(connection->sig_name_reply(), this, &ClanBotConnection::on_connection_name_reply);
		slots.connect(connection->sig_nick(), this, &ClanBotConnection::on_connection_nick);
		slots.connect(connection->sig_join(), this, &ClanBotConnection::on_connection_join);
		slots.connect(connection->sig_part(), this, &ClanBotConnection::on_connection_part);
		slots.connect(connection->sig_mode(), this, &ClanBotConnection::on_connection_mode);
		slots.connect(connection->sig_topic(), this, &ClanBotConnection::on_connection_topic);
		slots.connect(connection->sig_invite(), this, &ClanBotConnection::on_connection_invite);
		slots.connect(connection->sig_kick(), this, &ClanBotConnection::on_connection_kick);
		slots.connect(connection->sig_privmsg(), this, &ClanBotConnection::on_connection_privmsg);
		slots.connect(connection->sig_notice(), this, &ClanBotConnection::on_connection_notice);
		slots.connect(connection->sig_privmsg_ctcp(), this, &ClanBotConnection::on_connection_privmsg_ctcp);
		slots.connect(connection->sig_notice_ctcp(), this, &ClanBotConnection::on_connection_notice_ctcp);
		slots.connect(connection->sig_ping(), this, &ClanBotConnection::on_connection_ping);
	}
	catch (CL_Error err)
	{
		CL_Log::log("system", "Unable to connect to server %1", server);

		// Display console close message and wait for a key
		console.display_close_message();

		return 1;
	}

	bool quit = false;
	while (!quit)
	{
		try
		{
			CL_System::keep_alive(500);
		}
		catch (CL_Error error)
		{
			CL_Log::log("error", "Fatal error processing keep_alive: %1", error.message);

			// Display console close message and wait for a key
			console.display_close_message();
		}
	}
	
	delete connection;

	return 0;
}

void ClanBotConnection::on_log(const std::string &id, int severity, const std::string &message)
{
	std::cout << "[" << id << "] " << message << std::endl;
}

void ClanBotConnection::on_connection_command_received(const std::string &prefix, const std::string &command, const std::vector<std::string> &params)
{
}

void ClanBotConnection::on_connection_unknown_command_received(const std::string &prefix, const std::string &command, const std::vector<std::string> &params)
{
}

void ClanBotConnection::on_connection_numeric_reply(const std::string &prefix, int command, const std::vector<std::string> &params)
{
	std::string str_params;
	for (unsigned int i=0; i<params.size(); i++) str_params += " " + params[i];
	CL_Log::log(server, "%1 [%2]:%3", prefix, command, str_params);
	
	if (command == RPL_WELCOME)
	{
		connection->send_join("#clanlib");
		connected = true;
	}
}

void ClanBotConnection::on_connection_name_reply(const std::string &self, const std::string &channel, const std::vector<std::string> &users)
{
}

void ClanBotConnection::on_connection_nick(const std::string &old_nick, const std::string &new_nick)
{
}

void ClanBotConnection::on_connection_join(const std::string &nick, const std::string &channel)
{
	if (nick.find("ClanBot") != std::string::npos)
	{
		CL_Log::log(server, "Joined channel %1", channel);
	}
}

void ClanBotConnection::on_connection_part(const std::string &nick, const std::string &channel, const std::string &reason)
{
}

void ClanBotConnection::on_connection_mode(const std::string &prefix, const std::string &receiver, const std::string &mode, const std::vector<std::string> &params)
{
}

void ClanBotConnection::on_connection_topic(const std::string &prefix, const std::string &channel, const std::string &topic)
{
}

void ClanBotConnection::on_connection_invite(const std::string &prefix, const std::string &nick, const std::string &channel)
{
}

void ClanBotConnection::on_connection_kick(const std::string &prefix, const std::string &channel, const std::string &user, const std::string &comment)
{
}

void ClanBotConnection::on_connection_privmsg(const std::string &prefix, const std::string &receiver, const std::string &text)
{
	if (receiver[0] != '#')
	{
		connection->send_privmsg("#clanlib", text);
	}
	
	CL_Log::log(server, "%1 privmsg %2: %3", prefix, receiver, text);
}

void ClanBotConnection::on_connection_notice(const std::string &prefix, const std::string &receiver, const std::string &text)
{
//	if (receiver[0] != '#')
		CL_Log::log(server, "%1 notice %2: %3", prefix, receiver, text);
}

void ClanBotConnection::on_connection_privmsg_ctcp(const std::string &prefix, const std::string &receiver, const std::string &command, const std::string &data)
{
	std::string nick = prefix.substr(0,prefix.find("!"));

	if (command == "DCC")
	{
		CL_Log::log(server, "%1 DCC line: %2", nick, data);

		std::vector<std::string> tokens = CL_String::tokenize(data, " ");
		if (tokens.size() >= 4 && tokens[0] == "SEND")
		{
		}
		else if (tokens.size() == 4 && tokens[0] == "CHAT" && tokens[1] == "chat")
		{
		}
	}
	else if (command == "FINGER")
	{
		if (data.empty())
		{
			connection->send_privmsg_ctcp(nick, "FINGER", ":ClanLib's Example IRC Client");
		}
		else
		{
			CL_Log::log(server, "%1 FINGER REPLY: %2", nick, data.substr(1));
		}
	}
	else if (command == "VERSION")
	{
		if (data.empty())
		{
#ifdef WIN32
			connection->send_privmsg_ctcp(nick, "VERSION", "ClanBot:0.1:Windows");
#else
			connection->send_privmsg_ctcp(nick, "VERSION", "ClanBot:0.1:Unix");
#endif
		}
		else
		{
			CL_Log::log(server, "%1 VERSION REPLY: %2", nick, data);
		}
	}
	else if (command == "USERINFO")
	{
		if (data.empty())
		{
			connection->send_privmsg_ctcp(nick, "USERINFO", ":A ClanLib User");
		}
		else
		{
			CL_Log::log(server, "%1 USERINFO REPLY: %2", nick, data.substr(1));
		}
	}
	else if (command == "CLIENTINFO")
	{
		if (data.empty())
		{
			connection->send_privmsg_ctcp(nick, "CLIENTINFO", "ACTION DCC FINGER VERSION USERINFO CLIENTINFO PING TIME");
		}
		else
		{
			CL_Log::log(server, "%1 CLIENTINFO REPLY: %2", nick, data.substr(1));
		}
	}
	else if (command == "PING")
	{
		connection->send_notice_ctcp(nick, "PING", data);
	}
	else if (command == "TIME")
	{
		if (data.empty())
		{
			connection->send_privmsg_ctcp(nick, "TIME", "I havn't got the slightest clue what the time is!");
		}
		else
		{
			CL_Log::log(server, "%1 TIME REPLY: %2", nick, data.substr(1));
		}
	}
	else
	{
		CL_Log::log(server, "%1 CTCP %2: %3", nick, command, data);
	}
}

void ClanBotConnection::on_connection_notice_ctcp(const std::string &prefix, const std::string &receiver, const std::string &command, const std::string &data)
{
	if (command == "PING")
	{
		CL_Log::log(server, "%1 PING: %2 ms", prefix, CL_String::from_int(CL_String::to_int(data)-CL_System::get_time()));
	}
	else
	{
		on_connection_privmsg_ctcp(prefix, receiver, command, data);
	}
}

void ClanBotConnection::on_connection_ping(const std::string &daemon1, const std::string &daemon2)
{
	connection->send_pong(daemon1, daemon2);
}
