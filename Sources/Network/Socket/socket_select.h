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

#ifndef header_socket_select
#define header_socket_select

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef WIN32
#include <winsock2.h> // This has to be included before <list> else winsock.h somehow is included
#endif

#include <list>
#include "API/Core/System/thread.h"

class CL_Thread;
class CL_Mutex;
class CL_EventTrigger_Socket;

class CL_SocketSelect : CL_Runnable
{
//! Construction:
public:
	//: Creates the socket select listener thread.
	CL_SocketSelect();

	//: Shut down socket select listener thread.
	virtual ~CL_SocketSelect();

//! Attributes:
public:

//! Operations:
public:
	//: Start a read listen for the socket.
	void listen_read(CL_EventTrigger_Socket *socket);

	//: Start a write listen for the socket.
	void listen_write(CL_EventTrigger_Socket *socket);

	//: Start a exception listen for the socket.
	void listen_exception(CL_EventTrigger_Socket *socket);

	//: Remove socket from the read listening.
	void remove_read(CL_EventTrigger_Socket *socket);

	//: Remove socket from the write listening.
	void remove_write(CL_EventTrigger_Socket *socket);

	//: Remove socket from the exception listening.
	void remove_exception(CL_EventTrigger_Socket *socket);

//! Implementation:
private:
	//: Make the socket select thread break its listen and check for updates.
	//- <p>This is used to make it start listening on new sockets, and stop listening
	//- on old sockets.</p>
	void signal_listen_thread();

	//: Socket select thread function.
	virtual void run();

	CL_Thread *listen_thread;
	CL_Mutex *mutex;
	volatile bool stop_thread;

	std::list<CL_EventTrigger_Socket*> read_sockets;
	std::list<CL_EventTrigger_Socket*> write_sockets;
	std::list<CL_EventTrigger_Socket*> exception_sockets;

#ifdef WIN32
	WSAEVENT signal_event;
#else
	int signal_pipes[2];
#endif
};

#endif
