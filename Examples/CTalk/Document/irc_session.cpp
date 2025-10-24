
#include "precomp.h"
#include "irc_session.h"
#include "command.h"
#include <algorithm>

IRCSession::IRCSession()
: connect_status(status_disconnected)
{
	connection.func_message_received().set(this, &IRCSession::on_message_received);
	connection.func_disconnected().set(this, &IRCSession::on_disconnected);
}

IRCSession::~IRCSession()
{
	connection.send_quit(IRCText());
	disconnect_graceful();
}

void IRCSession::set_perform_list(const std::vector<CL_String> &new_perform_list)
{
	perform_list = new_perform_list;
}

void IRCSession::set_topic(const IRCChannel &channel, const IRCText &text)
{
	connection.send_topic(channel, text);
}

void IRCSession::connect(const CL_String &server, const CL_String &port, const CL_String &nick, const CL_String &username, const CL_String &full_name)
{
	set_connect_status(status_connecting);
	connection.connect(CL_SocketName(server, port));
	connection.send_nick(IRCNick::from_text(nick));
	connection.send_user(CL_StringHelp::text_to_utf8(username), "localhost", CL_StringHelp::text_to_utf8(server), CL_StringHelp::text_to_utf8(full_name));
	our_nickname = IRCNick::from_text(nick);
}

void IRCSession::disconnect_graceful()
{
	connection.disconnect_graceful();
}

void IRCSession::disconnect_abortive()
{
	connection.disconnect_abortive();
}

void IRCSession::set_connect_status(ConnectStatus new_status)
{
	connect_status = new_status;
	cb_connect_status_changed.invoke(new_status);
}

void IRCSession::set_connection_name(const CL_String &name)
{
	connection_name = name;
}

void IRCSession::set_nick(const CL_String &nick)
{
	connection.send_nick(IRCNick::from_text(nick));
	if (connect_status == status_connecting)
		our_nickname = IRCNick::from_text(nick);
}

IRCNick IRCSession::get_nick() const
{
	return our_nickname;
}

CL_String IRCSession::get_out_hostname() const
{
	return our_hostname;
}

void IRCSession::join(const IRCChannel &channel)
{
	connection.send_join(channel);
}

void IRCSession::part(const IRCChannel &channel)
{
	connection.send_part(channel);
}

IRCJoinedChannel IRCSession::get_channel_status(const IRCChannel &channel)
{
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i].name == channel)
			return channels[i];
	}
	IRCJoinedChannel status;
	status.name = channel;
	return status;
}

void IRCSession::set_channel_status(const IRCJoinedChannel &status)
{
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i].name == status.name)
		{
			channels[i] = status;
			return;
		}
	}
	channels.push_back(status);
}

void IRCSession::send_text(const IRCEntity &target, const IRCText &text)
{
	connection.send_privmsg(target, text);
}

void IRCSession::send_notice(const IRCEntity &target, const IRCText &text)
{
	connection.send_notice(target, text);
}

void IRCSession::on_message_received(const IRCMessage &message)
{
	IRCRawString s = IRCMessage::create_line(message.get_prefix().to_raw(), message.get_command(), message.get_params());
	cb_system_text.invoke(IRCText::from_raw(s.substr(0, s.length()-2)));

	switch (message.get_type())
	{
	case IRCMessage::type_numeric_reply: on_numeric_reply(IRCNumericReply(message)); break;
	case IRCMessage::type_nick: on_nick(IRCNickMessage(message)); break;
	case IRCMessage::type_join: on_join(IRCJoinMessage(message)); break;
	case IRCMessage::type_part: on_part(IRCPartMessage(message)); break;
	case IRCMessage::type_quit: on_quit(IRCQuitMessage(message)); break;
	case IRCMessage::type_channel_mode: on_channel_mode(IRCChannelModeMessage(message)); break;
	case IRCMessage::type_nick_mode: on_nick_mode(IRCNickModeMessage(message)); break;
	case IRCMessage::type_topic: on_topic(IRCTopicMessage(message)); break;
	case IRCMessage::type_privmsg: on_privmsg(IRCPrivateMessage(message)); break;
	case IRCMessage::type_notice: on_notice(IRCNoticeMessage(message)); break;
	case IRCMessage::type_ping: on_ping(IRCPingMessage(message)); break;
	case IRCMessage::type_unknown:
	default: on_unknown_message(message); break;
	}
}

void IRCSession::on_numeric_reply(const IRCNumericReply &message)
{
	switch (message.get_numeric())
	{
	case RPL_WELCOME: on_rpl_welcome(message); break;
	case RPL_USERHOST: on_rpl_userhost(message); break;
	case RPL_NAMREPLY: on_rpl_namereply(message); break;
	case RPL_ENDOFNAMES: on_rpl_endofnames(message); break;
	case RPL_TOPIC: on_rpl_topic(message); break;
	case RPL_TOPICWHOTIME: on_rpl_topicwhotime(message); break;
	case RPL_NOTOPIC: on_rpl_notopic(message); break;
	case ERR_NICKNAMEINUSE: on_err_nicknameinuse(message); break;
	default: break;
	}
}

void IRCSession::on_rpl_welcome(const IRCNumericReply &message)
{
	std::vector<IRCRawString> params;
	params.push_back(our_nickname.to_raw());
	connection.send_command("USERHOST", params);
	set_connect_status(status_connected);

	for (size_t i = 0; i < perform_list.size(); i++)
	{
		try
		{
			Command::execute(this, IRCEntity(), perform_list[i]);
		}
		catch (CL_Exception &e)
		{
			cb_system_text.invoke(IRCText::from_text(e.message));
			break;
		}
	}
}

void IRCSession::on_rpl_userhost(const IRCNumericReply &message)
{
	if (our_hostname.empty() && message.get_param_count() >= 2)
	{
		IRCRawString param1 = message.get_param(1);
		IRCRawString::size_type pos = param1.find("=+");
		if (pos == IRCRawString::npos) pos = param1.find("=-");
		if (pos != IRCRawString::npos)
		{
			our_hostname = CL_StringHelp::utf8_to_text(param1.substr(pos+2));
			pos = our_hostname.find(cl_text("@"));
			if (pos != IRCRawString::npos)
				our_hostname = CL_StringHelp::trim(our_hostname.substr(pos+1));
		}
	}
}

void IRCSession::on_rpl_namereply(const IRCNumericReply &message)
{
	if (message.get_param_count() >= 4)
	{
		IRCChannel channel = IRCChannel::from_raw(message.get_param(2));
		CL_String nicks = CL_StringHelp::utf8_to_text(message.get_param(3));
		std::vector<CL_TempString> nick_list = CL_StringHelp::split_text(nicks, " ");

		IRCJoinedChannel status = get_channel_status(channel);

		for (unsigned int i=0; i<nick_list.size(); i++)
		{
			IRCNick nick = IRCNick::from_raw(CL_StringHelp::text_to_utf8(nick_list[i]));
			status.users.push_back(nick);
		}

		set_channel_status(status);
		cb_channel_names_updated.invoke(channel);
	}
}

void IRCSession::on_rpl_endofnames(const IRCNumericReply &message)
{
//	cb_channel_names_updated.invoke(channel);
}

void IRCSession::on_rpl_topic(const IRCNumericReply &message)
{
	if (message.get_param_count() >= 3)
	{
		IRCNick target = IRCNick::from_raw(message.get_param(0));
		IRCChannel channel = IRCChannel::from_raw(message.get_param(1));
		IRCText text = IRCText::from_raw(message.get_param(2));

		IRCJoinedChannel status = get_channel_status(channel);
		status.topic = text;
		status.topic_time = 0;
		status.topic_author = IRCNick();
		set_channel_status(status);
	}
}

void IRCSession::on_rpl_topicwhotime(const IRCNumericReply &message)
{
	if (message.get_param_count() >= 3)
	{
		IRCNick target = IRCNick::from_raw(message.get_param(0));
		IRCChannel channel = IRCChannel::from_raw(message.get_param(1));
		IRCNick author = IRCNick::from_raw(message.get_param(2));
		unsigned int timestamp = CL_StringHelp::local8_to_uint(message.get_param(3));

		IRCJoinedChannel status = get_channel_status(channel);
		status.topic_time = timestamp;
		status.topic_author = author;
		set_channel_status(status);
		cb_channel_topic_updated.invoke(channel);
	}
}

void IRCSession::on_rpl_notopic(const IRCNumericReply &message)
{
	if (message.get_param_count() >= 2)
	{
		IRCNick target = IRCNick::from_raw(message.get_param(0));
		IRCChannel channel = IRCChannel::from_raw(message.get_param(1));

		IRCJoinedChannel status = get_channel_status(channel);
		status.topic = IRCText();
		status.topic_time = 0;
		status.topic_author = IRCNick();
		set_channel_status(status);
	}
}

void IRCSession::on_err_nicknameinuse(const IRCNumericReply &message)
{
}

void IRCSession::on_nick(const IRCNickMessage &message)
{
	if (our_nickname == message.get_old_nick())
		our_nickname = message.get_new_nick();

	std::vector<IRCJoinedChannel>::iterator it;
	for(it = channels.begin(); it != channels.end(); ++it)
	{
		IRCJoinedChannel channel = (*it);
		std::vector<IRCNick>::iterator it = std::find(channel.users.begin(), channel.users.end(), message.get_old_nick());
		if(it != channel.users.end())
		{
			channel.users.erase(it);
			channel.users.push_back(message.get_new_nick());
			set_channel_status(channel);
		}
	}

	cb_nick_changed.invoke(message.get_old_nick(), message.get_new_nick());
}

void IRCSession::on_join(const IRCJoinMessage &message)
{
	if (message.get_user() == our_nickname)
	{
		cb_joined.invoke(message.get_channel());
	}
	else
	{
		IRCJoinedChannel channel = get_channel_status(message.get_channel());
		channel.name = message.get_channel();
		channel.users.push_back(message.get_user());
		set_channel_status(channel);
		cb_user_joined.invoke(message.get_channel(), message.get_user());
	}
}

void IRCSession::on_part(const IRCPartMessage &message)
{
	for (size_t i=0; i<message.get_channel_count(); i++)
	{
		if (message.get_user() == our_nickname)
			cb_parted.invoke(message.get_channel(i));
		else
		{
			IRCJoinedChannel channel = get_channel_status(message.get_channel(i));
			channel.name = message.get_channel(i);
			std::vector<IRCNick>::iterator it = std::find(channel.users.begin(), channel.users.end(), message.get_user());
			if(it != channel.users.end())
			{
				channel.users.erase(it);
				set_channel_status(channel);
			}

			cb_user_parted.invoke(message.get_channel(i), message.get_user(), IRCText());
		}
	}
}

void IRCSession::on_quit(const IRCQuitMessage &message)
{
	IRCText text;
	if (message.has_text())
		text = message.get_text();
	cb_user_quit.invoke(message.get_user(), text);
}

void IRCSession::on_channel_mode(const IRCChannelModeMessage &message)
{
	size_t num_params = message.get_mode_parameter_count();
	std::vector<IRCRawString> params;
	params.push_back(message.get_mode_command());
	for (size_t i = 0; i < num_params; i++)
		params.push_back(message.get_mode_parameter(i));
	cb_channel_mode_change.invoke(message.get_executing_user(), message.get_channel(), params);
}

void IRCSession::on_nick_mode(const IRCNickModeMessage &message)
{
	std::vector<IRCRawString> params;
	params.push_back(message.get_mode_command());
	cb_channel_mode_change.invoke(message.get_executing_user(), message.get_target_user(), params);
}

