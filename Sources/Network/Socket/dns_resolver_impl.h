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

#pragma once


#include "API/Core/System/mutex.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/event.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Network/Socket/udp_socket.h"
#include "API/Network/Socket/dns_packet.h"
#include <vector>
#include <map>

class CL_DNSResolver_Impl
{
/// \name Construction
/// \{

public:
	CL_DNSResolver_Impl();

	~CL_DNSResolver_Impl();


/// \}
/// \name Attributes
/// \{

public:
	int query_id;

	std::vector<CL_SocketName> dns_servers;

	std::map<int, CL_DNSPacket> queries;

	std::map<int, CL_DNSPacket> answers;

	CL_UDPSocket udp_socket;

	CL_Mutex mutex;

	CL_Thread thread;

	CL_Event event_stop, event_bound;


/// \}
/// \name Operations
/// \{

public:
	void thread_main();


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


