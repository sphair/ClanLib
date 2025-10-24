/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "API/Network/NetGame/client.h"
#include "API/Network/NetGame/connection.h"
#include "API/Network/NetGame/event.h"
#include "API/Network/Socket/socket_name.h"
#include "network_event.h"
#include "client_impl.h"

CL_NetGameClient::CL_NetGameClient()
: impl(new CL_NetGameClient_Impl)
{
}

CL_NetGameClient::~CL_NetGameClient()
{
	impl->connection.reset();
}

void CL_NetGameClient::connect(const CL_String &server, const CL_String &port)
{
	disconnect();
	impl->connection.reset(new CL_NetGameConnection(this, CL_SocketName(server, port)));
}

void CL_NetGameClient::disconnect()
{
	impl->connection.reset();
	impl->events.clear();
}

CL_Event &CL_NetGameClient::get_event_arrived()
{
	return impl->event_arrived;
}

void CL_NetGameClient::process_events()
{
	impl->process();
}

void CL_NetGameClient::send_event(const CL_NetGameEvent &game_event)
{
	if (impl->connection != 0)
		impl->connection->send_event(game_event);
}

CL_Signal_v1<const CL_NetGameEvent &> &CL_NetGameClient::sig_event_received()
{
	return impl->sig_game_event_received;
}

CL_Signal_v0 &CL_NetGameClient::sig_connected()
{
	return impl->sig_game_connected;
}

CL_Signal_v0 &CL_NetGameClient::sig_disconnected()
{
	return impl->sig_game_disconnected;
}

void CL_NetGameClient::add_network_event(const CL_NetGameNetworkEvent &e)
{
	CL_MutexSection mutex_lock(&impl->mutex);
	impl->events.push_back(e);
	impl->event_arrived.set();
}

void CL_NetGameClient_Impl::process()
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
			sig_game_connected.invoke();
			break;
		case CL_NetGameNetworkEvent::event_received:
			sig_game_event_received.invoke(new_events[i].game_event);
			break;
		case CL_NetGameNetworkEvent::client_disconnected:
			sig_game_disconnected.invoke();
			connection.reset();
			break;
		default:
			throw CL_Exception("Unknown server event type");
		}
	}
}
