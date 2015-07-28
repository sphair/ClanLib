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

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;

#include <cmath>

// This is the Application class (That is instantiated by the Program Class)
class App : public clan::Application
{
public:
	App();
	bool update() override;

private:
	void on_mouse_down(const InputEvent &key);
	void on_mouse_up(const InputEvent &key);
	void on_mouse_move(const InputEvent &key, DisplayWindow &window);
	void on_window_close();
	void on_window_moved();
	void on_window_resize(int xpos, int ypos);
	void on_lost_focus();
	void on_input_up(const InputEvent &key);
	void draw_window_info(Canvas &canvas, Font &font, int ypos, DisplayWindowDescription &desc, DisplayWindow &window);
	void update_window_relative_positions();
private:

	bool drag_start;
	Pointf last_mouse_pos;
	bool quit;

	DisplayWindow window_2;
	DisplayWindow window_3;
	DisplayWindow window_4;
	DisplayWindow window_5;
	DisplayWindow window_main;

	SlotContainer slots;

	Canvas canvas_window_main;
	Font font;

	int num_mouse_multiple_move_events;
	bool mouse_move_event_triggered;

	DisplayWindowDescription desc_window_main;
	DisplayWindowDescription desc_window_2;
	DisplayWindowDescription desc_window_3;
	DisplayWindowDescription desc_window_4;
	DisplayWindowDescription desc_window_5;

};

clan::ApplicationInstance<App> clanapp;

App::App()
{
	clan::OpenGLTarget::enable();

	quit = false;
	drag_start = false;
	num_mouse_multiple_move_events = 0;

	desc_window_main.set_title("Main Window");
	desc_window_main.set_allow_resize(true);
	desc_window_main.show_caption(true);
	desc_window_main.set_position(Rect(256, 128, Size(512, 128)), true);

	window_main = DisplayWindow(desc_window_main);

	slots.connect(window_main.sig_window_close(), [&](){on_window_close(); });
	slots.connect(window_main.sig_window_moved(), [&](){on_window_moved(); });
	slots.connect(window_main.sig_resize(), bind_member(this, &App::on_window_resize));
	slots.connect(window_main.get_ic().get_mouse().sig_key_down(), this, &App::on_mouse_down);
	slots.connect(window_main.get_ic().get_mouse().sig_key_up(), this, &App::on_mouse_up);
	slots.connect(window_main.get_ic().get_mouse().sig_pointer_move(), [&](const InputEvent &key){on_mouse_move(key, window_main); });
	slots.connect(window_main.sig_lost_focus(), bind_member(this, &App::on_lost_focus));
	slots.connect(window_main.get_ic().get_keyboard().sig_key_up(), this, &App::on_input_up);

	desc_window_2.set_title("Window 2");
	desc_window_2.set_allow_resize(true);
	desc_window_2.show_caption(true);
	desc_window_2.set_owner_window(window_main);
	Rect rect = window_main.get_geometry();
	desc_window_2.set_position(rect.translate(0, rect.get_height()), false);
	window_2 = DisplayWindow(desc_window_2);
	slots.connect(window_2.get_ic().get_keyboard().sig_key_up(), this, &App::on_input_up);
	slots.connect(window_2.sig_window_close(), [&](){on_window_close(); });

	desc_window_3.set_title("Window 3");
	desc_window_3.set_allow_resize(true);
	desc_window_3.set_popup_window();
	desc_window_3.show_caption(false);
	desc_window_3.set_owner_window(window_2);
	rect = window_2.get_geometry();
	desc_window_3.set_position(rect.translate(0, rect.get_height()), false);
	window_3 = DisplayWindow(desc_window_3);
	slots.connect(window_3.get_ic().get_keyboard().sig_key_up(), this, &App::on_input_up);
	slots.connect(window_3.sig_window_close(), [&](){on_window_close(); });

	desc_window_4.set_title("Window 4");
	desc_window_4.set_allow_resize(false);
	desc_window_4.set_popup_window();
	desc_window_4.show_caption(false);
	desc_window_4.set_owner_window(window_3);
	rect = window_3.get_geometry();
	desc_window_4.set_position(rect.translate(0, rect.get_height()), true);
	window_4 = DisplayWindow(desc_window_4);
	slots.connect(window_4.get_ic().get_keyboard().sig_key_up(), this, &App::on_input_up);
	slots.connect(window_4.sig_window_close(), [&](){on_window_close(); });

	desc_window_5.set_title("Window 5");
	desc_window_5.set_allow_resize(true);
	desc_window_5.show_caption(true);
	rect = window_4.get_geometry();
	desc_window_5.set_position(rect.translate(0, rect.get_height()), false);
	window_5 = DisplayWindow(desc_window_5);
	slots.connect(window_5.get_ic().get_keyboard().sig_key_up(), this, &App::on_input_up);
	slots.connect(window_5.sig_window_close(), [&](){on_window_close(); });

	canvas_window_main = Canvas(window_main);
	font = Font("tahoma", 16);
}
bool App::update()
{
	mouse_move_event_triggered = false;
	//
	Canvas canvas(window_2);
	canvas.clear(Colorf(0.0f,0.0f,0.0f, 0.0f));
	font.draw_text(canvas, 8, 16, "Owned by Main Window");
	draw_window_info(canvas, font, 16, desc_window_2, window_2);
	window_2.flip(0);
	//
	canvas = Canvas(window_3);
	canvas.clear(Colorf(0.0f,0.0f,0.0f, 0.0f));
	font.draw_text(canvas, 8, 16, "Owned by Window 2");
	draw_window_info(canvas, font, 16, desc_window_3, window_3);
	window_3.flip(0);

	//
	canvas = Canvas(window_4);
	canvas.clear(Colorf(0.0f,0.0f,0.0f, 0.0f));
	font.draw_text(canvas, 8, 16, "Owned by Window 3");
	draw_window_info(canvas, font, 16, desc_window_4, window_4);
	window_4.flip(0);

	//
	canvas = Canvas(window_5);
	canvas.clear(Colorf(0.0f,0.0f,0.0f, 0.0f));
	draw_window_info(canvas, font, 16, desc_window_5, window_5);
	window_5.flip(0);

	//
	canvas_window_main.clear(Colorf(0.0f,0.0f,0.0f, 0.0f));
	font.draw_text(canvas_window_main, 8, 16, string_format("Hold down left mouse button inside this window to drag it. | MultipleMoveMove#%1", num_mouse_multiple_move_events));
	draw_window_info(canvas_window_main, font, 16, desc_window_main, window_main);
	window_main.flip(1);

	return !quit;
}

void App::draw_window_info(Canvas &canvas, Font &font, int ypos, DisplayWindowDescription &desc, DisplayWindow &window)
{
	const int ygap = 16;
	ypos += ygap;
	Rect rect;
	rect = desc.get_position();
	if (desc.get_position_client_area())
	{
		font.draw_text(canvas, 8, ypos, string_format("Initial (Client Area): x=%1, y=%2, width=%3, height=%4", rect.left, rect.top, rect.get_width(), rect.get_height()));
	}
	else
	{
		font.draw_text(canvas, 8, ypos, string_format("Initial (Window Area): x=%1, y=%2, width=%3, height=%4", rect.left, rect.top, rect.get_width(), rect.get_height()));
	}

	ypos += ygap;
	rect = window.get_viewport();
	font.draw_text(canvas, 8, ypos, string_format("Viewport (Client Area): x=%1, y=%2, width=%3, height=%4", rect.left, rect.top, rect.get_width(), rect.get_height()));
	ypos += ygap;
	rect = window.get_geometry();
	font.draw_text(canvas, 8, ypos, string_format("Geometry (Window Area): x=%1, y=%2, width=%3, height=%4", rect.left, rect.top, rect.get_width(), rect.get_height()));
	ypos += ygap;

	InputDevice &mouse = window.get_ic().get_mouse();
	Pointf pos = mouse.get_position();
	font.draw_text(canvas, 8, ypos, string_format("Mouse (Client Area): x=%1, y=%2", pos.x, pos.y));
	ypos += ygap;
	pos = window.client_to_screen(pos);
	font.draw_text(canvas, 8, ypos, string_format("Mouse (Screen Area): x=%1, y=%2", pos.x, pos.y));
	ypos += ygap;

	canvas.fill_rect(Rectf(0, ypos, canvas.get_width(), canvas.get_height()), Gradient(Colorf::blue, Colorf::white));
	canvas.fill_rect(0, ypos, 2, canvas.get_height(), Colorf::green);
	canvas.fill_rect(canvas.get_width()-2, ypos, canvas.get_width(), canvas.get_height(), Colorf::green);
}

void App::on_mouse_down(const InputEvent &key)
{
	last_mouse_pos = key.mouse_pos;
	drag_start = true;
}

void App::on_mouse_up(const InputEvent &key)
{
	drag_start = false;
}

void App::on_lost_focus()
{
	drag_start = false;
}

void App::on_mouse_move(const InputEvent &key, DisplayWindow &window)
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
		Rect geometry = window.get_geometry();
		geometry.translate(key.mouse_pos.x - last_mouse_pos.x, key.mouse_pos.y - last_mouse_pos.y);
		window.set_position(geometry.left, geometry.top);

		// This should update in on_window_moved()
		//update_window_relative_positions();
	}
}

void App::update_window_relative_positions()
{
	Rect geometry = window_main.get_geometry();

	Rect other_rect = window_2.get_geometry();
	window_2.set_position(Rect(geometry.left, geometry.bottom, other_rect.get_size()), false);

	geometry = other_rect;
	other_rect = window_3.get_geometry();
	window_3.set_position(Rect(geometry.left, geometry.bottom, other_rect.get_size()), false);

	geometry = other_rect;
	other_rect = window_4.get_geometry();
	window_4.set_position(Rect(geometry.left, geometry.bottom, other_rect.get_size()), false);

	geometry = other_rect;
	other_rect = window_5.get_geometry();
	window_5.set_position(Rect(geometry.left, geometry.bottom, other_rect.get_size()), false);

}

void App::on_window_moved()
{
	update_window_relative_positions();
}

void App::on_window_resize(int xpos, int ypos)
{
	update_window_relative_positions();
}

void App::on_window_close()
{
	quit = true;
}

void App::on_input_up(const InputEvent &key)
{
	if(key.id == keycode_escape)
	{
		quit = true;
	}
}

