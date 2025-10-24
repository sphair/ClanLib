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

#include "API/Network/NetObjects/netobject_controller.h"
#include "netobject_controller_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_NetObjectController construction:

CL_NetObject_Controller::CL_NetObject_Controller(CL_NetSession *netsession, const std::string &channel)
: impl(0)
{
	impl = new CL_NetObject_Controller_Generic(netsession, channel);
	impl->add_ref();
}

CL_NetObject_Controller::CL_NetObject_Controller(const CL_NetObject_Controller &copy)
: impl(copy.impl)
{
	if (impl) impl->add_ref();
}

CL_NetObject_Controller::~CL_NetObject_Controller()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObjectController attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Controller operations:

CL_NetObject_Controller &CL_NetObject_Controller::operator =(const CL_NetObject_Controller &copy)
{
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Controller signals:

CL_Signal_v3<CL_NetObject_Client &, int, CL_NetPacket &> &CL_NetObject_Controller::sig_create_object()
{
	return impl->sig_create_object;
}

/////////////////////////////////////////////////////////////////////////////
// CL_NetObject_Controller implementation:
