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
**
**	File purpose:
**
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

#include "API/Core/System/system.h"
#include "API/Core/System/setup_core.h"
#include "API/Core/System/exception.h"
#include <tchar.h>

// Win32 implementation of CL_System functions:

unsigned int CL_System::get_time()
{
	return (unsigned int) (get_microseconds() / 1000);
}

cl_uint64 CL_System::get_microseconds()
{
	static LARGE_INTEGER perf_counter;
	static double perf_frequency;
	static bool first_time = true;

	if (first_time)
	{
		LARGE_INTEGER perf_frequency_int64;
		QueryPerformanceFrequency(&perf_frequency_int64);
		perf_frequency = (double) perf_frequency_int64.QuadPart;
		first_time = false;
	}

	QueryPerformanceCounter(&perf_counter);
	double quad_part = (double) perf_counter.QuadPart;
	return (cl_uint64) (((1000000.0 * quad_part) / perf_frequency) + 0.5);
}

CL_String CL_System::get_exe_path()
{
	// Get path to executable:
	TCHAR szDllName[_MAX_PATH];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFilename[256];
	TCHAR szExt[256];
	GetModuleFileName(0, szDllName, _MAX_PATH);
#ifdef _CRT_INSECURE_DEPRECATE
	_tsplitpath_s(szDllName, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFilename, 256, szExt, 256);
#else
	_tsplitpath(szDllName, szDrive, szDir, szFilename, szExt);
#endif

	return CL_String(szDrive) + CL_String(szDir); 
}

