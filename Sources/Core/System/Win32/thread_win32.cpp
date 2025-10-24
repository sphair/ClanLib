/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
#include "thread_win32.h"
#include "API/Core/System/runnable.h"
#include "API/Core/System/thread_local_storage.h"
#include <process.h>

/////////////////////////////////////////////////////////////////////////////
// CL_Thread_Win32 Construction:

CL_Thread_Win32::CL_Thread_Win32()
: handle(NULL)
{
}

CL_Thread_Win32::~CL_Thread_Win32()
{
	if (handle != NULL)
		CloseHandle(handle);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Thread_Win32 Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_Thread_Win32 Operations:

void CL_Thread_Win32::start(CL_Runnable *runnable)
{
	if (handle != NULL)
		CloseHandle(handle);

	// MSDN Docs say to use _beginthreadex instead of CreateThread if using the CRT in the thread
	handle = (HANDLE)_beginthreadex(NULL, 0, &CL_Thread_Win32::thread_main, runnable, 0, NULL);
	if (handle == NULL)
		throw CL_Exception("Unable to create new thread");
}

void CL_Thread_Win32::join()
{
	if (handle != NULL)
	{
		WaitForSingleObject(handle, INFINITE);
		CloseHandle(handle);
		handle = NULL;
	}
}

void CL_Thread_Win32::kill()
{
	if (handle != NULL)
	{
		TerminateThread(handle, 0);
		CloseHandle(handle);
		handle = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_Thread_Win32 Implementation:

unsigned CL_Thread_Win32::thread_main(void *data)
{
	CL_Runnable *runnable = (CL_Runnable *)data;
	CL_ThreadLocalStorage tls;
	runnable->run();
	return 0;
}
