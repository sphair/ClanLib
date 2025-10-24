/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#include "Network/precomp.h"
#include "API/Network/IRC/irc_connection.h"
#include "irc_connection_generic.h"

#ifndef WIN32
#include "stdlib.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection construction:

CL_IRCConnection::CL_IRCConnection() : impl(0)
{
}

CL_IRCConnection::CL_IRCConnection(const CL_String &server, const CL_String &port) :
	impl(new CL_IRCConnection_Generic(server, port))
{
	impl->add_ref();
}

CL_IRCConnection::CL_IRCConnection(const CL_IRCConnection &copy) : impl(copy.impl)
{
	if (impl) impl->add_ref();
}

CL_IRCConnection::~CL_IRCConnection()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection attributes:

const CL_String8 &CL_IRCConnection::get_nick() const
{
	return impl->nick;
}

const CL_String8 &CL_IRCConnection::get_username() const
{
	return impl->username;
}

const CL_String8 &CL_IRCConnection::get_hostname() const
{
	return impl->hostname;
}

const CL_String8 &CL_IRCConnection::get_servername() const
{
	return impl->servername;
}

const CL_String8 &CL_IRCConnection::get_realname() const
{
	return impl->realname;
}

/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection operations:

CL_IRCConnection &CL_IRCConnection::operator =(const CL_IRCConnection &copy)
{
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();
	return *this;
}

void CL_IRCConnection::process_data()
{
	impl->process_data();
}

void CL_IRCConnection::send_command(const CL_String8 &prefix, const CL_String8 &command, const std::vector<CL_String8> &params)
{
	CL_String8 line;
	if (!prefix.empty())
	{
		line.append(":");
		line.append(prefix);
		line.append(" ");
	}
	
	line.append(command);
	
	int size = params.size();
	for (int i=0; i<size;i++)
	{
		line.append(" ");
		if (i == size-1) line.append(":");
		line.append(params[i]);
	}

	// Add low level escapes:
	CL_String8::size_type pos;

	pos = 0;
	while (pos != CL_String8::npos)
	{
		pos = line.find('\020', pos);
		if (pos == CL_String8::npos) break;
		line.replace( pos, 1, "\020\020" );
		pos++;
	}

	pos = 0;
	while (pos != CL_String8::npos)
	{
		pos = line.find('\0', pos);
		if (pos == CL_String8::npos) break;
		line.replace( pos, 1, "\020""0" );
		pos++;
	}

	pos = 0;
	while (pos != CL_String8::npos)
	{
		pos = line.find('\r', pos);
		if (pos == CL_String8::npos) break;
		line.replace( pos, 1, "\020r" );
		pos++;
	}

	pos = 0;
	while (pos != CL_String8::npos)
	{
		pos = line.find('\n', pos);
		if (pos == CL_String8::npos) break;
		line.replace( pos, 1, "\020n" );
		pos++;
	}

	line.append("\r\n");
	impl->send_data(line);
}

void CL_IRCConnection::send_pass(const CL_String8 &password)
{
	std::vector<CL_String8> params;
	params.push_back(password);
	send_command("", "PASS", params);
}

void CL_IRCConnection::send_nick(const CL_String8 &nick, int hopcount)
{
	std::vector<CL_String8> params;
	params.push_back(nick);
	// if (hopcount > 0) params.push_back(CL_String::from_int(hopcount));
	send_command("", "NICK", params);

	impl->nick = nick;
}

void CL_IRCConnection::send_user(const CL_String8 &username, const CL_String8 &hostname, const CL_String8 &servername, const CL_String8 &realname)
{
	std::vector<CL_String8> params;
	params.push_back(username);
	params.push_back(hostname);
	params.push_back(servername);
	params.push_back(realname);
	send_command("", "USER", params);

	impl->username = username;
	impl->hostname = hostname;
	impl->servername = servername;
	impl->realname = realname;
}

void CL_IRCConnection::send_oper(const CL_String8 &user, const CL_String8 &password)
{
	std::vector<CL_String8> params;
	params.push_back(user);
	params.push_back(password);
	send_command("", "OPER", params);
}

void CL_IRCConnection::send_quit(const CL_String8 &quitmessage)
{
	std::vector<CL_String8> params;
	if (!quitmessage.empty()) params.push_back(quitmessage);
	send_command("", "QUIT", params);
}

void CL_IRCConnection::send_join(const CL_String8 &channel, const CL_String8 &key)
{
	std::list<CL_String8> channels;
	std::list<CL_String8> keys;
	
	channels.push_back(channel);
	keys.push_back(key);
	
	send_join(channels, keys);
}

void CL_IRCConnection::send_join(const std::list<CL_String8> &channels, const std::list<CL_String8> &keys)
{
	std::list<CL_String8>::const_iterator it, it_keys;
	CL_String8 channel_list, key_list;
	bool first = true;
	
	// Add channels with keys first. Limitation in IRC protocol.
	for (it = channels.begin(), it_keys = keys.begin(); it != channels.end() && it_keys != keys.end(); ++it, ++it_keys)
	{
		if (it_keys->empty()) continue;
		
		if (!first)
		{
			channel_list.append(",");
			key_list.append(",");
		}
		first = false;
		channel_list.append(*it);
		key_list.append(*it_keys);
	}

	// Add channels without keys.
	for (it = channels.begin(), it_keys = keys.begin(); it != channels.end() && it_keys != keys.end(); ++it, ++it_keys)
	{
		if (!it_keys->empty()) continue;
		
		if (!first)
		{
			channel_list.append(",");
		}
		first = false;
		channel_list.append(*it);
	}

	std::vector<CL_String8> params;
	params.push_back(channel_list);
	if (!key_list.empty()) params.push_back(key_list);
	send_command("", "JOIN", params);
}

void CL_IRCConnection::send_part(const CL_String8 &channel, const CL_String8 &reason)
{
	std::list<CL_String8> channels;
	channels.push_back(channel);
	send_part(channels, reason);
}

void CL_IRCConnection::send_part(const std::list<CL_String8> &channels, const CL_String8 &reason)
{
	std::list<CL_String8>::const_iterator it;
	CL_String8 channel_list;
	bool first = true;

	for (it = channels.begin(); it != channels.end(); ++it)
	{
		if (!first) channel_list.append(",");
		first = false;
		channel_list.append(*it);
	}

	std::vector<CL_String8> params;
	params.push_back(channel_list);
	if (!reason.empty()) params.push_back(reason);
	send_command("", "PART", params);
}

void CL_IRCConnection::send_mode(const CL_String8 &channel_or_nick, const CL_String8 &mode, const std::vector<CL_String8> &args)
{
	std::vector<CL_String8> params;
	params.push_back(channel_or_nick);
	params.push_back(mode);
	int size = args.size();
	for (int i=0; i<size; i++) params.push_back(args[i]);
	send_command("", "MODE", params);
}

void CL_IRCConnection::send_topic(const CL_String8 &channel, const CL_String8 &topic)
{
	std::vector<CL_String8> params;
	params.push_back(channel);
	params.push_back(topic);
	send_command("", "TOPIC", params);
}

void CL_IRCConnection::send_topic(const CL_String8 &channel)
{
	std::vector<CL_String8> params;
	params.push_back(channel);
	send_command("", "TOPIC", params);
}

void CL_IRCConnection::send_names(const CL_String8 &channel)
{
	std::list<CL_String8> channels;
	channels.push_back(channel);
	send_names(channels);
}

void CL_IRCConnection::send_names(const std::list<CL_String8> &channels)
{
	std::list<CL_String8>::const_iterator it;
	CL_String8 channel_list;
	bool first = true;

	for (it = channels.begin(); it != channels.end(); ++it)
	{
		if (!first) channel_list.append(",");
		first = false;
		channel_list.append(*it);
	}

	std::vector<CL_String8> params;
	params.push_back(channel_list);
	send_command("", "NAMES", params);
}

void CL_IRCConnection::send_list(const CL_String8 &channel, const CL_String8 &server)
{
	std::list<CL_String8> channels;
	channels.push_back(channel);
	send_list(channels, server);
}

void CL_IRCConnection::send_list(const std::list<CL_String8> &channels, const CL_String8 &server)
{
	std::list<CL_String8>::const_iterator it;
	CL_String8 channel_list;
	bool first = true;

	for (it = channels.begin(); it != channels.end(); ++it)
	{
		if (!first) channel_list.append(",");
		first = false;
		channel_list.append(*it);
	}

	std::vector<CL_String8> params;
	params.push_back(channel_list);
	if (!server.empty()) params.push_back(server);
	send_command("", "LIST", params);
}

void CL_IRCConnection::send_invite(const CL_String8 &nickname, const CL_String8 &channel)
{
	std::vector<CL_String8> params;
	params.push_back(nickname);
	params.push_back(channel);
	send_command("", "INVITE", params);
}

