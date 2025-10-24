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
#include "API/Core/System/event_trigger.h"
#include "event_trigger_generic.h"

#ifdef WIN32
#include "Core/System/Win32/event_trigger_win32.h"
#else
#include "Core/System/Unix/event_trigger_unix.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger construction:

CL_EventTrigger::CL_EventTrigger()
{
#ifdef WIN32
	impl = new CL_EventTrigger_Win32;
#else
	impl = new CL_EventTrigger_Unix;
#endif

	reset();
}

CL_EventTrigger::CL_EventTrigger(const CL_EventTrigger &copy)
: impl(copy.impl)
{
}

CL_EventTrigger::~CL_EventTrigger()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger attributes:

bool CL_EventTrigger::get_flag() const
{
	return impl->get_flag();
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger operations:

void CL_EventTrigger::reset() const
{
	impl->reset();
}

void CL_EventTrigger::set_flag()
{
	impl->set_flag();
}

bool CL_EventTrigger::wait(int timeout)
{
	return impl->wait(timeout);
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger implementation:
