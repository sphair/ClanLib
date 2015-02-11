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
*/

#pragma once

#include "tests.h"

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	int start(const std::vector<std::string> &args);

private:
	void on_input_up(const clan::InputEvent &key);
	void on_window_close();
	void initial_pause();
	void initialise_1();
	void initialise_2();
	void write_result();
	void test();
	void draw_info(const std::string &text);
	int64_t run_test();
	uint64_t get_start_time() const;
private:
	bool quit;
	std::function<void()> cb_main;
	std::function<void()> cb_test;
	clan::DisplayWindow window;
	clan::Canvas canvas;
	clan::Font font;
	clan::GameTime game_time;
	Tests tests;
	uint64_t num_iterations;
	uint64_t base_line;
	int testlist_offset;
	std::vector<TestInfo> testlist;
	uint64_t tests_run_length_microseconds;
	float target_test_run_length_seconds;
	std::string priority_class;

};
