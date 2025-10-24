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

/// \addtogroup clanNetwork_Internet_Relay_Chat clanNetwork Internet Relay Chat
/// \{


#pragma once


#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif

#include "../api_network.h"
#include "../../Core/Text/string_types.h"
#include <vector>
#include <list>
#include <queue>
#include "../../Core/Signals/signal_v1.h"
#include "../../Core/Signals/signal_v2.h"
#include "../../Core/Signals/signal_v3.h"
#include "../../Core/Signals/signal_v4.h"
#include "irc_numerics.h"

class CL_IRCConnection_Generic;

/// \brief IRC Connection class.
///
///    <p>The CL_IRCConnection class represents a socket connection to an IRC
///    server. Upon construction it will connect to the server+port given and
///    then parse each line written by the IRC server. Each time
///    process_data() is called, the CL_IRCConnection will emit the
///    signals for those commands received.</p>
///    <p>Most IRC networks will expect the connecting IRC client to first send
///    a "nick" command and then a "user" command, as part of the logon
///    process. CL_IRCConnection will not do this, so the general logon
///    procedure looks somewhat like this:</p>
///    <pre>
///    CL_IRCConnection connection("irc.freenode.net", "6667");
///    connection.send_nick("MyNick");
///    connection.send_user("username", "our.hostname", "irc.freenode.net", "User Name");
///    </pre>
///    <p>IRC servers will occationally send a "ping" line, which have to be
///    replied with a "pong" command. If this isn't done within some server
///    specific timeout, the IRC network will disconnect the client.
///    CL_IRCConnection will also not do this automatically, so its important
///    you at least hook up the ping slot, to a function looking like this:</p>
///    <pre>
///    void on_ping(const CL_String8 &daemon1, const CL_String8 &daemon2)
///    { connection.send_pong(daemon1, daemon2); }
///    </pre> 
/// \xmlonly !group=Network/Internet Relay Chat! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_IRCConnection
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a CL_IRCConnection and connects a server.
	///
	/// \param server IRC server host name. Ie. "irc.freenode.net".
	/// \param port IRC server port. Defaults to "6667".
	CL_IRCConnection();

	CL_IRCConnection(const CL_String &server, const CL_String &port = cl_text("6667"));

	CL_IRCConnection(const CL_IRCConnection &copy);

	~CL_IRCConnection();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns our current nick name.
	const CL_String8 &get_nick() const;

	/// \brief Returns the username passed to send_user.
	const CL_String8 &get_username() const;

	/// \brief Returns hostname passed to send_user.
	const CL_String8 &get_hostname() const;

	/// \brief Returns server name passed to send_user.
	const CL_String8 &get_servername() const;

	/// \brief Returns the real name passed to send_user.
	const CL_String8 &get_realname() const;


/// \}
/// \name Operations
/// \{

public:
	CL_IRCConnection &operator =(const CL_IRCConnection &copy);

	/// \brief Send command to IRC server.
	///
	/// \param prefix Sender prefix.
	/// \param command IRC command.
	/// \param params List of parameters for IRC command.
	/// <p>This is the low-level IRC send command. Useful if you need to
	/// send a command not supported by the other higher level send
	/// functions.</p>
	void send_command(const CL_String8 &prefix, const CL_String8 &command, const std::vector<CL_String8> &params);

	/// \brief Send password.
	///
	/// \param password Password parameter to send with "pass" command.
	/// <p>Sends a "pass" command to IRC server.</p>
	void send_pass(const CL_String8 &password);

	/// \brief Send nick.
	///
	/// \param nick Nick name to use.
	/// \param hopcount Can't remember. Check the RFC. :-)
	/// <p>Sets or changes your nick.</p>
	void send_nick(const CL_String8 &nick, int hopcount = -1);

	/// \brief Send user login/identify command.
	///
	/// \param username Client username.
	/// \param hostname Client hostname.
	/// \param servername Hostname of IRC server.
	/// \param realname Real name of client user.
	/// <p>This command identifies the IRC client to the IRC network.
	/// Each of the parameters passsed will be used to create your fully
	/// qualified prefix on the network.</p>
	void send_user(const CL_String8 &username, const CL_String8 &hostname, const CL_String8 &servername, const CL_String8 &realname);

	/// \brief Send operator login command
	///
	/// \param user Operator login name.
	/// \param password Operator password.
	void send_oper(const CL_String8 &user, const CL_String8 &password);

	/// \brief Send quit command.
	void send_quit(const CL_String8 &quitmessage = "");

	/// \brief Send join channel command.
	///
	/// \param channel Channel to join, ie. "#clanlib"
	/// \param key Channel key, if any.
	/// \param channels List of channels to join.
	/// \param keys List of channel keys.
	void send_join(const CL_String8 &channel, const CL_String8 &key = "");

	void send_join(const std::list<CL_String8> &channels, const std::list<CL_String8> &keys);

	/// \brief Send part channel command.
	///
	/// \param channel Channel to leave.
	/// \param reason Reason for leaving.
	/// \param channels List of channels to leave.
	void send_part(const CL_String8 &channel, const CL_String8 &reason = "");

	void send_part(const std::list<CL_String8> &channels, const CL_String8 &reason = "");

	/// \brief Send set mode command.
	///
	/// \param target Channel or nick mode change apply to.
	/// \param mode Mode change to perform, ie. "+o".
	/// \param args Arguements to mode command.
	void send_mode(const CL_String8 &target, const CL_String8 &mode, const std::vector<CL_String8> &args);

	/// \brief Set/Get topic on channel.
	///
	/// \param channel Channel name.
	/// \param topic New topic to be set.
	/// <p>If no topic is specified, this will make the IRC server send
	/// back what the topic is on a given channel.</p>
	void send_topic(const CL_String8 &channel, const CL_String8 &topic);

	void send_topic(const CL_String8 &channel);

	/// \brief List nicks on channel(s).
	///
	/// \param channel Channel to list.
	void send_names(const CL_String8 &channel = "");

	void send_names(const std::list<CL_String8> &channels);

	/// \brief Send list of channels.
	///
	/// \param channel Channel name substring to match.
	/// \param server Server to limit results from.
	void send_list(const CL_String8 &channel = "", const CL_String8 &server = "");

	void send_list(const std::list<CL_String8> &channels, const CL_String8 &server = "");

	/// \brief Send channel join invitation.
	///
	/// \param nickname Nick to receive invitation.
	/// \param channel Channel being invited to.
	void send_invite(const CL_String8 &nickname, const CL_String8 &channel);

	/// \brief Send kick command
	void send_kick(const CL_String8 &channel, const CL_String8 &user, const CL_String8 &comment = "");

	void send_kick(const std::list<CL_String8> &channels, const std::list<CL_String8> &users, const CL_String8 &comment = "");

	/// \brief Send version command.
	void send_version(const CL_String8 &server = "");

	/// \brief Send statistics command.
	void send_stats(const CL_String8 &query = "", const CL_String8 &server = "");

	/// \brief Send links command.
	void send_links(const CL_String8 &remote_server = "", const CL_String8 &server_mask = "");

	/// \brief Send time command.
	void send_time(const CL_String8 &server = "");

	/// \brief Send connect command.
	void send_connect(const CL_String8 &target_server, int port = 0, const CL_String8 &remote_server = "");

	/// \brief Send trace command.
	void send_trace(const CL_String8 &server = CL_String8());

	/// \brief Send admin command.
	void send_admin(const CL_String8 &server = CL_String8());

	/// \brief Send info command.
	void send_info(const CL_String8 &server = CL_String8());

	/// \brief Send channel message or private message.
	///
	/// \param reciver Channel or nick to receive message.
	/// \param text Message to send.
	void send_privmsg(const CL_String8 &receiver, const CL_String8 &text);

	void send_privmsg(const std::list<CL_String8> &receivers, const CL_String8 &text);

	/// \brief Send Client To Client Protocol (CTCP) message.
	///
	/// \param reciver Channel or nick to receive message.
	/// \param command CTCP command.
	/// \param data CTCP data.
	void send_privmsg_ctcp(const CL_String8 &receiver, const CL_String8 &command, const CL_String8 &data);

	void send_privmsg_ctcp(std::list<CL_String8> &receivers, const CL_String8 &command, const CL_String8 &data);

	/// \brief Send notice message.
	///
	/// \param nickname Nick to receive notice.
	/// \param text Notice to send.
	void send_notice(const CL_String8 &nickname, const CL_String8 &text);

	/// \brief Send Client To Client Protocol (CTCP) notice.
	///
	/// \param nickname Nick to receive notice.
	/// \param command CTCP command.
	/// \param data CTCP data.
	void send_notice_ctcp(const CL_String8 &nickname, const CL_String8 &command, const CL_String8 &data);

	/// \brief Send who command.
	///
	/// \param name Nick to retrieve information about.
	/// \param oper If set to true, use IRC operator rights.
	void send_who(const CL_String8 &name, bool oper = false);

	/// \brief Send whois command.
	///
	/// \param nickmask Nick to retrieve information about
	/// \param server Server to query.
	void send_whois(const CL_String8 &nickmask);

	void send_whois(const CL_String8 &server, const CL_String8 &nickmask);

	void send_whois(const std::list<CL_String8> &nickmasks);

	void send_whois(const CL_String8 &server, const std::list<CL_String8> &nickmasks);

	/// \brief Send whowas command.
	///
	/// \param nickname Nick to retrieve information about
	/// \param count Amount of users with that nick to get information about.
	/// \param server Server to query.
	void send_whowas(const CL_String8 &nickname, int count = 0, const CL_String8 &server = "");

	/// \brief Send kill command.
	///
	/// \param nickname Nick to kill.
	/// \param comment Reason given for killing nick.
	void send_kill(const CL_String8 &nickname, const CL_String8 &comment);

	/// \brief Send ping command
	///
	/// \param server Server to send to.
	/// \param server2 Server to relay through.
	void send_ping(const CL_String8 &server, const CL_String8 &server2 = "");

	/// \brief Send pong command
	///
	/// \param daemon Server that sent ping command.
	/// \param daemon2 Server ping command was relayed through.
	void send_pong(const CL_String8 &daemon, const CL_String8 &daemon2 = "");

	/// \brief Extracts nick part of a fully qualified IRC prefix.
	static CL_String8 extract_nick(const CL_String8 &str);

	/// \brief Extracts user part of a fully qualified IRC prefix.
	static CL_String8 extract_user(const CL_String8 &str);

	/// \brief Extracts address part of a fully qualified IRC prefix.
	static CL_String8 extract_address(const CL_String8 &str);


/// \}
/// \name Signals
/// \{

public:
	/// \brief sig_socket_error(error_message)
	CL_Signal_v1<const CL_String &> &sig_socket_error();

	/// \brief sig_command_received(prefix, command, params)
	CL_Signal_v3<const CL_String8 &, const CL_String8 &, const std::vector<CL_String8> &> &sig_command_received();

	/// \brief sig_unknown_command_received(prefix, command, params)
	CL_Signal_v3<const CL_String8 &, const CL_String8 &, const std::vector<CL_String8> &> &sig_unknown_command_received();

	/// \brief sig_numeric_reply(prefix, code, params)
	CL_Signal_v3<const CL_String8 &, int, const std::vector<CL_String8> &> &sig_numeric_reply();

	/// \brief sig_name_reply(self, channel, users)
	CL_Signal_v3<const CL_String8 &, const CL_String8 &, const std::vector<CL_String8> &> &sig_name_reply();

	/// \brief sig_nick(old_nick, new_nick)
	CL_Signal_v2<const CL_String8 &, const CL_String8 &> &sig_nick();

	/// \brief sig_join(nick, channel)
	CL_Signal_v2<const CL_String8 &, const CL_String8 &> &sig_join();

	/// \brief sig_part(nick, channel, reason)
	CL_Signal_v3<const CL_String8 &, const CL_String8 &, const CL_String8 &> &sig_part();

	/// \brief sig_mode(prefix, receiver, mode, params)
	CL_Signal_v4<const CL_String8 &, const CL_String8 &, const CL_String8 &, const std::vector<CL_String8> &> &sig_mode();

	/// \brief sig_topic(prefix, channel, topic)
	CL_Signal_v3<const CL_String8 &, const CL_String8 &, const CL_String8 &> &sig_topic();

	/// \brief sig_invite(prefix, nick, channel)
	CL_Signal_v3<const CL_String8 &, const CL_String8 &, const CL_String8 &> &sig_invite();

	/// \brief sig_kick(prefix, chan, user, comment)
	CL_Signal_v4<const CL_String8 &, const CL_String8 &, const CL_String8 &, const CL_String8 &> &sig_kick();

	/// \brief sig_privmsg(prefix, receiver, text)
	CL_Signal_v3<const CL_String8 &, const CL_String8 &, const CL_String8 &> &sig_privmsg();

	/// \brief sig_notice(prefix, receiver, text)
	CL_Signal_v3<const CL_String8 &, const CL_String8 &, const CL_String8 &> &sig_notice();

	/// \brief sig_privmsg_ctcp(prefix, receiver, command, data)
	CL_Signal_v4<const CL_String8 &, const CL_String8 &, const CL_String8 &, const CL_String8 &> &sig_privmsg_ctcp();

	/// \brief sig_notice_ctcp(prefix, receiver, command, data)
	CL_Signal_v4<const CL_String8 &, const CL_String8 &, const CL_String8 &, const CL_String8 &> &sig_notice_ctcp();

	/// \brief sig_ping(daemon1, daemon2)
	CL_Signal_v2<const CL_String8 &, const CL_String8 &> &sig_ping();


/// \}
/// \name Implementation
/// \{

private:
	CL_IRCConnection_Generic *impl;
/// \}
};


/// \}
