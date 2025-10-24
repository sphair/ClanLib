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
**    (if your name is missing here, please add it)
*/

#pragma once


#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include "API/Network/NetSession/netcomputer.h"
#include "API/Core/Signals/signal_v3.h"
#include <map>
class CL_NetSession;
class CL_DataBuffer;
class CL_NetObject_Client;
class CL_NetObject_Client_Generic;
class CL_NetObject_Server_Generic;

class CL_NetObject_Controller_Generic
{
/// \name Construction
/// \{

public:
	CL_NetObject_Controller_Generic(CL_NetSession *netsession, const CL_String &channel);

	~CL_NetObject_Controller_Generic();


/// \}
/// \name Attributes
/// \{

public:
	CL_Signal_v3<CL_NetObject_Client &, int, CL_DataBuffer &> sig_create_object;

	std::map<int, CL_NetObject_Server_Generic *> server_objects;

	typedef std::pair<CL_NetComputer, int> ClientID;

	std::map<ClientID, CL_NetObject_Client_Generic *> client_objects;

	CL_NetSession *netsession;

	CL_String channel;

	int obj_id_counter;


/// \}
/// \name Operations
/// \{

public:


/// \}
/// \name Implementation
/// \{

public:
	void on_received_netpacket(CL_DataBuffer &packet, CL_NetComputer &from);

	CL_Slot slot_received_netpacket;
/// \}
};


