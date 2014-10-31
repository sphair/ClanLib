
#include "precomp.h"
#include "dcc_chat_connection.h"

DCCChatConnection::DCCChatConnection(const std::string &local_nick, const std::string &remote_nick, const clan::SocketName &socket_name)
: local_nick(local_nick), remote_nick(remote_nick), socket_name(socket_name), is_server(false)
{
}

DCCChatConnection::~DCCChatConnection()
{
	disconnect();
}

void DCCChatConnection::connect()
{
	is_server = false;
	thread.start(this, &DCCChatConnection::worker_main);
}

void DCCChatConnection::offer()
{
	is_server = true;
	thread.start(this, &DCCChatConnection::worker_main);
	started_event.wait();
}

void DCCChatConnection::disconnect()
{
	stop_event.set();
	thread.join();
}

void DCCChatConnection::send_line(const IRCText &text)
{
	clan::MutexSection mutex_lock(&mutex);
	send_queue.push_back(Message(Message::type_text, text.to_raw()));
	mutex_lock.unlock();
	send_event.set();
}

void DCCChatConnection::process()
{
	std::vector<Message> received_messages;
	clan::MutexSection mutex_lock(&mutex);
	received_messages.swap(receive_queue);
	mutex_lock.unlock();

	for (size_t i = 0; i < received_messages.size(); i++)
	{
		switch (received_messages[i].type)
		{
		case Message::type_system:
			sig_system_text(received_messages[i].text);
			break;
		case Message::type_text:
			{
				IRCRawString text = received_messages[i].text;
				if (text.length() >= 2 && text.substr(text.length() - 2, 2) == "\r\n")
					text = text.substr(0, text.length() - 2);
				else if (text.length() >= 1 && text[text.length() - 1] == '\n')
					text = text.substr(0, text.length() - 1);
				sig_text_line(IRCText::from_raw(text));
			}
			break;
		case Message::type_disconnect:
			sig_disconnected(received_messages[i].text);
			break;
		}
	}
}

void DCCChatConnection::worker_main()
{
	try
	{
		clan::TCPConnection connection;
		if (is_server)
		{
			queue_system(clan::string_format("Started listening on port %1", socket_name.get_port()));
			clan::TCPListen listen(socket_name);
			started_event.set();
			int wakeup_reason = clan::Event::wait(listen.get_accept_event(), stop_event);
			if (wakeup_reason != 0)
			{
				queue_disconnected(IRCRawString());
				return;
			}
			connection = listen.accept();
			clan::SocketName remote_name = connection.get_remote_name();
			try
			{
				queue_system(clan::string_format("Accepted connection from %1 port %2", remote_name.lookup_hostname(), remote_name.get_port()));
			}
			catch (clan::Exception &)
			{
				queue_system(clan::string_format("Accepted connection from %1 port %2", remote_name.get_address(), remote_name.get_port()));
			}
		}
		else
		{
			try
			{
				queue_system(clan::string_format("Connecting to %1 port %2", socket_name.lookup_hostname(), socket_name.get_port()));
			}
			catch (clan::Exception &)
			{
				queue_system(clan::string_format("Connecting to %1 port %2", socket_name.get_address(), socket_name.get_port()));
			}
			connection = clan::TCPConnection(socket_name);
			started_event.set();
		}

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
				wait_events.push_back(send_event);

			wait_events.push_back(stop_event);

			int wakeup_reason = clan::Event::wait(wait_events, -1);
			if (wakeup_reason == 0) // read_event flagged
			{
				continue_loop = read_connection_data(connection, read_line);
			}
			else if (wakeup_reason == 1) // write_event / send_event flagged
			{
				continue_loop = write_connection_data(write_line, write_pos, connection);
			}
			else // stop_event flagged
			{
				continue_loop = false;
			}
		}

		queue_disconnected(IRCRawString());
	}
	catch (clan::Exception &e)
	{
		started_event.set();
		queue_disconnected(e.message);
	}
}

bool DCCChatConnection::read_connection_data(clan::TCPConnection &connection, IRCRawString &read_line)
{
	char buffer[16*1024];
	int data_read = connection.read(buffer, 16*1024, false);
	if (data_read == 0) // EOF from server
	{
		//if (shutdown_graceful.get() == 0)
		//	throw clan::Exception("Unexpected EOF from server");
		return false;
	}
	int start_pos = 0;
	for (int i=0; i<data_read; i++)
	{
		if (buffer[i] == '\n')
		{
			read_line.append(buffer+start_pos, i-start_pos+1);
			start_pos = i+1;
			queue_line(read_line);
			read_line.clear();
		}
	}
	read_line.append(buffer+start_pos, data_read-start_pos);
	return true;
}

bool DCCChatConnection::write_connection_data(IRCRawString &write_line, IRCRawString::size_type &write_pos, clan::TCPConnection &connection)
{
	if (write_line.length() != write_pos)
	{
		write_pos += connection.write(write_line.data()+write_pos, write_line.length()-write_pos);
	}
	else
	{
		clan::MutexSection mutex_lock(&mutex);
		write_line.clear();
		write_pos = 0;
		if (!send_queue.empty())
		{
			if (send_queue.front().type == Message::type_disconnect)
			{
				send_queue.erase(send_queue.begin());
				return false;
			}
			else
			{
				write_line = send_queue.front().text + "\r\n";
				send_queue.erase(send_queue.begin());
			}
		}
	}
	return true;
}

void DCCChatConnection::queue_system(const IRCRawString &text)
{
	clan::MutexSection mutex_lock(&mutex);
	receive_queue.push_back(Message(Message::type_system, text));
	mutex_lock.unlock();
	set_wakeup_event();
}

void DCCChatConnection::queue_disconnected(const IRCRawString &reason)
{
	clan::MutexSection mutex_lock(&mutex);
	receive_queue.push_back(Message(Message::type_disconnect, reason));
	mutex_lock.unlock();
	set_wakeup_event();
}

void DCCChatConnection::queue_line(const IRCRawString &line)
{
	clan::MutexSection mutex_lock(&mutex);
	receive_queue.push_back(Message(Message::type_text, line));
	mutex_lock.unlock();
	set_wakeup_event();
}
