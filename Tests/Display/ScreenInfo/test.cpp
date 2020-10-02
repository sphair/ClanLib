/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
**    Harry Storbacka
*/

#include "test.h"

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
		ScreenInfo screen_info;
		int primary_screen;
		std::vector<Rectf> rects = screen_info.get_screen_geometries(primary_screen);
		for (std::vector<Rectf>::size_type i=0; i<rects.size(); i++)
		{
			Console::write_line("Screen geometry: l:%1 t:%2  r:%3 b:%4 %5", rects[i].left, rects[i].top, rects[i].right, rects[i].bottom, i == primary_screen ? string_format("- Primary (%1)", i) : "" );
		}

		console.display_close_message();
		console.wait_for_key();
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




