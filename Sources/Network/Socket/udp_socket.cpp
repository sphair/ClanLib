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
#include "API/Network/Socket/udp_socket.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Core/System/event.h"
#include "udp_socket_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_UDPSocket Construction:

CL_UDPSocket::CL_UDPSocket()
: impl(new CL_UDPSocket_Impl)
{
}

CL_UDPSocket::CL_UDPSocket(const CL_SocketName &local_name, bool force_bind)
: impl(new CL_UDPSocket_Impl(local_name, force_bind))
{
}

CL_UDPSocket::CL_UDPSocket(int socket, bool close_socket)
: impl(new CL_UDPSocket_Impl(socket, close_socket))
{
}

CL_UDPSocket::~CL_UDPSocket()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_UDPSocket Attributes:

int CL_UDPSocket::get_handle() const
{
	return impl->get_handle();
}

CL_SocketName CL_UDPSocket::get_local_name() const
{
	return impl->get_local_name();
}

CL_Event CL_UDPSocket::get_read_event()
{
	return impl->get_read_event();
}

CL_Event CL_UDPSocket::get_write_event()
{
	return impl->get_write_event();
}

/////////////////////////////////////////////////////////////////////////////
// CL_UDPSocket Operations:

void CL_UDPSocket::bind(const CL_SocketName &local_name, bool force_bind)
{
	impl->bind(local_name, force_bind);
}

void CL_UDPSocket::set_handle(int socket, bool close_socket)
{
	impl->set_handle(socket, close_socket);
}

int CL_UDPSocket::send(const void *data, int len, const CL_SocketName &to)
{
	return impl->send(data, len, to);
}

int CL_UDPSocket::receive(void *data, int len, CL_SocketName &out_from)
{
	return impl->receive(data, len, out_from);
}

int CL_UDPSocket::peek(void *data, int len, CL_SocketName &out_from)
{
	return impl->peek(data, len, out_from);
}

/////////////////////////////////////////////////////////////////////////////
// CL_UDPSocket Implementation:
