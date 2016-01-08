/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#include "precomp.h"
#include "app.h"

clan::ApplicationInstance<App> clanapp;

App::App()
{
	clan::OpenGLTarget::set_current();
	quit = false;

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib App Example");
	desc.set_size(clan::Size(1000, 700), true);
	desc.set_allow_resize(true);

	window = clan::DisplayWindow(desc);
	canvas = clan::Canvas(window);

	// Connect the Window close event
	sc.connect(window.sig_window_close(), this, &App::on_window_close);

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), this, &App::on_input_up);

	font = clan::Font("tahoma", 16);

	target_test_run_length_seconds = 0.5f;

	tests_run_length_microseconds = 0;
	num_iterations = 0;
	base_line = 0;

	Tests::Init(testlist);

#ifdef WIN32
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	DWORD dwPriClass = GetPriorityClass(GetCurrentProcess());
	switch (dwPriClass)
	{
		case NORMAL_PRIORITY_CLASS:
			priority_class = "Process Priority: NORMAL";
			break;
		case IDLE_PRIORITY_CLASS:
			priority_class = "Process Priority: IDLE";
			break;
		case HIGH_PRIORITY_CLASS:
			priority_class = "Process Priority: HIGH";
			break;
		case REALTIME_PRIORITY_CLASS:
			priority_class = "Process Priority: REALTIME";
			break;
		case BELOW_NORMAL_PRIORITY_CLASS:
			priority_class = "Process Priority: BELOW NORMAL";
			break;
		case ABOVE_NORMAL_PRIORITY_CLASS:
			priority_class = "Process Priority: ABOVE NORMAL";
			break;
	}
	
#endif

	cb_main = clan::bind_member(this, &App::initial_pause);
	game_time.reset();
}

bool App::update()
{
	game_time.update();

	clan::Sizef canvas_size = canvas.get_size();

	// Clear the display in a dark blue nuance
	canvas.clear(clan::Colorf(0.0f,0.0f,0.2f));

	std::string text("ClanLib Benchmark Utility");
	clan::Size text_size = clan::Size(font.measure_text(canvas, text).bbox_size);
	font.draw_text(canvas, ( ( canvas_size.width - text_size.width) / 2), 32, text, clan::Colorf::white);

	int ypos = 96;
	const int ygap = 18;

	if (!priority_class.empty())
	{
		font.draw_text(canvas, 10, ypos, priority_class);
		ypos += ygap;
	}

	if (tests_run_length_microseconds)
	{
		double seconds = (double)tests_run_length_microseconds / 1000000.0;
		font.draw_text(canvas, 10, ypos, clan::string_format("Simulation Test Run Length = %1 seconds", clan::StringHelp::float_to_text(seconds, 2)));
		ypos += ygap;
	}

	if (num_iterations)
	{
		font.draw_text(canvas, 10, ypos, clan::string_format("Using %1 Iterations", num_iterations));
		ypos += ygap;
	}
	font.draw_text(canvas, 10, ypos, "Time");
	font.draw_text(canvas, 80, ypos, ":  Function");
	ypos += ygap;
	for (unsigned int cnt=0; cnt<testlist.size(); cnt++)
	{
		if (testlist[cnt].result != 0.0f)
		{
			clan::Colorf colour = clan::Colorf::white;
			if (testlist_offset == cnt)
				colour = clan::Colorf::green;

			font.draw_text(canvas, 10, ypos, clan::StringHelp::float_to_text(testlist[cnt].result, 1), colour);
			font.draw_text(canvas, 80, ypos, clan::string_format(":  %1", testlist[cnt].name), colour);
			ypos += ygap;
		}
	}

	cb_main();

	return !quit;	
}

// A key was pressed
void App::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

void App::draw_info(const std::string &text)
{
	clan::Size text_size = clan::Size(font.measure_text(canvas, text).bbox_size);
	font.draw_text(canvas,  ( ( canvas.get_width() - text_size.width) / 2), 64, text, clan::Colorf::white);
	window.flip(1);
}

void App::initial_pause()
{
	draw_info("* Starting in 1 second *");

	cb_test = clan::bind_member(&tests, &Tests::test_empty);
	cb_test();
	
	if (game_time.get_current_time() >= 1.0f)
		cb_main = clan::bind_member(this, &App::initialise_1);
}

uint64_t App::get_start_time() const
{
	// Ensure that the time starts at the start of a new tick
	uint64_t start_time = clan::System::get_microseconds();
	while(true)
	{
		uint64_t time_now = clan::System::get_microseconds();
		if (time_now != start_time)
			return time_now;
	}
}

void App::initialise_1()
{
	draw_info("* Calculating Test Run Length *");

	cb_test = clan::bind_member(&tests, &Tests::test_empty);

	tests_run_length_microseconds = (uint64_t) (((double) target_test_run_length_seconds) * 1000000.0);

	const uint64_t num_block_iteration = 10000;

	uint64_t start_time = get_start_time();
	for (num_iterations = 0; ; num_iterations++)
	{
		uint64_t current_time = clan::System::get_microseconds();

		if ((current_time - start_time) >= tests_run_length_microseconds)
			break;
	
		for (uint64_t cnt=0; cnt < num_block_iteration; cnt++)
		{
			cb_test();
		}
		num_iterations+=num_block_iteration;
	}
	cb_main = clan::bind_member(this, &App::initialise_2);
}

void App::initialise_2()
{
	draw_info("* Adjusting Run Timer *");

	cb_test = clan::bind_member(&tests, &Tests::test_empty);
	tests_run_length_microseconds = run_test();
	testlist_offset = 0;
	cb_main = clan::bind_member(this, &App::test);
}

int64_t App::run_test()
{
	uint64_t start_time = get_start_time();
	for (uint64_t cnt=0; cnt < num_iterations; cnt++)
	{
		cb_test();
	}
	return (clan::System::get_microseconds() - start_time);

}

void App::test()
{
	
	draw_info(clan::string_format("* Running - %1 *", testlist[testlist_offset].name));

	cb_test = clan::bind_member(&tests, testlist[testlist_offset].func);
	int64_t microseconds = run_test();
	double result = ((double) microseconds / (double)tests_run_length_microseconds) ;
	testlist[testlist_offset].result = result + 0.05;	// Round up
	testlist_offset++;
	if (testlist_offset >= testlist.size())
		cb_main = clan::bind_member(this, &App::write_result);
}

void App::write_result()
{
	draw_info("* Writing Results To File *");

	std::string output;
#ifdef WIN32
	std::string newline = "\r\n";
#else
	std::string newline = "\r\n";
#endif

	output += "ClanLib Benchmark Utility Results" + newline + newline;

	if (!priority_class.empty())
		output += priority_class + newline;

	output += clan::string_format("Simulation Test Run Length = %1 seconds", clan::StringHelp::float_to_text((double)tests_run_length_microseconds / 1000000.0, 2)) + newline;
	output += clan::string_format("Using %1 Iterations", num_iterations) + newline;

	output+= newline;

	output += "Test Number) Time : Function" + newline;

	for (unsigned int cnt=0; cnt<testlist.size(); cnt++)
	{
		if (testlist[cnt].result != 0.0f)
		{
			output += clan::string_format("%1) %2 : %3", cnt+1, clan::StringHelp::float_to_text(testlist[cnt].result, 1), testlist[cnt].name) + newline;
		}
	}

	clan::File::write_text("results.txt", output);

	cb_main = clan::bind_member(this, &App::initialise_1);
}

