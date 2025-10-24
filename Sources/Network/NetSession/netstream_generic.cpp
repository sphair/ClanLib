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

#include "netstream_generic.h"
#include "netsession_generic.h"
#include "netcomputer_generic.h"
#include "API/Network/NetSession/netcomputer.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetStream_Generic construction:

CL_NetStream_Generic::CL_NetStream_Generic(
	const std::string &netstream,
	CL_NetComputer &dest,
	CL_NetSession_Generic *session)
: session(session), computer(dest.impl), channel_id(0), closed(false), ref_count(0)
{
	CL_MutexSection mutex_section(&computer->mutex);
	computer->add_ref();

	channel_id = computer->send_stream_connect(netstream, this);
}

CL_NetStream_Generic::CL_NetStream_Generic(
	int channel_id,
	CL_NetComputer_Generic *dest,
	CL_NetSession_Generic *session)
: session(session), computer(dest), channel_id(channel_id), closed(false), ref_count(0)
{
	CL_MutexSection mutex_section(&computer->mutex);
	computer->add_ref();
}

CL_NetStream_Generic::~CL_NetStream_Generic()
{
	if (!closed) computer->send_stream_close(channel_id);

	CL_MutexSection mutex_section(&computer->mutex);
	computer->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetStream_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_NetStream_Generic operations:

void CL_NetStream_Generic::add_ref()
{
	CL_MutexSection mutex_section(&mutex);
	ref_count++;
}

int CL_NetStream_Generic::release_ref()
{
	CL_MutexSection mutex_section(&mutex);
	ref_count--;
	if (ref_count == 0)
	{
		mutex_section.leave();
		delete this;
		return 0;
	}
	return ref_count;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetStream_Generic implementation:
