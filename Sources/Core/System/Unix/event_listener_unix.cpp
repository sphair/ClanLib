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
#include "event_listener_unix.h"
#include "event_trigger_unix.h"
#include "API/Core/System/system.h"

// note: this cannot be replaced by <ctime>! (timeval needs to be defined)
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener_Unix construction:

CL_EventListener_Unix::CL_EventListener_Unix(CL_EventListener *owner)
:
	CL_EventListener_Generic(owner)
{
}

CL_EventListener_Unix::~CL_EventListener_Unix()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener_Unix implementation:

bool CL_EventListener_Unix::wait(int timeout)
{
	if (triggers.empty()) // no triggers, just do the sleep.
	{
		CL_System::sleep(timeout);
		return false;
	}

	fd_set rfds;
	FD_ZERO(&rfds);
	int highest_fd = -1;

	std::list< CL_SharedPtr<CL_EventTrigger_Generic> >::iterator it;
	for (it = triggers.begin(); it != triggers.end(); ++it)
	{
		CL_EventTrigger_Unix *trigger = static_cast<CL_EventTrigger_Unix*>((*it).get());
		
		if (trigger->wait_pipe[0] > highest_fd) highest_fd = trigger->wait_pipe[0];
		FD_SET(trigger->wait_pipe[0], &rfds);
	}
	
	timeval tv;
	tv.tv_sec = timeout/1000;
	tv.tv_usec = (timeout%1000)*1000;

	int result = select(
		highest_fd+1,
		&rfds,
		NULL,
		NULL,
		(timeout == -1) ? NULL : &tv);

	if (result <= 0) return false;
	return true;
}
