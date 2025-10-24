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
**    (if your name is missing here, please add it)
*/

#pragma once


#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Network/Socket/tcp_connection.h"
#include "API/Network/IRC/irc_connection.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/mutex.h"
#include "API/Core/System/event.h"
#include "API/Core/Text/string_types.h"

class CL_IRCConnection_Generic
{
/// \name Construction
/// \{

public:
	CL_IRCConnection_Generic(const CL_String &server, const CL_String &port);

	~CL_IRCConnection_Generic();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief sig_socket_error(CL_String error_message)
	CL_Signal_v1<const CL_String &> sig_socket_error;

	/// \brief sig_command_received(prefix, command, params)
	CL_Signal_v3<const CL_String8 &, const CL_String8 &, const std::vector<CL_String8> &> sig_command_received;

	/// \brief sig_unknown_command_received(prefix, command, params)
	CL_Signal_v3<const CL_String8 &, const CL_String8 &, const std::vector<CL_String8> &> sig_unknown_command_received;

	/// \brief sig_numeric_reply(prefix, code, params)
	CL_Signal_v3<const CL_String8 &, int, const std::vector<CL_String8> &> sig_numeric_reply;

	/// \brief sig_name_reply(self, channel, users)
	CL_Signal_v3<const CL_String8 &, const CL_String8 &, const std::vector<CL_String8> &> sig_name_reply;

	/// \brief sig_nick(old_nick, new_nick)
	CL_Signal_v2<const CL_String8 &, const CL_String8 &> sig_nick;

	/// \brief sig_join(nick, channel)
	CL_Signal_v2<const CL_String8 &, const CL_String8 &> sig_join;

	/// \brief sig_part(nick, channel, reason)
	CL_Signal_v3<const CL_String8 &, const CL_String8 &, const CL_String8 &> sig_part;

	/// \brief sig_mode(prefix, receiver, mode, params)
	CL_Signal_v4<const CL_String8 &, const CL_String8 &, const CL_String8 &, const std::vector<CL_String8> &> sig_mode;

	/// \brief sig_topic(prefix, channel, topic)
	CL_Signal_v3<const CL_String8 &, const CL_String8 &, const CL_String8 &> sig_topic;

	/// \brief sig_invite(prefix, nick, channel)
	CL_Signal_v3<const CL_String8 &, const CL_String8 &, const CL_String8 &> sig_invite;

	/// \brief sig_kick(prefix, chan, user, comment)
	CL_Signal_v4<const CL_String8 &, const CL_String8 &, const CL_String8 &, const CL_String8 &> sig_kick;

	/// \brief sig_privmsg(prefix, receiver, text)
	CL_Signal_v3<const CL_String8 &, const CL_String8 &, const CL_String8 &> sig_privmsg;

	/// \brief sig_notice(prefix, receiver, text)
	CL_Signal_v3<const CL_String8 &, const CL_String8 &, const CL_String8 &> sig_notice;

	/// \brief sig_privmsg_ctcp(prefix, receiver, command, data)
	CL_Signal_v4<const CL_String8 &, const CL_String8 &, const CL_String8 &, const CL_String8 &> sig_privmsg_ctcp;

	/// \brief sig_notice_ctcp(prefix, receiver, command, data)
	CL_Signal_v4<const CL_String8 &, const CL_String8 &, const CL_String8 &, const CL_String8 &> sig_notice_ctcp;

	/// \brief sig_ping(daemon1, daemon2)
	CL_Signal_v2<const CL_String8 &, const CL_String8 &> sig_ping;

	CL_Thread thread;

	CL_Mutex mutex;

	CL_TCPConnection connection;

	std::queue<CL_String8> send_queue;

	std::queue<CL_String8> received_queue;

	CL_String8 nick, username, hostname, servername, realname;

	CL_Event event_quit, event_send;


/// \}
/// \name Operations
/// \{

public:
	void add_ref();

	void release_ref();

	void send_data(const CL_String8 &data);

	void process_data();


/// \}
/// \name Implementation
/// \{

private:
	void thread_main();

	CL_String server, port;

	bool signal_error;

	CL_String error_message;

	int ref_count;
/// \}
};


