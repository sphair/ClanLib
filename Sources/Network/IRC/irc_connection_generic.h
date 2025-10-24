/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#ifndef header_irc_connection_generic
#define header_irc_connection_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Network/Socket/socket.h"
#include "API/Network/IRC/irc_connection.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/keep_alive.h"
#include "API/Core/System/mutex.h"
#include "API/Core/System/event_trigger.h"

class CL_IRCConnection_Generic : private CL_Runnable, private CL_KeepAlive
{
//! Construction:
public:
	CL_IRCConnection_Generic(const std::string &server, const std::string &port);

	~CL_IRCConnection_Generic();

//! Attributes:
public:
	//: sig_socket_error(std::string error_message)
	CL_Signal_v1<const std::string &> sig_socket_error;

	//: sig_command_received(prefix, command, params)
	CL_Signal_v3<const std::string &, const std::string &, const std::vector<std::string> &> sig_command_received;

	//: sig_unknown_command_received(prefix, command, params)
	CL_Signal_v3<const std::string &, const std::string &, const std::vector<std::string> &> sig_unknown_command_received;

	//: sig_numeric_reply(prefix, code, params)
	CL_Signal_v3<const std::string &, int, const std::vector<std::string> &> sig_numeric_reply;
	
	//: sig_name_reply(self, channel, users)
	CL_Signal_v3<const std::string &, const std::string &, const std::vector<std::string> &> sig_name_reply;

	//: sig_nick(old_nick, new_nick)
	CL_Signal_v2<const std::string &, const std::string &> sig_nick;

	//: sig_join(nick, channel)
	CL_Signal_v2<const std::string &, const std::string &> sig_join;
	
	//: sig_part(nick, channel, reason)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> sig_part;
	
	//: sig_mode(prefix, receiver, mode, params)
	CL_Signal_v4<const std::string &, const std::string &, const std::string &, const std::vector<std::string> &> sig_mode;

	//: sig_topic(prefix, channel, topic)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> sig_topic;
	
	//: sig_invite(prefix, nick, channel)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> sig_invite;

	//: sig_kick(prefix, chan, user, comment)
	CL_Signal_v4<const std::string &, const std::string &, const std::string &, const std::string &> sig_kick;

	//: sig_privmsg(prefix, receiver, text)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> sig_privmsg;

	//: sig_notice(prefix, receiver, text)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> sig_notice;

	//: sig_privmsg_ctcp(prefix, receiver, command, data)
	CL_Signal_v4<const std::string &, const std::string &, const std::string &, const std::string &> sig_privmsg_ctcp;

	//: sig_notice_ctcp(prefix, receiver, command, data)
	CL_Signal_v4<const std::string &, const std::string &, const std::string &, const std::string &> sig_notice_ctcp;

	//: sig_ping(daemon1, daemon2)
	CL_Signal_v2<const std::string &, const std::string &> sig_ping;

	CL_Thread thread;
	
	CL_Mutex mutex;
	
	CL_Socket sock;

	std::queue<std::string> send_queue;

	std::queue<std::string> received_queue;

	std::string nick, username, hostname, servername, realname;
	
	CL_EventTrigger quit_trigger, send_trigger;

//! Operations:
public:
	void add_ref();

	void release_ref();
	
	void send_data(const std::string &data);

//! Implementation:
private:
	virtual void run();
	
	virtual void keep_alive();
	
	std::string server, port;
	
	bool signal_error;
	
	std::string error_message;
	
	int ref_count;
};

#endif
