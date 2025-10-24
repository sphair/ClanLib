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

#ifndef header_netobject_server_generic
#define header_netobject_server_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/signals.h"
#include <map>
class CL_NetComputer;
class CL_NetPacket;
class CL_NetGroup;
class CL_NetObject_Controller_Generic;

class CL_NetObject_Server_Generic
{
//! Construction:
public:
	CL_NetObject_Server_Generic(CL_NetObject_Controller_Generic *controller);

	~CL_NetObject_Server_Generic();

//! Attributes:
public:
	int obj_id;

	CL_NetObject_Controller_Generic *controller;

	std::map< int, CL_Signal_v2<CL_NetComputer &, CL_NetPacket &> > sig_received_message;

//! Operations:
public:
	void receive(int msg_type, CL_NetComputer &from, CL_NetPacket &packet);

	void send(CL_NetGroup &group, int msg_type, const CL_NetPacket &message, bool reliable);
	void send(CL_NetComputer &computer, int msg_type, const CL_NetPacket &message, bool reliable);

	int add_ref() { return ++ref_count; }

	int release_ref()
	{
		ref_count--;
		if (ref_count == 0) { delete this; return 0; }
		return ref_count;
	}

//! Implementation:
private:
	int ref_count;
};

#endif
