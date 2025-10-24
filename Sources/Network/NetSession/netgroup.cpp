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

#include "API/Network/NetSession/netgroup.h"
#include "API/Network/NetSession/netcomputer.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetGroup construction:

CL_NetGroup::CL_NetGroup()
{
}

CL_NetGroup::CL_NetGroup(const CL_NetComputer &computer)
{
	push_back(computer);
}

CL_NetGroup::CL_NetGroup(const std::list<CL_NetComputer> &group) : std::list<CL_NetComputer>(group)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetGroup operations:

void CL_NetGroup::send(const std::string &packet_channel, const CL_NetPacket &packet, bool reliable)
{
	for (iterator it = begin(); it != end(); it++)
	{
		(*it).send(packet_channel, packet, reliable);
	}
}

void CL_NetGroup::disconnect()
{
	for (iterator it = begin(); it != end(); it++)
	{
		(*it).disconnect();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetGroup implementation:
