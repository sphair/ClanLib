/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Thread_Win32 Construction:

Thread_Win32::Thread_Win32()
: handle(NULL)
{
}

Thread_Win32::~Thread_Win32()
{
	if (handle != NULL)
		CloseHandle(handle);
}

/////////////////////////////////////////////////////////////////////////////
// Thread_Win32 Attributes:

/////////////////////////////////////////////////////////////////////////////
// Thread_Win32 Operations:

void Thread_Win32::start(Runnable *runnable)
{
	if (handle != NULL)
		CloseHandle(handle);

	// MSDN Docs say to use _beginthreadex instead of CreateThread if using the CRT in the thread
	handle = (HANDLE)_beginthreadex(NULL, 0, &Thread_Win32::thread_main, runnable, 0, NULL);
	if (handle == NULL)
		throw Exception("Unable to create new thread");
}

void Thread_Win32::join()
{
	if (handle != NULL)
	{
		WaitForSingleObject(handle, INFINITE);
		CloseHandle(handle);
		handle = NULL;
	}
}

void Thread_Win32::kill()
{
	if (handle != NULL)
	{
		TerminateThread(handle, 0);
		CloseHandle(handle);
		handle = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Thread_Win32 Implementation:

unsigned Thread_Win32::thread_main(void *data)
{
	Runnable *runnable = (Runnable *)data;
	ThreadLocalStorage tls;
	runnable->run();
	return 0;
}

}
