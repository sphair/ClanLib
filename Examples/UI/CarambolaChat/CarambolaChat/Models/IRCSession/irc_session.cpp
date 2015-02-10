
#include "precomp.h"
#include "irc_session.h"
#include "irc_numerics.h"
#include "command.h"
#include <algorithm>

IRCSession::IRCSession()
: connect_status(status_disconnected), 	user_requested_disconnect(false)
{
	connection.func_message_received() = clan::bind_member(this, &IRCSession::on_message_received);
	connection.func_disconnected() = clan::bind_member(this, &IRCSession::on_disconnected);
	reconnect_timer.func_expired() = clan::bind_member(this, &IRCSession::on_reconnect_timer_expired);
	reconnect_timeout = 2000 + rand() * 500 / RAND_MAX;
}

IRCSession::~IRCSession()
{
	disconnect_abortive();
}

void IRCSession::set_perform_list(const std::vector<std::string> &new_perform_list)
{
	perform_list = new_perform_list;
}

void IRCSession::set_topic(const IRCChannel &channel, const IRCText &text)
{
	connection.send_topic(channel, text);
}

void IRCSession::connect(const std::string &server, const std::string &port, const std::string &nick, const std::string &alt_nick, const std::string &username, const std::string &full_name)
{
	user_requested_disconnect = false;
	connect_server = server;
	connect_port = port;
	connect_username = username;
	connect_full_name = full_name;
	connect_nick = nick;
	alt_connect_nick = alt_nick;
	set_connect_status(status_connecting);
	connection.connect(clan::SocketName(server, port));
	connection.send_nick(IRCNick::from_text(nick));
	connection.send_user(clan::StringHelp::text_to_utf8(username), "localhost", clan::StringHelp::text_to_utf8(server), clan::StringHelp::text_to_utf8(full_name));
	our_nickname = IRCNick::from_text(nick);
}

void IRCSession::on_reconnect_timer_expired()
{
	if (connect_status == status_disconnected)
	{
		set_connect_status(status_connecting);
		connection.connect(clan::SocketName(connect_server, connect_port));
		connection.send_nick(IRCNick::from_text(connect_nick));
		connection.send_user(clan::StringHelp::text_to_utf8(connect_username), "localhost", clan::StringHelp::text_to_utf8(connect_server), clan::StringHelp::text_to_utf8(connect_full_name));
		our_nickname = IRCNick::from_text(connect_nick);
	}
}

void IRCSession::disconnect_graceful(const std::string &quit_message)
{
	user_requested_disconnect = true;
	connection.send_quit(IRCText::from_text(quit_message));
	connection.disconnect();
}

void IRCSession::disconnect_abortive()
{
	user_requested_disconnect = true;
	connection.disconnect();
}

void IRCSession::set_connect_status(ConnectStatus new_status)
{
	connect_status = new_status;
	cb_connect_status_changed(new_status);
}

void IRCSession::set_connection_name(const std::string &name)
{
	connection_name = name;
}

void IRCSession::set_nick(const std::string &nick)
{
	connection.send_nick(IRCNick::from_text(nick));
	if (connect_status == status_connecting)
		our_nickname = IRCNick::from_text(nick);
}

IRCNick IRCSession::get_nick() const
{
	return our_nickname;
}

std::string IRCSession::get_our_hostname() const
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
/*
	// Do not remove this code.  It is useful to see exactly what the server writes to us.
	IRCRawString s = IRCMessage::create_line(message.get_prefix().to_raw(), message.get_command(), message.get_params());
	cb_system_text(IRCText::from_raw(s.substr(0, s.length()-2)));
*/
	switch (message.get_type())
	{
	case IRCMessage::type_numeric_reply: on_numeric_reply(IRCNumericReply(message)); break;
	case IRCMessage::type_nick: on_nick(IRCNickMessage(message)); break;
	case IRCMessage::type_join: on_join(IRCJoinMessage(message)); break;
	case IRCMessage::type_part: on_part(IRCPartMessage(message)); break;
	case IRCMessage::type_kick: on_kick(IRCKickMessage(message)); break;
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

struct NumericToString
{
	int numeric;
	const char *str;
	int flags;
};

enum NumericToStringFlags
{
	n2s_invisible = 0x01,
	n2s_hide_numeric = 0x02
};

static NumericToString numeric_to_string[]=
{
	RPL_WELCOME, "RPL_WELCOME", n2s_hide_numeric,
	RPL_YOURHOST, "RPL_YOURHOST", n2s_hide_numeric,
	RPL_CREATED, "RPL_CREATED", n2s_hide_numeric,
	RPL_MYINFO, "RPL_MYINFO", n2s_invisible,
	RPL_BOUNCE, "RPL_BOUNCE", n2s_invisible,
	RPL_MAP, "RPL_MAP", 0,
	RPL_PROTOCTL, "RPL_PROTOCTL", 0,
	RPL_MAPMORE, "RPL_MAPMORE", 0,
	RPL_MAPEND, "RPL_MAPEND", 0,
	RPL_SNOMASK, "RPL_SNOMASK", 0,
	RPL_STATMEMTOT, "RPL_STATMEMTOT", 0,
	RPL_STATMEM, "RPL_STATMEM", 0,
	RPL_YOURCOOKIE, "RPL_YOURCOOKIE", 0,
	RPL_TRACELINK, "RPL_TRACELINK", 0,
	RPL_TRACECONNECTING, "RPL_TRACECONNECTING", 0,
	RPL_TRACEHANDSHAKE, "RPL_TRACEHANDSHAKE", 0,
	RPL_TRACEUNKNOWN, "RPL_TRACEUNKNOWN", 0,
	RPL_TRACEOPERATOR, "RPL_TRACEOPERATOR", 0,
	RPL_TRACEUSER, "RPL_TRACEUSER", 0,
	RPL_TRACESERVER, "RPL_TRACESERVER", 0,
	RPL_TRACESERVICE, "RPL_TRACESERVICE", 0,
	RPL_TRACENEWTYPE, "RPL_TRACENEWTYPE", 0,
	RPL_TRACECLASS, "RPL_TRACECLASS", 0,
	RPL_TRACERECONNECT, "RPL_TRACERECONNECT", 0,
	RPL_STATSLINKINFO, "RPL_STATSLINKINFO", 0,
	RPL_STATSCOMMANDS, "RPL_STATSCOMMANDS", 0,
	RPL_STATSCLINE, "RPL_STATSCLINE", 0,
	RPL_STATSNLINE, "RPL_STATSNLINE", 0,
	RPL_STATSILINE, "RPL_STATSILINE", 0,
	RPL_STATSKLINE, "RPL_STATSKLINE", 0,
	RPL_STATSQLINE, "RPL_STATSQLINE", 0,
	RPL_STATSPLINE_U, "RPL_STATSPLINE_U", 0,
	RPL_STATSYLINE, "RPL_STATSYLINE", 0,
	RPL_ENDOFSTATS, "RPL_ENDOFSTATS", 0,
	RPL_STATSPLINE_E, "RPL_STATSPLINE_E", 0,
	RPL_UMODEIS, "RPL_UMODEIS", 0,
	RPL_STATSBLINE_ED, "RPL_STATSBLINE_ED", 0,
	RPL_STATSELINE, "RPL_STATSELINE", 0,
	RPL_STATSFLINE, "RPL_STATSFLINE", 0,
	RPL_STATSDLINE_E, "RPL_STATSDLINE_E", n2s_hide_numeric,
	RPL_STATSZLINE, "RPL_STATSZLINE", 0,
	RPL_STATSCOUNT, "RPL_STATSCOUNT", 0,
	RPL_STATSGLINE_D, "RPL_STATSGLINE_D", 0,
	RPL_SERVICEINFO, "RPL_SERVICEINFO", 0,
	RPL_ENDOFSERVICES, "RPL_ENDOFSERVICES", 0,
	RPL_SERVICE, "RPL_SERVICE", 0,
	RPL_SERVLIST, "RPL_SERVLIST", 0,
	RPL_SERVLISTEND, "RPL_SERVLISTEND", 0,
	RPL_STATSIAUTH, "RPL_STATSIAUTH", 0,
	RPL_STATSVLINE, "RPL_STATSVLINE", 0,
	RPL_STATSLLINE, "RPL_STATSLLINE", 0,
	RPL_STATSUPTIME, "RPL_STATSUPTIME", 0,
	RPL_STATSOLINE, "RPL_STATSOLINE", 0,
	RPL_STATSHLINE, "RPL_STATSHLINE", 0,
	RPL_STATSSLINE, "RPL_STATSSLINE", 0,
	RPL_STATSPING, "RPL_STATSPING", 0,
	RPL_STATSTLINE, "RPL_STATSTLINE", 0,
	RPL_STATSULINE_D, "RPL_STATSULINE_D", 0,
	RPL_STATSBLINE_I, "RPL_STATSBLINE_I", 0,
	RPL_STATSGLINE_U, "RPL_STATSGLINE_U", 0,
	RPL_STATSXLINE, "RPL_STATSXLINE", 0,
	RPL_STATSDEFINE, "RPL_STATSDEFINE", 0,
	RPL_STATSULINE_EU, "RPL_STATSULINE_EU", 0,
	RPL_STATSDEBUG, "RPL_STATSDEBUG", 0,
	RPL_STATSDLINE_I, "RPL_STATSDLINE_I", n2s_hide_numeric,
	RPL_STATSCONN, "RPL_STATSCONN", 0,
	RPL_LUSERCLIENT, "RPL_LUSERCLIENT", n2s_hide_numeric,
	RPL_LUSEROP, "RPL_LUSEROP", n2s_hide_numeric,
	RPL_LUSERUNKNOWN, "RPL_LUSERUNKNOWN", n2s_hide_numeric,
	RPL_LUSERCHANNELS, "RPL_LUSERCHANNELS", n2s_hide_numeric,
	RPL_LUSERME, "RPL_LUSERME", n2s_hide_numeric,
	RPL_ADMINME, "RPL_ADMINME", 0,
	RPL_ADMINLOC1, "RPL_ADMINLOC1", 0,
	RPL_ADMINLOC2, "RPL_ADMINLOC2", 0,
	RPL_ADMINEMAIL, "RPL_ADMINEMAIL", 0,
	RPL_TRACELOG, "RPL_TRACELOG", 0,
	RPL_TRACEEND, "RPL_TRACEEND", 0,
	RPL_ENDOFTRACE, "RPL_ENDOFTRACE", 0,
	RPL_TRACEPING, "RPL_TRACEPING", 0,
	RPL_TRYAGAIN, "RPL_TRYAGAIN", 0,
	RPL_LOAD2HI, "RPL_LOAD2HI", 0,
	RPL_LOCALUSERS, "RPL_LOCALUSERS", n2s_hide_numeric,
	RPL_GLOBALUSERS, "RPL_GLOBALUSERS", n2s_hide_numeric,
	RPL_SILELIST, "RPL_SILELIST", 0,
	RPL_ENDOFSILELIST, "RPL_ENDOFSILELIST", 0,
	RPL_STATSDELTA, "RPL_STATSDELTA", 0,
	RPL_STATSDLINE, "RPL_STATSDLINE", n2s_hide_numeric,
	RPL_GLIST, "RPL_GLIST", 0,
	RPL_ENDOFGLIST, "RPL_ENDOFGLIST", 0,
	RPL_HELPHDR, "RPL_HELPHDR", 0,
	RPL_HELPOP, "RPL_HELPOP", 0,
	RPL_HELPTLR, "RPL_HELPTLR", 0,
	RPL_HELPHLP, "RPL_HELPHLP", 0,
	RPL_HELPFWD, "RPL_HELPFWD", 0,
	RPL_HELPIGN, "RPL_HELPIGN", 0,
	RPL_NONE, "RPL_NONE", 0,
	RPL_AWAY, "RPL_AWAY", 0,
	RPL_USERHOST, "RPL_USERHOST", n2s_hide_numeric,
	RPL_ISON, "RPL_ISON", 0,
	RPL_TEXT, "RPL_TEXT", 0,
	RPL_UNAWAY, "RPL_UNAWAY", 0,
	RPL_NOWAWAY, "RPL_NOWAWAY", 0,
	RPL_USERIP, "RPL_USERIP", 0,
	RPL_WHOISREGNICK, "RPL_WHOISREGNICK", 0,
	RPL_WHOISADMIN, "RPL_WHOISADMIN", 0,
	RPL_WHOISSADMIN, "RPL_WHOISSADMIN", 0,
	RPL_WHOISSVCMSG, "RPL_WHOISSVCMSG", 0,
	RPL_WHOISUSER, "RPL_WHOISUSER", 0,
	RPL_WHOISSERVER, "RPL_WHOISSERVER", 0,
	RPL_WHOISOPERATOR, "RPL_WHOISOPERATOR", 0,
	RPL_WHOWASUSER, "RPL_WHOWASUSER", 0,
	RPL_ENDOFWHO, "RPL_ENDOFWHO", 0,
	RPL_WHOISCHANOP, "RPL_WHOISCHANOP", 0,
	RPL_WHOISIDLE, "RPL_WHOISIDLE", 0,
	RPL_ENDOFWHOIS, "RPL_ENDOFWHOIS", 0,
	RPL_WHOISCHANNELS, "RPL_WHOISCHANNELS", 0,
	RPL_LISTSTART, "RPL_LISTSTART", 0,
	RPL_LIST, "RPL_LIST", 0,
	RPL_LISTEND, "RPL_LISTEND", 0,
	RPL_CHANNELMODEIS, "RPL_CHANNELMODEIS", 0,
	RPL_UNIQOPIS, "RPL_UNIQOPIS", 0,
	RPL_NOCHANPASS, "RPL_NOCHANPASS", 0,
	RPL_CHPASSUNKNOWN, "RPL_CHPASSUNKNOWN", 0,
	RPL_CREATIONTIME, "RPL_CREATIONTIME", 0,
	RPL_NOTOPIC, "RPL_NOTOPIC", n2s_invisible,
	RPL_TOPIC, "RPL_TOPIC", n2s_invisible,
	RPL_TOPICWHOTIME, "RPL_TOPICWHOTIME", n2s_invisible,
	RPL_LISTUSAGE, "RPL_LISTUSAGE", 0,
	RPL_COMMANDSYNTAX, "RPL_COMMANDSYNTAX", 0,
	RPL_CHANPASSOK, "RPL_CHANPASSOK", 0,
	RPL_BADCHANPASS, "RPL_BADCHANPASS", 0,
	RPL_INVITING, "RPL_INVITING", 0,
	RPL_SUMMONING, "RPL_SUMMONING", 0,
	RPL_INVITELIST, "RPL_INVITELIST", 0,
	RPL_ENDOFINVITELIST, "RPL_ENDOFINVITELIST", 0,
	RPL_EXCEPTLIST, "RPL_EXCEPTLIST", 0,
	RPL_ENDOFEXCEPTLIST, "RPL_ENDOFEXCEPTLIST", 0,
	RPL_VERSION, "RPL_VERSION", 0,
	RPL_WHOREPLY, "RPL_WHOREPLY", 0,
	RPL_NAMREPLY, "RPL_NAMREPLY", n2s_invisible,
	RPL_WHOSPCRPL, "RPL_WHOSPCRPL", 0,
	RPL_KILLDONE, "RPL_KILLDONE", 0,
	RPL_CLOSING, "RPL_CLOSING", 0,
	RPL_CLOSEEND, "RPL_CLOSEEND", 0,
	RPL_LINKS, "RPL_LINKS", 0,
	RPL_ENDOFLINKS, "RPL_ENDOFLINKS", 0,
	RPL_ENDOFNAMES, "RPL_ENDOFNAMES", n2s_invisible,
	RPL_BANLIST, "RPL_BANLIST", 0,
	RPL_ENDOFBANLIST, "RPL_ENDOFBANLIST", 0,
	RPL_ENDOFWHOWAS, "RPL_ENDOFWHOWAS", 0,
	RPL_INFO, "RPL_INFO", 0,
	RPL_MOTD, "RPL_MOTD", n2s_hide_numeric,
	RPL_INFOSTART, "RPL_INFOSTART", 0,
	RPL_ENDOFINFO, "RPL_ENDOFINFO", 0,
	RPL_MOTDSTART, "RPL_MOTDSTART", n2s_hide_numeric,
	RPL_ENDOFMOTD, "RPL_ENDOFMOTD", n2s_hide_numeric,
	RPL_YOUREOPER, "RPL_YOUREOPER", 0,
	RPL_REHASHING, "RPL_REHASHING", 0,
	RPL_YOURESERVICE, "RPL_YOURESERVICE", 0,
	RPL_MYPORTIS, "RPL_MYPORTIS", 0,
	RPL_NOTOPERANYMORE, "RPL_NOTOPERANYMORE", 0,
	RPL_TIME, "RPL_TIME", 0,
	RPL_USERSSTART, "RPL_USERSSTART", 0,
	RPL_USERS, "RPL_USERS", 0,
	RPL_ENDOFUSERS, "RPL_ENDOFUSERS", 0,
	RPL_NOUSERS, "RPL_NOUSERS", 0,
	ERR_NOSUCHNICK, "ERR_NOSUCHNICK", 0,
	ERR_NOSUCHSERVER, "ERR_NOSUCHSERVER", 0,
	ERR_NOSUCHCHANNEL, "ERR_NOSUCHCHANNEL", 0,
	ERR_CANNOTSENDTOCHAN, "ERR_CANNOTSENDTOCHAN", 0,
	ERR_TOOMANYCHANNELS, "ERR_TOOMANYCHANNELS", 0,
	ERR_WASNOSUCHNICK, "ERR_WASNOSUCHNICK", 0,
	ERR_TOOMANYTARGETS, "ERR_TOOMANYTARGETS", 0,
	ERR_NOSUCHSERVICE, "ERR_NOSUCHSERVICE", 0,
	ERR_NOCOLORSONCHAN, "ERR_NOCOLORSONCHAN", 0,
	ERR_NOORIGIN, "ERR_NOORIGIN", 0,
	ERR_NORECIPIENT, "ERR_NORECIPIENT", 0,
	ERR_NOTEXTTOSEND, "ERR_NOTEXTTOSEND", 0,
	ERR_NOTOPLEVEL, "ERR_NOTOPLEVEL", 0,
	ERR_WILDTOPLEVEL, "ERR_WILDTOPLEVEL", 0,
	ERR_BADMASK, "ERR_BADMASK", 0,
	ERR_TOOMANYMATCHES, "ERR_TOOMANYMATCHES", 0,
	ERR_QUERYTOOLONG, "ERR_QUERYTOOLONG", 0,
	ERR_UNKNOWNCOMMAND, "ERR_UNKNOWNCOMMAND", 0,
	ERR_NOMOTD, "ERR_NOMOTD", n2s_hide_numeric,
	ERR_NOADMININFO, "ERR_NOADMININFO", 0,
	ERR_FILEERROR, "ERR_FILEERROR", 0,
	ERR_TOOMANYAWAY, "ERR_TOOMANYAWAY", 0,
	ERR_NONICKNAMEGIVEN, "ERR_NONICKNAMEGIVEN", 0,
	ERR_ERRONEUSNICKNAME, "ERR_ERRONEUSNICKNAME", 0,
	ERR_NICKNAMEINUSE, "ERR_NICKNAMEINUSE", 0,
	ERR_SERVICENAMEINUSE, "ERR_SERVICENAMEINUSE", 0,
	ERR_SERVICECONFUSED, "ERR_SERVICECONFUSED", 0,
	ERR_BANONCHAN, "ERR_BANONCHAN", 0,
	ERR_NICKCOLLISION, "ERR_NICKCOLLISION", 0,
	ERR_UNAVAILRESOURCE, "ERR_UNAVAILRESOURCE", 0,
	ERR_BANNICKCHANGE, "ERR_BANNICKCHANGE", 0,
	ERR_DEAD, "ERR_DEAD", 0,
	ERR_NICKTOOFAST, "ERR_NICKTOOFAST", 0,
	ERR_NCHANGETOOFAST, "ERR_NCHANGETOOFAST", 0,
	ERR_TARGETTOOFAST, "ERR_TARGETTOOFAST", 0,
	ERR_SERVICESDOWN, "ERR_SERVICESDOWN", 0,
	ERR_USERNOTINCHANNEL, "ERR_USERNOTINCHANNEL", 0,
	ERR_NOTONCHANNEL, "ERR_NOTONCHANNEL", 0,
	ERR_USERONCHANNEL, "ERR_USERONCHANNEL", 0,
	ERR_NOLOGIN, "ERR_NOLOGIN", 0,
	ERR_SUMMONDISABLED, "ERR_SUMMONDISABLED", 0,
	ERR_USERSDISABLED, "ERR_USERSDISABLED", 0,
	ERR_NOTREGISTERED, "ERR_NOTREGISTERED", 0,
	ERR_IDCOLLISION, "ERR_IDCOLLISION", 0,
	ERR_NICKLOST, "ERR_NICKLOST", 0,
	ERR_HOSTILENAME, "ERR_HOSTILENAME", 0,
	ERR_NEEDMOREPARAMS, "ERR_NEEDMOREPARAMS", 0,
	ERR_ALREADYREGISTRED, "ERR_ALREADYREGISTRED", 0,
	ERR_NOPERMFORHOST, "ERR_NOPERMFORHOST", 0,
	ERR_PASSWDMISMATCH, "ERR_PASSWDMISMATCH", 0,
	ERR_YOUREBANNEDCREEP, "ERR_YOUREBANNEDCREEP", 0,
	ERR_YOUWILLBEBANNED, "ERR_YOUWILLBEBANNED", 0,
	ERR_KEYSET, "ERR_KEYSET", 0,
	ERR_INVALIDUSERNAME, "ERR_INVALIDUSERNAME", 0,
	ERR_ONLYSERVERSCANCHANGE, "ERR_ONLYSERVERSCANCHANGE", 0,
	ERR_CHANNELISFULL, "ERR_CHANNELISFULL", 0,
	ERR_UNKNOWNMODE, "ERR_UNKNOWNMODE", 0,
	ERR_INVITEONLYCHAN, "ERR_INVITEONLYCHAN", 0,
	ERR_BANNEDFROMCHAN, "ERR_BANNEDFROMCHAN", 0,
	ERR_BADCHANNELKEY, "ERR_BADCHANNELKEY", 0,
	ERR_BADCHANMASK, "ERR_BADCHANMASK", 0,
	ERR_MODELESS, "ERR_MODELESS", 0,
	ERR_NOCHANMODES, "ERR_NOCHANMODES", 0,
	ERR_NEEDREGGEDNICK, "ERR_NEEDREGGEDNICK", 0,
	ERR_BANLISTFULL, "ERR_BANLISTFULL", 0,
	ERR_BADCHANNAME, "ERR_BADCHANNAME", 0,
	ERR_NOPRIVILEGES, "ERR_NOPRIVILEGES", 0,
	ERR_CHANOPRIVSNEEDED, "ERR_CHANOPRIVSNEEDED", 0,
	ERR_CANTKILLSERVER, "ERR_CANTKILLSERVER", 0,
	ERR_DESYNC, "ERR_DESYNC", 0,
	ERR_ISCHANSERVICE, "ERR_ISCHANSERVICE", 0,
	ERR_UNIQOPPRIVSNEEDED, "ERR_UNIQOPPRIVSNEEDED", 0,
	ERR_CHANTOORECENT, "ERR_CHANTOORECENT", 0,
	ERR_TSLESSCHAN, "ERR_TSLESSCHAN", 0,
	ERR_VOICENEEDED, "ERR_VOICENEEDED", 0,
	ERR_NOOPERHOST, "ERR_NOOPERHOST", 0,
	ERR_NOSERVICEHOST, "ERR_NOSERVICEHOST", 0,
	ERR_UMODEUNKNOWNFLAG, "ERR_UMODEUNKNOWNFLAG", 0,
	ERR_USERSDONTMATCH, "ERR_USERSDONTMATCH", 0,
	ERR_GHOSTEDCLIENT, "ERR_GHOSTEDCLIENT", 0,
	ERR_LAST_ERR_MSG, "ERR_LAST_ERR_MSG", 0,
	ERR_SILELISTFULL, "ERR_SILELISTFULL", 0,
	ERR_NOSUCHGLINE, "ERR_NOSUCHGLINE", 0,
	ERR_TOOMANYWATCH, "ERR_TOOMANYWATCH", 0,
	ERR_BADPING, "ERR_BADPING", 0,
	ERR_TOOMANYDCC, "ERR_TOOMANYDCC", 0,
	ERR_LISTSYNTAX, "ERR_LISTSYNTAX", 0,
	ERR_WHOSYNTAX, "ERR_WHOSYNTAX", 0,
	ERR_WHOLIMEXCEED, "ERR_WHOLIMEXCEED", 0,
	RPL_LOGON, "RPL_LOGON", 0,
	RPL_LOGOFF, "RPL_LOGOFF", 0,
	RPL_WATCHOFF, "RPL_WATCHOFF", 0,
	RPL_WATCHSTAT, "RPL_WATCHSTAT", 0,
	RPL_NOWON, "RPL_NOWON", 0,
	RPL_NOWOFF, "RPL_NOWOFF", 0,
	RPL_WATCHLIST, "RPL_WATCHLIST", 0,
	RPL_ENDOFWATCHLIST, "RPL_ENDOFWATCHLIST", 0,
	RPL_DCCSTATUS, "RPL_DCCSTATUS", 0,
	RPL_DCCLIST, "RPL_DCCLIST", 0,
	RPL_ENDOFDCCLIST, "RPL_ENDOFDCCLIST", 0,
	RPL_DCCINFO, "RPL_DCCINFO", 0,
	ERR_NUMERIC_ERR, "ERR_NUMERIC_ERR", 0,
	0, 0, 0
};

void IRCSession::on_numeric_reply(const IRCNumericReply &message)
{
	our_nickname = IRCEntity::from_raw(message.get_param(0));

	bool invisible = false;
	int numeric = message.get_numeric();
	std::string numeric_name = clan::StringHelp::int_to_text(numeric);
	for (int i = 0; numeric_to_string[i].str != 0; i++)
	{
		if (numeric_to_string[i].numeric == numeric)
		{
			if (numeric_to_string[i].flags & n2s_hide_numeric)
				numeric_name.clear();
			else
				numeric_name = numeric_to_string[i].str;
			invisible = (numeric_to_string[i].flags & n2s_invisible) == n2s_invisible;
			break;
		}
	}

	if (!invisible)
	{
		std::string system_message;
		for (size_t i = 1; i < message.get_param_count(); i++)
		{
			if (!system_message.empty())
				system_message.append(" ");
			system_message += IRCText::from_raw(message.get_param(i)).get_text();
		}
		if (!numeric_name.empty())
			system_message += clan::string_format(" (%1)", numeric_name);
		if (numeric < 400)
			cb_system_text(IRCText::from_text(system_message));
		else
			cb_error_text(IRCText::from_text(system_message));
	}

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
	default:
		{
//			IRCRawString s = IRCMessage::create_line(message.get_prefix().to_raw(), message.get_command(), message.get_params());
//			cb_system_text(IRCText::from_raw(s.substr(0, s.length()-2)));
		}
		break;
	}
}

