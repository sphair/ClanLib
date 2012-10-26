/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;

#include <cmath>

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	int start(const std::vector<std::string> &args);

private:
	void on_mouse_down(const InputEvent &key, const InputState &state);
	void on_mouse_up(const InputEvent &key, const InputState &state);
	void on_mouse_move(const InputEvent &key, const InputState &state, DisplayWindow *window);
	void on_window_close(DisplayWindow *window);
	void on_window_moved(DisplayWindow *window);
	void on_window_resize(int xpos, int ypos);
	void on_lost_focus();
	void on_input_up(const InputEvent &key, const InputState &state);
	void draw_window_info(GraphicContext &gc, Font &font, int ypos, DisplayWindowDescription &desc, DisplayWindow &window);
	void update_window_relative_positions();
private:

	bool drag_start;
	Point last_mouse_pos;
	bool quit;

	DisplayWindow *window_2_ptr;
	DisplayWindow *window_3_ptr;
	DisplayWindow *window_4_ptr;
	DisplayWindow *window_5_ptr;
	DisplayWindow *window_main_ptr;

	int num_mouse_multiple_move_events;
	bool mouse_move_event_triggered;

};

// This is the Program class that is called by Application
class Program
{
public:
	static int main(const std::vector<std::string> &args)
	{
		// Initialize ClanLib base components
		SetupCore setup_core;

		// Initialize the ClanLib display component
		SetupDisplay setup_display;

		// Initilize the OpenGL drivers
		SetupGL setup_gl;

		// Start the Application
		App app;
		int retval = app.start(args);
		return retval;
	}
};

// Instantiate Application, informing it where the Program is located
Application app(&Program::main);

// The start of the Application
int App::start(const std::vector<std::string> &args)
{
	quit = false;
	drag_start = false;
	window_main_ptr = NULL;
	num_mouse_multiple_move_events = 0;

	try
	{
		DisplayWindowDescription desc_window_main;
		desc_window_main.set_title("Main Window");
		desc_window_main.set_allow_resize(true);
		desc_window_main.show_caption(true);
		desc_window_main.set_position(Rect(256, 128, Size(512, 128)), true);


		DisplayWindow window_main(desc_window_main);
		std::vector<Slot> slots;

		slots.push_back(window_main.sig_window_close().connect(this, &App::on_window_close, &window_main));
		slots.push_back(window_main.sig_window_moved().connect(this, &App::on_window_moved, &window_main));
		slots.push_back(window_main.sig_resize().connect(this, &App::on_window_resize));
		slots.push_back(window_main.get_ic().get_mouse().sig_key_down().connect(this, &App::on_mouse_down));
		slots.push_back(window_main.get_ic().get_mouse().sig_key_up().connect(this, &App::on_mouse_up));
		slots.push_back(window_main.get_ic().get_mouse().sig_pointer_move().connect(this, &App::on_mouse_move, &window_main));
		slots.push_back(window_main.sig_lost_focus().connect(this, &App::on_lost_focus));
		slots.push_back(window_main.get_ic().get_keyboard().sig_key_up().connect(this, &App::on_input_up));

		DisplayWindowDescription desc_window_2;
		desc_window_2.set_title("Window 2");
		desc_window_2.set_allow_resize(true);
		desc_window_2.show_caption(true);
		desc_window_2.set_owner_window(window_main);
		Rect rect = window_main.get_geometry();
		desc_window_2.set_position(rect.translate(0, rect.get_height()), false);
		DisplayWindow window_2(desc_window_2);
		slots.push_back(window_2.get_ic().get_keyboard().sig_key_up().connect(this, &App::on_input_up));
		slots.push_back(window_2.sig_window_close().connect(this, &App::on_window_close, &window_2));

		DisplayWindowDescription desc_window_3;
		desc_window_3.set_title("Window 3");
		desc_window_3.set_allow_resize(true);
		desc_window_3.show_caption(false);
		desc_window_3.set_owner_window(window_2);
		rect = window_2.get_geometry();
		desc_window_3.set_position(rect.translate(0, rect.get_height()), false);
		DisplayWindow window_3(desc_window_3);
		slots.push_back(window_3.get_ic().get_keyboard().sig_key_up().connect(this, &App::on_input_up));
		slots.push_back(window_3.sig_window_close().connect(this, &App::on_window_close, &window_3));

		DisplayWindowDescription desc_window_4;
		desc_window_4.set_title("Window 4");
		desc_window_4.set_allow_resize(false);
		desc_window_4.show_caption(false);
		desc_window_4.set_owner_window(window_3);
		rect = window_3.get_geometry();
		desc_window_4.set_position(rect.translate(0, rect.get_height()), true);
		DisplayWindow window_4(desc_window_4);
		slots.push_back(window_4.get_ic().get_keyboard().sig_key_up().connect(this, &App::on_input_up));
		slots.push_back(window_4.sig_window_close().connect(this, &App::on_window_close, &window_4));

		DisplayWindowDescription desc_window_5;
		desc_window_5.set_title("Window 5");
		desc_window_5.set_allow_resize(true);
		desc_window_5.show_caption(true);
		rect = window_4.get_geometry();
		desc_window_5.set_position(rect.translate(0, rect.get_height()), false);
		DisplayWindow window_5(desc_window_5);
		slots.push_back(window_5.get_ic().get_keyboard().sig_key_up().connect(this, &App::on_input_up));
		slots.push_back(window_5.sig_window_close().connect(this, &App::on_window_close, &window_5));

		window_main_ptr = &window_main;
		window_2_ptr = &window_2;
		window_3_ptr = &window_3;
		window_4_ptr = &window_4;
		window_5_ptr = &window_5;


		Font font(window_main.get_gc(), "tahoma", 16);

		// Run until someone presses escape
		while (!quit)
		{
			mouse_move_event_triggered = false;
			//
			GraphicContext gc = window_2.get_gc();
			gc.clear(Colorf(0.0f,0.0f,0.0f, 0.0f));
			font.draw_text(gc, 8, 16, "Owned by Main Window");
			draw_window_info(gc, font, 16, desc_window_2, window_2);
			window_2.flip(0);

			//
			gc = window_3.get_gc();
			gc.clear(Colorf(0.0f,0.0f,0.0f, 0.0f));
			font.draw_text(gc, 8, 16, "Owned by Window 2");
			draw_window_info(gc, font, 16, desc_window_3, window_3);
			window_3.flip(0);

			//
			gc = window_4.get_gc();
			gc.clear(Colorf(0.0f,0.0f,0.0f, 0.0f));
			font.draw_text(gc, 8, 16, "Owned by Window 3");
			draw_window_info(gc, font, 16, desc_window_4, window_4);
			window_4.flip(0);

			//
			gc = window_5.get_gc();
			gc.clear(Colorf(0.0f,0.0f,0.0f, 0.0f));
			draw_window_info(gc, font, 16, desc_window_5, window_5);
			window_5.flip(0);

			//
			gc = window_main.get_gc();
			gc.clear(Colorf(0.0f,0.0f,0.0f, 0.0f));
			font.draw_text(gc, 8, 16, string_format("Hold down left mouse button inside this window to drag it. | MultipleMoveMove#%1", num_mouse_multiple_move_events));
			draw_window_info(gc, font, 16, desc_window_main, window_main);
			window_main.flip(1);

			// This call processes user input and other events
			KeepAlive::process();
		}
	}
	catch(Exception &exception)
	{
		// Create a console window for text-output if not available
		ConsoleWindow console("Console", 80, 160);
		Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
		console.display_close_message();

		return -1;
	}

	return 0;
}

