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
*/

#include "Network/precomp.h"
#include "API/Network/Socket/tcp_connection.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Core/System/event.h"
#include "Core/IOData/iodevice_impl.h"
#include "iodevice_provider_tcp_connection.h"

/////////////////////////////////////////////////////////////////////////////
// CL_TCPConnection Construction:

CL_TCPConnection::CL_TCPConnection()
: CL_IODevice(new CL_IODeviceProvider_TCPConnection())
{
}
	
CL_TCPConnection::CL_TCPConnection(const CL_SocketName &remote)
: CL_IODevice(new CL_IODeviceProvider_TCPConnection(remote))
{
}
	
CL_TCPConnection::CL_TCPConnection(const CL_SocketName &remote, const CL_SocketName &local)
: CL_IODevice(new CL_IODeviceProvider_TCPConnection(remote, local))
{
}
	
CL_TCPConnection::CL_TCPConnection(int socket, bool close_socket)
: CL_IODevice(new CL_IODeviceProvider_TCPConnection(socket, close_socket))
{
}
	
CL_TCPConnection::~CL_TCPConnection()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_TCPConnection Attributes:

int CL_TCPConnection::get_handle() const
{
	const CL_IODeviceProvider_TCPConnection *provider = dynamic_cast<const CL_IODeviceProvider_TCPConnection*>(impl->provider);
	return provider->get_handle();
}

CL_SocketName CL_TCPConnection::get_local_name() const
{
	const CL_IODeviceProvider_TCPConnection *provider = dynamic_cast<const CL_IODeviceProvider_TCPConnection*>(impl->provider);
	return provider->get_local_name();
}
	
CL_SocketName CL_TCPConnection::get_remote_name() const
{
	const CL_IODeviceProvider_TCPConnection *provider = dynamic_cast<const CL_IODeviceProvider_TCPConnection*>(impl->provider);
	return provider->get_remote_name();
}

CL_Event CL_TCPConnection::get_read_event()
{
	CL_IODeviceProvider_TCPConnection *provider = dynamic_cast<CL_IODeviceProvider_TCPConnection*>(impl->provider);
	return provider->get_read_event();
}

CL_Event CL_TCPConnection::get_write_event()
{
	CL_IODeviceProvider_TCPConnection *provider = dynamic_cast<CL_IODeviceProvider_TCPConnection*>(impl->provider);
	return provider->get_write_event();
}

/////////////////////////////////////////////////////////////////////////////
// CL_TCPConnection Operations:

void CL_TCPConnection::connect(const CL_SocketName &remote)
{
	CL_IODeviceProvider_TCPConnection *provider = dynamic_cast<CL_IODeviceProvider_TCPConnection*>(impl->provider);
	provider->connect(remote);
}

void CL_TCPConnection::connect(const CL_SocketName &remote, const CL_SocketName &local)
{
	CL_IODeviceProvider_TCPConnection *provider = dynamic_cast<CL_IODeviceProvider_TCPConnection*>(impl->provider);
	provider->connect(remote, local);
}
	
void CL_TCPConnection::set_handle(int socket, bool close_socket)
{
	CL_IODeviceProvider_TCPConnection *provider = dynamic_cast<CL_IODeviceProvider_TCPConnection*>(impl->provider);
	provider->set_handle(socket, close_socket);
}

void CL_TCPConnection::disconnect_graceful()
{
	CL_IODeviceProvider_TCPConnection *provider = dynamic_cast<CL_IODeviceProvider_TCPConnection*>(impl->provider);
	provider->disconnect_graceful();
}

void CL_TCPConnection::disconnect_abortive()
{
	CL_IODeviceProvider_TCPConnection *provider = dynamic_cast<CL_IODeviceProvider_TCPConnection*>(impl->provider);
	provider->disconnect_abortive();
}

void CL_TCPConnection::set_nodelay(bool enable)
{
	CL_IODeviceProvider_TCPConnection *provider = dynamic_cast<CL_IODeviceProvider_TCPConnection*>(impl->provider);
	provider->set_nodelay(enable);
}

void CL_TCPConnection::set_keep_alive(bool enable, int timeout, int interval)
{
	CL_IODeviceProvider_TCPConnection *provider = dynamic_cast<CL_IODeviceProvider_TCPConnection*>(impl->provider);
	provider->set_keep_alive(enable, timeout, interval);
}

/////////////////////////////////////////////////////////////////////////////
// CL_TCPConnection Implementation:
