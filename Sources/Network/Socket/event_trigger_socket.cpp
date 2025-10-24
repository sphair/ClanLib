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

#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif

#include "event_trigger_socket.h"
#include "socket_generic.h"
#include "API/Core/System/cl_assert.h"

#ifdef WIN32
#include "Core/System/Win32/event_trigger_win32.h"
#endif


/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_EventTrigger_Socket::CL_EventTrigger_Socket(CL_Socket_Generic *sock, Type type)
: sock(sock), type(type)
{
	// Someone forgot to call CL_SetupNetwork::init().
	cl_assert(socket_select != 0);

	start_listen();
}

CL_EventTrigger_Socket::~CL_EventTrigger_Socket()
{
	switch (type)
	{
	case read:      socket_select->remove_read(this); break;
	case write:     socket_select->remove_write(this); break;
	case exception: socket_select->remove_exception(this); break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

/////////////////////////////////////////////////////////////////////////////
// Operations:

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void CL_EventTrigger_Socket::start_listen()
{
	reset();
	switch (type)
	{
	case read:      socket_select->listen_read(this); break;
	case write:     socket_select->listen_write(this); break;
	case exception: socket_select->listen_exception(this); break;
	}
}

CL_SocketSelect *CL_EventTrigger_Socket::socket_select = 0;
