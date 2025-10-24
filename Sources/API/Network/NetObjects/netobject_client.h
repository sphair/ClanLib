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
#include "../../Core/Signals/signal_v1.h"
#include "../../Core/System/sharedptr.h"

class CL_NetObject_Client_Generic;
class CL_DataBuffer;
class CL_NetObject_Controller_Generic;
class CL_NetComputer;

/// \brief Network replication object, client version.
///
///  <p>The netobject is a message dispatcher for objects being replicated over the network.</p>
///    <p>It provides an easy system for a server object to send messages to replicated clients
///    on remote computers, as well as getting answers back.</p> 
/// \xmlonly !group=Network/NetObjects! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_NetObject_Client
{
/// \name Construction
/// \{

public:
	/// \brief NetObject client constructor.
	CL_NetObject_Client(int obj_id, const CL_NetComputer &server, CL_NetObject_Controller_Generic *controller);

	/// \brief Copy constructor.
	CL_NetObject_Client(const CL_NetObject_Client &copy);

	/// \brief NetObject destructor.
	virtual ~CL_NetObject_Client();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the netobject handle that identifies it over the network.
	int get_obj_id() const;

	/// \brief Returns the netcomputer handle to the server of this object.
	CL_NetComputer &get_server();


/// \}
/// \name Operations
/// \{

public:
	/// \brief Send a message to the server object.
	void send(int msg_type, const CL_DataBuffer &message);


/// \}
/// \name Signals
/// \{

public:
	/// \brief Signal emitted when receiving messages of the specified type from the server.
	CL_Signal_v1<CL_DataBuffer &> &sig_received_message(int msg_type);


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_NetObject_Client_Generic> impl;
/// \}
};


/// \}
