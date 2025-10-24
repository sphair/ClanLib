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

#include "API/Core/System/cl_assert.h"
#include "API/Network/NetSession/inputsource_netpacket.h"
#include "API/Network/NetSession/netpacket.h"
#include "inputsource_netpacket_generic.h"
#include "netpacket_generic.h"
#include <cstring>

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_NetPacket construction:

CL_InputSource_NetPacket::CL_InputSource_NetPacket(const CL_NetPacket &netpacket)
: impl(new CL_InputSource_NetPacket_Generic(netpacket.impl, true))
{
}

CL_InputSource_NetPacket::~CL_InputSource_NetPacket()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_NetPacket attributes:

int CL_InputSource_NetPacket::tell() const
{
	return impl->position;
}

int CL_InputSource_NetPacket::size() const
{
	return impl->netpacket->size;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_NetPacket operations:

#define cl_min(a, b) ((a < b) ? a : b)

int CL_InputSource_NetPacket::read(void *data, int size)
{
	int received = cl_min(size, impl->netpacket->size - impl->position);
	memcpy(data, impl->netpacket->data+impl->position, received);
	impl->position += received;
	return received;
}

void CL_InputSource_NetPacket::open()
{
}

void CL_InputSource_NetPacket::close()
{
}

CL_InputSource *CL_InputSource_NetPacket::clone() const
{
	return new CL_InputSource_NetPacket(*this);
}

void CL_InputSource_NetPacket::seek(int pos, SeekEnum seek_type)
{
	switch (seek_type)
	{
	case seek_set:
		impl->position = pos;
		break;

	case seek_cur:
		impl->position += pos;
		break;

	case seek_end:
		impl->position = size()+pos;
		break;
	}
}

void CL_InputSource_NetPacket::push_position()
{
	cl_assert(false); // not implemented.
}

void CL_InputSource_NetPacket::pop_position()
{
	cl_assert(false); // not implemented.
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_NetPacket implementation:

CL_InputSource_NetPacket::CL_InputSource_NetPacket()
: impl(new CL_InputSource_NetPacket_Generic(0, false))
{
}
