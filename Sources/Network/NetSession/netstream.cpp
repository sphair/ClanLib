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

#include "Network/precomp.h"
#include "API/Network/NetSession/netstream.h"
#include "API/Network/NetSession/netsession.h"
#include "API/Core/System/event.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetStream construction:

CL_NetStream::CL_NetStream(
	const CL_String &netstream,
	CL_NetComputer &dest)
{
}

CL_NetStream::CL_NetStream(const CL_NetStream &copy)
: CL_IODevice(copy)
{
}

CL_NetStream::~CL_NetStream()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetStream attributes:

CL_Event CL_NetStream::get_read_event()
{
	return CL_Event();
}

CL_Event CL_NetStream::get_write_event()
{
	return CL_Event();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetStream operations:

CL_NetStream &CL_NetStream::operator =(const CL_NetStream &copy)
{
	impl = copy.impl;
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetStream implementation:
