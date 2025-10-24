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

/// \addtogroup clanNetwork_NetObjects clanNetwork NetObjects
/// \{


#pragma once


#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include "../api_network.h"
#include "../../Core/Signals/signal_v3.h"
#include "../../Core/System/databuffer.h"
#include "../NetSession/netcomputer.h"

class CL_NetObject_Controller_Generic;
class CL_NetGroup;
class CL_NetObject_Client;

/// \brief Network object replication channel.
///
///    <p>Manages a netchannel with netobject replication data. It dispatches incoming
///    messages to the correct netobject.</p> 
/// \xmlonly !group=Network/NetObjects! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_NetObject_Controller
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a netobject controller.
	CL_NetObject_Controller(class CL_NetSession *netsession, const CL_String &channel);

	/// \brief Copy constructor.
	CL_NetObject_Controller(const CL_NetObject_Controller &copy);

	/// \brief NetObject Controller destructor.
	virtual ~CL_NetObject_Controller();


/// \}
/// \name Attributes
/// \{

public:


/// \}
/// \name Operations
/// \{

public:
	// Copy assignment operator.
	CL_NetObject_Controller &operator =(const CL_NetObject_Controller &copy);


/// \}
/// \name Signals
/// \{

public:
	/// \brief Signal that is called whenever the netobject channel receives a message for a netobject
	/// \brief it doesnt know. The application should hook a slot into this signal and determine what
	/// \brief to do with the message.
	/** <p>CL_Signal_v3<netobject, msgtype, message>.</p>
	    <p>Signal parameters:
	    CL_NetObject &netobj_handle - Handle identifying the netobject created.
	    int msg_type - Message type of the message (the msg_type param used to send it with netobj.send()).
	    CL_NetPacket &message - The message itself.</p>*/
	CL_Signal_v3<CL_NetObject_Client &, int, CL_DataBuffer &> &sig_create_object();


/// \}
/// \name Implementation
/// \{

public:
	/// \brief NetObjectController implementation.
	CL_SharedPtr<CL_NetObject_Controller_Generic> impl;
/// \}
};


/// \}
