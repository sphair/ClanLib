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

#include "Core/precomp.h"
#include "event_listener_generic.h"
#include "event_trigger_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener_Generic construction:

CL_EventListener_Generic::CL_EventListener_Generic(CL_EventListener *owner)
: owner(owner)
{
}

CL_EventListener_Generic::~CL_EventListener_Generic()
{
	clear();
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener_Generic operations:

void CL_EventListener_Generic::add_trigger(const CL_EventTrigger *trigger)
{
	triggers.push_back(trigger->impl);
}

void CL_EventListener_Generic::remove_trigger(const CL_EventTrigger *trigger)
{
	triggers.remove(trigger->impl);
}

void CL_EventListener_Generic::clear()
{
	triggers.clear();
}

void CL_EventListener_Generic::reset_all()
{
	std::list< CL_SharedPtr<CL_EventTrigger_Generic> >::iterator it;
	for (it = triggers.begin(); it != triggers.end(); it++) (*it)->reset();
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener_Generic implementation:
