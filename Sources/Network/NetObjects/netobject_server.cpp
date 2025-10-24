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

#include "API/Network/NetObjects/netobject_server.h"
#include "API/Network/NetObjects/netobject_controller.h"
#include "netobject_server_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Server construction:

CL_NetObject_Server::CL_NetObject_Server(CL_NetObject_Controller *controller)
: impl(0)
{
	impl = new CL_NetObject_Server_Generic(controller->impl);
	impl->add_ref();
}

CL_NetObject_Server::CL_NetObject_Server(const CL_NetObject_Server &copy)
: impl(copy.impl)
{
	if (impl) impl->add_ref();
}

CL_NetObject_Server::~CL_NetObject_Server()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Server attributes:

int CL_NetObject_Server::get_obj_id() const
{
	return impl->obj_id;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Server operations:

void CL_NetObject_Server::send(CL_NetGroup &group, int msg_type, const CL_NetPacket &message, bool reliable)
{
	impl->send(group, msg_type, message, reliable);
}

void CL_NetObject_Server::send(CL_NetComputer &computer, int msg_type, const CL_NetPacket &message, bool reliable)
{
	impl->send(computer, msg_type, message, reliable);
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Server signals:

CL_Signal_v2<CL_NetComputer &, CL_NetPacket &> &CL_NetObject_Server::sig_received_message(int msg_type)
{
	return impl->sig_received_message[msg_type];
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Server implementation:
