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
: handle(-1), close_socket(false)
{
	handle = socket(AF_INET, SOCK_DGRAM, 0);
	if (handle == -1)
		throw CL_Exception(cl_format(cl_text("Unable to create socket - %1"), CL_SocketError::get_last_error_message()));

	close_socket = true;
#ifdef WIN32
	wsa_event_handler.start_select(handle);
	read_event = CL_Event(
		new CL_EventProvider_Win32Socket(
			&wsa_event_handler,
			CL_EventProvider_Win32Socket::socket_event_read));
	write_event = CL_Event(
		new CL_EventProvider_Win32Socket(
			&wsa_event_handler,
			CL_EventProvider_Win32Socket::socket_event_write));
#else
	read_event = CL_Event(
		new CL_EventProvider_UnixSocket(handle, CL_EventProvider::type_fd_read));
	write_event = CL_Event(
		new CL_EventProvider_UnixSocket(handle, CL_EventProvider::type_fd_write));
#endif
}

CL_UDPSocket_Impl::CL_UDPSocket_Impl(const CL_SocketName &local_name, bool force_bind)
: handle(-1), close_socket(false)
{
	handle = socket(AF_INET, SOCK_DGRAM, 0);
	if (handle == -1)
		throw CL_Exception(cl_format(cl_text("Unable to create socket - %1"), CL_SocketError::get_last_error_message()));

	close_socket = true;
#ifdef WIN32
	wsa_event_handler.start_select(handle);
	read_event = CL_Event(
		new CL_EventProvider_Win32Socket(
			&wsa_event_handler,
			CL_EventProvider_Win32Socket::socket_event_read));
	write_event = CL_Event(
		new CL_EventProvider_Win32Socket(
			&wsa_event_handler,
			CL_EventProvider_Win32Socket::socket_event_write));
#else
	read_event = CL_Event(
		new CL_EventProvider_UnixSocket(handle, CL_EventProvider::type_fd_read));
	write_event = CL_Event(
		new CL_EventProvider_UnixSocket(handle, CL_EventProvider::type_fd_write));
#endif

	int result = -1;

	if (force_bind)
	{
		int value = 1;
		result = setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, (const char *) &value, sizeof(int));
		if (result == -1)
			throw CL_Exception(cl_format(cl_text("Unable to force bind socket - %1"), CL_SocketError::get_last_error_message()));
	}

	sockaddr_in addr;
	local_name.to_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	result = ::bind(handle, (sockaddr *) &addr, sizeof(sockaddr_in));
	if (result == -1)
	{
		close_handle();
		
		if (!local_name.get_address().empty())
			throw CL_Exception(cl_format(cl_text("Unable to bind socket on device %1 port %2 - %3"), local_name.get_address(), local_name.get_port(), CL_SocketError::get_last_error_message()));
		else
			throw CL_Exception(cl_format(cl_text("Unable to bind socket on port %1 - %2"), local_name.get_port(), CL_SocketError::get_last_error_message()));
	}
}

CL_UDPSocket_Impl::CL_UDPSocket_Impl(int handle, bool close_socket)
: handle(handle), close_socket(close_socket)
{
}

CL_UDPSocket_Impl::~CL_UDPSocket_Impl()
{
	close_handle();
}

/////////////////////////////////////////////////////////////////////////////
// CL_UDPSocket_Impl Attributes:

int CL_UDPSocket_Impl::get_handle() const
{
	return handle;
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

bool CL_UDPSocket_Impl::bind(const CL_SocketName &local_name, bool force_bind)
{
	int result = -1;

	if (force_bind)
	{
		int value = 1;
		result = setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, (const char *) &value, sizeof(int));
		if (result == -1)
			throw CL_Exception(cl_format(cl_text("Unable to force bind socket - %1"), CL_SocketError::get_last_error_message()));
	}

	sockaddr_in addr;
	local_name.to_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	result = ::bind(handle, (sockaddr *) &addr, sizeof(sockaddr_in));
	if (result == -1)
		return false;

	return true;
}

void CL_UDPSocket_Impl::set_handle(int sock, bool close_sock)
{
	handle = sock;
	close_socket = close_sock;
}

int CL_UDPSocket_Impl::send(const void *data, int len, const CL_SocketName &to)
{
	sockaddr_in addr;
	to.to_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));

	int result = ::sendto(handle, (const char*) data, len, 0, (sockaddr *) &addr, sizeof(sockaddr_in));
	if (result < 0)
		throw CL_Exception(cl_format(cl_text("Socket send failed - %1"), CL_SocketError::get_last_error_message()));

	return result;
}

int CL_UDPSocket_Impl::receive(void *data, int len, CL_SocketName &out_from)
{
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	socklen_t addr_len = sizeof(sockaddr_in);
	int result = ::recvfrom(handle, (char *) data, len, 0, (sockaddr *) &addr, &addr_len);
	if (result < 0)
		throw CL_Exception(cl_format(cl_text("Socket recvfrom failed - %1"), CL_SocketError::get_last_error_message()));

	out_from.from_sockaddr(AF_INET, (sockaddr *) &addr, addr_len);
	return result;
}

int CL_UDPSocket_Impl::peek(void *data, int len, CL_SocketName &out_from)
{
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	socklen_t addr_len = sizeof(sockaddr_in);
	int result = ::recvfrom(handle, (char *) data, len, MSG_PEEK, (sockaddr *) &addr, &addr_len);
	if (result < 0)
		throw CL_Exception(cl_format(cl_text("Socket recvfrom failed - %1"), CL_SocketError::get_last_error_message()));

	out_from.from_sockaddr(AF_INET, (sockaddr *) &addr, addr_len);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CL_UDPSocket_Impl Implementation:

void CL_UDPSocket_Impl::close_handle()
{
	if (close_socket)
	{
#ifdef WIN32
		wsa_event_handler.stop_select();
		closesocket(handle);
#else
		close(handle);
#endif
		close_socket = false;
	}
	handle = -1;
}
