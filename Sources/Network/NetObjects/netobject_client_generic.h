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

#pragma once


#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/Signals/signal_v1.h"
#include "API/Network/NetSession/netcomputer.h"
#include <map>

class CL_NetObject_Controller_Generic;

class CL_NetObject_Client_Generic
{
/// \name Construction
/// \{

public:
	CL_NetObject_Client_Generic(
		int obj_id,
		const CL_NetComputer &server,
		CL_NetObject_Controller_Generic *controller);

	~CL_NetObject_Client_Generic();


/// \}
/// \name Attributes
/// \{

public:
	int obj_id;

	CL_NetComputer server;

	CL_NetObject_Controller_Generic *controller;

	std::map< int, CL_Signal_v1<CL_DataBuffer &> > sig_received_message;


/// \}
/// \name Operations
/// \{

public:
	void receive(int msg_type, CL_DataBuffer &packet);

	void send(int msg_type, const CL_DataBuffer &message);


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


