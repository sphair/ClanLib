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
#include "event_trigger_win32.h"

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Win32 construction:

CL_EventTrigger_Win32::CL_EventTrigger_Win32()
:
	wait_handle(NULL)
{
	wait_handle = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CL_EventTrigger_Win32::~CL_EventTrigger_Win32()
{
	CloseHandle(wait_handle);
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Win32 attributes:

bool CL_EventTrigger_Win32::get_flag() const
{
	return WaitForSingleObject(wait_handle, 0) != WAIT_TIMEOUT;
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Win32 operations:

void CL_EventTrigger_Win32::reset() const
{
	ResetEvent(wait_handle);
}

void CL_EventTrigger_Win32::set_flag()
{
	SetEvent(wait_handle);
}

bool CL_EventTrigger_Win32::wait(int timeout) const
{
	DWORD result = WaitForSingleObject(
		wait_handle,
		(timeout == -1) ? INFINITE : timeout);

	return (result != WAIT_TIMEOUT);
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventTrigger_Win32 implementation:
