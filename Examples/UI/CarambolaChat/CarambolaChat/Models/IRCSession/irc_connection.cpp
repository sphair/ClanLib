
#include "precomp.h"
#include "irc_connection.h"
#include "irc_message.h"

IRCConnection::IRCConnection()
{
}

IRCConnection::~IRCConnection()
{
	disconnect();
}

void IRCConnection::connect(const clan::SocketName &new_server)
{
	disconnect();

	std::unique_lock<std::mutex> lock(mutex);
	stop_flag = false;
	queues.reset();
	server = new_server;
	lock.unlock();
	change_event.notify();

	thread_worker = std::thread(&IRCConnection::worker_main, this);
}

void IRCConnection::disconnect()
{
	if (!thread_worker.joinable())
		return;

	std::unique_lock<std::mutex> lock(mutex);
	stop_flag = true;
	lock.unlock();
	change_event.notify();

	thread_worker.join();
}

void IRCConnection::send_command(const IRCRawString &command, const std::vector<IRCRawString> params)
{
	std::unique_lock<std::mutex> lock(mutex);
	IRCRawString line = IRCMessage::create_line(IRCRawString(), command, params);
	queues.push_send(line);
	lock.unlock();
	change_event.notify();
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
		std::unique_lock<std::mutex> lock(mutex);

		IRCRawString line = queues.pop_received();
		if (line.empty())
			break;

		lock.unlock();

		IRCMessage message = IRCMessage::parse_line(line);
		if (cb_message_received)
			cb_message_received(message);
	}

	std::unique_lock<std::mutex> lock(mutex);
	std::string reason;
	bool was_disconnected = queues.pop_disconnected(reason);
	lock.unlock();

	if (was_disconnected && cb_disconnected)
		cb_disconnected(reason);
}

void IRCConnection::worker_main()
{
	try
	{
		clan::TCPConnection connection(server);

		IRCRawString read_line, write_line;
		IRCRawString::size_type write_pos = 0;

		std::unique_lock<std::mutex> lock(mutex);

		while (true)
		{
			if (stop_flag)
				break;

			if (!read_connection_data(connection, read_line))
				break;

			write_connection_data(write_line, write_pos, connection);

			clan::NetworkEvent *events[] = { &connection };
			change_event.wait(lock, 1, events);
		}

		queues.set_disconnected(std::string());
		clan::RunLoop::main_thread_async(clan::bind_member(this, &IRCConnection::process));
	}
	catch (clan::Exception &e)
	{
		queues.set_disconnected(e.message);
		clan::RunLoop::main_thread_async(clan::bind_member(this, &IRCConnection::process));
	}
}

bool IRCConnection::read_connection_data(clan::TCPConnection &connection, IRCRawString &read_line)
{
	while (true)
	{
		char buffer[16 * 1024];
		int data_read = connection.read(buffer, 16 * 1024);
		if (data_read == -1)
			return true;
		else if (data_read == 0) // EOF from server
			return false;

		int start_pos = 0;
		for (int i = 0; i < data_read; i++)
		{
			if (buffer[i] == '\n')
			{
				read_line.append(buffer + start_pos, i - start_pos + 1);
				start_pos = i + 1;
				queues.push_received(read_line);

				clan::RunLoop::main_thread_async(clan::bind_member(this, &IRCConnection::process));

				read_line.clear();
			}
		}
		read_line.append(buffer + start_pos, data_read - start_pos);
	}
}

void IRCConnection::write_connection_data(IRCRawString &write_line, IRCRawString::size_type &write_pos, clan::TCPConnection &connection)
{
	while (true)
	{
		if (write_line.length() != write_pos)
		{
			int result = connection.write(write_line.data() + write_pos, write_line.length() - write_pos);
			if (result == -1)
				break;
			write_pos += result;
		}
		else
		{
			write_line = queues.pop_send();
			write_pos = 0;

			if (write_line.empty())
				break;
		}
	}
}
