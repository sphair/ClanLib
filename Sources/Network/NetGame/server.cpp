/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
	MutexSection mutex_lock(&impl->mutex);
	impl->events.push_back(e);
	impl->set_wakeup_event();
}

void NetGameServer::send_event(const NetGameEvent &game_event)
{
	MutexSection mutex_lock(&impl->mutex);
	for (unsigned int i = 0; i < impl->connections.size(); i++)
	{
		impl->connections[i]->send_event(game_event);
	}
}

void NetGameServer::start(const std::string &port)
{
	stop();
	impl->stop_event.reset();
	impl->tcp_listen.reset(new TCPListen(SocketName(port)));
	impl->listen_thread.start(this, &NetGameServer::listen_thread_main);
}

void NetGameServer::start(const std::string &address, const std::string &port)
{
	stop();
	impl->stop_event.reset();
	impl->tcp_listen.reset(new TCPListen(SocketName(address, port)));
	impl->listen_thread.start(this, &NetGameServer::listen_thread_main);
}

void NetGameServer::stop()
{
	impl->stop_event.set();
	impl->listen_thread.join();
	impl->tcp_listen.reset();

	for (unsigned int i = 0; i < impl->connections.size(); i++)
	{
		delete impl->connections[i];
	}
	impl->connections.clear();
}

void NetGameServer::listen_thread_main()
{
	while (true)
	{
		Event accept_event = impl->tcp_listen->get_accept_event();
		int wakeup_reason = Event::wait(impl->stop_event, accept_event);
		if (wakeup_reason != 1)
			break;

		TCPConnection connection = impl->tcp_listen->accept();
		std::unique_ptr<NetGameConnection> game_connection(new NetGameConnection(this, connection));
		MutexSection mutex_lock(&impl->mutex);
		impl->connections.push_back(game_connection.release());
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
	MutexSection mutex_lock(&mutex);
	std::vector<NetGameNetworkEvent> new_events;
	new_events.swap(events);
	mutex_lock.unlock();

	for (unsigned int i = 0; i < new_events.size(); i++)
	{
		switch (new_events[i].type)
		{
		case NetGameNetworkEvent::client_connected:
			sig_game_client_connected(new_events[i].connection);
			break;
		case NetGameNetworkEvent::event_received:
			sig_game_event_received(new_events[i].connection, new_events[i].game_event);
			break;
		case NetGameNetworkEvent::client_disconnected:
			{
				std::string reason = new_events[i].game_event.get_name();
				sig_game_client_disconnected(new_events[i].connection, reason);
			}

			// Destroy connection object
			{
				MutexSection mutex_lock(&mutex);
				std::vector<NetGameConnection *>::iterator connection_it;
				connection_it = std::find(connections.begin(), connections.end(), new_events[i].connection);
				if (connection_it != connections.end())
				{
					connections.erase( connection_it );
				}
				delete new_events[i].connection;
			}
			break;
		default:
			throw Exception("Unknown server event type");
		}
	}
}

}
