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

#include "API/Network/NetSession/netpacket.h"
#include "API/Network/NetSession/netcomputer.h"
#include "netpacket_generic.h"
#include "inputsource_netpacket_generic.h"
#include "outputsource_netpacket_generic.h"
#include "netcomputer_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetPacket construction:

CL_NetPacket::CL_NetPacket()
: impl(new CL_NetPacket_Generic)
{
	input.impl->netpacket = impl;
	output.impl->netpacket = impl;
	impl->add_ref();
}

CL_NetPacket::CL_NetPacket(const void *data, int size)
: impl(new CL_NetPacket_Generic)
{
	input.impl->netpacket = impl;
	output.impl->netpacket = impl;
	impl->add_ref();

	output.write(data, size);
}

CL_NetPacket::CL_NetPacket(const std::string &data)
: impl(new CL_NetPacket_Generic)
{
	input.impl->netpacket = impl;
	output.impl->netpacket = impl;
	impl->add_ref();

	output.write(data.data(), data.length());
}

CL_NetPacket::CL_NetPacket(const CL_NetPacket &copy)
: impl(copy.impl)
{
	input.impl->netpacket = impl;
	output.impl->netpacket = impl;
	input.impl->position = copy.input.impl->position;
	output.impl->position = copy.output.impl->position;

	if (impl) impl->add_ref();
}

CL_NetPacket::~CL_NetPacket()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetPacket attributes:

unsigned char *CL_NetPacket::get_data()
{
	return impl->data;
}

const unsigned char *CL_NetPacket::get_data() const
{
	return impl->data;
}

int CL_NetPacket::get_size() const
{
	return impl->size;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetPacket operations:

void CL_NetPacket::resize(int new_size)
{
	impl->resize(new_size);
}

CL_NetPacket &CL_NetPacket::operator =(const CL_NetPacket &copy)
{
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();

	input.impl->netpacket = impl;
	output.impl->netpacket = impl;
	input.impl->position = copy.input.impl->position;
	output.impl->position = copy.output.impl->position;

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetPacket implementation:
