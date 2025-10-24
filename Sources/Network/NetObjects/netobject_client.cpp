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

#include "API/Network/NetObjects/netobject_client.h"
#include "API/Network/NetObjects/netobject_controller.h"
#include "netobject_client_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Client construction:

CL_NetObject_Client::CL_NetObject_Client(int obj_id, const CL_NetComputer &server, CL_NetObject_Controller_Generic *controller)
: impl(0)
{
	impl = new CL_NetObject_Client_Generic(obj_id, server, controller);
	impl->add_ref();
}

CL_NetObject_Client::CL_NetObject_Client(const CL_NetObject_Client &copy)
: impl(copy.impl)
{
	if (impl) impl->add_ref();
}

CL_NetObject_Client::~CL_NetObject_Client()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Client attributes:

int CL_NetObject_Client::get_obj_id() const
{
	return impl->obj_id;
}

CL_NetComputer &CL_NetObject_Client::get_server() const
{
	return impl->server;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Client operations:

void CL_NetObject_Client::send(int msg_type, const CL_NetPacket &message, bool reliable)
{
	impl->send(msg_type, message, reliable);
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Client signals:

CL_Signal_v1<CL_NetPacket &> &CL_NetObject_Client::sig_received_message(int msg_type)
{
	return impl->sig_received_message[msg_type];
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Client implementation:
