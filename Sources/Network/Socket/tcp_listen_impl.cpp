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
**    Kenneth Gangstoe
*/

#include "Network/precomp.h"
#include "tcp_listen_impl.h"
#include "socket_error.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Network/Socket/tcp_connection.h"
#include "API/Core/System/exception.h"
#include "API/Core/Text/string_format.h"

/////////////////////////////////////////////////////////////////////////////
// CL_TCPListen_Impl Construction:

CL_TCPListen_Impl::CL_TCPListen_Impl(const CL_SocketName &name, int queue_size, bool force_bind)
{
	socket.create_tcp();
#ifdef WIN32
	accept_event = CL_Event(new CL_EventProvider_Win32Socket(&socket, CL_EventProvider_Win32Socket::socket_event_read));
#else
	accept_event = CL_Event(new CL_EventProvider_UnixSocket(socket.get_handle(), CL_EventProvider::type_fd_read));
#endif

	socket.bind(name, force_bind);
	socket.listen(queue_size);
}

CL_TCPListen_Impl::~CL_TCPListen_Impl()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_TCPListen_Impl Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_TCPListen_Impl Operations:

CL_TCPConnection CL_TCPListen_Impl::accept()
{
	CL_SocketName socketname;
	return CL_TCPConnection(socket.accept(socketname), true);
}

/////////////////////////////////////////////////////////////////////////////
// CL_TCPListen_Impl Implementation:
