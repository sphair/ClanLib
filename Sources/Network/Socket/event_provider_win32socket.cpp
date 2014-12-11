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
#include "event_provider_win32socket.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// EventProvider_Win32Socket Construction:

EventProvider_Win32Socket::EventProvider_Win32Socket(Win32Socket *socket, SocketEventType type)
: socket(socket), type(type)
{
}

EventProvider_Win32Socket::~EventProvider_Win32Socket()
{
}

/////////////////////////////////////////////////////////////////////////////
// EventProvider_Win32Socket Attributes:

HANDLE EventProvider_Win32Socket::get_event_handle(int index)
{
	if (index == 0)
	{
		return socket->get_event_handle();
	}
	else if (index == 1)
	{
		switch (type)
		{
		case socket_event_read:
			return socket->get_receive_handle();
		case socket_event_write:
			return socket->get_send_handle();
		case socket_event_exception:
			return socket->get_except_handle();
		default:
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int EventProvider_Win32Socket::get_num_event_handles()
{
	return 2;
}

/////////////////////////////////////////////////////////////////////////////
// EventProvider_Win32Socket Operations:

bool EventProvider_Win32Socket::check_before_wait()
{
	return false;
}

bool EventProvider_Win32Socket::check_after_wait(int index)
{
	if (index == 0)
	{
		socket->process_events();
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// EventProvider_Win32Socket Implementation:

}
