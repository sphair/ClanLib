
#pragma once

#include "irc_connection.h"
#include "irc_text.h"
#include "irc_message.h"

class IRCJoinedChannel
{
public:
	IRCJoinedChannel() : topic_time(0) { }

	IRCChannel name;
	std::vector<IRCNick> users;
	IRCText topic;
	IRCNick topic_author;
	unsigned int topic_time;
};

class IRCSession
{
public:
	IRCSession();
	~IRCSession();

	void set_perform_list(const std::vector<CL_String> &perform_list);

	void connect(const CL_String &server, const CL_String &port, const CL_String &nick, const CL_String &username, const CL_String &full_name);
	void disconnect_graceful();
	void disconnect_abortive();

	enum ConnectStatus
	{
		status_connecting,
		status_connected,
		status_disconnected
	};

	ConnectStatus get_connect_status() const { return connect_status; }

	void set_connection_name(const CL_String &connection_name);
	CL_String get_connection_name() const { return connection_name; }

	void set_nick(const CL_String &nick);
	IRCNick get_nick() const;

	CL_String get_out_hostname() const;

	void join(const IRCChannel &channel);
	void part(const IRCChannel &channel);

	IRCJoinedChannel get_channel_status(const IRCChannel &channel);

	void send_text(const IRCEntity &target, const IRCText &text);
	void send_notice(const IRCEntity &target, const IRCText &text);

	void set_topic(const IRCChannel &channel, const IRCText &text);

	void execute_command(const IRCEntity &filter, CL_String command_line);

	CL_Signal_v2<const IRCNick &, const IRCNick &> cb_nick_changed;
	CL_Signal_v1<const IRCChannel &> cb_joined;
	CL_Signal_v1<const IRCChannel &> cb_parted;
	CL_Signal_v2<const IRCChannel &, const IRCNick &> cb_user_joined;
	CL_Signal_v3<const IRCChannel &, const IRCNick &, const IRCText &> cb_user_parted;
	CL_Signal_v2<const IRCNick &, const IRCText &> cb_user_quit;

	CL_Signal_v3<const IRCChannel &, const IRCNick &, const IRCText &> cb_channel_text;
	CL_Signal_v3<const IRCChannel &, const IRCNick &, const IRCText &> cb_channel_notice;
	CL_Signal_v3<const IRCChannel &, const IRCNick &, const IRCText &> cb_channel_action;
	CL_Signal_v2<const IRCNick &, const IRCText &> cb_private_text;
	CL_Signal_v2<const IRCNick &, const IRCText &> cb_private_notice;
	CL_Signal_v2<const IRCNick &, const IRCText &> cb_private_action;
	CL_Signal_v1<const IRCText &> cb_error_text;
	CL_Signal_v1<const IRCText &> cb_system_text;

	CL_Signal_v3<const IRCNick &, const IRCChannel &, const std::vector<IRCRawString> &> cb_channel_mode_change;
	CL_Signal_v3<const IRCNick &, const IRCNick &, const std::vector<IRCRawString> &> cb_nick_mode_change;

	CL_Signal_v1<const IRCChannel &> cb_channel_topic_updated;
	CL_Signal_v1<const IRCChannel &> cb_channel_names_updated;

	CL_Signal_v1<ConnectStatus> cb_connect_status_changed;

private:
	void set_connect_status(ConnectStatus new_status);

	void set_channel_status(const IRCJoinedChannel &status);

	void on_message_received(const IRCMessage &message);
	void on_disconnected(const CL_String &reason);
	void on_numeric_reply(const IRCNumericReply &message);
	void on_rpl_welcome(const IRCNumericReply &message);
	void on_rpl_userhost(const IRCNumericReply &message);
	void on_rpl_namereply(const IRCNumericReply &message);
	void on_rpl_endofnames(const IRCNumericReply &message);
	void on_rpl_topic(const IRCNumericReply &message);
	void on_rpl_topicwhotime(const IRCNumericReply &message);
	void on_rpl_notopic(const IRCNumericReply &message);
	void on_err_nicknameinuse(const IRCNumericReply &message);
	void on_nick(const IRCNickMessage &message);
	void on_join(const IRCJoinMessage &message);
	void on_part(const IRCPartMessage &message);
	void on_quit(const IRCQuitMessage &message);
	void on_channel_mode(const IRCChannelModeMessage &message);
	void on_nick_mode(const IRCNickModeMessage &message);
	void on_topic(const IRCTopicMessage &message);
	void on_privmsg(const IRCPrivateMessage &message);
	void on_notice(const IRCNoticeMessage &message);
	void on_ping(const IRCPingMessage &message);
	void on_unknown_message(const IRCMessage &message);

	void extract_ctcp_command(const IRCText &ctcp_data, IRCRawString &command, IRCRawString &data);
	void on_ctcp_privmsg(const IRCRawString &command, const IRCText &data, const IRCPrivateMessage &message);
	void on_ctcp_notice(const IRCRawString &command, const IRCText &data, const IRCNoticeMessage &message);

	void on_ctcp_privmsg_action(const IRCText &data, const IRCPrivateMessage &message);
	void on_ctcp_privmsg_dcc(const IRCText &data, const IRCPrivateMessage &message);
	void on_ctcp_privmsg_finger(const IRCText &data, const IRCPrivateMessage &message);
	void on_ctcp_privmsg_version(const IRCText &data, const IRCPrivateMessage &message);
	void on_ctcp_privmsg_userinfo(const IRCText &data, const IRCPrivateMessage &message);
	void on_ctcp_privmsg_clientinfo(const IRCText &data, const IRCPrivateMessage &message);
	void on_ctcp_privmsg_ping(const IRCText &data, const IRCPrivateMessage &message);
	void on_ctcp_privmsg_time(const IRCText &data, const IRCPrivateMessage &message);

	void on_ctcp_notice_finger(const IRCText &data, const IRCNoticeMessage &message);
	void on_ctcp_notice_version(const IRCText &data, const IRCNoticeMessage &message);
	void on_ctcp_notice_userinfo(const IRCText &data, const IRCNoticeMessage &message);
	void on_ctcp_notice_clientinfo(const IRCText &data, const IRCNoticeMessage &message);
	void on_ctcp_notice_ping(const IRCText &data, const IRCNoticeMessage &message);
	void on_ctcp_notice_time(const IRCText &data, const IRCNoticeMessage &message);

	IRCNick our_nickname;
	CL_String our_hostname;
	std::vector<IRCJoinedChannel> channels;
	IRCConnection connection;
	ConnectStatus connect_status;
	std::vector<CL_String> perform_list;

	CL_String connection_name;

	friend class Command;
};