void IRCSession::on_topic(const IRCTopicMessage &message)
{
	IRCJoinedChannel status = get_channel_status(message.get_channel());
	status.topic = message.get_topic();
	status.topic_author = message.get_prefix();
	set_channel_status(status);
	cb_channel_topic_updated.invoke(message.get_channel());
}

void IRCSession::on_privmsg(const IRCPrivateMessage &message)
{
	if (message.get_text().is_ctcp())
	{
		IRCRawString command, data;
		extract_ctcp_command(IRCText::from_raw(message.get_text().get_ctcp_data()), command, data);
		on_ctcp_privmsg(command, IRCText::from_raw(data), message);
	}
	else
	{
		if (message.get_target().is_channel())
			cb_channel_text.invoke(message.get_target(), message.get_sender(), message.get_text());
		else
			cb_private_text.invoke(message.get_sender(), message.get_text());
	}
}

void IRCSession::on_notice(const IRCNoticeMessage &message)
{
	if (message.get_text().is_ctcp())
	{
		IRCRawString command, data;
		extract_ctcp_command(IRCText::from_raw(message.get_text().get_ctcp_data()), command, data);
		on_ctcp_notice(command, IRCText::from_raw(data), message);
	}
	else
	{
		if (message.get_target().is_channel())
			cb_channel_notice.invoke(message.get_target(), message.get_sender(), message.get_text());
		else
			cb_private_notice.invoke(message.get_sender(), message.get_text());
	}
}

void IRCSession::on_ping(const IRCPingMessage &message)
{
	if (!message.has_daemon2())
		connection.send_pong(message.get_daemon1());
	else
		connection.send_pong(message.get_daemon1(), message.get_daemon2());
}

void IRCSession::on_unknown_message(const IRCMessage &message)
{
}

void IRCSession::on_disconnected(const CL_String &reason)
{
	set_connect_status(status_disconnected);
	cb_error_text.invoke(IRCText::from_text(reason.empty() ? reason : cl_format(L"Disconnected (%1)", reason)));
}

void IRCSession::execute_command(const IRCEntity &filter, CL_String command_line)
{
	Command::execute(this, filter, command_line);
}

void IRCSession::extract_ctcp_command(const IRCText &ctcp_data, IRCRawString &command, IRCRawString &data)
{
	command = ctcp_data.to_raw();
	data = ctcp_data.to_raw();
	IRCRawString::size_type pos = command.find(" ");
	if (pos != IRCRawString::npos)
	{
		command = command.substr(0, pos);
		data = data.substr(pos+1);
	}
	else
	{
		data.clear();
	}
}

void IRCSession::on_ctcp_privmsg(const IRCRawString &command, const IRCText &data, const IRCPrivateMessage &message)
{
	if (CL_StringHelp::compare(command, "ACTION", true) == 0)
		on_ctcp_privmsg_action(data, message);
	else if (CL_StringHelp::compare(command, "DCC", true) == 0)
		on_ctcp_privmsg_dcc(data, message);
	else if (CL_StringHelp::compare(command, "FINGER", true) == 0)
		on_ctcp_privmsg_finger(data, message);
	else if (CL_StringHelp::compare(command, "VERSION", true) == 0)
		on_ctcp_privmsg_version(data, message);
	else if (CL_StringHelp::compare(command, "USERINFO", true) == 0)
		on_ctcp_privmsg_userinfo(data, message);
	else if (CL_StringHelp::compare(command, "CLIENTINFO", true) == 0)
		on_ctcp_privmsg_clientinfo(data, message);
	else if (CL_StringHelp::compare(command, "PING", true) == 0)
		on_ctcp_privmsg_ping(data, message);
	else if (CL_StringHelp::compare(command, "TIME", true) == 0)
		on_ctcp_privmsg_time(data, message);
}

