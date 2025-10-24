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

//! clanNetwork="NetObjects"
//! header=network.h

#ifndef header_netobject_controller
#define header_netobject_controller

#ifdef CL_API_DLL
#ifdef CL_NETWORK_EXPORT
#define CL_API_NETWORK __declspec(dllexport)
#else
#define CL_API_NETWORK __declspec(dllimport)
#endif
#else
#define CL_API_NETWORK
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include <string>
#include "../../signals.h"
#include "../NetSession/netcomputer.h"
#include "../NetSession/netpacket.h"

class CL_NetObject_Controller_Generic;
class CL_NetGroup;
class CL_NetObject_Client;

//: Network object replication channel.
//- !group=Network/NetObjects!
//- !header=network.h!
//- <p>Manages a netchannel with netobject replication data. It dispatches incoming
//- messages to the correct netobject.</p>
class CL_API_NETWORK CL_NetObject_Controller
{
//! Construction:
public:
	//: Constructs a netobject controller.
	CL_NetObject_Controller(class CL_NetSession *netsession, const std::string &channel);

	//: Copy constructor.
	CL_NetObject_Controller(const CL_NetObject_Controller &copy);

	//: NetObject Controller destructor.
	virtual ~CL_NetObject_Controller();

//! Attributes:
public:

//! Operations:
public:
	// Copy assignment operator.
	CL_NetObject_Controller &operator =(const CL_NetObject_Controller &copy);

//! Signals:
public:
	//: Signal that is called whenever the netobject channel receives a message for a netobject
	//: it doesnt know. The application should hook a slot into this signal and determine what
	//: to do with the message.
	//- <p>CL_Signal_v3<netobject, msgtype, message>.</p>
	//- <p>Signal parameters:
	//- CL_NetObject &netobj_handle - Handle identifying the netobject created.
	//- int msg_type - Message type of the message (the msg_type param used to send it with netobj.send()).
	//- CL_NetPacket &message - The message itself.</p>
	CL_Signal_v3<CL_NetObject_Client &, int, CL_NetPacket &> &sig_create_object();

//! Implementation:
public:
	//: NetObjectController implementation.
	CL_NetObject_Controller_Generic *impl;
};

#endif
