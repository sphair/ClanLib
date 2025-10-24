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
**    Harry Storbacka
*/

#include "Core/precomp.h"
#include "console_window_generic.h"
#include "API/Core/Text/console.h"

#include <iostream>
#include <cstdio>

#ifdef WIN32
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
static HANDLE scrbuf = NULL;
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_ConsoleWindow_Generic Construction:

CL_ConsoleWindow_Generic::CL_ConsoleWindow_Generic(
	const CL_StringRef &title,
	int width,
	int height)
{
#ifdef WIN32
	AllocConsole();
	SetConsoleTitle(CL_StringHelp::utf8_to_ucs2(title).c_str());
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

	if(scrbuf == INVALID_HANDLE_VALUE)
		throw CL_Exception("Unable to allocate console screen buffer");

	SetStdHandle(STD_OUTPUT_HANDLE, scrbuf);
	SetConsoleActiveScreenBuffer(scrbuf);
	SetConsoleScreenBufferSize(scrbuf, coord);

#endif
}

CL_ConsoleWindow_Generic::~CL_ConsoleWindow_Generic()
{
#ifdef WIN32
	CloseHandle(scrbuf);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CL_ConsoleWindow_Generic Operations:

void CL_ConsoleWindow_Generic::wait_for_key()
{
#ifdef WIN32
	// If your application crashes here, you are linking with a single threaded
	// libc in your application! -- mbn 13. Jan 2001.
	#ifdef __BORLANDC__
		std::string foo;
		cin >> foo;
	#else
		#ifdef _CRT_INSECURE_DEPRECATE
			while (!_kbhit()) Sleep(250);
		#else
			while (!kbhit()) Sleep(250);
		#endif
	#endif
#endif
}

void CL_ConsoleWindow_Generic::display_close_message()
{
#ifdef WIN32
	CL_Console::write_line("(press any key to close this console window)");
	CL_Console::wait_for_key();
#endif
}
