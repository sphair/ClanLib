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
#include "API/Network/Socket/tcp_connection.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Core/System/event.h"
#include "Core/IOData/iodevice_impl.h"
#include "iodevice_provider_tcp_connection.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// TCPConnection Construction:

TCPConnection::TCPConnection()
: IODevice(new IODeviceProvider_TCPConnection())
{
}
	
TCPConnection::TCPConnection(const SocketName &remote)
: IODevice(new IODeviceProvider_TCPConnection(remote))
{
}
	
TCPConnection::TCPConnection(const SocketName &remote, const SocketName &local)
: IODevice(new IODeviceProvider_TCPConnection(remote, local))
{
}
	
TCPConnection::TCPConnection(int socket, bool close_socket)
: IODevice(new IODeviceProvider_TCPConnection(socket, close_socket))
{
}
	
TCPConnection::~TCPConnection()
{
}

/////////////////////////////////////////////////////////////////////////////
// TCPConnection Attributes:

int TCPConnection::get_handle() const
{
	const IODeviceProvider_TCPConnection *provider = dynamic_cast<const IODeviceProvider_TCPConnection*>(impl->provider);
	return provider->get_handle();
}

SocketName TCPConnection::get_local_name() const
{
	const IODeviceProvider_TCPConnection *provider = dynamic_cast<const IODeviceProvider_TCPConnection*>(impl->provider);
	return provider->get_local_name();
}
	
SocketName TCPConnection::get_remote_name() const
{
	const IODeviceProvider_TCPConnection *provider = dynamic_cast<const IODeviceProvider_TCPConnection*>(impl->provider);
	return provider->get_remote_name();
}

Event TCPConnection::get_read_event()
{
	IODeviceProvider_TCPConnection *provider = dynamic_cast<IODeviceProvider_TCPConnection*>(impl->provider);
	return provider->get_read_event();
}

Event TCPConnection::get_write_event()
{
	IODeviceProvider_TCPConnection *provider = dynamic_cast<IODeviceProvider_TCPConnection*>(impl->provider);
	return provider->get_write_event();
}

/////////////////////////////////////////////////////////////////////////////
// TCPConnection Operations:

void TCPConnection::connect(const SocketName &remote)
{
	IODeviceProvider_TCPConnection *provider = dynamic_cast<IODeviceProvider_TCPConnection*>(impl->provider);
	provider->connect(remote);
}

void TCPConnection::connect(const SocketName &remote, const SocketName &local)
{
	IODeviceProvider_TCPConnection *provider = dynamic_cast<IODeviceProvider_TCPConnection*>(impl->provider);
	provider->connect(remote, local);
}
	
void TCPConnection::set_handle(int socket, bool close_socket)
{
	IODeviceProvider_TCPConnection *provider = dynamic_cast<IODeviceProvider_TCPConnection*>(impl->provider);
	provider->set_handle(socket, close_socket);
}

void TCPConnection::disconnect_graceful()
{
	IODeviceProvider_TCPConnection *provider = dynamic_cast<IODeviceProvider_TCPConnection*>(impl->provider);
	provider->disconnect_graceful();
}

void TCPConnection::disconnect_abortive()
{
	IODeviceProvider_TCPConnection *provider = dynamic_cast<IODeviceProvider_TCPConnection*>(impl->provider);
	provider->disconnect_abortive();
}

void TCPConnection::set_nodelay(bool enable)
{
	IODeviceProvider_TCPConnection *provider = dynamic_cast<IODeviceProvider_TCPConnection*>(impl->provider);
	provider->set_nodelay(enable);
}

void TCPConnection::set_keep_alive(bool enable, int timeout, int interval)
{
	IODeviceProvider_TCPConnection *provider = dynamic_cast<IODeviceProvider_TCPConnection*>(impl->provider);
	provider->set_keep_alive(enable, timeout, interval);
}

/////////////////////////////////////////////////////////////////////////////
// TCPConnection Implementation:

}