void App::draw_window_info(GraphicContext &gc, Font &font, int ypos, DisplayWindowDescription &desc, DisplayWindow &window)
{
	const int ygap = 16;
	ypos += ygap;
	Rect rect;
	rect = desc.get_position();
	if (desc.get_position_client_area())
	{
		font.draw_text(gc, 8, ypos, string_format("Initial (Client Area): x=%1, y=%2, width=%3, height=%4", rect.left, rect.top, rect.get_width(), rect.get_height()));
	}
	else
	{
		font.draw_text(gc, 8, ypos, string_format("Initial (Window Area): x=%1, y=%2, width=%3, height=%4", rect.left, rect.top, rect.get_width(), rect.get_height()));
	}

	ypos += ygap;
	rect = window.get_viewport();
	font.draw_text(gc, 8, ypos, string_format("Viewport (Client Area): x=%1, y=%2, width=%3, height=%4", rect.left, rect.top, rect.get_width(), rect.get_height()));
	ypos += ygap;
	rect = window.get_geometry();
	font.draw_text(gc, 8, ypos, string_format("Geometry (Window Area): x=%1, y=%2, width=%3, height=%4", rect.left, rect.top, rect.get_width(), rect.get_height()));
	ypos += ygap;

	InputDevice &mouse = window.get_ic().get_mouse();
	Point pos = mouse.get_position();
	font.draw_text(gc, 8, ypos, string_format("Mouse (Client Area): x=%1, y=%2", pos.x, pos.y));
	ypos += ygap;
	pos = window.client_to_screen(pos);
	font.draw_text(gc, 8, ypos, string_format("Mouse (Screen Area): x=%1, y=%2", pos.x, pos.y));
	ypos += ygap;

	Draw::gradient_fill(gc, Rectf(0, ypos, gc.get_width(), gc.get_height()), Gradient(Colorf::blue, Colorf::white));
	Draw::fill(gc, 0, ypos, 2, gc.get_height(), Colorf::green);
	Draw::fill(gc, gc.get_width()-2, ypos, gc.get_width(), gc.get_height(), Colorf::green);

}

void App::on_mouse_down(const InputEvent &key, const InputState &state)
{
	last_mouse_pos = key.mouse_pos;
	drag_start = true;
}

void App::on_mouse_up(const InputEvent &key, const InputState &state)
{
	drag_start = false;
}

void App::on_lost_focus()
{
	drag_start = false;
}

void App::on_mouse_move(const InputEvent &key, const InputState &state, DisplayWindow *window)
{
	if (mouse_move_event_triggered)
	{
		num_mouse_multiple_move_events++;
	}
	else
	{
		mouse_move_event_triggered = true;
	}
	if (drag_start)
	{
		Rect geometry = window->get_geometry();
		geometry.translate(key.mouse_pos.x - last_mouse_pos.x, key.mouse_pos.y - last_mouse_pos.y);
		window->set_position(geometry.left, geometry.top);

		// This should update in on_window_moved()
		//update_window_relative_positions();
	}
}

void App::update_window_relative_positions()
{
	Rect geometry = window_main_ptr->get_geometry();

	Rect other_rect = window_2_ptr->get_geometry();
	window_2_ptr->set_position(Rect(geometry.left, geometry.bottom, other_rect.get_size()), false);

	geometry = other_rect;
	other_rect = window_3_ptr->get_geometry();
	window_3_ptr->set_position(Rect(geometry.left, geometry.bottom, other_rect.get_size()), false);

	geometry = other_rect;
	other_rect = window_4_ptr->get_geometry();
	window_4_ptr->set_position(Rect(geometry.left, geometry.bottom, other_rect.get_size()), false);

	geometry = other_rect;
	other_rect = window_5_ptr->get_geometry();
	window_5_ptr->set_position(Rect(geometry.left, geometry.bottom, other_rect.get_size()), false);

}

void App::on_window_moved(DisplayWindow *window)
{
	update_window_relative_positions();
}

void App::on_window_resize(int xpos, int ypos)
{
	update_window_relative_positions();
}

void App::on_window_close(DisplayWindow *window)
{
	quit = true;
}

void App::on_input_up(const InputEvent &key, const InputState &state)
{
	if(key.id == KEY_ESCAPE)
	{
		quit = true;
	}
}

