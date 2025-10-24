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
**    Kenneth Gangstoe
**    Mark Page
*/

#include "Core/precomp.h"
#include "API/Core/Text/console.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"

void CL_Console::write(const CL_StringRef &text)
{
#ifdef WIN32
	/*
		Do not replace WriteConsole with the commented WriteFile code unless you
		really know what you are doing and understand when ANSI code pages are used and
		when OEM code pages are used.  -- mbn 29. jan 2008

	CL_String8 t = CL_StringHelp::text_to_local8(text);
	DWORD written = 0;
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), t.data(), t.length(), &written, 0);
	*/

	DWORD written = 0;

	CL_String16 str = CL_StringHelp::utf8_to_ucs2(text);
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), str.c_str(), str.length(), &written, 0);

#else
	::write(1, text.data(), text.length());
#endif
}

/// \brief Block until a key is pressed in the console window.
void CL_Console::wait_for_key()
{
	#ifdef WIN32
		while (!_kbhit()) Sleep(250);
		_getch();	// Required, else the next call to wait_for_key would fail
	#endif
/*
		write_line("Press any key");
	#ifdef WIN32
		TCHAR buffer[1];
		DWORD written = 0;
		BOOL result = ReadConsole(GetStdHandle(STD_INPUT_HANDLE), buffer, 1, &written, 0);
	#else
		char buffer[1];
		read(0, buffer, 1);
	#endif
*/	
}
