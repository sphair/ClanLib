
#include "precomp.h"
#include "irc_connection.h"
#include "irc_message.h"

IRCConnection::IRCConnection()
{
}

IRCConnection::~IRCConnection()
{
	disconnect_abortive();
}

void IRCConnection::connect(const clan::SocketName &new_server)
{
	disconnect_abortive();
	stop_event.reset();
	queues.reset();
	shutdown_graceful.set(0);
	server = new_server;
	
	thread_worker.start(this, &IRCConnection::worker_main);
}

void IRCConnection::disconnect_graceful()
{
	shutdown_graceful.set(1);
	/*while (queues.receive_event.wait(2000))
	{
		queues.pop_received();
		std::string reason;
		if (queues.pop_disconnected(reason))
			break;
	}
	shutdown_graceful.set(0);*/
	stop_event.set();
	thread_worker.join();
}

void IRCConnection::disconnect_abortive()
{
	shutdown_graceful.set(0);
	stop_event.set();
	thread_worker.join();
}

void IRCConnection::send_command(const IRCRawString &command, const std::vector<IRCRawString> params)
{
	IRCRawString line = IRCMessage::create_line(IRCRawString(), command, params);
	queues.push_send(line);
}

void IRCConnection::send_pass(const IRCRawString &password)
{
	std::vector<IRCRawString> params;
	params.push_back(password);
	send_command("PASS", params);
}

void IRCConnection::send_nick(const IRCNick &nickname)
{
	std::vector<IRCRawString> params;
	params.push_back(nickname.to_raw());
	send_command("NICK", params);
}

void IRCConnection::send_user(const IRCRawString &username, const IRCRawString &hostname, const IRCRawString &servername, const IRCRawString &realname)
{
	std::vector<IRCRawString> params;
	params.push_back(username);
	params.push_back(hostname);
	params.push_back(servername);
	params.push_back(realname);
	send_command("USER", params);
}

void IRCConnection::send_oper(const IRCRawString &user, const IRCRawString &password)
{
	std::vector<IRCRawString> params;
	params.push_back(user);
	params.push_back(password);
	send_command("OPER", params);
}

void IRCConnection::send_quit(const IRCText &message)
{
	std::vector<IRCRawString> params;
	params.push_back(message.to_raw());
	send_command("QUIT", params);
}

void IRCConnection::send_join(const IRCChannel &channel, const IRCRawString &password)
{
	std::vector<IRCRawString> params;
	params.push_back(channel.to_raw());
	if (!password.empty())
		params.push_back(password);
	send_command("JOIN", params);
}

void IRCConnection::send_part(const IRCChannel &channel)
{
	std::vector<IRCRawString> params;
	params.push_back(channel.to_raw());
	send_command("PART", params);
}

void IRCConnection::send_channel_mode(const IRCChannel &channel, const IRCRawString &mode_command, const std::vector<IRCRawString> &mode_params)
{
	std::vector<IRCRawString> params;
	params.push_back(channel.to_raw());
	params.push_back(mode_command);
	params.insert(params.end(), mode_params.begin(), mode_params.end());
	send_command("MODE", params);
}

void IRCConnection::send_nick_mode(const IRCNick &nickname, const IRCRawString &mode_command)
{
	std::vector<IRCRawString> params;
	params.push_back(nickname.to_raw());
	params.push_back(mode_command);
	send_command("MODE", params);
}

void IRCConnection::send_topic(const IRCChannel &channel, const IRCText &topic)
{
	std::vector<IRCRawString> params;
	params.push_back(channel.to_raw());
	params.push_back(topic.to_raw());
	send_command("TOPIC", params);
}

void IRCConnection::send_names(const IRCChannel &channel)
{
	std::vector<IRCRawString> params;
	params.push_back(channel.to_raw());
	send_command("NAMES", params);
}

void IRCConnection::send_list(const IRCChannel &channel)
{
	std::vector<IRCRawString> params;
	params.push_back(channel.to_raw());
	send_command("LIST", params);
}

void IRCConnection::send_invite(const IRCNick &nickname, const IRCChannel &channel)
{
	std::vector<IRCRawString> params;
	params.push_back(nickname.to_raw());
	params.push_back(channel.to_raw());
	send_command("INVITE", params);
}

void IRCConnection::send_kick(const IRCChannel &channel, const IRCNick &nickname, const IRCText &comment)
{
	std::vector<IRCRawString> params;
	params.push_back(channel.to_raw());
	params.push_back(nickname.to_raw());
	params.push_back(comment.to_raw());
	send_command("KICK", params);
}

void IRCConnection::send_privmsg(const IRCEntity &receiver, const IRCText &text)
{
	std::vector<IRCRawString> params;
	params.push_back(receiver.to_raw());
	params.push_back(text.to_raw());
	send_command("PRIVMSG", params);
}

void IRCConnection::send_notice(const IRCEntity &receiver, const IRCText &text)
{
	std::vector<IRCRawString> params;
	params.push_back(receiver.to_raw());
	params.push_back(text.to_raw());
	send_command("PRIVMSG", params);
}

void IRCConnection::send_who(const IRCEntity &name_search, bool only_operators)
{
	std::vector<IRCRawString> params;
	params.push_back(name_search.to_raw());
	if (only_operators)
		params.push_back("o");
	send_command("WHO", params);
}

void IRCConnection::send_whois(const IRCEntity &name_search)
{
	std::vector<IRCRawString> params;
	params.push_back(name_search.to_raw());
	send_command("WHOIS", params);
}

void IRCConnection::send_whowas(const IRCEntity &name)
{
	std::vector<IRCRawString> params;
	params.push_back(name.to_raw());
	send_command("WHOWAS", params);
}

void IRCConnection::send_kill(const IRCEntity &target, const IRCText &comment)
{
	std::vector<IRCRawString> params;
	params.push_back(target.to_raw());
	if (!comment.empty())
		params.push_back(comment.to_raw());
	send_command("KILL", params);
}

void IRCConnection::send_ping(const IRCServer &server, const IRCServer &server2)
{
	std::vector<IRCRawString> params;
	params.push_back(server.to_raw());
	if (!server2.empty())
		params.push_back(server2.to_raw());
	send_command("PING", params);
}

void IRCConnection::send_pong(const IRCRawString &daemon1, const IRCRawString &daemon2)
{
	std::vector<IRCRawString> params;
	params.push_back(daemon1);
	if (!daemon2.empty())
		params.push_back(daemon2);
	send_command("PONG", params);
}

std::function<void(const IRCMessage &)> &IRCConnection::func_message_received()
{
	return cb_message_received;
}

std::function<void(const std::string &)> &IRCConnection::func_disconnected()
{
	return cb_disconnected;
}

void IRCConnection::process()
{
	while (true)
	{
		IRCRawString line = queues.pop_received();
		if (line.empty())
			break;

		IRCMessage message = IRCMessage::parse_line(line);
		if (cb_message_received)
			cb_message_received(message);
	}

	std::string reason;
	bool was_disconnected = queues.pop_disconnected(reason);
	if (was_disconnected && cb_disconnected)
		cb_disconnected(reason);
}

void IRCConnection::worker_main()
{
	try
	{
		//clan::ConsoleWindow console("Debug");
		clan::TCPConnection connection(server);
		connection.set_nodelay(true);
		connection.set_keep_alive(true, 60*1000, 60*1000);

		clan::Event read_event = connection.get_read_event();
		clan::Event write_event = connection.get_write_event();

		IRCRawString read_line, write_line;
		IRCRawString::size_type write_pos = 0;

		std::vector<clan::Event> wait_events;
		bool continue_loop = true;
		while (continue_loop)
		{
			wait_events.clear();
			wait_events.push_back(read_event);

			if (write_line.length() != write_pos)
				wait_events.push_back(write_event);
			else
				wait_events.push_back(queues.send_event);

			wait_events.push_back(stop_event);

			int wakeup_reason = clan::Event::wait(wait_events, -1);
			if (wakeup_reason == 0) // read_event flagged
			{
				continue_loop = read_connection_data(connection, read_line);
			}
			else if (wakeup_reason == 1) // write_event / queues.send_event flagged
			{
				continue_loop = write_connection_data(write_line, write_pos, connection);
			}
			else // stop_event flagged
			{
				continue_loop = false;
			}
		}

		queues.set_disconnected(std::string());
		set_wakeup_event();
	}
	catch (clan::Exception &e)
	{
		queues.set_disconnected(e.message);
		set_wakeup_event();
	}
}

bool IRCConnection::read_connection_data(clan::TCPConnection &connection, IRCRawString &read_line)
{
	char buffer[16*1024];
	int data_read = connection.read(buffer, 16*1024, false);
	if (data_read == 0) // EOF from server
	{
		connection.disconnect_graceful();
		return false;
	}
	int start_pos = 0;
	for (int i=0; i<data_read; i++)
	{
		if (buffer[i] == '\n')
		{
			read_line.append(buffer+start_pos, i-start_pos+1);
			start_pos = i+1;
			queues.push_received(read_line);
			set_wakeup_event();
			read_line.clear();
		}
	}
	read_line.append(buffer+start_pos, data_read-start_pos);
	return true;
}

bool IRCConnection::write_connection_data(IRCRawString &write_line, IRCRawString::size_type &write_pos, clan::TCPConnection &connection)
{
	if (write_line.length() != write_pos)
	{
		//int old_pos = write_pos;
		write_pos += connection.write(write_line.data()+write_pos, write_line.length()-write_pos);
		//clan::Console::write_line(std::string8(write_line.data()+old_pos, write_pos-old_pos, false));
	}
	else
	{
		write_line = queues.pop_send();
		write_pos = 0;
	}
	return true;
}
