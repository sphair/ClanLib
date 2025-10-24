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

#include "API/Network/NetSession/inputsource_netstream.h"
#include "API/Network/NetSession/netstream.h"
#include "netstream_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_NetStream construction:

CL_InputSource_NetStream::CL_InputSource_NetStream(const CL_NetStream &sock)
: netstream(sock.impl), ref_count_netstream(true)
{
	netstream->add_ref();
}

CL_InputSource_NetStream::~CL_InputSource_NetStream()
{
	if (ref_count_netstream) netstream->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_NetStream attributes:

int CL_InputSource_NetStream::tell() const
{
	return 0;
}

int CL_InputSource_NetStream::size() const
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_NetStream operations:

int CL_InputSource_NetStream::read(void *data, int size)
{
	return CL_NetStream(netstream).recv(data, size);
}

void CL_InputSource_NetStream::open()
{
}

void CL_InputSource_NetStream::close()
{
}

CL_InputSource *CL_InputSource_NetStream::clone() const
{
	return new CL_InputSource_NetStream(CL_NetStream(netstream));
}

void CL_InputSource_NetStream::seek(int pos, SeekEnum seek_type)
{
}

void CL_InputSource_NetStream::push_position()
{
}

void CL_InputSource_NetStream::pop_position()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_NetStream implementation:
