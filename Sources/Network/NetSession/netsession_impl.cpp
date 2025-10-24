/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
#include "API/Core/System/databuffer.h"
#include "netsession_impl.h"
#include "packets.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession_Impl Construction:

CL_NetSession_Impl::CL_NetSession_Impl()
: show_debug(false)
{
	worker_thread.start(this, &CL_NetSession_Impl::worker_thread_main);
}

CL_NetSession_Impl::~CL_NetSession_Impl()
{
	stop_event.set();
	worker_thread.join();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession_Impl Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession_Impl Operations:

/////////////////////////////////////////////////////////////////////////////
// CL_NetSession_Impl Implementation:

void CL_NetSession_Impl::worker_thread_main()
{
	CL_UDPSocket unnamed_socket;
	CL_DataBuffer binpacket_buffer;
	CL_SocketName binpacket_source;
	CL_NetSessionPacket packet;
	while (true)
	{
		std::vector<CL_UDPSocket>::size_type i;

		// Find the list of sockets to work on:
		CL_MutexSection mutex_lock(&mutex);
		std::vector<CL_UDPSocket> sockets = named_sockets;
		sockets.push_back(unnamed_socket);
		for (i = 0; i < named_sockets.size(); i++)
			sockets.push_back(named_sockets[i]);
		mutex_lock.unlock();

		// Wait for activity on sockets:
		std::vector<CL_Event> events;
		events.push_back(stop_event);
		events.push_back(update_event);
		for (i = 0; i < sockets.size(); i++)
			events.push_back(sockets[i].get_read_event());
		int result = CL_Event::wait(events);
		if (result == -1 || result == 0)
			break;
		update_event.reset();
		if (result == 1)
			continue;

		// Read from sockets that got data for us:
		mutex_lock.lock();
		for (i = 0; i < sockets.size(); i++)
		{
			if (sockets[i].get_read_event().wait(0))
			{
				binpacket_buffer.set_size(CL_MAX_PACKET_SIZE);
				int size = sockets[i].receive(
					binpacket_buffer.get_data(),
					binpacket_buffer.get_size(),
					binpacket_source);
				binpacket_buffer.set_size(size);
				cl_read_packet(binpacket_buffer, packet);
				worker_thread_received_packet(packet, binpacket_source);
			}
		}

		// Write to sockets ready to receive data:
		// todo: actually write some here.

		mutex_lock.unlock();
	}
}

void CL_NetSession_Impl::worker_thread_received_packet(
	const CL_NetSessionPacket &packet,
	const CL_SocketName &source)
{
	switch (packet.type)
	{
	case CL_NetSessionPacket::type_hello:
		worker_thread_received_hello(packet, source);
		break;
	case CL_NetSessionPacket::type_hello_ack:
		worker_thread_received_hello_ack(packet, source);
		break;
	case CL_NetSessionPacket::type_goodbye:
		worker_thread_received_goodbye(packet, source);
		break;
	case CL_NetSessionPacket::type_goodbye_ack:
		worker_thread_received_goodbye_ack(packet, source);
		break;
	case CL_NetSessionPacket::type_message:
		worker_thread_received_message(packet, source);
		break;
	}
}

void CL_NetSession_Impl::worker_thread_received_hello(
	const CL_NetSessionPacket &packet,
	const CL_SocketName &source)
{
}

void CL_NetSession_Impl::worker_thread_received_hello_ack(
	const CL_NetSessionPacket &packet,
	const CL_SocketName &source)
{
}

void CL_NetSession_Impl::worker_thread_received_goodbye(
	const CL_NetSessionPacket &packet,
	const CL_SocketName &source)
{
}

void CL_NetSession_Impl::worker_thread_received_goodbye_ack(
	const CL_NetSessionPacket &packet,
	const CL_SocketName &source)
{
}

void CL_NetSession_Impl::worker_thread_received_message(
	const CL_NetSessionPacket &packet,
	const CL_SocketName &source)
{
}