void IRCSession::on_ctcp_notice(const IRCRawString &command, const IRCText &data, const IRCNoticeMessage &message)
{
	if (CL_StringHelp::compare(command, "FINGER", true) == 0)
		on_ctcp_notice_finger(data, message);
	else if (CL_StringHelp::compare(command, "VERSION", true) == 0)
		on_ctcp_notice_version(data, message);
	else if (CL_StringHelp::compare(command, "USERINFO", true) == 0)
		on_ctcp_notice_userinfo(data, message);
	else if (CL_StringHelp::compare(command, "CLIENTINFO", true) == 0)
		on_ctcp_notice_clientinfo(data, message);
	else if (CL_StringHelp::compare(command, "PING", true) == 0)
		on_ctcp_notice_ping(data, message);
	else if (CL_StringHelp::compare(command, "TIME", true) == 0)
		on_ctcp_notice_time(data, message);
}

void IRCSession::on_ctcp_privmsg_action(const IRCText &data, const IRCPrivateMessage &message)
{
	if (message.get_target().is_channel())
		cb_channel_action.invoke(message.get_target(), message.get_prefix(), data);
	else
		cb_private_action.invoke(message.get_prefix(), data);
}

void IRCSession::on_ctcp_privmsg_dcc(const IRCText &data, const IRCPrivateMessage &message)
{
/*
	add_line(cl_format(L"%1 DCC line: %2", CL_IRCConnection::extract_nick(nick), data), color_text);

	std::vector<CL_TempString> tokens = CL_StringHelp::split_text(data, " ");
	if (tokens.size() >= 4 && tokens[0] == "SEND")
	{
		CL_String filename = "dcc_received_file"; // tokens[1];
		unsigned int address = CL_StringHelp::text_to_int(tokens[2]);
		CL_String ip_address = cl_format(
			L"%1.%2.%3.%4",
			CL_StringHelp::int_to_text((address>>24)&0xff),
			CL_StringHelp::int_to_text((address>>16)&0xff),
			CL_StringHelp::int_to_text((address>>8)&0xff),
			CL_StringHelp::int_to_text(address&0xff));
		CL_String port = tokens[3];
		int size = 0;
		if (tokens.size() >= 5) size = CL_StringHelp::text_to_int(tokens[4]);

		try
		{
			CL_TCPConnection sock;
			sock.connect(CL_SocketName(ip_address, port));

			CL_File outputfile(filename, CL_File::create_always);
			char buffer[64*1024];
			while (true)
			{
				int data_read = sock.read(buffer, 64*1024);
				if (data_read == 0) break;
				outputfile.write(buffer, data_read);
				sock.write_int32(htonl(data_read));
			}
		}
		catch (CL_Exception& exception)
		{
			add_line(cl_format(L"%1 DCC failed: %2", CL_IRCConnection::extract_nick(nick), exception.message), color_text);
		}
	}
	else if (tokens.size() == 4 && tokens[0] == "CHAT" && tokens[1] == "chat")
	{
		// Todo: connect and establish dedicated chat connection.
	}
*/
}

void IRCSession::on_ctcp_privmsg_finger(const IRCText &data, const IRCPrivateMessage &message)
{
/*
	if (data.empty())
	{
		connection->send_privmsg_ctcp(nick, "FINGER", ":ClanLib's Example IRC Client");
	}
	else
	{
		add_line(cl_format(L"%1 FINGER REPLY: %2", CL_IRCConnection::extract_nick(nick), data.substr(1)), color_text);
	}
*/
}

void IRCSession::on_ctcp_privmsg_version(const IRCText &data, const IRCPrivateMessage &message)
{
#ifdef WIN32
	send_notice(message.get_prefix(), IRCText::from_ctcp_data("VERSION Carambola:1.0:Windows"));
#elif __APPLE__
	send_notice(message.get_prefix(), IRCText::from_ctcp_data("VERSION Carambola:1.0:OSX"));
#else
	send_notice(message.get_prefix(), IRCText::from_ctcp_data("VERSION Carambola:1.0:Unix"));
#endif
}

void IRCSession::on_ctcp_privmsg_userinfo(const IRCText &data, const IRCPrivateMessage &message)
{
/*
	if (data.empty())
	{
		connection->send_privmsg_ctcp(nick, "USERINFO", ":A ClanLib User");
	}
	else
	{
		add_line(cl_format(L"%1 USERINFO REPLY: %2", CL_IRCConnection::extract_nick(nick), data.substr(1)), color_text);
	}
*/
}

void IRCSession::on_ctcp_privmsg_clientinfo(const IRCText &data, const IRCPrivateMessage &message)
{
/*
	if (data.empty())
	{
		connection->send_privmsg_ctcp(nick, "CLIENTINFO", "ACTION DCC FINGER VERSION USERINFO CLIENTINFO PING TIME");
	}
	else
	{
		add_line(cl_format(L"%1 CLIENTINFO REPLY: %2", CL_IRCConnection::extract_nick(nick), data.substr(1)), color_text);
	}
*/
}

void IRCSession::on_ctcp_privmsg_ping(const IRCText &data, const IRCPrivateMessage &message)
{
//	connection->send_notice_ctcp(nick, "PING", data);
}

void IRCSession::on_ctcp_privmsg_time(const IRCText &data, const IRCPrivateMessage &message)
{
/*
	if (data.empty())
	{
		connection->send_privmsg_ctcp(nick, "TIME", "I havn't got the slightest clue what the time is!");
	}
	else
	{
		add_line(cl_format(L"%1 TIME REPLY: %2", CL_IRCConnection::extract_nick(nick), data.substr(1)), color_text);
	}
*/
}

void IRCSession::on_ctcp_notice_finger(const IRCText &data, const IRCNoticeMessage &message)
{

}

void IRCSession::on_ctcp_notice_version(const IRCText &data, const IRCNoticeMessage &message)
{
/*
	add_line(cl_format(L"%1 VERSION REPLY: %2", CL_IRCConnection::extract_nick(nick), data), color_text);
*/
}

void IRCSession::on_ctcp_notice_userinfo(const IRCText &data, const IRCNoticeMessage &message)
{

}

void IRCSession::on_ctcp_notice_clientinfo(const IRCText &data, const IRCNoticeMessage &message)
{

}

void IRCSession::on_ctcp_notice_ping(const IRCText &data, const IRCNoticeMessage &message)
{
//	add_line(cl_format(L"%1 PING: %2 ms", CL_IRCConnection::extract_nick(prefix), CL_StringHelp::int_to_text(CL_System::get_time()-CL_StringHelp::text_to_int(data))), color_text);
}

void IRCSession::on_ctcp_notice_time(const IRCText &data, const IRCNoticeMessage &message)
{

}
/*
void Chat::on_connection_mode(const IRCRawString &prefix, const IRCRawString &receiver, const IRCRawString &mode, const std::vector<IRCRawString> &params)
{
	if (matches_filter(receiver) || receiver.substr(0,1) != "#")
		add_line(cl_format(L"%1 sets mode %3 on %2", CL_IRCConnection::extract_nick(prefix), CL_IRCConnection::extract_nick(receiver), mode), color_channel);
}

void Chat::on_connection_invite(const IRCRawString &prefix, const IRCRawString &nick, const IRCRawString &channel)
{
	add_line(cl_format(L"%1 invites %2 to %3", CL_IRCConnection::extract_nick(prefix), CL_IRCConnection::extract_nick(nick), channel), color_channel);
}

void Chat::on_connection_kick(const IRCRawString &prefix, const IRCRawString &channel, const IRCRawString &user, const IRCRawString &comment)
{
	if (matches_filter(channel) || filter.empty())
	{
		add_line(cl_format(L"%1 kicked %3 from %2 (%4)", CL_IRCConnection::extract_nick(prefix), channel, CL_IRCConnection::extract_nick(user), comment), color_channel);
	}
}
*/
