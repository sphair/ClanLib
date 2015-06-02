/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "Network/precomp.h"
#include "API/Network/NetGame/server.h"
#include "API/Network/NetGame/connection.h"
#include "API/Network/Socket/socket_name.h"
#include "network_event.h"
#include "server_impl.h"
#include <algorithm>
#include "API/Network/Socket/tcp_connection.h"

namespace clan
{

NetGameServer::NetGameServer()
: impl(std::make_shared<NetGameServer_Impl>())
{
}

NetGameServer::~NetGameServer()
{
	stop();
}

void NetGameServer::process_events()
{
	impl->process();
}

void NetGameServer::add_network_event(const NetGameNetworkEvent &e)
{
	std::unique_lock<std::mutex> mutex_lock(impl->mutex);
	impl->events.push_back(e);
}

void NetGameServer::send_event(const NetGameEvent &game_event)
{
	std::unique_lock<std::mutex> mutex_lock(impl->mutex);
	for (auto & elem : impl->connections)
	{
		elem->send_event(game_event);
	}
}

void NetGameServer::start(const std::string &port)
{
	stop();
	std::unique_lock<std::mutex> lock(impl->mutex);
	impl->stop_flag = false;
	lock.unlock();
	impl->tcp_listen.reset(new TCPListen(SocketName(port)));
	impl->listen_thread = std::thread(&NetGameServer::listen_thread_main, this);
}

void NetGameServer::start(const std::string &address, const std::string &port)
{
	stop();
	std::unique_lock<std::mutex> lock(impl->mutex);
	impl->stop_flag = false;
	lock.unlock();
	impl->tcp_listen.reset(new TCPListen(SocketName(address, port)));
	impl->listen_thread = std::thread(&NetGameServer::listen_thread_main, this);
}

void NetGameServer::stop()
{
	std::unique_lock<std::mutex> lock(impl->mutex);
	impl->stop_flag = true;
	lock.unlock();
	impl->worker_event.notify();
	if (impl->listen_thread.joinable())
		impl->listen_thread.join();
	impl->tcp_listen.reset();

	for (auto & elem : impl->connections)
	{
		delete elem;
	}
	impl->connections.clear();
}

void NetGameServer::listen_thread_main()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(impl->mutex);
		if (impl->stop_flag)
			break;

		NetworkEvent *events[] = { impl->tcp_listen.get() };
		impl->worker_event.wait(lock, 1, events);

		SocketName peer_endpoint;
		TCPConnection connection = impl->tcp_listen->accept(peer_endpoint);
		if (!connection.is_null())
		{
			std::unique_ptr<NetGameConnection> game_connection(new NetGameConnection(this, connection));
			impl->connections.push_back(game_connection.release());
		}
	}
}

Signal<void(NetGameConnection *)> &NetGameServer::sig_client_connected()
{
	return impl->sig_game_client_connected;
}

Signal<void(NetGameConnection *, const std::string &)> &NetGameServer::sig_client_disconnected()
{
	return impl->sig_game_client_disconnected;
}

Signal<void(NetGameConnection *, const NetGameEvent &)> &NetGameServer::sig_event_received()
{
	return impl->sig_game_event_received;
}

void NetGameServer_Impl::process()
{
	std::unique_lock<std::mutex> mutex_lock(mutex);
	std::vector<NetGameNetworkEvent> new_events;
	new_events.swap(events);
	mutex_lock.unlock();

	for (auto & new_event : new_events)
	{
		switch (new_event.type)
		{
		case NetGameNetworkEvent::client_connected:
			sig_game_client_connected(new_event.connection);
			break;
		case NetGameNetworkEvent::event_received:
			sig_game_event_received(new_event.connection, new_event.game_event);
			break;
		case NetGameNetworkEvent::client_disconnected:
			{
				std::string reason = new_event.game_event.get_name();
				sig_game_client_disconnected(new_event.connection, reason);
			}

			// Destroy connection object
			{
				std::unique_lock<std::mutex> mutex_lock(mutex);
				std::vector<NetGameConnection *>::iterator connection_it;
				connection_it = std::find(connections.begin(), connections.end(), new_event.connection);
				if (connection_it != connections.end())
				{
					connections.erase( connection_it );
				}
				delete new_event.connection;
			}
			break;
		default:
			throw Exception("Unknown server event type");
		}
	}
}

}
