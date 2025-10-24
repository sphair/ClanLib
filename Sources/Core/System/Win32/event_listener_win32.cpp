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
#include "event_listener_win32.h"
#include "event_trigger_win32.h"
#include "API/Core/System/system.h"

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener_Win32 construction:

CL_EventListener_Win32::CL_EventListener_Win32(CL_EventListener *owner)
:
	CL_EventListener_Generic(owner)
{
}

CL_EventListener_Win32::~CL_EventListener_Win32()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventListener_Win32 implementation:

bool CL_EventListener_Win32::wait(int timeout)
{
	if (triggers.empty()) // no triggers, just do the sleep.
	{
		CL_System::sleep(timeout);
		return false;
	}

	int num_handles = triggers.size();
	HANDLE *handles = new HANDLE[num_handles];

	int i = 0;
	std::list< CL_SharedPtr<CL_EventTrigger_Generic> >::iterator it;
	for (it = triggers.begin(); it != triggers.end(); it++)
	{
		CL_EventTrigger_Win32 *trigger = static_cast<CL_EventTrigger_Win32*>((*it).get());
		handles[i++] = trigger->wait_handle;
	}

	DWORD result = WaitForMultipleObjects(
		num_handles,
		handles,
		FALSE,
		timeout == -1 ? INFINITE : timeout);

	delete[] handles;

	if (result == WAIT_TIMEOUT) return false;
	return true;
}
