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

#include "netpacket_generic.h"
#include <cstring>

/////////////////////////////////////////////////////////////////////////////
// CL_NetPacket_Generic construction:

CL_NetPacket_Generic::CL_NetPacket_Generic()
: data(0), size(0), allocated_size(0), ref_count(0)
{
}

CL_NetPacket_Generic::~CL_NetPacket_Generic()
{
	delete[] data;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetPacket_Generic attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_NetPacket_Generic operations:

int CL_NetPacket_Generic::add_ref()
{
	return ++ref_count;
}

int CL_NetPacket_Generic::release_ref()
{
	ref_count--;
	if (ref_count > 0) return ref_count;

	delete this;
	return 0;
}

#define cl_min(a, b) ((a < b) ? a : b)

void CL_NetPacket_Generic::resize(int new_size)
{
	if (new_size <= allocated_size)
	{
		size = new_size;
		return;
	}

	int new_allocated_size = new_size + 8*1024;
	unsigned char *old_data = data;
	data = new unsigned char[new_allocated_size];
	memcpy(data, old_data, cl_min(size, new_size));
	delete[] old_data;
	allocated_size = new_allocated_size;
	size = new_size;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetPacket_Generic implementation:
