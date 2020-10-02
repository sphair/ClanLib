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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "test.h"

void TestApp::test_game_time(void)
{
	Console::write_line(" Header: game_time.h");
	Console::write_line("  Class: GameTime");

	Console::write_line("   Function: various");


	GameTime game_time;
	uint64_t current_time_ms = 0;
	uint64_t current_time_micro = 0;
	int total_ms = 0;
	uint64_t total_micro = 0;

	do
	{
		game_time.update();

		clan::System::sleep(10);

		total_ms += game_time.get_time_elapsed_ms();
		total_micro += game_time.get_time_elapsed_microseconds();

		current_time_ms = game_time.get_current_time_ms();
		current_time_micro = game_time.get_current_time_microseconds();

	} while (current_time_ms < 10000);

	int diff_micro = total_micro - current_time_micro;
	if (diff_micro)
	{
		Console::write_line(string_format("microseconds: Expected 0, got %1", diff_micro));
		fail();
	}

	int diff_ms = total_ms - current_time_ms;
	if (diff_ms)
	{
		Console::write_line(string_format("ms: Expected 0, got %1", diff_ms));
		fail();
	}



}
