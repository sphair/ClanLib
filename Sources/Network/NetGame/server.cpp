/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

CL_NetGameServer::CL_NetGameServer()
: impl(new CL_NetGameServer_Impl)
{
}

CL_NetGameServer::~CL_NetGameServer()
{
	stop();
}

void CL_NetGameServer::process_events()
{
	impl->process();
}

void CL_NetGameServer::add_network_event(const CL_NetGameNetworkEvent &e)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	impl->events.push_back(e);
	impl->event_arrived.set();
}

CL_Event &CL_NetGameServer::get_event_arrived()
{
	return impl->event_arrived; 
}

void CL_NetGameServer::send_event(const CL_NetGameEvent &game_event)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	for (unsigned int i = 0; i < impl->connections.size(); i++)
	{
		impl->connections[i]->send_event(game_event);
	}
}

void CL_NetGameServer::start(const CL_String &port)
{
	stop();
	impl->stop_event.reset();
	impl->tcp_listen.reset(new CL_TCPListen(CL_SocketName(port)));
	impl->listen_thread.start(this, &CL_NetGameServer::listen_thread_main);
}

void CL_NetGameServer::start(const CL_String &address, const CL_String &port)
{
	stop();
	impl->stop_event.reset();
	impl->tcp_listen.reset(new CL_TCPListen(CL_SocketName(address, port)));
	impl->listen_thread.start(this, &CL_NetGameServer::listen_thread_main);
}

void CL_NetGameServer::stop()
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

void CL_NetGameServer::listen_thread_main()
{
	while (true)
	{
		CL_Event accept_event = impl->tcp_listen->get_accept_event();
		int wakeup_reason = CL_Event::wait(impl->stop_event, accept_event);
		if (wakeup_reason != 1)
			break;

		CL_TCPConnection connection = impl->tcp_listen->accept();
		CL_AutoPtr<CL_NetGameConnection> game_connection(new CL_NetGameConnection(this, connection));
		CL_MutexSection mutex_lock(&impl->mutex);
		impl->connections.push_back(game_connection.release());
	}
}

CL_Signal_v1<CL_NetGameConnection *> &CL_NetGameServer::sig_client_connected()
{
	return impl->sig_game_client_connected; 
}

CL_Signal_v1<CL_NetGameConnection *> &CL_NetGameServer::sig_client_disconnected() 
{ 
	return impl->sig_game_client_disconnected; 
}

CL_Signal_v2<CL_NetGameConnection *, const CL_NetGameEvent &> &CL_NetGameServer::sig_event_received()
{ 
	return impl->sig_game_event_received; 
}

void CL_NetGameServer_Impl::process()
{
	CL_MutexSection mutex_lock(&mutex);
	event_arrived.reset();
	std::vector<CL_NetGameNetworkEvent> new_events;
	new_events.swap(events);
	mutex_lock.unlock();

	for (unsigned int i = 0; i < new_events.size(); i++)
	{
		switch (new_events[i].type)
		{
		case CL_NetGameNetworkEvent::client_connected:
			sig_game_client_connected.invoke(new_events[i].connection);
			break;
		case CL_NetGameNetworkEvent::event_received:
			sig_game_event_received.invoke(new_events[i].connection, new_events[i].game_event);
			break;
		case CL_NetGameNetworkEvent::client_disconnected:
			sig_game_client_disconnected.invoke(new_events[i].connection);

			// Destroy connection object
			{
				CL_MutexSection mutex_lock(&mutex);
				std::vector<CL_NetGameConnection *>::iterator connection_it;
				connection_it = std::find(connections.begin(), connections.end(), new_events[i].connection);
				if (connection_it != connections.end())
				{
					connections.erase( connection_it );
				}
				delete new_events[i].connection;
			}
			break;
		default:
			throw CL_Exception("Unknown server event type");
		}
	}
}
