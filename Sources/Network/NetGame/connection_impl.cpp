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
**    Mark Page
*/

#include "Network/precomp.h"
#include "API/Network/NetGame/connection.h"
#include "API/Network/NetGame/connection_site.h"
#include "network_event.h"
#include "network_data.h"
#include "connection_impl.h"

CL_NetGameConnection_Impl::CL_NetGameConnection_Impl()
{
}

void CL_NetGameConnection_Impl::start(CL_NetGameConnection *xbase, CL_NetGameConnectionSite *xsite, const CL_TCPConnection &xconnection)
{
	base = xbase;
	site = xsite;
	connection = xconnection;
	is_connected = true;
	thread.start(this, &CL_NetGameConnection_Impl::connection_main);
}

void CL_NetGameConnection_Impl::start(CL_NetGameConnection *xbase, CL_NetGameConnectionSite *xsite, const CL_SocketName &xsocket_name)
{
	base = xbase;
	site = xsite;
	socket_name = xsocket_name;
	is_connected = false;
	thread.start(this, &CL_NetGameConnection_Impl::connection_main);
}

CL_NetGameConnection_Impl::~CL_NetGameConnection_Impl()
{
	stop_event.set();
	thread.join();
}

void CL_NetGameConnection_Impl::set_data(const CL_StringRef &name, void *new_data)
{
	for (std::vector<AttachedData>::iterator it = data.begin(); it != data.end(); ++it)
	{
		if (it->name == name)
		{
			it->data = new_data;
			return;
		}
	}
	AttachedData d;
	d.name = name;
	d.data = new_data;
	data.push_back(d);
}

void *CL_NetGameConnection_Impl::get_data(const CL_StringRef &name) const
{
	for (std::vector<AttachedData>::const_iterator it = data.begin(); it != data.end(); ++it)
	{
		if (it->name == name)
			return it->data;
	}
	return 0;
}

void CL_NetGameConnection_Impl::send_event(const CL_NetGameEvent &game_event)
{
	CL_MutexSection mutex_lock(&mutex);
	Message message;
	message.type = Message::type_message;
	message.event = game_event;
	send_queue.push_back(message);
	queue_event.set();
}

void CL_NetGameConnection_Impl::disconnect()
{
	CL_MutexSection mutex_lock(&mutex);
	Message message;
	message.type = Message::type_disconnect;
	send_queue.push_back(message);
	queue_event.set();
}

void CL_NetGameConnection_Impl::connection_main()
{
	try
	{
		if (!is_connected)
			connection = CL_TCPConnection(socket_name);
		is_connected = true;
		site->add_network_event(CL_NetGameNetworkEvent(base, CL_NetGameNetworkEvent::client_connected));

		connection.set_nodelay(true);
		while (true)
		{
			CL_Event read_event = connection.get_read_event();
			int wakeup_reason = CL_Event::wait(stop_event, read_event, queue_event);
			if (wakeup_reason <= 0)
			{
				break;
			}
			else if (wakeup_reason == 1) // we got data to receive
			{
				CL_NetGameEvent incoming_event = CL_NetGameNetworkData::receive_data(connection);
				if (incoming_event.get_name() == "_close")
					break;
				site->add_network_event(CL_NetGameNetworkEvent(base, incoming_event));
			}
			else if (wakeup_reason == 2) // we got data to send
			{
				CL_MutexSection mutex_lock(&mutex);
				queue_event.reset();
				std::vector<Message> new_send_queue;
				send_queue.swap(new_send_queue);
				mutex_lock.unlock();
				for (unsigned int i = 0; i < new_send_queue.size(); i++)
				{
					if (new_send_queue[i].type == Message::type_message)
					{
						CL_NetGameNetworkData::send_data(connection, new_send_queue[i].event);
					}
					else if (new_send_queue[i].type == Message::type_disconnect)
					{
						connection.disconnect_graceful();
						site->add_network_event(CL_NetGameNetworkEvent(base, CL_NetGameNetworkEvent::client_disconnected));
						return;
					}
				}
			}
		}
	}
	catch (CL_Exception e)
	{
		// to do: pass on e.message to the CL_NetGameNetworkEvent::client_disconnected event
	}
	site->add_network_event(CL_NetGameNetworkEvent(base, CL_NetGameNetworkEvent::client_disconnected));
}
