
#include "precomp.h"
#include "command.h"
#include "irc_session.h"

void Command::execute(IRCSession *session, const IRCEntity &filter, std::string command_line)
{
	CommandFunc functions[] =
	{
		"/nick", &Command::execute_nick,
		"/oper", &Command::execute_oper,
		"/quit", &Command::execute_quit,
		"/join", &Command::execute_join,
		"/part", &Command::execute_part,
		"/mode", &Command::execute_mode,
		"/topic", &Command::execute_topic,
		"/names", &Command::execute_names,
		"/list", &Command::execute_list,
		"/invite", &Command::execute_invite,
		"/kick", &Command::execute_kick,
		"/send_version", &Command::execute_send_version,
		"/stats", &Command::execute_stats,
		"/links", &Command::execute_links,
		"/send_time", &Command::execute_send_time,
		"/connect", &Command::execute_connect,
		"/trace", &Command::execute_trace,
		"/admin", &Command::execute_admin,
		"/info", &Command::execute_info,
		"/msg", &Command::execute_privmsg,
		"/privmsg", &Command::execute_privmsg,
		"/ctcp", &Command::execute_ctcp,
		"/privmsg_ctcp", &Command::execute_ctcp,
		"/notice", &Command::execute_notice,
		"/notice_ctcp", &Command::execute_notice_ctcp,
		"/who", &Command::execute_who,
		"/whois", &Command::execute_whois,
		"/whowas", &Command::execute_whowas,
		"/kill", &Command::execute_kill,
		"/me", &Command::execute_me,
		"/action", &Command::execute_me,
		"/dcc", &Command::execute_dcc,
		"/finger", &Command::execute_finger,
		"/version", &Command::execute_version,
		"/userinfo", &Command::execute_userinfo,
		"/clientinfo", &Command::execute_clientinfo,
		"/ping", &Command::execute_ping,
		"/time", &Command::execute_time,
		0, 0
	};

	std::vector<std::string> args = clan::StringHelp::split_text(command_line, " ");
	for (int i=0; functions[i].name != 0; i++)
	{
		std::string str(functions[i].name);
		if (str == args[0])
		{
			functions[i].func(session, filter, args);
			break;
		}
	}
}

void Command::execute_nick(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 2)
	{
		session->set_nick(args[1]);
	}
	else
	{
		throw clan::Exception("syntax: /nick <new nick>");
	}
}

void Command::execute_quit(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 1)
	{
		session->connection.send_quit(IRCText());
	}
	else if (args.size() >= 2)
	{
		std::string reason;
		for (unsigned int i=1; i<args.size(); i++)
		{
			if (i > 1) reason += " ";
			reason += args[i];
		}

		session->connection.send_quit(IRCText::from_text(reason));
	}

	// exit_with_code(1);
}

void Command::execute_join(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 2)
	{
		session->connection.send_join(IRCChannel::from_text(args[1]));
	}
	else if (args.size() == 3)
	{
		session->connection.send_join(IRCChannel::from_text(args[1]), clan::StringHelp::text_to_utf8(args[2]));
	}
	else
	{
		throw clan::Exception("syntax: /join <channel> [key]");
	}
}

void Command::execute_part(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 2)
	{
		session->connection.send_part(IRCChannel::from_text(args[1]));
	}
/*	else if (args.size() >= 3)
	{
		std::string reason;
		for (unsigned int i=2; i<args.size(); i++)
		{
			if (i > 2) reason += " ";
			reason += args[i];
		}

		session->connection.send_part(args[1], reason);
	}*/
	else
	{
		throw clan::Exception("syntax: /part <channel>");
		//throw clan::Exception("syntax: /part <channel> [reason]");
	}
}

void Command::execute_mode(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() >= 3)
	{
//		std::vector<std::string> remaining_args;
//		for (unsigned int i=3; i<args.size(); i++) remaining_args.push_back(args[i]);
//		session->connection.send_mode(args[1], args[2], remaining_args);
	}
	else
	{
		throw clan::Exception("syntax: /mode <channel or nick> <mode> [args]");
	}
}

void Command::execute_topic(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 2)
	{
//		session->connection.send_topic(args[1]);
	}
	else if (args.size() > 2)
	{
		std::string new_topic;
		for (unsigned int i=2; i<args.size(); i++)
		{
			if (i > 2) new_topic += " ";
			new_topic += args[i];
		}
		session->connection.send_topic(IRCChannel::from_text(args[1]), IRCText::from_text(new_topic));
	}
	else
	{
		throw clan::Exception("syntax: /topic <channel> [new topic]");
	}
}

