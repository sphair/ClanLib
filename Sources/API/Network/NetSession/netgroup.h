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
#include <vector>
#include "netcomputer.h"

/// \brief Represents a group of computers.
///
/// \xmlonly !group=Network/NetSessions! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_NetGroup : public std::vector<CL_NetComputer>
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a group.
	///
	/// \param computer Add this computer to group.
	/// \param group Add this group of computers to the group.
	CL_NetGroup();

	CL_NetGroup(const CL_NetComputer &computer);

	CL_NetGroup(const std::vector<CL_NetComputer> &group);


/// \}
/// \name Operations
/// \{

public:
	/// \brief Send packet to all the computers in the group.
	void send(const CL_String &packet_channel, const CL_DataBuffer &packet, bool reliable = true);

	/// \brief Disconnects all computers in the group.
	void disconnect(const CL_String &reason);


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


/// \}
