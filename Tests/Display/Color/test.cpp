/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "test.h"

int g_bConstructor = 0;
int g_bDestructor = 0;

int main(int argc, char** argv)
{
	TestApp program;
	program.main();
}

int TestApp::main()
{
	// Create a console window for text-output if not available
	ConsoleWindow console("Console");

	try
	{

		Console::write_line("ClanLib Test Suite:");
		Console::write_line("-------------------");
#ifdef WIN32
		Console::write_line("Target: WIN32");
#else
		Console::write_line("Target: LINUX");
#endif
		Console::write_line("Directory: API/Core/Math");

		test_hsv();
		test_hsl();
	
		Console::write_line("All Tests Complete");
		console.display_close_message();
	}

	catch(Exception error)
	{
		Console::write_line("Exception caught:");
		Console::write_line(error.message);
		console.display_close_message();
		return -1;
	}

	return 0;
}

void TestApp::fail(void)
{
	throw Exception("Failed Test");
}



