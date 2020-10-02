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
*/

#include "Network/precomp.h"
#include "API/Network/NetGame/client.h"
#include "API/Network/NetGame/connection.h"
#include "API/Network/NetGame/event.h"
#include "API/Network/Socket/socket_name.h"
#include "network_event.h"
#include "client_impl.h"

namespace clan
{
	NetGameClient::NetGameClient()
		: impl(std::make_shared<NetGameClient_Impl>())
	{
	}

	NetGameClient::~NetGameClient()
	{
		impl->connection.reset();
	}

	void NetGameClient::connect(const std::string &server, const std::string &port)
	{
		disconnect();
		impl->connection.reset(new NetGameConnection(this, SocketName(server, port)));
	}

	void NetGameClient::disconnect()
	{
		if (impl->connection.get() != nullptr)
			impl->connection->disconnect();
		impl->connection.reset();
		impl->events.clear();
	}

	void NetGameClient::process_events()
	{
		impl->process();
	}

	void NetGameClient::send_event(const NetGameEvent &game_event)
	{
		if (impl->connection.get() != nullptr)
			impl->connection->send_event(game_event);
	}

	Signal<void(const NetGameEvent &)> &NetGameClient::sig_event_received()
	{
		return impl->sig_game_event_received;
	}

	Signal<void()> &NetGameClient::sig_connected()
	{
		return impl->sig_game_connected;
	}

	Signal<void()> &NetGameClient::sig_disconnected()
	{
		return impl->sig_game_disconnected;
	}

	void NetGameClient::add_network_event(const NetGameNetworkEvent &e)
	{
		std::unique_lock<std::recursive_mutex> mutex_lock(impl->mutex);
		impl->events.push_back(e);
	}

	void NetGameClient_Impl::process()
	{
		std::unique_lock<std::recursive_mutex> mutex_lock(mutex);
		std::vector<NetGameNetworkEvent> new_events;
		new_events.swap(events);
		mutex_lock.unlock();
		for (auto & new_event : new_events)
		{
			switch (new_event.type)
			{
			case NetGameNetworkEvent::Type::client_connected:
				sig_game_connected();
				break;
			case NetGameNetworkEvent::Type::event_received:
				sig_game_event_received(new_event.game_event);
				break;
			case NetGameNetworkEvent::Type::client_disconnected:
				sig_game_disconnected();
				connection.reset();
				break;
			default:
				throw Exception("Unknown server event type");
			}
		}
	}
}
