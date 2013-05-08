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

#pragma once


#include "API/Core/System/event.h"

#ifdef WIN32
#include "event_provider_win32socket.h"
#include "win32_socket.h"
#else
#include "event_provider_unixsocket.h"
#include "unix_socket.h"
#endif

namespace clan
{

class SocketName;
class Event;

class UDPSocket_Impl
{
/// \name Construction
/// \{

public:
	UDPSocket_Impl();
	UDPSocket_Impl(const SocketName &local_name, bool force_bind);
	UDPSocket_Impl(int socket, bool close_socket);
	~UDPSocket_Impl();


/// \}
/// \name Attributes
/// \{

public:
	int get_handle() const;
	SocketName get_local_name() const;
	Event get_read_event();
	Event get_write_event();


/// \}
/// \name Operations
/// \{

public:
	void bind(const SocketName &local_name, bool force_bind = true);
	void set_handle(int socket, bool close_socket);
	int send(const void *data, int len, const SocketName &to);
	int receive(void *data, int len, SocketName &out_from);
	int peek(void *data, int len, SocketName &out_from);


/// \}
/// \name Implementation
/// \{

private:
#ifdef WIN32
	Win32Socket socket;
#else
	UnixSocket socket;
#endif

	Event read_event;
	Event write_event;
/// \}
};

}
