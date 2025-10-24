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

#include "API/Network/setupnetwork.h"
#include "Socket/event_trigger_socket.h"
#include "Socket/socket_select.h"
#include <signal.h>

#ifndef sighandler_t
#define sighandler_t sig_t
#endif

static int ref_count = 0;
static sighandler_t old_handler = 0;

CL_SetupNetwork::CL_SetupNetwork(bool register_resources_only)
{
	CL_SetupNetwork::init(register_resources_only);
}

CL_SetupNetwork::~CL_SetupNetwork()
{
	CL_SetupNetwork::deinit();
}

/////////////////////////////////////////////////////////////////////////////
// Initialize network.

void CL_SetupNetwork::init(bool register_resources_only)
{
	ref_count++;
	if (ref_count > 1) return;

	// Dont make broken pipes terminate the program (doh):
	old_handler = signal(13, SIG_IGN); // 13 = SIGPIPE

	CL_EventTrigger_Socket::socket_select = new CL_SocketSelect();
}

/////////////////////////////////////////////////////////////////////////////
// Deinitialize network.

void CL_SetupNetwork::deinit()
{
	ref_count--;
	if (ref_count > 0) return;

	delete CL_EventTrigger_Socket::socket_select;
	CL_EventTrigger_Socket::socket_select = 0;
	
	signal(13, old_handler);
}
