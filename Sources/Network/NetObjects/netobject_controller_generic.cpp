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

#include "netobject_controller_generic.h"
#include "netobject_server_generic.h"
#include "netobject_client_generic.h"
#include "API/Network/NetObjects/netobject_client.h"
#include "API/Network/NetSession/netpacket.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Controller_Generic construction:

CL_NetObject_Controller_Generic::CL_NetObject_Controller_Generic(
	CL_NetSession *netsession,
	const std::string &channel)
: netsession(netsession), channel(channel), obj_id_counter(0), ref_count(0)
{
	slot_received_netpacket = netsession->sig_netpacket_receive(channel).connect(
		this, &CL_NetObject_Controller_Generic::on_received_netpacket);
}

CL_NetObject_Controller_Generic::~CL_NetObject_Controller_Generic()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Controller_Generic attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Controller_Generic operations:


/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Controller_Generic implementation:

void CL_NetObject_Controller_Generic::on_received_netpacket(
	CL_NetPacket &packet,
	CL_NetComputer &from)
{
	bool server_obj = packet.input.read_bool8();
	int obj_id = packet.input.read_int32();
	int msg_type = packet.input.read_int32();
	int sub_pos = packet.input.tell();
	CL_NetPacket subpacket(packet.get_data()+sub_pos, packet.get_size()-sub_pos);

	if (server_obj)
	{
		std::map<int, CL_NetObject_Server_Generic *>::iterator it;
		it = server_objects.find(obj_id);
		if (it != server_objects.end())
		{
			it->second->receive(msg_type, from, subpacket);
		}
		else
		{
			// No such server object. Junk message.
			// Maybe we should allow application to deal with this kind of messages?
		}
	}
	else
	{
		std::map<ClientID, CL_NetObject_Client_Generic *>::iterator it;
		it = client_objects.find(ClientID(from, obj_id));
		if (it != client_objects.end())
		{
			it->second->receive(msg_type, subpacket);
		}
		else
		{
			// No such client object. Construct new one and inform application.

			CL_NetObject_Client netobj(obj_id, from, this);
			sig_create_object(netobj, msg_type, subpacket);
		}
	}
}
