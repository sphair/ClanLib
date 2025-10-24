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
**
**	File purpose:
**    This file is the WinMain entry point. It will setup the clanCore
**    win32 environment.
**
**    This file also contain the win32 specific implementations
**    of the CL_System class.
**
**    The win32 versions of CL_SetupCore functions are also defined here.
*/

#include "Core/precomp.h" // visual c++: precompiled header file MUST be mentioned FIRST.

#ifdef BORLAND
#include <stdio.h>
#pragma hdrstop
#endif

#include "init_win32.h"
#include "API/Core/System/keep_alive.h"
#include "API/Core/System/setupcore.h"
#include "API/Core/System/error.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/System/cl_assert.h"


class FastTimer
{
public:

	FastTimer(int a);

	unsigned int GetTicks() 
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&end);

		diff = ((end - start) * 1000) / freq;
		return (unsigned int)(diff & 0xffffffff);
	}
 
private:
	__int64 freq;
	__int64 start;
	__int64 end;
	__int64 diff;
};


FastTimer::FastTimer(int a)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&start);
}

class CL_Win32Event_Dispatcher : public CL_KeepAlive
{
public:
	virtual void keep_alive();
};

// Setup a CL_System::keep_alive() listener that will read win32 events
// and dispatch them.
CL_Win32Event_Dispatcher *event_dispatcher = NULL;

void CL_SetupCore::set_instance(HINSTANCE hInstance)
{
	CL_System_Win32::hInstance = hInstance;
}

void init_system()
{
	event_dispatcher = new CL_Win32Event_Dispatcher;

	// if you get this assertion, you forgot to call CL_SetupCore::set_instance()
	// prior to CL_SetupCore::init().
	cl_assert(CL_System_Win32::hInstance != NULL);

	// Redirect C++ output streams to the output window in developer studio:
//	std::cout = iostream(&debug_buf);
//	cerr = iostream(&debug_buf);
}

void deinit_system()
{
	delete event_dispatcher;
	event_dispatcher = NULL;
}

void CL_Win32Event_Dispatcher::keep_alive()
{
	// Check for win32 events and dispatch them to MainMessageHandler().

	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE)
	{
		int ret = GetMessage(&msg, NULL, 0, 0);
		if (ret > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (ret == 0)
				throw CL_Error("WM_QUIT");
			else
				throw CL_Error(CL_String::from_int(GetLastError()));
		}
	}
}

// Win32 implementation of CL_System functions:

unsigned int CL_System::sys_time()
{
	static FastTimer fastTimer(0);
	return fastTimer.GetTicks();
}

void CL_System::sleep(int millis)
{
	Sleep(millis);
}

int CL_System::get_num_cores() {
	SYSTEM_INFO sysinf;
	memset(&sysinf, 0, sizeof(SYSTEM_INFO));
	GetSystemInfo(&sysinf);
	long cpus = sysinf.dwNumberOfProcessors;
	return (cpus < 1)?-1 : static_cast<int>(cpus);
}
 
std::string CL_System::get_exe_path()
{
	// Get path to executable:
	TCHAR szDllName[_MAX_PATH];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFilename[256];
	TCHAR szExt[256];
	GetModuleFileName(0, szDllName, _MAX_PATH);
	_splitpath(szDllName, szDrive, szDir, szFilename, szExt);

	return std::string(szDrive) + std::string(szDir); 
}

// Global vars:
HINSTANCE CL_System_Win32::hInstance = NULL;
