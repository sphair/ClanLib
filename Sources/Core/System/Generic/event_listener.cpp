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
#include "API/Core/System/event_listener.h"
#include "event_listener_generic.h"

#ifdef WIN32
#include "Core/System/Win32/event_listener_win32.h"
#else
#include "Core/System/Unix/event_listener_unix.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener construction:

CL_EventListener::CL_EventListener()
: impl(NULL)
{
#ifdef WIN32
	impl = new CL_EventListener_Win32(this);
#else
	impl = new CL_EventListener_Unix(this);
#endif
}

CL_EventListener::CL_EventListener(CL_EventTrigger *trigger)
: impl(NULL)
{
#ifdef WIN32
	impl = new CL_EventListener_Win32(this);
#else
	impl = new CL_EventListener_Unix(this);
#endif

	add_trigger(trigger);
}

CL_EventListener::~CL_EventListener()
{
	if (impl) delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener operations:

void CL_EventListener::add_trigger(CL_EventTrigger *trigger)
{
	if (impl) impl->add_trigger(trigger);
}

void CL_EventListener::remove_trigger(CL_EventTrigger *trigger)
{
	if (impl) impl->remove_trigger(trigger);
}

void CL_EventListener::clear()
{
	if (impl) impl->clear();
}

void CL_EventListener::reset_all()
{
	if (impl) impl->reset_all();
}

bool CL_EventListener::wait(int timeout)
{
	if (impl) return impl->wait(timeout);
	else return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener implementation:
