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

#include "precomp.h"
#include "app.h"

App::App() : quit(false)
{
}

// The start of the Application
int App::start(const std::vector<std::string> &args)
{
	// Setup the window
	DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("Input Example");
	win_desc.set_size(Size( 1000, 700 ), false);
	window = DisplayWindow(win_desc);

	// Connect the slots that we require
	Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
	Slot slot_input_down = (window.get_ic().get_keyboard()).sig_key_down().connect(this, &App::on_input_down);
	Slot slot_mouse_down = (window.get_ic().get_mouse()).sig_key_down().connect(this, &App::on_mouse_down);
	Slot slot_mouse_dblclick = (window.get_ic().get_mouse()).sig_key_dblclk().connect(this, &App::on_mouse_down);

	std::vector<Slot> slot_joystick;
	int max_joysticks = window.get_ic().get_joystick_count();
	for (int joystick_number=0; joystick_number < max_joysticks; joystick_number++)
	{
		Slot current_joystick = window.get_ic().get_joystick(joystick_number).sig_key_down().connect(this, &App::on_joystick_down, joystick_number);
		slot_joystick.push_back(current_joystick);
	}

	canvas = Canvas(window);

	font = clan::Font(canvas, "tahoma", 16);
	vector_font = Font_Vector(canvas, "Bitstream Vera Sans", 256, "../../Game/DiceWar/Resources/bitstream_vera_sans/VeraBd.ttf");

	calculate_matrix(canvas);

	while(!quit)
	{
		canvas.set_map_mode(map_2d_upper_left);

		canvas.fill_rect(Rect(0, 0, canvas.get_width(), canvas.get_height()/2), Gradient(Colorf(0.2f, 0.2f, 0.8f, 1.0f), Colorf(0.0f, 0.0f, 0.2f, 1.0f)));
		canvas.fill_rect(Rect(0, canvas.get_height()/2, canvas.get_width(), canvas.get_height()), Gradient(Colorf(0.0f, 0.0f, 0.2f, 1.0f), Colorf(0.2f, 0.2f, 0.8f, 1.0f)));

		font.draw_text(canvas, 8, 20, "Press any key, mouse button or joystick button to fire text. Use mouse to control direction.");

		int yoffset = canvas.get_height() - 20;
		const int y_gap = 20;

		// Draw Keyboard Information
		draw_keyboard_state(canvas, yoffset);
		yoffset -= y_gap;

		// Draw Mouse Information
		draw_mouse_state(canvas, yoffset);
		yoffset -= y_gap;
	
		// Draw Joysticks Information
		for (int joystick_number=0; joystick_number < max_joysticks; joystick_number++)
		{
			draw_joystick_state(canvas, joystick_number, yoffset);
			yoffset -= y_gap;
		}

		// Draw Tablet Information
		int max_tablets = window.get_ic().get_tablet_count();
		for (int tablet_number=0; tablet_number < max_tablets; tablet_number++)
		{
			draw_tablet_state(canvas, tablet_number, yoffset);
			yoffset -= y_gap;
		}

		canvas.set_map_mode(map_user_projection);
		canvas.set_projection(projection_matrix);

		draw_text_shooter(canvas);

		window.flip(1);

		KeepAlive::process();
	}

	return 0;
}

void App::on_input_down(const InputEvent &key)
{
	if(key.id == keycode_escape)
	{
		quit = true;
	}
	else
	{
		create_shooter(key, key.str, true, false, false);
	}
}

void App::create_shooter( const InputEvent &key, const std::string &str, bool use_red, bool use_green, bool use_blue)
{
	// Set the firing line
	Vec3f vector(0.0f, 0.0f, 20.0f);
	float width = (float) canvas.get_width();
	float height = (float) canvas.get_height();

	vector.x = ((key.mouse_pos.x - width/2)* 10.0f) / width;
	vector.y = -((key.mouse_pos.y - height/2) * 10.0f) / height;

	// Create the text offset
	TextShooter shooter(vector_font, str);
	shooter.set_start_time(System::get_time());
	shooter.set_duration(2 * 1000);
	shooter.set_initial_white_time(1000);
	shooter.set_end_fade_time(1000);
	shooter.set_start_position(Vec3f(0.0f, 0.0f, 0.2f));
	shooter.set_end_position(vector);
	shooter.set_color_component(use_red, use_green, use_blue);
	text_shooter.push_back(shooter);
}

void App::on_mouse_down(const InputEvent &key)
{
	const char *str="Unknown";
	switch (key.id)
	{
		case mouse_left:
			str="Left";
			break;
		case mouse_right:
			str="Right";
			break;
		case mouse_middle:
			str="Middle";
			break;
		case mouse_wheel_up:
			str="Up";
			break;
		case mouse_wheel_down:
			str="Down";
			break;
		case mouse_xbutton1:
			str="Button 1";
			break;
		case mouse_xbutton2:
			str="Button 2";
			break;
		default:
			break;
	}
	create_shooter(key, str, false, true, false);
}