void CL_IRCConnection::send_kick(const CL_String8 &channel, const CL_String8 &user, const CL_String8 &comment)
{
	std::list<CL_String8> channels, users;
	channels.push_back(channel);
	users.push_back(user);
	send_kick(channels, users, comment);
}

void CL_IRCConnection::send_kick(const std::list<CL_String8> &channels, const std::list<CL_String8> &users, const CL_String8 &comment)
{
	std::list<CL_String8>::const_iterator it;
	CL_String8 channel_list, user_list;
	bool first = true;

	for (it = channels.begin(); it != channels.end(); ++it)
	{
		if (!first) channel_list.append(",");
		first = false;
		channel_list.append(*it);
	}
	
	first = true;
	for (it = users.begin(); it != users.end(); ++it)
	{
		if (!first) user_list.append(",");
		first = false;
		user_list.append(*it);
	}

	std::vector<CL_String8> params;
	params.push_back(channel_list);
	params.push_back(user_list);
	if (!comment.empty()) params.push_back(comment);
	send_command("", "KICK", params);
}

void CL_IRCConnection::send_version(const CL_String8 &server)
{
	std::vector<CL_String8> params;
	if (!server.empty()) params.push_back(server);
	send_command("", "VERSION", params);
}

void CL_IRCConnection::send_stats(const CL_String8 &query, const CL_String8 &server)
{
	std::vector<CL_String8> params;
	if (!query.empty()) params.push_back(query);
	if (!server.empty()) params.push_back(server);
	send_command("", "STATS", params);
}

void CL_IRCConnection::send_links(const CL_String8 &remote_server, const CL_String8 &server_mask)
{
	std::vector<CL_String8> params;
	if (!remote_server.empty()) params.push_back(remote_server);
	if (!server_mask.empty()) params.push_back(server_mask);
	send_command("", "LINKS", params);
}

void CL_IRCConnection::send_time(const CL_String8 &server)
{
	std::vector<CL_String8> params;
	if (!server.empty()) params.push_back(server);
	send_command("", "TIME", params);
}

void CL_IRCConnection::send_connect(const CL_String8 &target_server, int port, const CL_String8 &remote_server)
{
	std::vector<CL_String8> params;
	params.push_back(target_server);
	// if (port) params.push_back(CL_String::from_int(port));
	if (!remote_server.empty()) params.push_back(remote_server);
	send_command("", "CONNECT", params);
}

void CL_IRCConnection::send_trace(const CL_String8 &server)
{
	std::vector<CL_String8> params;
	if (!server.empty()) params.push_back(server);
	send_command("", "TRACE", params);
}

void CL_IRCConnection::send_admin(const CL_String8 &server)
{
	std::vector<CL_String8> params;
	if (!server.empty()) params.push_back(server);
	send_command("", "ADMIN", params);
}

void CL_IRCConnection::send_info(const CL_String8 &server)
{
	std::vector<CL_String8> params;
	if (!server.empty()) params.push_back(server);
	send_command("", "INFO", params);
}

void CL_IRCConnection::send_privmsg(const CL_String8 &receiver, const CL_String8 &text)
{
	std::list<CL_String8> receivers;
	receivers.push_back(receiver);
	send_privmsg(receivers, text);
}

void CL_IRCConnection::send_privmsg(const std::list<CL_String8> &receivers, const CL_String8 &text)
{
	std::list<CL_String8>::const_iterator it;
	CL_String8 receiver_list;
	bool first = true;

	for (it = receivers.begin(); it != receivers.end(); ++it)
	{
		if (!first) receiver_list.append(",");
		first = false;
		receiver_list.append(*it);
	}

	std::vector<CL_String8> params;
	params.push_back(receiver_list);
	params.push_back(text);
	send_command("", "PRIVMSG", params);
}

void CL_IRCConnection::send_privmsg_ctcp(const CL_String8 &receiver, const CL_String8 &command, const CL_String8 &data)
{
	std::list<CL_String8> receivers;
	receivers.push_back(receiver);
	send_privmsg_ctcp(receivers, command, data);
}

void CL_IRCConnection::send_privmsg_ctcp(std::list<CL_String8> &receivers, const CL_String8 &command, const CL_String8 &data)
{
	CL_String8 line;
	if (data.empty())
	{
		line = command;
	}
	else
	{
		line = command + CL_String8(" ") + data;
	}

	// Add CTCP escapes:
	CL_String8::size_type pos;

	pos = 0;
	while (pos != CL_String8::npos)
	{
		pos = line.find('\\', pos);
		if (pos == CL_String8::npos) break;
		line.replace( pos, 1, "\\\\" );
		pos++;
	}

	pos = 0;
	while (pos != CL_String8::npos)
	{
		pos = line.find('\001', pos);
		if (pos == CL_String8::npos) break;
		line.replace( pos, 1, "\\a" );
		pos++;
	}

	CL_String8 ctcp = CL_String8("\001") + line + CL_String8("\001");

	std::list<CL_String8>::const_iterator it;
	CL_String8 receiver_list;
	bool first = true;

	for (it = receivers.begin(); it != receivers.end(); ++it)
	{
		if (!first) receiver_list.append(",");
		first = false;
		receiver_list.append(*it);
	}

	std::vector<CL_String8> params;
	params.push_back(receiver_list);
	params.push_back(ctcp);
	send_command("", "PRIVMSG", params);
}

void CL_IRCConnection::send_notice(const CL_String8 &nickname, const CL_String8 &text)
{
	std::vector<CL_String8> params;
	params.push_back(nickname);
	params.push_back(text);
	send_command("", "NOTICE", params);
}

void CL_IRCConnection::send_notice_ctcp(const CL_String8 &nickname, const CL_String8 &command, const CL_String8 &data)
{
	CL_String8 line;
	if (data.empty())
	{
		line = command;
	}
	else
	{
		line = command + CL_String8(" ") + data;
	}

	// Add CTCP escapes:
	CL_String8::size_type pos;

	pos = 0;
	while (pos != CL_String8::npos)
	{
		pos = line.find('\\', pos);
		if (pos == CL_String8::npos) break;
		line.replace( pos, 1, "\\\\" );
		pos++;
	}

	pos = 0;
	while (pos != CL_String8::npos)
	{
		pos = line.find('\001', pos);
		if (pos == CL_String8::npos) break;
		line.replace( pos, 1, "\\a" );
		pos++;
	}

	CL_String8 ctcp = CL_String8("\001") + line + CL_String8("\001");

	std::vector<CL_String8> params;
	params.push_back(nickname);
	params.push_back(ctcp);
	send_command("", "NOTICE", params);
}

void CL_IRCConnection::send_who(const CL_String8 &name, bool oper)
{
	std::vector<CL_String8> params;
	params.push_back(name);
	if (oper) params.push_back("o");
	send_command("", "WHO", params);
}

void CL_IRCConnection::send_whois(const CL_String8 &nickmask)
{
	std::vector<CL_String8> params;
	params.push_back(nickmask);
	send_command("", "WHOIS", params);
}

void CL_IRCConnection::send_whois(const CL_String8 &server, const CL_String8 &nickmask)
{
	std::vector<CL_String8> params;
	params.push_back(server);
	params.push_back(nickmask);
	send_command("", "WHOIS", params);
}

void CL_IRCConnection::send_whois(const std::list<CL_String8> &nickmasks)
{
	send_whois("", nickmasks);
}

void CL_IRCConnection::send_whois(const CL_String8 &server, const std::list<CL_String8> &nickmasks)
{
	std::list<CL_String8>::const_iterator it;
	CL_String8 nickmask_list;
	bool first = true;

	for (it = nickmasks.begin(); it != nickmasks.end(); ++it)
	{
		if (!first) nickmask_list.append(",");
		first = false;
		nickmask_list.append(*it);
	}

	std::vector<CL_String8> params;
	if (!server.empty()) params.push_back(server);
	params.push_back(nickmask_list);
	send_command("", "WHOIS", params);
}

void CL_IRCConnection::send_whowas(const CL_String8 &nickname, int count, const CL_String8 &server)
{
	std::vector<CL_String8> params;
	params.push_back(nickname);
	// if (count > 0) params.push_back(CL_String::from_int(count));
	if (!server.empty()) params.push_back(server);
	send_command("", "WHOWAS", params);
}

void CL_IRCConnection::send_kill(const CL_String8 &nickname, const CL_String8 &comment)
{
	std::vector<CL_String8> params;
	params.push_back(nickname);
	if (!comment.empty()) params.push_back(comment);
	send_command("", "KILL", params);
}

void CL_IRCConnection::send_ping(const CL_String8 &server, const CL_String8 &server2)
{
	std::vector<CL_String8> params;
	params.push_back(server);
	if (!server2.empty()) params.push_back(server2);
	send_command("", "PING", params);
}

void CL_IRCConnection::send_pong(const CL_String8 &daemon, const CL_String8 &daemon2)
{
	std::vector<CL_String8> params;
	params.push_back(daemon);
	if (!daemon2.empty()) params.push_back(daemon2);
	send_command("", "PONG", params);
}

CL_String8 CL_IRCConnection::extract_nick(const CL_String8 &str)
{
	CL_String8::size_type pos = str.find_first_of('!');
	if (pos == CL_String8::npos) return str;
	return str.substr(0, pos);
}

CL_String8 CL_IRCConnection::extract_user(const CL_String8 &str)
{
	CL_String8::size_type pos = str.find_first_of('!');
	if (pos == CL_String8::npos) return CL_String8();

	CL_String8::size_type pos_at = str.find_first_of('@', pos);
	if (pos_at == CL_String8::npos) return str.substr(pos+1);

	return str.substr(pos+1, pos_at-pos-1);
}

CL_String8 CL_IRCConnection::extract_address(const CL_String8 &str)
{
	CL_String8::size_type pos = str.find_first_of('!');
	if (pos == CL_String8::npos) return CL_String8();

	CL_String8::size_type pos_at = str.find_first_of('@', pos);
	if (pos_at == CL_String8::npos) return CL_String8();

	return str.substr(pos_at+1);
}

/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection signals:

CL_Signal_v1<const CL_String &> &CL_IRCConnection::sig_socket_error()
{
	return impl->sig_socket_error;
}

CL_Signal_v3<const CL_String8 &, const CL_String8 &, const std::vector<CL_String8> &> &CL_IRCConnection::sig_command_received()
{
	return impl->sig_command_received;
}

CL_Signal_v3<const CL_String8 &, const CL_String8 &, const std::vector<CL_String8> &> &CL_IRCConnection::sig_unknown_command_received()
{
	return impl->sig_unknown_command_received;
}

CL_Signal_v3<const CL_String8 &, int, const std::vector<CL_String8> &> &CL_IRCConnection::sig_numeric_reply()
{
	return impl->sig_numeric_reply;
}
	
CL_Signal_v3<const CL_String8 &, const CL_String8 &, const std::vector<CL_String8> &> &CL_IRCConnection::sig_name_reply()
{
	return impl->sig_name_reply;
}

CL_Signal_v2<const CL_String8 &, const CL_String8 &> &CL_IRCConnection::sig_nick()
{
	return impl->sig_nick;
}

CL_Signal_v2<const CL_String8 &, const CL_String8 &> &CL_IRCConnection::sig_join()
{
	return impl->sig_join;
}
	
CL_Signal_v3<const CL_String8 &, const CL_String8 &, const CL_String8 &> &CL_IRCConnection::sig_part()
{
	return impl->sig_part;
}
	
CL_Signal_v4<const CL_String8 &, const CL_String8 &, const CL_String8 &, const std::vector<CL_String8> &> &CL_IRCConnection::sig_mode()
{
	return impl->sig_mode;
}

CL_Signal_v3<const CL_String8 &, const CL_String8 &, const CL_String8 &> &CL_IRCConnection::sig_topic()
{
	return impl->sig_topic;
}

CL_Signal_v3<const CL_String8 &, const CL_String8 &, const CL_String8 &> &CL_IRCConnection::sig_invite()
{
	return impl->sig_invite;
}

CL_Signal_v4<const CL_String8 &, const CL_String8 &, const CL_String8 &, const CL_String8 &> &CL_IRCConnection::sig_kick()
{
	return impl->sig_kick;
}

CL_Signal_v3<const CL_String8 &, const CL_String8 &, const CL_String8 &> &CL_IRCConnection::sig_privmsg()
{
	return impl->sig_privmsg;
}

CL_Signal_v3<const CL_String8 &, const CL_String8 &, const CL_String8 &> &CL_IRCConnection::sig_notice()
{
	return impl->sig_notice;
}
	
CL_Signal_v4<const CL_String8 &, const CL_String8 &, const CL_String8 &, const CL_String8 &> &CL_IRCConnection::sig_privmsg_ctcp()
{
	return impl->sig_privmsg_ctcp;
}

CL_Signal_v4<const CL_String8 &, const CL_String8 &, const CL_String8 &, const CL_String8 &> &CL_IRCConnection::sig_notice_ctcp()
{
	return impl->sig_notice_ctcp;
}
	
CL_Signal_v2<const CL_String8 &, const CL_String8 &> &CL_IRCConnection::sig_ping()
{
	return impl->sig_ping;
}

/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection implementation:

