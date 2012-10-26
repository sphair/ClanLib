/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "API/Network/Socket/udp_socket.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Core/System/event.h"
#include "udp_socket_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// UDPSocket Construction:

UDPSocket::UDPSocket()
: impl(new UDPSocket_Impl)
{
}

UDPSocket::UDPSocket(const SocketName &local_name, bool force_bind)
: impl(new UDPSocket_Impl(local_name, force_bind))
{
}

UDPSocket::UDPSocket(int socket, bool close_socket)
: impl(new UDPSocket_Impl(socket, close_socket))
{
}

UDPSocket::~UDPSocket()
{
}

/////////////////////////////////////////////////////////////////////////////
// UDPSocket Attributes:

int UDPSocket::get_handle() const
{
	return impl->get_handle();
}

SocketName UDPSocket::get_local_name() const
{
	return impl->get_local_name();
}

Event UDPSocket::get_read_event()
{
	return impl->get_read_event();
}

Event UDPSocket::get_write_event()
{
	return impl->get_write_event();
}

/////////////////////////////////////////////////////////////////////////////
// UDPSocket Operations:

void UDPSocket::bind(const SocketName &local_name, bool force_bind)
{
	impl->bind(local_name, force_bind);
}

void UDPSocket::set_handle(int socket, bool close_socket)
{
	impl->set_handle(socket, close_socket);
}

int UDPSocket::send(const void *data, int len, const SocketName &to)
{
	return impl->send(data, len, to);
}

int UDPSocket::receive(void *data, int len, SocketName &out_from)
{
	return impl->receive(data, len, out_from);
}

int UDPSocket::peek(void *data, int len, SocketName &out_from)
{
	return impl->peek(data, len, out_from);
}

/////////////////////////////////////////////////////////////////////////////
// UDPSocket Implementation:

}
