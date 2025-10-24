/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "API/Core/System/error.h"
#include "API/Network/NetSession/netcomputer.h"
#include "API/Network/NetSession/netsession.h"
#include "API/Core/System/thread.h"
#include "netcomputer_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer construction:

CL_NetComputer::CL_NetComputer()
: impl(0)
{
}

CL_NetComputer::CL_NetComputer(const CL_NetComputer &copy)
: impl(copy.impl)
{
	if (impl) impl->add_ref();
}

CL_NetComputer::~CL_NetComputer()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer attributes:

CL_IPAddress CL_NetComputer::get_address() const
{
	if (impl) return impl->address;
	return CL_IPAddress(0);
}

CL_NetSession CL_NetComputer::get_session()
{
	if (impl == 0)
		throw CL_Error("CL_NetComputer::get_session() not allowed on an unattached netcomputer object");

	return CL_NetSession(impl->netsession);
}

bool CL_NetComputer::is_disconnected() const
{
	if(impl == 0)
		return true;
	else
		return impl->disconnected;
}

const std::string &CL_NetComputer::get_disconnect_reason() const
{
	static std::string null_msg("Unattached netcomputer object");

	if (impl == 0)
		return null_msg;
	else
		return impl->disconnect_reason;
}

bool CL_NetComputer::operator == (const CL_NetComputer &other_instance) const
{
	return (impl == other_instance.impl);
}

bool CL_NetComputer::operator < (const CL_NetComputer &other_instance) const
{
	return (impl < other_instance.impl);
}

bool CL_NetComputer::operator > (const CL_NetComputer &other_instance) const
{
	return (impl > other_instance.impl);
}

CL_Thread const & CL_NetComputer::get_thread_handle() const
{
	return impl->get_thread_handle();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer operations:

void CL_NetComputer::disconnect(std::string reason)
{
	impl->disconnect(reason);
}

void CL_NetComputer::send(const std::string &packet_channel, const CL_NetPacket &packet, bool reliable)
{
	if (impl == NULL)
		throw CL_Error("CL_NetComputer::send not allowed on an unattached netcomputer object");

	if (reliable)
	{
		impl->send_packet(packet_channel, packet.impl);
	}
	else
	{
		impl->send_packet_udp(packet_channel, packet.impl);
	}
}

CL_NetComputer &CL_NetComputer::operator = (const CL_NetComputer &other_instance)
{
	if (impl) impl->release_ref();
	impl = other_instance.impl;
	if (impl) impl->add_ref();
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer implementation:

CL_NetComputer::CL_NetComputer(class CL_NetComputer_Generic *impl)
: impl(impl)
{
	if (impl) impl->add_ref();
}
