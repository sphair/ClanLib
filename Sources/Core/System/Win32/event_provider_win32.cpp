/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#include "Core/precomp.h"
#include "API/Core/System/exception.h"
#include "event_provider_win32.h"

/////////////////////////////////////////////////////////////////////////////
// CL_EventProvider_Win32 Construction:

CL_EventProvider_Win32::CL_EventProvider_Win32(bool manual_reset, bool initial_state)
: handle(INVALID_HANDLE_VALUE)
{
	handle = CreateEvent(
		0,
		manual_reset ? TRUE : FALSE,
		initial_state ? TRUE : FALSE,
		0);
	if (handle == INVALID_HANDLE_VALUE)
		throw CL_Exception("Unable to create event!");
}

CL_EventProvider_Win32::~CL_EventProvider_Win32()
{
	if (handle != INVALID_HANDLE_VALUE)
		CloseHandle(handle);
	handle = INVALID_HANDLE_VALUE;
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventProvider_Win32 Attributes:

CL_EventProvider::EventType CL_EventProvider_Win32::get_event_type(int index)
{
	return type_native;
}

HANDLE CL_EventProvider_Win32::get_event_handle(int index)
{
	return handle;
}

int CL_EventProvider_Win32::get_num_event_handles()
{
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventProvider_Win32 Operations:

bool CL_EventProvider_Win32::set()
{
	BOOL result = SetEvent(handle);
	return result != FALSE;
}

bool CL_EventProvider_Win32::reset()
{
	BOOL result = ResetEvent(handle);
	return result != FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CL_EventProvider_Win32 Implementation:
