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

/// \addtogroup clanNetwork_NetSessions clanNetwork NetSessions
/// \{


#pragma once


#if _MSC_VER > 1000
#pragma once
#endif

#include "../api_network.h"
#include "../../Core/System/sharedptr.h"

class CL_DataBuffer;
class CL_SocketName;
class CL_NetSession;
class CL_NetComputer_Impl;

/// \brief Class representing a computer in a network game.
///
/// \xmlonly !group=Network/NetSessions! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_NetComputer
{
/// \name Construction
/// \{

public:
	/// \brief Creates a NetComputer object.
	CL_NetComputer();

	CL_NetComputer(const CL_NetComputer &copy);

	/// \brief NetComputer Destructor.
	virtual ~CL_NetComputer();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the name (address and port) of the computer.
	CL_SocketName get_name() const;

	/// \brief Returns the netsession that the computer is attached to.
	CL_NetSession get_session();

	/// \brief Returns true if computer is disconnected.
	bool is_disconnected() const;

	/// \brief Returns the disconnect reason.
	const CL_String &get_disconnect_reason() const;

	/// \brief Returns true if objects are the same.
	bool operator == (const CL_NetComputer &other_instance) const;

	/// \brief Returns true if the other netcomputer is less.
	/** <p>This is used for sorting purposes (eg. if you use a std::map<CL_NetComputer, Player>).</p>*/
	bool operator < (const CL_NetComputer &other_instance) const;

	/// \brief Returns true if the other netcomputer is greater.
	/** <p>This is used for sorting purposes (eg. if you use a std::map<CL_NetComputer, Player>).</p>*/
	bool operator > (const CL_NetComputer &other_instance) const;


/// \}
/// \name Operations
/// \{

public:
	/// \brief Disconnects the computer from server.
	void disconnect(const CL_String &reason);

	/// \brief Send packet to the specified channel.
	/** <p>set reliable to false to send with udp.</p>*/
	void send(const CL_String &packet_channel, const CL_DataBuffer &packet, bool reliable = true);

	/// \brief Copy Operator.
	CL_NetComputer &operator = (const CL_NetComputer &other_instance);


/// \}
/// \name Implementation
/// \{

public:
	CL_NetComputer(CL_SharedPtr<CL_NetComputer_Impl> impl);

private:
	CL_SharedPtr<CL_NetComputer_Impl> impl;
/// \}
};


/// \}
