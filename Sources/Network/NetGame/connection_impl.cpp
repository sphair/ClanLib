/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Mark Page
*/

#include "Network/precomp.h"
#include "API/Network/NetGame/connection.h"
#include "API/Network/NetGame/connection_site.h"
#include "API/Core/System/databuffer.h"
#include "network_event.h"
#include "network_data.h"
#include "connection_impl.h"

namespace clan
{
	NetGameConnection_Impl::NetGameConnection_Impl()
	{
	}

	void NetGameConnection_Impl::start(NetGameConnection *xbase, NetGameConnectionSite *xsite, const TCPConnection &xconnection)
	{
		base = xbase;
		site = xsite;
		connection = xconnection;
		socket_name = connection.get_remote_name();
		is_connected = true;
		thread = std::thread(&NetGameConnection_Impl::connection_main, this);
	}

	void NetGameConnection_Impl::start(NetGameConnection *xbase, NetGameConnectionSite *xsite, const SocketName &xsocket_name)
	{
		base = xbase;
		site = xsite;
		socket_name = xsocket_name;
		is_connected = false;
		thread = std::thread(&NetGameConnection_Impl::connection_main, this);
	}

	NetGameConnection_Impl::~NetGameConnection_Impl()
	{
		std::unique_lock<std::mutex> mutex_lock(mutex);
		stop_flag = true;
		mutex_lock.unlock();
		worker_event.notify();
		if (thread.joinable())
			thread.join();
	}

	void NetGameConnection_Impl::set_data(const std::string &name, void *new_data)
	{
		for (auto & elem : data)
		{
			if (elem.name == name)
			{
				elem.data = new_data;
				return;
			}
		}
		AttachedData d;
		d.name = name;
		d.data = new_data;
		data.push_back(d);
	}

	void *NetGameConnection_Impl::get_data(const std::string &name) const
	{
		for (const auto & elem : data)
		{
			if (elem.name == name)
				return elem.data;
		}
		return nullptr;
	}

	void NetGameConnection_Impl::send_event(const NetGameEvent &game_event)
	{
		std::unique_lock<std::mutex> mutex_lock(mutex);
		Message message;
		message.type = Message::type_message;
		message.event = game_event;
		send_queue.push_back(message);
		mutex_lock.unlock();
		worker_event.notify();
	}

	void NetGameConnection_Impl::disconnect()
	{
		std::unique_lock<std::mutex> mutex_lock(mutex);
		Message message;
		message.type = Message::type_disconnect;
		send_queue.push_back(message);
		mutex_lock.unlock();
		worker_event.notify();
	}

	SocketName NetGameConnection_Impl::get_remote_name() const
	{
		return socket_name;
	}

	bool NetGameConnection_Impl::read_connection_data(DataBuffer &receive_buffer, int &bytes_received)
	{
		while (true)
		{
			int bytes = connection.read(receive_buffer.get_data() + bytes_received, receive_buffer.get_size() - bytes_received);
			if (bytes < 0)
				return false;

			if (bytes == 0)
			{
				connection.close();
				return true;
			}

			bytes_received += bytes;

			int bytes_consumed = 0;
			bool exit = read_data(receive_buffer.get_data(), bytes_received, bytes_consumed);

			if (bytes_consumed >= 0)
			{
				memmove(receive_buffer.get_data(), receive_buffer.get_data() + bytes_consumed, bytes_received - bytes_consumed);
				bytes_received -= bytes_consumed;
			}

			if (exit)
				return exit;
		}

	}

	bool NetGameConnection_Impl::write_connection_data(DataBuffer &send_buffer, int &bytes_sent, bool &send_graceful_close)
	{
		while (true)
		{
			int bytes = connection.write(send_buffer.get_data() + bytes_sent, send_buffer.get_size() - bytes_sent);
			if (bytes < 0)
				return false;

			bytes_sent += bytes;

			if (bytes_sent == send_buffer.get_size())
			{
				if (send_graceful_close)
				{
					connection.close();
					return true;
				}
				else
				{
					bytes_sent = 0;
					send_buffer.set_size(0);
					send_graceful_close = write_data(send_buffer);
					if (send_buffer.get_size() == 0)
						return false;
				}
			}
		}
	}

	void NetGameConnection_Impl::connection_main()
	{
		try
		{
			if (!is_connected)
				connection = TCPConnection(socket_name);
			is_connected = true;
			site->add_network_event(NetGameNetworkEvent(base, NetGameNetworkEvent::client_connected));

			int bytes_received = 0;
			int bytes_sent = 0;
			const int max_event_packet_size = 32000 + 2;
			DataBuffer receive_buffer(max_event_packet_size);
			DataBuffer send_buffer;

			bool send_graceful_close = false;

			while (true)
			{
				if (read_connection_data(receive_buffer, bytes_received))
					break;
				if (write_connection_data(send_buffer, bytes_sent, send_graceful_close))
					break;

				std::unique_lock<std::mutex> lock(mutex);
				if (stop_flag)
					break;
				NetworkEvent *events[] = { &connection };
				worker_event.wait(lock, 1, events);
			}

			site->add_network_event(NetGameNetworkEvent(base, NetGameNetworkEvent::client_disconnected));
		}
		catch (const Exception& e)
		{
			site->add_network_event(NetGameNetworkEvent(base, NetGameNetworkEvent::client_disconnected, NetGameEvent(e.message)));
		}
	}

	bool NetGameConnection_Impl::read_data(const void *data, int size, int &bytes_consumed)
	{
		bytes_consumed = 0;
		while (bytes_consumed != size)
		{
			int bytes = 0;
			NetGameEvent incoming_event = NetGameNetworkData::receive_data(static_cast<const char*>(data)+bytes_consumed, size - bytes_consumed, bytes);
			bytes_consumed += bytes;

			if (bytes == 0)
			{
				return false;
			}
			else if (incoming_event.get_name() == "_close")
			{
				return true;
			}

			site->add_network_event(NetGameNetworkEvent(base, incoming_event));
		}
		return false;
	}

	bool NetGameConnection_Impl::write_data(DataBuffer &buffer)
	{
		std::unique_lock<std::mutex> mutex_lock(mutex);
		std::vector<Message> new_send_queue;
		send_queue.swap(new_send_queue);
		mutex_lock.unlock();
		for (auto & elem : new_send_queue)
		{
			if (elem.type == Message::type_message)
			{
				DataBuffer packet = NetGameNetworkData::send_data(elem.event);

				int pos = buffer.get_size();
				buffer.set_size(pos + packet.get_size());
				memcpy(buffer.get_data() + pos, packet.get_data(), packet.get_size());
			}
			else if (elem.type == Message::type_disconnect)
			{
				return true;
			}
		}
		return false;
	}
}
