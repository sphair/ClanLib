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
#include "dns_resolver_impl.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Text/logger.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// DNSResolver_Impl Construction:

DNSResolver_Impl::DNSResolver_Impl()
: query_id(0)
{
	thread.start(this, &DNSResolver_Impl::thread_main);
}

DNSResolver_Impl::~DNSResolver_Impl()
{
	event_stop.set();
	thread.join();
}

/////////////////////////////////////////////////////////////////////////////
// DNSResolver_Impl Attributes:

/////////////////////////////////////////////////////////////////////////////
// DNSResolver_Impl Operations:

void DNSResolver_Impl::thread_main()
{
	// Wait for socket to become bound. This will happen when the first query has been sent.
	int result = Event::wait(event_stop, event_bound);
	if (result != 1)
		return;

	while (true)
	{
		Event event_read = udp_socket.get_read_event();
		int result = Event::wait(event_stop, event_read);
		if (result != 0)
		{
			SocketName from;
			DataBuffer buffer(64*1024);
			int bytes_read = udp_socket.receive(buffer.get_data(), buffer.get_size(), from);
			buffer.set_size(bytes_read);
			try
			{
				DNSPacket packet(buffer);
				std::unique_lock<std::recursive_mutex> mutex_lock(mutex);
				int query_id = packet.get_query_id();
				auto it = queries.find(query_id);
				if (it != queries.end())
					answers[query_id] = packet;
			}
			catch (const Exception& e)
			{
				log_event("dns", "Exception during parsing of response dns packet: %1", e.message);
			}
		}
		else
		{
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// DNSResolver_Impl Implementation:

}
