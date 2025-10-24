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

#pragma once


#include "API/Core/System/event.h"
#include "API/Core/IOData/iodevice_provider.h"
#ifdef WIN32
#include "event_provider_win32socket.h"
#include "win32_socket.h"
#else
#include "event_provider_unixsocket.h"
#include "unix_socket.h"
#endif

class CL_SocketName;
class CL_Event;

class CL_IODeviceProvider_TCPConnection : public CL_IODeviceProvider
{
/// \name Construction
/// \{

public:
	CL_IODeviceProvider_TCPConnection();
	CL_IODeviceProvider_TCPConnection(const CL_SocketName &remote);
	CL_IODeviceProvider_TCPConnection(const CL_SocketName &remote, const CL_SocketName &local);
	CL_IODeviceProvider_TCPConnection(int handle, bool close_socket);
	~CL_IODeviceProvider_TCPConnection();


/// \}
/// \name Attributes
/// \{

public:
	int get_handle() const { return socket.get_handle(); }
	CL_SocketName get_local_name() const;
	CL_SocketName get_remote_name() const;
	CL_Event get_read_event();
	CL_Event get_write_event();

/// \}
/// \name Operations
/// \{

public:
	void connect(const CL_SocketName &remote);
	void connect(const CL_SocketName &remote, const CL_SocketName &local);
	void set_handle(int socket, bool close_socket);
	void disconnect_graceful();
	void disconnect_abortive();
	void set_nodelay(bool enable);
	void set_keep_alive(bool enable, int timeout, int interval);
	int send(const void *data, int len, bool send_all);
	int receive(void *data, int len, bool receive_all);
	int peek(void *data, int len);
	CL_IODeviceProvider *duplicate();

/// \}
/// \name Implementation
/// \{

private:
	void create_events();

#ifdef WIN32
	CL_Win32Socket socket;
#else
	CL_UnixSocket socket;
#endif
	CL_Event read_event;
	CL_Event write_event;
	CL_Event except_event;
	int timeout;
/// \}
};


