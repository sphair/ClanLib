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

#include "Core/precomp.h"
#include "API/Core/System/exception.h"
#include "event_provider_socketpair.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// EventProvider_Socketpair Construction:

EventProvider_Socketpair::EventProvider_Socketpair(bool manual_reset, bool initial_state)
: manual_reset(manual_reset), state(false)
{
	// Mac OS X (10.3.8) fails socketpair creation for PF_UNIX protocol.
	// Not sure if its better to use 0 or PF_UNIX for Linux then. The
	// documentation for socketpair is not that clear about the
	// difference. -- mbn 15 april 2005
#ifdef __APPLE__
	int result = socketpair(AF_UNIX, SOCK_DGRAM, 0, wait_sockets);
#else
	int result = socketpair(AF_UNIX, SOCK_DGRAM, PF_UNIX, wait_sockets);
#endif
	if (result == -1)
	{
		switch (errno)
		{
		case EMFILE:
			throw Exception("Could not create event waiting socket pair! Too many descriptors are in use by this process.");
		case EAFNOSUPPORT:
			throw Exception("Could not create event waiting socket pair! The specified address family is not supported on this machine.");
		case EPROTONOSUPPORT:
			throw Exception("Could not create event waiting socket pair! The specified protocol is not supported on this machine.");
#ifdef EOPNOSUPPORT
		case EOPNOSUPPORT:
			throw Exception("Could not create event waiting socket pair! The specified protocol does not support creation of socket pairs.");
#endif
		case EFAULT:
			throw Exception("Could not create event waiting socket pair! wait_sockets does not point at a valid part of the process address space.");
		default:
			throw Exception("Could not create event waiting socket pair!");
		}
	}

	if (initial_state)
		set();
}

EventProvider_Socketpair::~EventProvider_Socketpair()
{
	close(wait_sockets[0]);
	close(wait_sockets[1]);
}

/////////////////////////////////////////////////////////////////////////////
// EventProvider_Socketpair Attributes:

EventProvider::EventType EventProvider_Socketpair::get_event_type(int index)
{
	return type_fd_read;
}

int EventProvider_Socketpair::get_event_handle(int index)
{
	return wait_sockets[1];
}

int EventProvider_Socketpair::get_num_event_handles()
{
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// EventProvider_Socketpair Operations:

bool EventProvider_Socketpair::check_after_wait(int index)
{
	if (!manual_reset)
	{
		// For automatic reset, check if we are first
		// thread:
		MutexSection mutex_lock(&mutex);
		if (state == true)
		{
			int msg = 0;
			recv(wait_sockets[1], &msg, sizeof(int), 0);
			state = false;
			return true;
		}

		// Someone beat us to it, go back and wait.
		return false;
	}
	else
	{
		return true;
	}
}

bool EventProvider_Socketpair::set()
{
	MutexSection mutex_lock(&mutex);
	if (state == false)
	{
		state = true;
		int msg = 1;
		int result = send(wait_sockets[0], &msg, sizeof(int), 0);
		if (result < 0)
			throw Exception("CL_EventProvider_Socketpair::set failed");
	}
	return true;
}

bool EventProvider_Socketpair::reset()
{
	MutexSection mutex_lock(&mutex);
	if (state == true)
	{
		int msg = 0;
		int result = recv(wait_sockets[1], &msg, sizeof(int), 0);
		if (result < 0)
			throw Exception("CL_EventProvider_Socketpair::reset failed");
		state = false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// EventProvider_Socketpair Implementation:

}
