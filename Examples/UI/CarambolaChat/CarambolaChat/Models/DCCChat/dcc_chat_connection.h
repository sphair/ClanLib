
#pragma once

#include "../IRCSession/irc_text.h"

class DCCChatConnection : public clan::KeepAliveObject
{
public:
	DCCChatConnection(const std::string &local_nick, const std::string &remote_nick, const clan::SocketName &socket_name);
	~DCCChatConnection();

	void connect();
	void offer();
	void send_line(const IRCText &text);
	void disconnect();

	std::string local_nick;
	std::string remote_nick;
	clan::Signal<void(const std::string &)> sig_system_text;
	clan::Signal<void(const std::string &)> sig_disconnected;
	clan::Signal<void(const IRCText &)> sig_text_line;

private:
	void process();
	void worker_main();
	bool read_connection_data(clan::TCPConnection &connection, IRCRawString &read_line);
	bool write_connection_data(IRCRawString &write_line, IRCRawString::size_type &write_pos, clan::TCPConnection &connection);
	void queue_system(const IRCRawString &text);
	void queue_disconnected(const IRCRawString &reason);
	void queue_line(const IRCRawString &line);

	struct Message
	{
		enum Type
		{
			type_system,
			type_text,
			type_disconnect
		};
		Message(Type type, IRCRawString text) : type(type), text(text) { }
		Type type;
		IRCRawString text;
	};

	clan::SocketName socket_name;
	std::thread thread;
	clan::Event stop_event;
	clan::Event send_event;
	clan::Event started_event;
	bool is_server;
	std::recursive_mutex mutex;
	std::vector<Message> send_queue;
	std::vector<Message> receive_queue;
};