void App::on_joystick_down(const InputEvent &key, int joystick_number)
{
	std::string str(string_format("J%1 B%2", joystick_number, key.id));
	create_shooter(key, str, true, true, false);
}
void App::on_window_close()
{
	quit = true;
}

void App::calculate_matrix(Canvas &canvas)
{
	float lens_zoom = 3.2f;
	float lens_near = 0.1f;
	float lens_far = 10000.0f;
	float lens_aspect = 1.0f;

	float fov = 2.0f * atan2(1.0f, lens_zoom);
	float aspect = 1.0f;
	float width = (float) canvas.get_width();
	float height = (float) canvas.get_height();

	if (height)
		aspect = ( width * lens_aspect) / height;

	fov = (fov * 180.0f) / PI;
	projection_matrix = Mat4f::perspective( fov, aspect, lens_near, lens_far, handed_left, clip_negative_positive_w);
}

void App::draw_text_shooter(Canvas &canvas)
{
	ubyte64 current_time = System::get_time();

	std::list<TextShooter>::iterator it;
	for (it = text_shooter.begin(); it != text_shooter.end();)
	{
		if (it->draw(canvas, current_time))
		{
			++it;
		}
		else
		{
			it = text_shooter.erase(it);
		}
	}
}

void App::draw_keyboard_state(Canvas &canvas, int yoffset)
{
	InputDevice &keyboard = window.get_ic().get_keyboard();

	std::string text(string_format("Keyboard : %1 : %2 is %3", 
		keyboard.get_name(),
		keyboard.get_key_name(keycode_space),
		keyboard.get_keycode(keycode_space) ? "Pressed" : "Released"));

	font.draw_text(canvas, 8, yoffset, text);
}

void App::draw_mouse_state(Canvas &canvas, int yoffset)
{
	InputDevice &mouse = window.get_ic().get_mouse();

	std::string text(string_format("Mouse : %1 : ", 
		mouse.get_name() ));

	text = text + string_format("Left %1 : ", 
		mouse.get_keycode(mouse_left) ? "Pressed" : "Released" );

	text = text + string_format("Middle %1 : ", 
		mouse.get_keycode(mouse_middle) ? "Pressed" : "Released" );

	text = text + string_format("Right %1 : ", 
		mouse.get_keycode(mouse_right) ? "Pressed" : "Released" );

	Point position = mouse.get_position();
	text = text + string_format("Position (%1,%2)", 
		 position.x, position.y);

	font.draw_text(canvas, 8, yoffset, text);
}

void App::draw_joystick_state(Canvas &canvas, int joystick_number, int yoffset)
{
	InputDevice &joystick = window.get_ic().get_joystick(joystick_number);

	std::string text(string_format("Joystick (%1) : %2 : ",
		joystick_number,
		joystick.get_name() ));

	std::vector<int> axis_ids = joystick.get_axis_ids();
	int num_axis = axis_ids.size();
	text = text + string_format("%1 Axis (", 
		num_axis );

	for (int axis_count = 0; axis_count < num_axis; axis_count++)
	{
		// Only display the maximum of 4 axis
		if (axis_count == 4)
		{
			text = text + ", ...";
			break;
		}

		text = text + string_format("%1 %2", 
			axis_count ? ", " : "",
			joystick.get_axis(axis_ids[axis_count]) );
	}

	int num_buttons = joystick.get_button_count();
	text = text + string_format(") : %1 Buttons ", num_buttons);

	for (int button_count = 0; button_count < num_buttons; button_count++)
	{
		text = text + string_format("%1", joystick.get_keycode(button_count) ? "X" : "-" );
	}

	font.draw_text(canvas, 8, yoffset, text);
}

void App::draw_tablet_state(Canvas &canvas, int tablet_number, int yoffset)
{
	InputDevice &tablet = window.get_ic().get_tablet(tablet_number);

	std::string text(string_format("Tablet (%1) : %2 : ",
		tablet_number,
		tablet.get_name() ));

	int num_axis = tablet.get_axis_ids().size();
	text = text + string_format("%1 Axis (", 
		num_axis );

	for (int axis_count = 0; axis_count < num_axis; axis_count++)
	{
		// Only display the maximum of 4 axis
		if (axis_count == 4)
		{
			text = text + ", ...";
			break;
		}

		text = text + string_format("%1 %2", 
			axis_count ? ", " : "",
			tablet.get_axis(axis_count) );
	}

	int num_buttons = tablet.get_button_count();
	text = text + string_format(") : %1 Buttons ", num_buttons);

	for (int button_count = 0; button_count < num_buttons; button_count++)
	{
		text = text + string_format("%1", tablet.get_keycode(button_count) ? "X" : "-" );
	}

	font.draw_text(canvas, 8, yoffset, text);
}
