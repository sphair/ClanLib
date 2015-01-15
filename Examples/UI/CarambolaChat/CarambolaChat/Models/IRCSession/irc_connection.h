
#pragma once

#include "irc_entity.h"
#include "irc_connection_queues.h"

class IRCMessage;
class IRCText;

class IRCConnection
{
public:
	IRCConnection();
	~IRCConnection();

	void connect(const clan::SocketName &server);
	void disconnect();

	void send_command(const IRCRawString &command, const std::vector<IRCRawString> params);

	void send_pass(const IRCRawString &password);
	void send_nick(const IRCNick &nickname);
	void send_user(const IRCRawString &username, const IRCRawString &hostname, const IRCRawString &servername, const IRCRawString &realname);
	void send_oper(const IRCRawString &user, const IRCRawString &password);
	void send_quit(const IRCText &message);
	void send_join(const IRCChannel &channel, const IRCRawString &password = IRCRawString());
	void send_part(const IRCChannel &channel);
	void send_channel_mode(const IRCChannel &channel, const IRCRawString &mode_command, const std::vector<IRCRawString> &mode_params);
	void send_nick_mode(const IRCNick &nickname, const IRCRawString &mode_command);
	void send_topic(const IRCChannel &channel, const IRCText &topic);
	void send_names(const IRCChannel &channel); // Request (RPL_NAMREPLY, RPL_ENDOFNAMES) for a channel
	void send_list(const IRCChannel &channel); // Request (RPL_LISTSTART, RPL_LIST, RPL_LISTEND) for a channel
	void send_invite(const IRCNick &nickname, const IRCChannel &channel);
	void send_kick(const IRCChannel &channel, const IRCNick &nickname, const IRCText &comment);

	void send_privmsg(const IRCEntity &receiver, const IRCText &text);
	void send_notice(const IRCEntity &receiver, const IRCText &text);

	void send_who(const IRCEntity &name_search, bool only_operators);
	void send_whois(const IRCEntity &name_search);
	void send_whowas(const IRCEntity &name);

	void send_kill(const IRCEntity &target, const IRCText &comment);

	void send_ping(const IRCServer &server, const IRCServer &server2 = IRCServer());
	void send_pong(const IRCRawString &daemon1, const IRCRawString &daemon2 = IRCRawString());

	// on_message_received(const IRCMessage &message)
	std::function<void(const IRCMessage &)> &func_message_received();

	// on_disconnected(const std::string &reason)
	std::function<void(const std::string &)> &func_disconnected();

private:
	void process();

	void worker_main();
	bool read_connection_data(clan::TCPConnection &connection, IRCRawString &read_line);
	void write_connection_data(IRCRawString &write_line, IRCRawString::size_type &write_pos, clan::TCPConnection &connection);

	clan::SocketName server;
	std::thread thread_worker;
	std::mutex mutex;
	clan::NetworkConditionVariable change_event;
	bool stop_flag = false;
	IRCConnectionQueues queues;

	std::function<void(const IRCMessage &)> cb_message_received;
	std::function<void(const std::string &)> cb_disconnected;
};
