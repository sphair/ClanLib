/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

#include "Network/precomp.h"
#include "netobject_server_generic.h"
#include "netobject_controller_generic.h"
#include "API/Core/IOData/iodevice_memory.h"
#include "API/Core/System/databuffer.h"
#include "API/Network/NetSession/netgroup.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Server_Generic construction:

CL_NetObject_Server_Generic::CL_NetObject_Server_Generic(
	CL_NetObject_Controller_Generic *controller)
: obj_id(controller->obj_id_counter++), controller(controller)
{
	controller->server_objects[obj_id] = this;
}

CL_NetObject_Server_Generic::~CL_NetObject_Server_Generic()
{
	controller->server_objects.erase(controller->server_objects.find(obj_id));
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Server_Generic attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Server_Generic operations:

void CL_NetObject_Server_Generic::receive(int msg_type, CL_NetComputer &from, CL_DataBuffer &packet)
{
	std::map< int, CL_Signal_v2<CL_NetComputer &, CL_DataBuffer &> >::iterator it;
	it = sig_received_message.find(msg_type);
	if (it != sig_received_message.end())
	{
		it->second.invoke(from, packet);
	}
	else
	{
		// No message handler found.
		// Inform application somehow?
	}
}

void CL_NetObject_Server_Generic::send(CL_NetGroup &group, int msg_type, const CL_DataBuffer &message)
{
	CL_IODevice_Memory netpacket;
	netpacket.write_int8(0);
	netpacket.write_int32(obj_id);
	netpacket.write_int32(msg_type);
	netpacket.write(message.get_data(), message.get_size());
	group.send(controller->channel, netpacket.get_data());
}

void CL_NetObject_Server_Generic::send(CL_NetComputer &computer, int msg_type, const CL_DataBuffer &message)
{
	CL_IODevice_Memory netpacket;
	netpacket.write_int8(0);
	netpacket.write_int32(obj_id);
	netpacket.write_int32(msg_type);
	netpacket.write(message.get_data(), message.get_size());
	computer.send(controller->channel, netpacket.get_data());
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Server_Generic implementation:
