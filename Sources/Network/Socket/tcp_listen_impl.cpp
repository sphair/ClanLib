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
#include "tcp_listen_impl.h"
#include "socket_error.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Network/Socket/tcp_connection.h"
#include "API/Core/System/exception.h"
#include "API/Core/Text/string_format.h"
#ifdef WIN32
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_TCPListen_Impl Construction:

CL_TCPListen_Impl::CL_TCPListen_Impl(const CL_SocketName &name, int queue_size, bool force_bind)
: handle(-1)
{
	int result = -1;
	handle = socket(AF_INET, SOCK_STREAM, 0);
	if (handle == -1)
		throw CL_Exception(cl_format(cl_text("Unable to create socket - %1"), CL_SocketError::get_last_error_message()));

#ifdef WIN32
	wsa_event_handler.start_select(handle);
	accept_event = CL_Event(
		new CL_EventProvider_Win32Socket(
			&wsa_event_handler,
			CL_EventProvider_Win32Socket::socket_event_read));
#else
	accept_event = CL_Event(
		new CL_EventProvider_UnixSocket(handle, CL_EventProvider::type_fd_read));

#ifdef SO_NOSIGPIPE
	int value = 1;
	result = setsockopt(handle, SOL_SOCKET, SO_NOSIGPIPE, (const char *) &value, sizeof(int));
#endif
#endif

	if (force_bind)
	{
		int value = 1;
		result = setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, (const char *) &value, sizeof(int));
		if (result == -1)
			throw CL_Exception(cl_format(cl_text("Unable to force bind socket - %1"), CL_SocketError::get_last_error_message()));
	}

	sockaddr_in addr;
	name.to_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
	result = bind(handle, (sockaddr *) &addr, sizeof(sockaddr_in));
	if (result == -1)
	{
		close_handle();
		
		if (!name.get_address().empty())
			throw CL_Exception(cl_format(cl_text("Unable to bind socket on device %1 port %2 - %3"), name.get_address(), name.get_port(), CL_SocketError::get_last_error_message()));
		else
			throw CL_Exception(cl_format(cl_text("Unable to bind socket on port %1 - %2"), name.get_port(), CL_SocketError::get_last_error_message()));
	}

	result = listen(handle, queue_size);
	if (result == -1)
	{
		close_handle();
		throw CL_Exception(cl_format(cl_text("Unable to set socket into listen state - %1"), CL_SocketError::get_last_error_message()));
	}
}

CL_TCPListen_Impl::~CL_TCPListen_Impl()
{
	close_handle();
}

/////////////////////////////////////////////////////////////////////////////
// CL_TCPListen_Impl Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_TCPListen_Impl Operations:

CL_TCPConnection CL_TCPListen_Impl::accept()
{
	sockaddr_in new_addr;
	memset(&new_addr, 0, sizeof(sockaddr_in));
	socklen_t size = sizeof(sockaddr_in);
	int new_handle = ::accept(handle, (sockaddr *) &new_addr, &size);
	if (new_handle == -1)
		throw CL_Exception(cl_format(cl_text("Socket accept failed - %1"), CL_SocketError::get_last_error_message()));

	return CL_TCPConnection(new_handle, true);
}
	
/////////////////////////////////////////////////////////////////////////////
// CL_TCPListen_Impl Implementation:

void CL_TCPListen_Impl::close_handle()
{
#ifdef WIN32
	if (handle != -1)
		closesocket(handle);
	wsa_event_handler.stop_select();
#else
	if (handle != -1)
		close(handle);
#endif
	handle = -1;
}
