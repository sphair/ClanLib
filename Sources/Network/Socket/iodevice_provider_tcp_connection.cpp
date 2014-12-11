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
#include "iodevice_provider_tcp_connection.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Core/System/event.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/logger.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// IODeviceProvider_TCPConnection Construction:

IODeviceProvider_TCPConnection::IODeviceProvider_TCPConnection()
: timeout(15000)
{
	socket.create_tcp();
	create_events();
}
	
IODeviceProvider_TCPConnection::IODeviceProvider_TCPConnection(const SocketName &remote)
: timeout(15000)
{
	socket.create_tcp();
	create_events();
	connect(remote);
}
	
IODeviceProvider_TCPConnection::IODeviceProvider_TCPConnection(const SocketName &remote, const SocketName &local)
: timeout(15000)
{
	socket.create_tcp();
	create_events();
	connect(remote, local);
}
	
IODeviceProvider_TCPConnection::IODeviceProvider_TCPConnection(int handle, bool close_socket)
: timeout(15000)
{
	socket.set_handle(handle, close_socket);
	create_events();
}

IODeviceProvider_TCPConnection::~IODeviceProvider_TCPConnection()
{
}

/////////////////////////////////////////////////////////////////////////////
// IODeviceProvider_TCPConnection Attributes:

SocketName IODeviceProvider_TCPConnection::get_local_name() const
{
	return socket.get_local_name();
}

SocketName IODeviceProvider_TCPConnection::get_remote_name() const
{
	return socket.get_remote_name();
}

Event IODeviceProvider_TCPConnection::get_read_event()
{
	return read_event;
}

Event IODeviceProvider_TCPConnection::get_write_event()
{
	return write_event;
}

/////////////////////////////////////////////////////////////////////////////
// IODeviceProvider_TCPConnection Operations:

void IODeviceProvider_TCPConnection::connect(const SocketName &remote)
{
	socket.connect(remote);
	int wakeup_reason = Event::wait(write_event, except_event, timeout);
	switch (wakeup_reason)
	{
	case 0: return;
	case 1: receive(nullptr, 0, false); throw Exception("Connection attempt failed");
	case -1: throw Exception("Connection attempt timed out");
	}
}

void IODeviceProvider_TCPConnection::connect(
	const SocketName &remote,
	const SocketName &local)
{
	socket.bind(local, false);
	connect(remote);
}
	
void IODeviceProvider_TCPConnection::set_handle(int handle, bool close_socket)
{
	socket.set_handle(handle, close_socket);
}

void IODeviceProvider_TCPConnection::disconnect_abortive()
{
	socket.disconnect_abortive();
}

void IODeviceProvider_TCPConnection::disconnect_graceful()
{
	socket.disconnect_graceful(timeout);
}

void IODeviceProvider_TCPConnection::set_nodelay(bool enable)
{
	socket.set_nodelay(enable);
}

void IODeviceProvider_TCPConnection::set_keep_alive(bool enable, int timeout, int interval)
{
	socket.set_keep_alive(enable, timeout, interval);
}

int IODeviceProvider_TCPConnection::send(const void *data, int len, bool send_all)
{
	if (!send_all)
	{
		return socket.send(data, len);
	}
	else
	{
		const char *d = (const char *) data;
		int pos = 0;
		while (pos < len)
		{
			if (!write_event.wait(timeout))
				throw Exception("Send timed out");
			pos += socket.send(d+pos, len-pos);
		}
		return pos;
	}
}

int IODeviceProvider_TCPConnection::receive(void *data, int len, bool receive_all)
{
	if (!receive_all)
	{
		return socket.receive(data, len);
	}
	else
	{
		char *d = (char *) data;
		int pos = 0;
		while (pos < len)
		{
			if (!read_event.wait(timeout))
				throw Exception("Receive timed out");
			int received = socket.receive(d+pos, len-pos);
			if (received == 0)
				throw Exception("Unable to receive all data: connection closed by peer");
			pos += received;
		}
		return pos;
	}
}

int IODeviceProvider_TCPConnection::peek(void *data, int len)
{
	return socket.peek(data, len);
}

IODeviceProvider *IODeviceProvider_TCPConnection::duplicate()
{
	throw Exception("IODeviceProvider_TCPConnection::duplicate() - duplicate not supported for TCP connections.");
	return nullptr;
}

/////////////////////////////////////////////////////////////////////////////
// IODeviceProvider_TCPConnection Implementation:

void IODeviceProvider_TCPConnection::create_events()
{
#ifdef WIN32
	read_event = Event(new EventProvider_Win32Socket(&socket, EventProvider_Win32Socket::socket_event_read));
	write_event = Event(new EventProvider_Win32Socket(&socket, EventProvider_Win32Socket::socket_event_write));
	except_event = Event(new EventProvider_Win32Socket(&socket, EventProvider_Win32Socket::socket_event_exception));
#else
	read_event = Event(new EventProvider_UnixSocket(socket.get_handle(), EventProvider::type_fd_read));
	write_event = Event(new EventProvider_UnixSocket(socket.get_handle(), EventProvider::type_fd_write));
	except_event = Event(new EventProvider_UnixSocket(socket.get_handle(), EventProvider::type_fd_exception));
#endif
}

}