void IRCSession::on_rpl_welcome(const IRCNumericReply &message)
{
	reconnect_timeout = 2000 + rand() * 500 / RAND_MAX;

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
		catch (clan::Exception &e)
		{
			cb_system_text(IRCText::from_text(e.message));
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
			our_hostname = clan::StringHelp::utf8_to_text(param1.substr(pos+2));
			pos = our_hostname.find("@");
			if (pos != IRCRawString::npos)
				our_hostname = clan::StringHelp::trim(our_hostname.substr(pos+1));
		}
	}
}

void IRCSession::on_rpl_namereply(const IRCNumericReply &message)
{
	if (message.get_param_count() >= 4)
	{
		IRCChannel channel = IRCChannel::from_raw(message.get_param(2));
		std::string nicks = clan::StringHelp::utf8_to_text(message.get_param(3));
		std::vector<std::string> nick_list = clan::StringHelp::split_text(nicks, " ");

		IRCJoinedChannel status = get_channel_status(channel);

		for (unsigned int i=0; i<nick_list.size(); i++)
		{
			IRCNick nick = IRCNick::from_raw(clan::StringHelp::text_to_utf8(nick_list[i]));
			status.users.push_back(nick);
		}

		set_channel_status(status);
		cb_channel_names_updated(channel);
	}
}

void IRCSession::on_rpl_endofnames(const IRCNumericReply &message)
{
//	cb_channel_names_updated(channel);
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
		unsigned int timestamp = clan::StringHelp::local8_to_uint(message.get_param(3));

		IRCJoinedChannel status = get_channel_status(channel);
		status.topic_time = timestamp;
		status.topic_author = author;
		set_channel_status(status);
		cb_channel_topic_updated(channel);
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
	if (connect_status == status_connecting)
	{
		if (!(our_nickname == IRCNick::from_text(alt_connect_nick)))
			set_nick(alt_connect_nick);
	}
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

	cb_nick_changed(message.get_old_nick(), message.get_new_nick());
}

void IRCSession::on_join(const IRCJoinMessage &message)
{
	if (message.get_user() == our_nickname)
	{
		IRCJoinedChannel channel = get_channel_status(message.get_channel());
		channel.name = message.get_channel();
		channel.users.clear();
		set_channel_status(channel);

		cb_joined(message.get_channel());
	}
	else
	{
		IRCJoinedChannel channel = get_channel_status(message.get_channel());
		channel.name = message.get_channel();
		channel.users.push_back(message.get_user());
		set_channel_status(channel);
		cb_user_joined(message.get_channel(), message.get_user());
	}
}

void IRCSession::on_part(const IRCPartMessage &message)
{
	for (size_t i=0; i<message.get_channel_count(); i++)
	{
		if (message.get_user() == our_nickname)
			cb_parted(message.get_channel(i));
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

			cb_user_parted(message.get_channel(i), message.get_user(), IRCText());
		}
	}
}

void IRCSession::on_kick(const IRCKickMessage &message)
{
	IRCJoinedChannel channel = get_channel_status(message.get_channel());
	channel.name = message.get_channel();
	std::vector<IRCNick>::iterator it = std::find(channel.users.begin(), channel.users.end(), message.get_user());
	if(it != channel.users.end())
	{
		channel.users.erase(it);
		set_channel_status(channel);
	}

	if (message.has_comment())
		cb_user_kicked(message.get_user(), message.get_channel(), message.get_target(), message.get_comment());
	else
		cb_user_kicked(message.get_user(), message.get_channel(), message.get_target(), IRCText());
}

void IRCSession::on_quit(const IRCQuitMessage &message)
{
	IRCText text;
	if (message.has_text())
		text = message.get_text();
	cb_user_quit(message.get_user(), text);
}

void IRCSession::on_channel_mode(const IRCChannelModeMessage &message)
{
	size_t num_params = message.get_mode_parameter_count();
	std::vector<IRCRawString> params;
	params.push_back(message.get_mode_command());
	for (size_t i = 0; i < num_params; i++)
		params.push_back(message.get_mode_parameter(i));
	cb_channel_mode_change(message.get_executing_user(), message.get_channel(), params);
}

void IRCSession::on_nick_mode(const IRCNickModeMessage &message)
{
	std::vector<IRCRawString> params;
	params.push_back(message.get_mode_command());
	cb_channel_mode_change(message.get_executing_user(), message.get_target_user(), params);
}

void IRCSession::on_topic(const IRCTopicMessage &message)
{
	IRCJoinedChannel status = get_channel_status(message.get_channel());
	status.topic = message.get_topic();
	status.topic_author = message.get_prefix();
	set_channel_status(status);
	cb_channel_topic_updated(message.get_channel());
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
			cb_text(message.get_target(), message.get_sender(), message.get_text());
		else
			cb_text(message.get_sender(), message.get_sender(), message.get_text());
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
			cb_notice(message.get_target(), message.get_sender(), message.get_text());
		else
			cb_notice(message.get_sender(), message.get_sender(), message.get_text());
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
	IRCRawString s = IRCMessage::create_line(message.get_prefix().to_raw(), message.get_command(), message.get_params());
	cb_system_text(IRCText::from_raw(s.substr(0, s.length()-2)));
}

void IRCSession::on_disconnected(const std::string &reason)
{
	set_connect_status(status_disconnected);
	cb_error_text(IRCText::from_text(reason.empty() ? reason : clan::string_format("Disconnected (%1)", reason)));

	if (!user_requested_disconnect)
	{
		reconnect_timer.start(reconnect_timeout, false);
		reconnect_timeout = reconnect_timeout * 2 + rand() * 500 / RAND_MAX;
	}
}

void IRCSession::execute_command(const IRCEntity &filter, std::string command_line)
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
	if (clan::StringHelp::compare(command, "ACTION", true) == 0)
		on_ctcp_privmsg_action(data, message);
	else if (clan::StringHelp::compare(command, "DCC", true) == 0)
		on_ctcp_privmsg_dcc(data, message);
	else if (clan::StringHelp::compare(command, "FINGER", true) == 0)
		on_ctcp_privmsg_finger(data, message);
	else if (clan::StringHelp::compare(command, "VERSION", true) == 0)
		on_ctcp_privmsg_version(data, message);
	else if (clan::StringHelp::compare(command, "USERINFO", true) == 0)
		on_ctcp_privmsg_userinfo(data, message);
	else if (clan::StringHelp::compare(command, "CLIENTINFO", true) == 0)
		on_ctcp_privmsg_clientinfo(data, message);
	else if (clan::StringHelp::compare(command, "PING", true) == 0)
		on_ctcp_privmsg_ping(data, message);
	else if (clan::StringHelp::compare(command, "TIME", true) == 0)
		on_ctcp_privmsg_time(data, message);
}

void IRCSession::on_ctcp_notice(const IRCRawString &command, const IRCText &data, const IRCNoticeMessage &message)
{
	if (clan::StringHelp::compare(command, "FINGER", true) == 0)
		on_ctcp_notice_finger(data, message);
	else if (clan::StringHelp::compare(command, "VERSION", true) == 0)
		on_ctcp_notice_version(data, message);
	else if (clan::StringHelp::compare(command, "USERINFO", true) == 0)
		on_ctcp_notice_userinfo(data, message);
	else if (clan::StringHelp::compare(command, "CLIENTINFO", true) == 0)
		on_ctcp_notice_clientinfo(data, message);
	else if (clan::StringHelp::compare(command, "PING", true) == 0)
		on_ctcp_notice_ping(data, message);
	else if (clan::StringHelp::compare(command, "TIME", true) == 0)
		on_ctcp_notice_time(data, message);
}

void IRCSession::on_ctcp_privmsg_action(const IRCText &data, const IRCPrivateMessage &message)
{
	if (message.get_target().is_channel())
		cb_action(message.get_target(), message.get_prefix(), data);
	else
		cb_action(message.get_prefix(), message.get_prefix(), data);
}

void IRCSession::on_ctcp_privmsg_dcc(const IRCText &data, const IRCPrivateMessage &message)
{
	clan::Console::write_line(data.get_text());

	std::vector<std::string> tokens = split_command_line(data.get_text());
	if (tokens.size() >= 4 && tokens[0] == "SEND")
	{
		IRCFileOffer offer;
		offer.sender = message.get_prefix();

		unsigned int address = clan::StringHelp::text_to_uint(tokens[2]);
		std::string ip_address = clan::string_format(
			"%1.%2.%3.%4",
			clan::StringHelp::int_to_text((address>>24)&0xff),
			clan::StringHelp::int_to_text((address>>16)&0xff),
			clan::StringHelp::int_to_text((address>>8)&0xff),
			clan::StringHelp::int_to_text(address&0xff));
		std::string port = tokens[3];
		offer.host = clan::SocketName(ip_address, port);

		offer.size = 0;
		offer.size_provided = false;
		if (tokens.size() >= 5)
		{
			offer.size = clan::StringHelp::text_to_uint(tokens[4]);
			offer.size_provided = true;
		}

		offer.filename = clan::PathHelp::get_filename(tokens[1]);
		cb_dcc_file_offer(offer);
	}
	else if (tokens.size() == 4 && tokens[0] == "CHAT" && tokens[1] == "chat")
	{
		std::string ip_address = tokens[2];
		std::string port = tokens[3];
		clan::SocketName host(ip_address, port);
		cb_dcc_chat_offer(message.get_prefix(), host);
	}
}

std::vector<std::string> IRCSession::split_command_line(std::string command_line)
{
	std::vector<std::string> args;
	bool new_arg = true;

	bool in_quoted_string = false;
	bool in_escape = false;
	std::string arg;
	for (size_t i = 0; i < command_line.length(); i++)
	{
		if (in_escape)
		{
			arg.push_back(command_line[i]);
			in_escape = false;
		}
		else if (command_line[i] == '\\')
		{
			in_escape = true;
		}
		else if (command_line[i] == '"')
		{
			in_quoted_string = !in_quoted_string;
		}
		else if (in_quoted_string || command_line[i] != ' ')
		{
			arg.push_back(command_line[i]);
		}
		else if (!arg.empty())
		{
			args.push_back(arg);
			arg.clear();
		}
	}
	if (!arg.empty())
		args.push_back(arg);
	return args;
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
		add_line(clan::string_format("%1 FINGER REPLY: %2", clan::IRCConnection::extract_nick(nick), data.substr(1)), color_text);
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
		add_line(clan::string_format("%1 USERINFO REPLY: %2", clan::IRCConnection::extract_nick(nick), data.substr(1)), color_text);
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
		add_line(clan::string_format("%1 CLIENTINFO REPLY: %2", clan::IRCConnection::extract_nick(nick), data.substr(1)), color_text);
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
		add_line(clan::string_format("%1 TIME REPLY: %2", clan::IRCConnection::extract_nick(nick), data.substr(1)), color_text);
	}
*/
}

void IRCSession::on_ctcp_notice_finger(const IRCText &data, const IRCNoticeMessage &message)
{

}

void IRCSession::on_ctcp_notice_version(const IRCText &data, const IRCNoticeMessage &message)
{
/*
	add_line(clan::string_format("%1 VERSION REPLY: %2", clan::IRCConnection::extract_nick(nick), data), color_text);
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
//	add_line(clan::string_format("%1 PING: %2 ms", clan::IRCConnection::extract_nick(prefix), clan::StringHelp::int_to_text(clan::System::get_time()-clan::StringHelp::text_to_int(data))), color_text);
}

void IRCSession::on_ctcp_notice_time(const IRCText &data, const IRCNoticeMessage &message)
{

}
/*
void Chat::on_connection_mode(const IRCRawString &prefix, const IRCRawString &receiver, const IRCRawString &mode, const std::vector<IRCRawString> &params)
{
	if (matches_filter(receiver) || receiver.substr(0,1) != "#")
		add_line(clan::string_format("%1 sets mode %3 on %2", clan::IRCConnection::extract_nick(prefix), clan::IRCConnection::extract_nick(receiver), mode), color_channel);
}

void Chat::on_connection_invite(const IRCRawString &prefix, const IRCRawString &nick, const IRCRawString &channel)
{
	add_line(clan::string_format("%1 invites %2 to %3", clan::IRCConnection::extract_nick(prefix), clan::IRCConnection::extract_nick(nick), channel), color_channel);
}

void Chat::on_connection_kick(const IRCRawString &prefix, const IRCRawString &channel, const IRCRawString &user, const IRCRawString &comment)
{
	if (matches_filter(channel) || filter.empty())
	{
		add_line(clan::string_format("%1 kicked %3 from %2 (%4)", clan::IRCConnection::extract_nick(prefix), channel, clan::IRCConnection::extract_nick(user), comment), color_channel);
	}
}
*/
