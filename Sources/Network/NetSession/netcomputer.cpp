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
#include "API/Core/System/exception.h"
#include "API/Network/NetSession/netcomputer.h"
#include "API/Network/NetSession/netsession.h"
#include "netcomputer_impl.h"
#include "netsession_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer construction:

CL_NetComputer::CL_NetComputer()
{
}

CL_NetComputer::CL_NetComputer(const CL_NetComputer &copy)
: impl(copy.impl)
{
}

CL_NetComputer::~CL_NetComputer()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer attributes:

CL_SocketName CL_NetComputer::get_name() const
{
	if (impl)
		return impl->name;
	return CL_SocketName();
}

CL_NetSession CL_NetComputer::get_session()
{
	if (!impl)
		throw CL_Exception(cl_text("CL_NetComputer::get_session() not allowed on an unattached netcomputer object"));

	return CL_NetSession(impl->netsession);
}

bool CL_NetComputer::is_disconnected() const
{
	if (!impl)
		return true;
	else
		return impl->disconnected;
}

const CL_String &CL_NetComputer::get_disconnect_reason() const
{
	static CL_String null_msg(cl_text("Unattached netcomputer object"));

	if (!impl)
		return null_msg;
	else
		return impl->disconnect_reason;
}

bool CL_NetComputer::operator == (const CL_NetComputer &other_instance) const
{
	return (impl == other_instance.impl);
}

bool CL_NetComputer::operator < (const CL_NetComputer &other_instance) const
{
	return (impl.get() < other_instance.impl.get());
}

bool CL_NetComputer::operator > (const CL_NetComputer &other_instance) const
{
	return (impl.get() > other_instance.impl.get());
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer operations:

void CL_NetComputer::disconnect(const CL_String &reason)
{
}

void CL_NetComputer::send(const CL_String &packet_channel, const CL_DataBuffer &packet, bool reliable)
{
	if (!impl)
		throw CL_Exception(cl_text("CL_NetComputer::send not allowed on an unattached netcomputer object"));
}

CL_NetComputer &CL_NetComputer::operator = (const CL_NetComputer &other_instance)
{
	impl = other_instance.impl;
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetComputer implementation:

CL_NetComputer::CL_NetComputer(CL_SharedPtr<CL_NetComputer_Impl> impl)
: impl(impl)
{
}
