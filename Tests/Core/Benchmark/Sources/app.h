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
**    Mark Page
*/

#pragma once

#include "tests.h"

class TestInfo
{
public:
	TestInfo(const std::string &name, void (Tests::*func)()) : name(name), func(func), result(unset_value){}

	std::string name;
	void (Tests::*func)();
	int result;
	static const int unset_value = -1232311;

};

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	int start(const std::vector<std::string> &args);

private:
	void on_input_up(const clan::InputEvent &key);
	void on_window_close();
	void initialise_1();
	void initialise_2();
	void write_result();
	void test();
	void draw_info(const std::string &text);
	int run_test();

private:
	bool quit;
	clan::Callback_v0 cb_main;
	clan::Callback_v0 cb_test;
	clan::DisplayWindow window;
	clan::Canvas canvas;
	clan::Font font;
	clan::GameTime game_time;
	Tests tests;
	clan::ubyte64 num_iterations;
	clan::ubyte64 base_line;
	int testlist_offset;
	std::vector<TestInfo> testlist;
};