void Command::execute_names(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 2)
	{
		session->connection.send_names(IRCChannel::from_text(args[1]));
	}
	else
	{
		throw clan::Exception("syntax: /names <channel>");
	}
}

void Command::execute_list(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 1)
	{
//		session->connection.send_list();
	}
	else if (args.size() == 2)
	{
//		session->connection.send_list(args[1]);
	}
	else if (args.size() == 3)
	{
//		session->connection.send_list(args[1], args[2]);
	}
	else
	{
		throw clan::Exception("syntax: /list [channel] [server]");
	}
}

void Command::execute_invite(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 3)
	{
		session->connection.send_invite(IRCNick::from_text(args[1]), IRCChannel::from_text(args[2]));
	}
	else
	{
		throw clan::Exception("syntax: /invite <nick> <channel>");
	}
}

void Command::execute_kick(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() >= 3)
	{
		std::string comment;
		for (unsigned int i=3; i<args.size(); i++)
		{
			if (i > 3) comment += " ";
			comment += args[i];
		}
		session->connection.send_kick(IRCChannel::from_text(args[1]), IRCNick::from_text(args[2]), IRCText::from_text(comment));
	}
	else
	{
		throw clan::Exception("syntax: /kick <channel> <user> [comment]");
	}
}

void Command::execute_send_version(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 1)
	{
//		session->connection.send_version();
	}
	else if (args.size() == 2)
	{
//		session->connection.send_version(args[1]);
	}
	else
	{
		throw clan::Exception("syntax: /send_version [server]");
	}
}

void Command::execute_stats(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 1)
	{
//		session->connection.send_stats();
	}
	else if (args.size() == 2)
	{
//		session->connection.send_stats(args[1]);
	}
	else if (args.size() == 3)
	{
//		session->connection.send_stats(args[1], args[2]);
	}
	else
	{
		throw clan::Exception("syntax: /stats [query] [server]");
	}
}

void Command::execute_links(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 1)
	{
//		session->connection.send_links();
	}
	else if (args.size() == 2)
	{
//		session->connection.send_links(args[1]);
	}
	else if (args.size() == 3)
	{
//		session->connection.send_links(args[1], args[2]);
	}
	else
	{
		throw clan::Exception("syntax: /links [remote server] [server mask]");
	}
}

void Command::execute_send_time(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 1)
	{
//		session->connection.send_time();
	}
	else if (args.size() == 2)
	{
//		session->connection.send_time(args[1]);
	}
	else
	{
		throw clan::Exception("syntax: /send_time [server]");
	}
}

void Command::execute_connect(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 2)
	{
//		session->connection.send_connect(args[1]);
	}
	else if (args.size() == 3)
	{
//		session->connection.send_connect(args[1], clan::StringHelp::text_to_int(args[2]));
	}
	else if (args.size() == 4)
	{
//		session->connection.send_connect(args[1], clan::StringHelp::text_to_int(args[2]), args[3]);
	}
	else
	{
		throw clan::Exception("syntax: /connect <target server> [port] [remote server]");
	}
}

void Command::execute_trace(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 1)
	{
//		session->connection.send_trace();
	}
	else if (args.size() == 2)
	{
//		session->connection.send_trace(args[1]);
	}
	else
	{
		throw clan::Exception("syntax: /trace [server]");
	}
}

void Command::execute_admin(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 1)
	{
//		session->connection.send_admin();
	}
	else if (args.size() == 2)
	{
//		session->connection.send_admin(args[1]);
	}
	else
	{
		throw clan::Exception("syntax: /admin [server]");
	}
}

void Command::execute_info(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 1)
	{
//		session->connection.send_info();
	}
	else if (args.size() == 2)
	{
//		session->connection.send_info(args[1]);
	}
	else
	{
		throw clan::Exception("syntax: /info [server]");
	}
}

void Command::execute_privmsg(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() >= 2)
	{
		std::string text;
		for (unsigned int i=2; i<args.size(); i++)
		{
			if (i > 2) text += " ";
			text += args[i];
		}
		session->connection.send_privmsg(IRCEntity::from_text(args[1]), IRCText::from_text(text));
	}
	else
	{
		throw clan::Exception("syntax: /privmsg <nick or channel> <text>");
	}
}

void Command::execute_ctcp(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() >= 3)
	{
/*		std::string data;
		for (unsigned int i=3; i<args.size(); i++)
		{
			if (i > 3) data += " ";
			data += args[i];
		}
		session->connection.send_privmsg_ctcp(args[1], args[2], data);*/
	}
	else
	{
		throw clan::Exception("syntax: /ctcp <nick or channel> <command> [data]");
	}
}

void Command::execute_notice(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() >= 2)
	{
		std::string text;
		for (unsigned int i=2; i<args.size(); i++)
		{
			if (i > 2) text += " ";
			text += args[i];
		}
		session->connection.send_notice(IRCEntity::from_text(args[1]), IRCText::from_text(text));
	}
	else
	{
		throw clan::Exception("syntax: /notice <nick or channel> <text>");
	}
}

void Command::execute_notice_ctcp(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() >= 3)
	{
/*		std::string data;
		for (unsigned int i=3; i<args.size(); i++)
		{
			if (i > 3) data += " ";
			data += args[i];
		}
		session->connection.send_notice_ctcp(args[1], args[2], data);*/
	}
	else
	{
		throw clan::Exception("syntax: /notice_ctcp <nick or channel> <command> [data]");
	}
}

void Command::execute_who(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 2)
	{
		session->connection.send_who(IRCEntity::from_text(args[1]), false);
	}
	else if (args.size() == 3)
	{
		session->connection.send_who(IRCEntity::from_text(args[1]), args[2] == "true");
	}
	else
	{
		throw clan::Exception("syntax: /whois <name> [oper = false]");
	}
}

void Command::execute_whois(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 2)
	{
		session->connection.send_whois(IRCEntity::from_text(args[1]));
	}
	else if (args.size() == 3)
	{
//		session->connection.send_whois(IRCEntity::from_text(args[1]), IRCEntity::from_text(args[2]));
	}
	else
	{
		throw clan::Exception("syntax: /whois <nickmask>");
		// throw clan::Exception("syntax: /whois <server> <nickmask>");
	}
}

void Command::execute_whowas(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 2)
	{
		session->connection.send_whowas(IRCEntity::from_text(args[1]));
	}
	else if (args.size() == 3)
	{
//		session->connection.send_whowas(IRCEntity::from_text(args[1]), clan::StringHelp::text_to_int(args[2]));
	}
	else if (args.size() == 4)
	{
//		session->connection.send_whowas(IRCEntity::from_text(args[1]), clan::StringHelp::text_to_int(args[2]), IRCEntity::from_text(args[3]));
	}
	else
	{
		throw clan::Exception("syntax: /whowas <nick> [count] [server]");
	}
}

void Command::execute_kill(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() >= 3)
	{
		std::string text;
		for (unsigned int i=2; i<args.size(); i++)
		{
			if (i > 2) text += " ";
			text += args[i];
		}
		session->connection.send_kill(IRCEntity::from_text(args[1]), IRCText::from_text(text));
	}
	else
	{
		throw clan::Exception("syntax: /kill <nick> <comment>");
	}
}

void Command::execute_me(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	std::string text;
	for (std::string::size_type i=1; i<args.size(); i++)
	{
		if (i > 1) text += " ";
		text += args[i];
	}
	session->send_text(filter, IRCText::from_ctcp_data("ACTION "+IRCText::from_text(text).to_raw()));
	session->cb_action(filter, session->our_nickname, IRCText::from_text(text));
}

void Command::execute_finger(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 2)
	{
		//session->connection.send_privmsg_ctcp(args[1], "FINGER", std::string());
	}
	else
	{
		throw clan::Exception("syntax: /finger <channel or user>");
	}
}

void Command::execute_version(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 2)
	{
		//session->connection.send_privmsg_ctcp(args[1], "VERSION", std::string());
	}
	else
	{
		throw clan::Exception("syntax: /version <channel or user>");
	}
}

void Command::execute_userinfo(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 2)
	{
		//session->connection.send_privmsg_ctcp(args[1], "USERINFO", std::string());
	}
	else
	{
		throw clan::Exception("syntax: /userinfo <channel or user>");
	}
}

void Command::execute_clientinfo(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 2)
	{
		//session->connection.send_privmsg_ctcp(args[1], "CLIENTINFO", std::string());
	}
	else
	{
		throw clan::Exception("syntax: /clientinfo <channel or user>");
	}
}

void Command::execute_ping(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 2)
	{
		//session->connection.send_privmsg_ctcp(args[1], "PING", clan::StringHelp::int_to_text(clan::System::get_time()));
	}
	else
	{
		throw clan::Exception("syntax: /ping <channel or user>");
	}
}

void Command::execute_time(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	if (args.size() == 2)
	{
		//session->connection.send_privmsg_ctcp(args[1], "TIME", std::string());
	}
	else
	{
		throw clan::Exception("syntax: /time <channel or user>");
	}
}

void Command::execute_oper(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{
	// session->connection.send_oper(const std::string &user, const std::string &password);
}

void Command::execute_dcc(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args)
{

}
