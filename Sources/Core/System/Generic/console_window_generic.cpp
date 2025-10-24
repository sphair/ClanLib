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
#include "API/Core/System/cl_assert.h"
#include "console_window_generic.h"

#include <iostream>
#include <cstdio>

#ifdef WIN32
#include <stdio.h>
static HANDLE scrbuf = NULL;
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_ConsoleWindow_Generic Construction:

CL_ConsoleWindow_Generic::CL_ConsoleWindow_Generic(
	const std::string &title,
	int width,
	int height)
{
#ifdef WIN32
	AllocConsole();
	SetConsoleTitle(title.c_str());
	COORD coord;
	coord.X = width;
	coord.Y = height;
	scrbuf =
		CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			CONSOLE_TEXTMODE_BUFFER,
			NULL);

	cl_assert(scrbuf != INVALID_HANDLE_VALUE);

	SetConsoleActiveScreenBuffer(scrbuf);
	SetConsoleScreenBufferSize(scrbuf, coord);

	fstdout = 0;
	fstdin = 0;
	fstderr = 0;
#endif
}

CL_ConsoleWindow_Generic::~CL_ConsoleWindow_Generic()
{
#ifdef WIN32
	close_file_handles();
	CloseHandle(scrbuf);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CL_ConsoleWindow_Generic Operations:

void CL_ConsoleWindow_Generic::redirect_stdio()
{
#ifdef WIN32
	close_file_handles();
	fstdin = freopen("CONIN$","rt",stdin);
	fstderr = freopen("CONOUT$", "wt", stderr);
	fstdout = freopen("CONOUT$", "wt", stdout);
#endif
}

void CL_ConsoleWindow_Generic::redirect_stdio(const std::string &file)
{
	close_file_handles();
	fstdout = freopen(file.c_str(),"wt",stdout);
	fstderr = freopen(file.c_str(),"wt",stderr);
}

void CL_ConsoleWindow_Generic::wait_for_key()
{
#ifdef WIN32
	// If your application crashes here, you are linking with a single threaded
	// libc in your application! -- mbn 13. Jan 2001.
  #ifdef __BORLANDC__
	std::string foo;
	cin >> foo;
  #else
	while (!kbhit()) Sleep(250);
  #endif
#endif
}

void CL_ConsoleWindow_Generic::display_close_message()
{
#ifdef WIN32
	std::cout << std::endl << "(press any key to close this console window)";
	std::cout.flush();

	wait_for_key();
#endif
}

void CL_ConsoleWindow_Generic::close_file_handles()
{
#ifdef WIN32
	if (fstdin)
	{
		fclose(fstdin); fstdin = 0;
	}
	if (fstderr)
	{
		fclose(fstderr); fstderr = 0;
	}
	if (fstdout)
	{
		fclose(fstdout); fstdout = 0;
	}
#endif
}
