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
**    Kenneth Gangstoe
*/

#include "Network/precomp.h"
#include "udp_socket_impl.h"
#include "socket_error.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Core/System/event.h"
#include "API/Core/Text/string_format.h"
#ifdef WIN32
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_UDPSocket_Impl Construction:

CL_UDPSocket_Impl::CL_UDPSocket_Impl()
{
	socket.create_udp();
#ifdef WIN32
	read_event = CL_Event(new CL_EventProvider_Win32Socket(&socket, CL_EventProvider_Win32Socket::socket_event_read));
	write_event = CL_Event(new CL_EventProvider_Win32Socket(&socket, CL_EventProvider_Win32Socket::socket_event_write));
#else
	read_event = CL_Event(new CL_EventProvider_UnixSocket(socket.get_handle(), CL_EventProvider::type_fd_read));
	write_event = CL_Event(new CL_EventProvider_UnixSocket(socket.get_handle(), CL_EventProvider::type_fd_write));
#endif
}

CL_UDPSocket_Impl::CL_UDPSocket_Impl(const CL_SocketName &local_name, bool force_bind)
{
	socket.create_udp();
#ifdef WIN32
	read_event = CL_Event(new CL_EventProvider_Win32Socket(&socket, CL_EventProvider_Win32Socket::socket_event_read));
	write_event = CL_Event(new CL_EventProvider_Win32Socket(&socket, CL_EventProvider_Win32Socket::socket_event_write));
#else
	read_event = CL_Event(new CL_EventProvider_UnixSocket(socket.get_handle(), CL_EventProvider::type_fd_read));
	write_event = CL_Event(new CL_EventProvider_UnixSocket(socket.get_handle(), CL_EventProvider::type_fd_write));
#endif
	bind(local_name, force_bind);
}

CL_UDPSocket_Impl::CL_UDPSocket_Impl(int handle, bool close_socket)
{
	set_handle(handle, close_socket);
}

CL_UDPSocket_Impl::~CL_UDPSocket_Impl()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_UDPSocket_Impl Attributes:

int CL_UDPSocket_Impl::get_handle() const
{
	return socket.get_handle();
}

CL_SocketName CL_UDPSocket_Impl::get_local_name() const
{
	return CL_SocketName();
}

CL_Event CL_UDPSocket_Impl::get_read_event()
{
	return read_event;
}

CL_Event CL_UDPSocket_Impl::get_write_event()
{
	return write_event;
}

/////////////////////////////////////////////////////////////////////////////
// CL_UDPSocket_Impl Operations:

void CL_UDPSocket_Impl::bind(const CL_SocketName &local_name, bool force_bind)
{
	socket.bind(local_name, force_bind);
}

void CL_UDPSocket_Impl::set_handle(int sock, bool close_sock)
{
	socket.set_handle(sock, close_sock);
#ifndef WIN32
	read_event = CL_Event(new CL_EventProvider_UnixSocket(socket.get_handle(), CL_EventProvider::type_fd_read));
	write_event = CL_Event(new CL_EventProvider_UnixSocket(socket.get_handle(), CL_EventProvider::type_fd_write));
#endif
}

int CL_UDPSocket_Impl::send(const void *data, int len, const CL_SocketName &to)
{
	return socket.send_to(data, len, to);
}

int CL_UDPSocket_Impl::receive(void *data, int len, CL_SocketName &out_from)
{
	return socket.receive_from(data, len, out_from);
}

int CL_UDPSocket_Impl::peek(void *data, int len, CL_SocketName &out_from)
{
	return socket.peek_from(data, len, out_from);
}

/////////////////////////////////////////////////////////////////////////////
// CL_UDPSocket_Impl Implementation:
