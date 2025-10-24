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

//! clanNetwork="Internet Relay Chat"
//! header=network.h

#ifndef header_irc_connection
#define header_irc_connection

#ifdef CL_API_DLL
#ifdef CL_NETWORK_EXPORT
#define CL_API_NETWORK __declspec(dllexport)
#else
#define CL_API_NETWORK __declspec(dllimport)
#endif
#else
#define CL_API_NETWORK
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif

#include <string>
#include <vector>
#include <list>
#include <queue>

#include "../../signals.h"
#include "irc_numerics.h"

class CL_IRCConnection_Generic;

//: IRC Connection class.
//- !group=Network/Internet Relay Chat!
//- !header=network.h!
//- <p>The CL_IRCConnection class represents a socket connection to an IRC
//- server. Upon construction it will connect to the server+port given and
//- then parse each line written by the IRC server. Each time
//- CL_System::keep_alive() is called, the CL_IRCConnection will emit the
//- signals for those commands received.</p>
//- <p>Most IRC networks will expect the connecting IRC client to first send
//- a "nick" command and then a "user" command, as part of the logon
//- process. CL_IRCConnection will not do this, so the general logon
//- procedure looks somewhat like this:</p>
//- <pre>
//- CL_IRCConnection connection("irc.freenode.net", "6667");
//- connection.send_nick("MyNick");
//- connection.send_user("username", "our.hostname", "irc.freenode.net", "User Name");
//- </pre>
//- <p>IRC servers will occationally send a "ping" line, which have to be
//- replied with a "pong" command. If this isn't done within some server
//- specific timeout, the IRC network will disconnect the client.
//- CL_IRCConnection will also not do this automatically, so its important
//- you at least hook up the ping slot, to a function looking like this:</p>
//- <pre>
//- void on_ping(const std::string &daemon1, const std::string &daemon2)
//- { connection.send_pong(daemon1, daemon2); }
//- </pre>
class CL_API_NETWORK CL_IRCConnection
{
//! Construction:
public:
	//: Constructs a CL_IRCConnection and connects a server.
	//param server: IRC server host name. Ie. "irc.freenode.net".
	//param port: IRC server port. Defaults to "6667".
	CL_IRCConnection();

	CL_IRCConnection(const std::string &server, const std::string &port = "6667");
	
	CL_IRCConnection(const CL_IRCConnection &copy);

	~CL_IRCConnection();
	
//! Attributes:
public:
	//: Returns our current nick name.
	const std::string &get_nick() const;

	//: Returns the username passed to send_user.
	const std::string &get_username() const;

	//: Returns hostname passed to send_user.
	const std::string &get_hostname() const;

	//: Returns server name passed to send_user.
	const std::string &get_servername() const;

	//: Returns the real name passed to send_user.
	const std::string &get_realname() const;

//! Operations:
public:
	CL_IRCConnection &operator =(const CL_IRCConnection &copy);

	//: Send command to IRC server.
	//param prefix: Sender prefix.
	//param command: IRC command.
	//param params: List of parameters for IRC command.
	//- <p>This is the low-level IRC send command. Useful if you need to
	//- send a command not supported by the other higher level send
	//- functions.</p>
	void send_command(const std::string &prefix, const std::string &command, const std::vector<std::string> &params);

	//: Send password.
	//param password: Password parameter to send with "pass" command.
	//- <p>Sends a "pass" command to IRC server.</p>
	void send_pass(const std::string &password);

	//: Send nick.
	//param nick: Nick name to use.
	//param hopcount: Can't remember. Check the RFC. :-)
	//- <p>Sets or changes your nick.</p>
	void send_nick(const std::string &nick, int hopcount = -1);

	//: Send user login/identify command.
	//param username: Client username.
	//param hostname: Client hostname.
	//param servername: Hostname of IRC server.
	//param realname: Real name of client user.
	//- <p>This command identifies the IRC client to the IRC network.
	//- Each of the parameters passsed will be used to create your fully
	//- qualified prefix on the network.</p>
	void send_user(const std::string &username, const std::string &hostname, const std::string &servername, const std::string &realname);

	//: Send operator login command
	//param user: Operator login name.
	//param password: Operator password.
	void send_oper(const std::string &user, const std::string &password);

	//: Send quit command.	
	void send_quit(const std::string &quitmessage = "");
	
	//: Send join channel command.
	//param channel: Channel to join, ie. "#clanlib"
	//param key: Channel key, if any.
	//param channels: List of channels to join.
	//param keys: List of channel keys.
	void send_join(const std::string &channel, const std::string &key = "");
	
	void send_join(const std::list<std::string> &channels, const std::list<std::string> &keys);

	//: Send part channel command.
	//param channel: Channel to leave.
	//param reason: Reason for leaving.
	//param channels: List of channels to leave.
	void send_part(const std::string &channel, const std::string &reason = "");

	void send_part(const std::list<std::string> &channels, const std::string &reason = "");

	//: Send set mode command.
	//param target: Channel or nick mode change apply to.
	//param mode: Mode change to perform, ie. "+o".
	//param args: Arguements to mode command.
	void send_mode(const std::string &target, const std::string &mode, const std::vector<std::string> &args);

	//: Set/Get topic on channel.
	//param channel: Channel name.
	//param topic: New topic to be set.
	//- <p>If no topic is specified, this will make the IRC server send
	//- back what the topic is on a given channel.</p>
	void send_topic(const std::string &channel, const std::string &topic);
	
	void send_topic(const std::string &channel);

	//: List nicks on channel(s).
	//param channel: Channel to list.
	void send_names(const std::string &channel = "");
	
	void send_names(const std::list<std::string> &channels);

	//: Send list of channels.
	//param channel: Channel name substring to match.
	//param server: Server to limit results from.
	void send_list(const std::string &channel = "", const std::string &server = "");
	
	void send_list(const std::list<std::string> &channels, const std::string &server = "");

	//: Send channel join invitation.
	//param nickname: Nick to receive invitation.
	//param channel: Channel being invited to.
	void send_invite(const std::string &nickname, const std::string &channel);

	//: Send kick command	
	void send_kick(const std::string &channel, const std::string &user, const std::string &comment = "");
	
	void send_kick(const std::list<std::string> &channels, const std::list<std::string> &users, const std::string &comment = "");

	//: Send version command.
	void send_version(const std::string &server = "");

	//: Send statistics command.
	void send_stats(const std::string &query = "", const std::string &server = "");

	//: Send links command.	
	void send_links(const std::string &remote_server = "", const std::string &server_mask = "");
	
	//: Send time command.
	void send_time(const std::string &server = "");

	//: Send connect command.
	void send_connect(const std::string &target_server, int port = 0, const std::string &remote_server = "");
	
	//: Send trace command.
	void send_trace(const std::string &server = 0);
	
	//: Send admin command.
	void send_admin(const std::string &server = 0);

	//: Send info command.	
	void send_info(const std::string &server = 0);
	
	//: Send channel message or private message.
	//param reciver: Channel or nick to receive message.
	//param text: Message to send.
	void send_privmsg(const std::string &receiver, const std::string &text);
	
	void send_privmsg(const std::list<std::string> &receivers, const std::string &text);

	//: Send Client To Client Protocol (CTCP) message.
	//param reciver: Channel or nick to receive message.
	//param command: CTCP command.
	//param data: CTCP data.
	void send_privmsg_ctcp(const std::string &receiver, const std::string &command, const std::string &data);

	void send_privmsg_ctcp(std::list<std::string> &receivers, const std::string &command, const std::string &data);

	//: Send notice message.
	//param nickname: Nick to receive notice.
	//param text: Notice to send.
	void send_notice(const std::string &nickname, const std::string &text);

	//: Send Client To Client Protocol (CTCP) notice.
	//param nickname: Nick to receive notice.
	//param command: CTCP command.
	//param data: CTCP data.
	void send_notice_ctcp(const std::string &nickname, const std::string &command, const std::string &data);

	//: Send who command.
	//param name: Nick to retrieve information about.
	//param oper: If set to true, use IRC operator rights.
	void send_who(const std::string &name, bool oper = false);

	//: Send whois command.
	//param nickmask: Nick to retrieve information about
	//param server: Server to query.
	void send_whois(const std::string &nickmask);
	
	void send_whois(const std::string &server, const std::string &nickmask);
	
	void send_whois(const std::list<std::string> &nickmasks);
	
	void send_whois(const std::string &server, const std::list<std::string> &nickmasks);

	//: Send whowas command.
	//param nickname: Nick to retrieve information about
	//param count: Amount of users with that nick to get information about.	
	//param server: Server to query.
	void send_whowas(const std::string &nickname, int count = 0, const std::string &server = "");

	//: Send kill command.
	//param nickname: Nick to kill.
	//param comment: Reason given for killing nick.
	void send_kill(const std::string &nickname, const std::string &comment);

	//: Send ping command
	//param server: Server to send to.
	//param server2: Server to relay through.
	void send_ping(const std::string &server, const std::string &server2 = "");

	//: Send pong command
	//param daemon: Server that sent ping command.
	//param daemon2: Server ping command was relayed through.
	void send_pong(const std::string &daemon, const std::string &daemon2 = "");

	//: Extracts nick part of a fully qualified IRC prefix.
	static std::string extract_nick(const std::string &str);

	//: Extracts user part of a fully qualified IRC prefix.
	static std::string extract_user(const std::string &str);

	//: Extracts address part of a fully qualified IRC prefix.
	static std::string extract_address(const std::string &str);

//! Signals:
public:
	//: sig_socket_error(error_message)
	CL_Signal_v1<const std::string &> &sig_socket_error();

	//: sig_command_received(prefix, command, params)
	CL_Signal_v3<const std::string &, const std::string &, const std::vector<std::string> &> &sig_command_received();

	//: sig_unknown_command_received(prefix, command, params)
	CL_Signal_v3<const std::string &, const std::string &, const std::vector<std::string> &> &sig_unknown_command_received();

	//: sig_numeric_reply(prefix, code, params)
	CL_Signal_v3<const std::string &, int, const std::vector<std::string> &> &sig_numeric_reply();
	
	//: sig_name_reply(self, channel, users)
	CL_Signal_v3<const std::string &, const std::string &, const std::vector<std::string> &> &sig_name_reply();

	//: sig_nick(old_nick, new_nick)
	CL_Signal_v2<const std::string &, const std::string &> &sig_nick();

	//: sig_join(nick, channel)
	CL_Signal_v2<const std::string &, const std::string &> &sig_join();
	
	//: sig_part(nick, channel, reason)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> &sig_part();
	
	//: sig_mode(prefix, receiver, mode, params)
	CL_Signal_v4<const std::string &, const std::string &, const std::string &, const std::vector<std::string> &> &sig_mode();

	//: sig_topic(prefix, channel, topic)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> &sig_topic();
	
	//: sig_invite(prefix, nick, channel)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> &sig_invite();

	//: sig_kick(prefix, chan, user, comment)
	CL_Signal_v4<const std::string &, const std::string &, const std::string &, const std::string &> &sig_kick();

	//: sig_privmsg(prefix, receiver, text)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> &sig_privmsg();

	//: sig_notice(prefix, receiver, text)
	CL_Signal_v3<const std::string &, const std::string &, const std::string &> &sig_notice();

	//: sig_privmsg_ctcp(prefix, receiver, command, data)
	CL_Signal_v4<const std::string &, const std::string &, const std::string &, const std::string &> &sig_privmsg_ctcp();

	//: sig_notice_ctcp(prefix, receiver, command, data)
	CL_Signal_v4<const std::string &, const std::string &, const std::string &, const std::string &> &sig_notice_ctcp();

	//: sig_ping(daemon1, daemon2)
	CL_Signal_v2<const std::string &, const std::string &> &sig_ping();

//! Implementation:
private:
	CL_IRCConnection_Generic *impl;
};

#endif
