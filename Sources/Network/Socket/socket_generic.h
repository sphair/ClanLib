/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#ifndef socket_generic
#define socket_generic

#include "socket_select.h"
#include "API/Core/System/keep_alive.h"
#include "API/Network/Socket/socket.h"
#include "event_trigger_socket.h"
#include <stack>

#ifndef WIN32
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <sys/time.h>
	#include <unistd.h> 
#else
	#include <winsock2.h>
	#include <windows.h>
	typedef int socklen_t;
#endif

class CL_EventTrigger_Socket;
class CL_Socket_Generic;

class CL_Socket_KeepAliveHelper : public CL_KeepAlive
{
public:
	CL_Socket_KeepAliveHelper(CL_Socket_Generic *impl) : impl(impl) { return; }

	virtual void keep_alive();

	CL_Socket_Generic *impl;

	CL_Signal_v0 sig_read_triggered;

	CL_Signal_v0 sig_write_triggered;

	CL_Signal_v0 sig_exception_triggered;
	
	CL_Signal_v0 sig_disconnected;
};

class CL_Socket_Generic
{
//! Static operations:
public:
	static CL_IPAddress create_ip_address(const sockaddr_in &addr_in);

//! Construction:
public:
	CL_Socket_Generic();

	virtual ~CL_Socket_Generic();

//! Attributes:
public:
	int sock;

	int domain; // PF_INET, PF_INET6

	int protocol; // SOCK_STREAM, SOCK_DGRAM

	std::stack<std::string> push_stack;

	CL_EventTrigger_Socket *read, *write, *exception;

	CL_Socket_KeepAliveHelper *keep_alive_helper;

//! Operations:
public:
	void add_ref();

	void release_ref();

	static std::string get_error_message();

//! Overridables:
public:
	virtual CL_EventTrigger_Socket *create_read_trigger();

	virtual CL_EventTrigger_Socket *create_write_trigger();

	virtual CL_EventTrigger_Socket *create_exception_trigger();

//! Implementation:
private:
	int ref_count;
	
	//: True if the socket is connected, false if it is no longer in use
	bool valid;
	
	//: Internal function to shutdown a socket that has disconnected
	void disconnect();

	friend class CL_Socket_KeepAliveHelper;
};

#endif
