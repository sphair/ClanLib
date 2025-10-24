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

#include "API/Network/NetSession/netpacket.h"
#include "netobject_client_generic.h"
#include "netobject_controller_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Client construction:

CL_NetObject_Client_Generic::CL_NetObject_Client_Generic(
	int obj_id,
	const CL_NetComputer &server,
	CL_NetObject_Controller_Generic *controller)
: obj_id(obj_id), server(server), controller(controller), ref_count(0)
{
	CL_NetObject_Controller_Generic::ClientID client_id(server, obj_id);
	controller->client_objects[client_id] = this;
}

CL_NetObject_Client_Generic::~CL_NetObject_Client_Generic()
{
	CL_NetObject_Controller_Generic::ClientID client_id(server, obj_id);
	controller->client_objects.erase(controller->client_objects.find(client_id));
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Client_Generic attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Client_Generic operations:

void CL_NetObject_Client_Generic::receive(int msg_type, CL_NetPacket &packet)
{
	std::map< int, CL_Signal_v1<CL_NetPacket &> >::iterator it;
	it = sig_received_message.find(msg_type);
	if (it != sig_received_message.end())
	{
		it->second(packet);
	}
	else
	{
		// No message handler found.
		// Inform application somehow?
	}
}

void CL_NetObject_Client_Generic::send(int msg_type, const CL_NetPacket &message, bool reliable)
{
	CL_NetPacket netpacket;
	netpacket.output.write_bool8(true);
	netpacket.output.write_int32(obj_id);
	netpacket.output.write_int32(msg_type);
	netpacket.output.write(message.get_data(), message.get_size());
	server.send(controller->channel, netpacket, reliable);
}
