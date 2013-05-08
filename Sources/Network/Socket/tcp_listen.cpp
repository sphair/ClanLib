/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "API/Network/Socket/tcp_listen.h"
#include "API/Network/Socket/tcp_connection.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Core/System/event.h"
#include "tcp_listen_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// TCPListen Construction:

TCPListen::TCPListen(const SocketName &name, int queue_size, bool force_bind)
: impl(new TCPListen_Impl(name, queue_size, force_bind))
{
}

TCPListen::~TCPListen()
{
}

/////////////////////////////////////////////////////////////////////////////
// TCPListen Attributes:

Event TCPListen::get_accept_event() const
{
	return impl->accept_event;
}

int TCPListen::get_handle() const
{
	return impl->socket.get_handle();
}

/////////////////////////////////////////////////////////////////////////////
// TCPListen Operations:

TCPConnection TCPListen::accept()
{
	return impl->accept();
}
	
/////////////////////////////////////////////////////////////////////////////
// TCPListen Implementation:

}
