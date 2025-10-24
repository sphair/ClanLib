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

#include "test.h"

int TestApp::main(int argc, char **argv)
{
	CL_ConsoleWindow console("Console");

	try
	{
		CL_SetupCore setup_core;
		run_test();
		console.display_close_message();
	}
	catch(CL_Exception error)
	{
		CL_Console::write_line("Unhandled exception: %1", error.message);
		console.display_close_message();
		return -1;
	}

	return 0;
}

void TestApp::run_test()
{
	CL_File file("test.xhtml", CL_File::open_existing, CL_File::access_read);
	CL_DomDocument document(file);
	file.close();
}
