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

#include "GDI/precomp.h"
#include "gdi_display_message_queue_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GDIDisplayMessageQueue_Provider Construction:

CL_GDIDisplayMessageQueue_Provider::CL_GDIDisplayMessageQueue_Provider()
{
}

CL_GDIDisplayMessageQueue_Provider::~CL_GDIDisplayMessageQueue_Provider()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GDIDisplayMessageQueue_Provider Attributes:

bool CL_GDIDisplayMessageQueue_Provider::has_messages() const
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GDIDisplayMessageQueue_Provider Operations:

int CL_GDIDisplayMessageQueue_Provider::wait()
{
	return 0;
}

void CL_GDIDisplayMessageQueue_Provider::wait_message()
{
}

void CL_GDIDisplayMessageQueue_Provider::process(bool only_first_message)
{
}

void CL_GDIDisplayMessageQueue_Provider::add_wakeup_event(CL_Event &event)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GDIDisplayMessageQueue_Provider Implementation:
