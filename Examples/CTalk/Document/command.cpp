
#include "precomp.h"
#include "command.h"
#include "irc_session.h"

void Command::execute(IRCSession *session, const IRCEntity &filter, CL_String command_line)
{
	CommandFunc functions[] =
	{
		L"/nick", &Command::execute_nick,
		L"/oper", &Command::execute_oper,
		L"/quit", &Command::execute_quit,
		L"/join", &Command::execute_join,
		L"/part", &Command::execute_part,
		L"/mode", &Command::execute_mode,
		L"/topic", &Command::execute_topic,
		L"/names", &Command::execute_names,
		L"/list", &Command::execute_list,
		L"/invite", &Command::execute_invite,
		L"/kick", &Command::execute_kick,
		L"/send_version", &Command::execute_send_version,
		L"/stats", &Command::execute_stats,
		L"/links", &Command::execute_links,
		L"/send_time", &Command::execute_send_time,
		L"/connect", &Command::execute_connect,
		L"/trace", &Command::execute_trace,
		L"/admin", &Command::execute_admin,
		L"/info", &Command::execute_info,
		L"/msg", &Command::execute_privmsg,
		L"/privmsg", &Command::execute_privmsg,
		L"/ctcp", &Command::execute_ctcp,
		L"/privmsg_ctcp", &Command::execute_ctcp,
		L"/notice", &Command::execute_notice,
		L"/notice_ctcp", &Command::execute_notice_ctcp,
		L"/who", &Command::execute_who,
		L"/whois", &Command::execute_whois,
		L"/whowas", &Command::execute_whowas,
		L"/kill", &Command::execute_kill,
		L"/me", &Command::execute_me,
		L"/action", &Command::execute_me,
		L"/dcc", &Command::execute_dcc,
		L"/finger", &Command::execute_finger,
		L"/version", &Command::execute_version,
		L"/userinfo", &Command::execute_userinfo,
		L"/clientinfo", &Command::execute_clientinfo,
		L"/ping", &Command::execute_ping,
		L"/time", &Command::execute_time,
		0, 0
	};

	std::vector<CL_TempString> args = CL_StringHelp::split_text(command_line, " ");
	for (int i=0; functions[i].name != 0; i++)
	{
		CL_StringRef str(functions[i].name);
		if (str == args[0])
		{
			functions[i].func(session, filter, args);
			break;
		}
	}
}

void Command::execute_nick(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() == 2)
	{
		session->set_nick(args[1]);
	}
	else
	{
		throw CL_Exception("syntax: /nick <new nick>");
	}
}

void Command::execute_quit(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() == 1)
	{
		session->connection.send_quit(IRCText::from_text("ClanLib - the biggest overkill library ever written for an IRC client!"));
	}
	else if (args.size() >= 2)
	{
		CL_String reason;
		for (unsigned int i=1; i<args.size(); i++)
		{
			if (i > 1) reason += " ";
			reason += args[i];
		}

		session->connection.send_quit(IRCText::from_text(reason));
	}

	// exit_with_code(1);
}

void Command::execute_join(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() == 2)
	{
		session->connection.send_join(IRCChannel::from_text(args[1]));
	}
	else if (args.size() == 3)
	{
		session->connection.send_join(IRCChannel::from_text(args[1]), CL_StringHelp::text_to_utf8(args[2]));
	}
	else
	{
		throw CL_Exception("syntax: /join <channel> [key]");
	}
}

void Command::execute_part(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() == 2)
	{
		session->connection.send_part(IRCChannel::from_text(args[1]));
	}
/*	else if (args.size() >= 3)
	{
		CL_String reason;
		for (unsigned int i=2; i<args.size(); i++)
		{
			if (i > 2) reason += " ";
			reason += args[i];
		}

		session->connection.send_part(args[1], reason);
	}*/
	else
	{
		throw CL_Exception("syntax: /part <channel>");
		//throw CL_Exception("syntax: /part <channel> [reason]");
	}
}

void Command::execute_mode(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() >= 3)
	{
//		std::vector<CL_String> remaining_args;
//		for (unsigned int i=3; i<args.size(); i++) remaining_args.push_back(args[i]);
//		session->connection.send_mode(args[1], args[2], remaining_args);
	}
	else
	{
		throw CL_Exception("syntax: /mode <channel or nick> <mode> [args]");
	}
}

void Command::execute_topic(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() == 2)
	{
//		session->connection.send_topic(args[1]);
	}
	else if (args.size() > 2)
	{
		CL_String new_topic;
		for (unsigned int i=2; i<args.size(); i++)
		{
			if (i > 2) new_topic += " ";
			new_topic += args[i];
		}
		session->connection.send_topic(IRCChannel::from_text(args[1]), IRCText::from_text(new_topic));
	}
	else
	{
		throw CL_Exception("syntax: /topic <channel> [new topic]");
	}
}

void Command::execute_names(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() == 2)
	{
		session->connection.send_names(IRCChannel::from_text(args[1]));
	}
	else
	{
		throw CL_Exception("syntax: /names <channel>");
	}
}

void Command::execute_list(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
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
		throw CL_Exception("syntax: /list [channel] [server]");
	}
}

void Command::execute_invite(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() == 3)
	{
		session->connection.send_invite(IRCNick::from_text(args[1]), IRCChannel::from_text(args[2]));
	}
	else
	{
		throw CL_Exception("syntax: /invite <nick> <channel>");
	}
}

void Command::execute_kick(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() >= 3)
	{
		CL_String comment;
		for (unsigned int i=2; i<args.size(); i++)
		{
			if (i > 2) comment += " ";
			comment += args[i];
		}
		session->connection.send_kick(IRCChannel::from_text(args[1]), IRCNick::from_text(args[2]), IRCText::from_text(comment));
	}
	else
	{
		throw CL_Exception("syntax: /kick <channel> <user> [comment]");
	}
}

void Command::execute_send_version(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
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
		throw CL_Exception("syntax: /send_version [server]");
	}
}

void Command::execute_stats(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
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
		throw CL_Exception("syntax: /stats [query] [server]");
	}
}

void Command::execute_links(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
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
		throw CL_Exception("syntax: /links [remote server] [server mask]");
	}
}

void Command::execute_send_time(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
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
		throw CL_Exception("syntax: /send_time [server]");
	}
}

void Command::execute_connect(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() == 2)
	{
//		session->connection.send_connect(args[1]);
	}
	else if (args.size() == 3)
	{
//		session->connection.send_connect(args[1], CL_StringHelp::text_to_int(args[2]));
	}
	else if (args.size() == 4)
	{
//		session->connection.send_connect(args[1], CL_StringHelp::text_to_int(args[2]), args[3]);
	}
	else
	{
		throw CL_Exception("syntax: /connect <target server> [port] [remote server]");
	}
}

void Command::execute_trace(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
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
		throw CL_Exception("syntax: /trace [server]");
	}
}

void Command::execute_admin(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
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
		throw CL_Exception("syntax: /admin [server]");
	}
}

void Command::execute_info(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
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
		throw CL_Exception("syntax: /info [server]");
	}
}

void Command::execute_privmsg(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() >= 2)
	{
		CL_String text;
		for (unsigned int i=2; i<args.size(); i++)
		{
			if (i > 2) text += " ";
			text += args[i];
		}
		session->connection.send_privmsg(IRCEntity::from_text(args[1]), IRCText::from_text(text));
	}
	else
	{
		throw CL_Exception("syntax: /privmsg <nick or channel> <text>");
	}
}

void Command::execute_ctcp(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() >= 3)
	{
/*		CL_String data;
		for (unsigned int i=3; i<args.size(); i++)
		{
			if (i > 3) data += " ";
			data += args[i];
		}
		session->connection.send_privmsg_ctcp(args[1], args[2], data);*/
	}
	else
	{
		throw CL_Exception("syntax: /ctcp <nick or channel> <command> [data]");
	}
}

void Command::execute_notice(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() >= 2)
	{
		CL_String text;
		for (unsigned int i=2; i<args.size(); i++)
		{
			if (i > 2) text += " ";
			text += args[i];
		}
		session->connection.send_notice(IRCEntity::from_text(args[1]), IRCText::from_text(text));
	}
	else
	{
		throw CL_Exception("syntax: /notice <nick or channel> <text>");
	}
}

void Command::execute_notice_ctcp(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() >= 3)
	{
/*		CL_String data;
		for (unsigned int i=3; i<args.size(); i++)
		{
			if (i > 3) data += " ";
			data += args[i];
		}
		session->connection.send_notice_ctcp(args[1], args[2], data);*/
	}
	else
	{
		throw CL_Exception("syntax: /notice_ctcp <nick or channel> <command> [data]");
	}
}

void Command::execute_who(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() == 2)
	{
		session->connection.send_who(IRCEntity::from_text(args[1]), false);
	}
	else if (args.size() == 3)
	{
		session->connection.send_who(IRCEntity::from_text(args[1]), args[2] == cl_text("true"));
	}
	else
	{
		throw CL_Exception("syntax: /whois <name> [oper = false]");
	}
}

void Command::execute_whois(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
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
		throw CL_Exception("syntax: /whois <nickmask>");
		// throw CL_Exception("syntax: /whois <server> <nickmask>");
	}
}

void Command::execute_whowas(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() == 2)
	{
		session->connection.send_whowas(IRCEntity::from_text(args[1]));
	}
	else if (args.size() == 3)
	{
//		session->connection.send_whowas(IRCEntity::from_text(args[1]), CL_StringHelp::text_to_int(args[2]));
	}
	else if (args.size() == 4)
	{
//		session->connection.send_whowas(IRCEntity::from_text(args[1]), CL_StringHelp::text_to_int(args[2]), IRCEntity::from_text(args[3]));
	}
	else
	{
		throw CL_Exception("syntax: /whowas <nick> [count] [server]");
	}
}

void Command::execute_kill(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() >= 3)
	{
		CL_String text;
		for (unsigned int i=2; i<args.size(); i++)
		{
			if (i > 2) text += " ";
			text += args[i];
		}
		session->connection.send_kill(IRCEntity::from_text(args[1]), IRCText::from_text(text));
	}
	else
	{
		throw CL_Exception("syntax: /kill <nick> <comment>");
	}
}

void Command::execute_me(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	CL_String text;
	for (CL_String::size_type i=1; i<args.size(); i++)
	{
		if (i > 1) text += " ";
		text += args[i];
	}
	session->send_text(filter, IRCText::from_ctcp_data("ACTION "+IRCText::from_text(text).to_raw()));
	if (filter.is_channel())
		session->cb_channel_action.invoke(filter, session->our_nickname, IRCText::from_text(text));
//	else
//		session->cb_private_action.invoke(filter, session->our_nickname, IRCText(text));
}

void Command::execute_finger(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() == 2)
	{
		//session->connection.send_privmsg_ctcp(args[1], "FINGER", CL_String());
	}
	else
	{
		throw CL_Exception("syntax: /finger <channel or user>");
	}
}

void Command::execute_version(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() == 2)
	{
		//session->connection.send_privmsg_ctcp(args[1], "VERSION", CL_String());
	}
	else
	{
		throw CL_Exception("syntax: /version <channel or user>");
	}
}

void Command::execute_userinfo(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() == 2)
	{
		//session->connection.send_privmsg_ctcp(args[1], "USERINFO", CL_String());
	}
	else
	{
		throw CL_Exception("syntax: /userinfo <channel or user>");
	}
}

void Command::execute_clientinfo(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() == 2)
	{
		//session->connection.send_privmsg_ctcp(args[1], "CLIENTINFO", CL_String());
	}
	else
	{
		throw CL_Exception("syntax: /clientinfo <channel or user>");
	}
}

void Command::execute_ping(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() == 2)
	{
		//session->connection.send_privmsg_ctcp(args[1], "PING", CL_StringHelp::int_to_text(CL_System::get_time()));
	}
	else
	{
		throw CL_Exception("syntax: /ping <channel or user>");
	}
}

void Command::execute_time(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	if (args.size() == 2)
	{
		//session->connection.send_privmsg_ctcp(args[1], "TIME", CL_String());
	}
	else
	{
		throw CL_Exception("syntax: /time <channel or user>");
	}
}

void Command::execute_oper(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{
	// session->connection.send_oper(const CL_String &user, const CL_String &password);
}

void Command::execute_dcc(IRCSession *session, const IRCEntity &filter, std::vector<CL_TempString> &args)
{

}
