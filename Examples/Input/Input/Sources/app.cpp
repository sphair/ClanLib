/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
int App::start(const std::vector<CL_String> &args)
{
	// Setup the window
	CL_DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("Input Example");
	win_desc.set_size(CL_Size( 700, 700 ), false);
	window = CL_DisplayWindow(win_desc);

	// Connect the slots that we require
	CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
	CL_Slot slot_input_down = (window.get_ic().get_keyboard()).sig_key_down().connect(this, &App::on_input_down);
	CL_Slot slot_mouse_down = (window.get_ic().get_mouse()).sig_key_down().connect(this, &App::on_mouse_down);

	std::vector<CL_Slot> slot_joystick;
	int max_joysticks = window.get_ic().get_joystick_count();
	for (int joystick_number=0; joystick_number < max_joysticks; joystick_number++)
	{
		CL_Slot current_joystick = window.get_ic().get_joystick(joystick_number).sig_key_down().connect(this, &App::on_joystick_down, joystick_number);
		slot_joystick.push_back(current_joystick);
	}

	CL_GraphicContext gc = window.get_gc();

	font = CL_Font(gc, "tahoma", 16);
	vector_font = CL_Font_Vector("../../Game/DiceWar/Resources/bitstream_vera_sans/VeraBd.ttf", 256);

	calculate_matrix(gc);

	while(!quit)
	{
		gc.set_map_mode(cl_map_2d_upper_left);

		CL_Draw::gradient_fill(gc, CL_Rect(0, 0, gc.get_width(), gc.get_height()/2), CL_Gradient(CL_Colorf(0.2f, 0.2f, 0.8f, 1.0f), CL_Colorf(0.0f, 0.0f, 0.2f, 1.0f)));
		CL_Draw::gradient_fill(gc, CL_Rect(0, gc.get_height()/2, gc.get_width(), gc.get_height()), CL_Gradient(CL_Colorf(0.0f, 0.0f, 0.2f, 1.0f), CL_Colorf(0.2f, 0.2f, 0.8f, 1.0f)));

		font.draw_text(gc, 8, 20, "Press any key, mouse button or joystick button to fire text. Use mouse to control direction.");

		int yoffset = gc.get_height() - 20;
		const int y_gap = 20;

		// Draw Keyboard Information
		draw_keyboard_state(gc, yoffset);
		yoffset -= y_gap;

		// Draw Mouse Information
		draw_mouse_state(gc, yoffset);
		yoffset -= y_gap;
	
		// Draw Joysticks Information
		for (int joystick_number=0; joystick_number < max_joysticks; joystick_number++)
		{
			draw_joystick_state(gc, joystick_number, yoffset);
			yoffset -= y_gap;
		}

		// Draw Tablet Information
		int max_tablets = window.get_ic().get_tablet_count();
		for (int tablet_number=0; tablet_number < max_tablets; tablet_number++)
		{
			draw_tablet_state(gc, tablet_number, yoffset);
			yoffset -= y_gap;
		}

		gc.set_map_mode(cl_user_projection);
		gc.set_projection(projection_matrix);
		gc.set_modelview(modelview_matrix);

		draw_text_shooter(gc);

		window.flip(1);

		CL_KeepAlive::process();
	}

	return 0;
}

void App::on_input_down(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
	else
	{
		create_shooter(key, key.str, true, false, false);
	}
}

void App::create_shooter( const CL_InputEvent &key, const CL_StringRef &str, bool use_red, bool use_green, bool use_blue)
{
	// Set the firing line
	CL_Vec3f vector(0.0f, 0.0f, 20.0f);
	CL_GraphicContext gc = window.get_gc();
	float width = (float) gc.get_width();
	float height = (float) gc.get_height();

	vector.x = ((key.mouse_pos.x - width/2)* 10.0f) / width;
	vector.y = -((key.mouse_pos.y - height/2) * 10.0f) / height;

	// Create the text offset
	TextShooter shooter(vector_font, str);
	shooter.set_start_time(CL_System::get_time());
	shooter.set_duration(2 * 1000);
	shooter.set_initial_white_time(1000);
	shooter.set_end_fade_time(1000);
	shooter.set_start_position(CL_Vec3f(0.0f, 0.0f, 0.2f));
	shooter.set_end_position(vector);
	shooter.set_color_component(use_red, use_green, use_blue);
	text_shooter.push_back(shooter);
}

void App::on_mouse_down(const CL_InputEvent &key, const CL_InputState &state)
{
	const char *str="Unknown";
	switch (key.id)
	{
		case CL_MOUSE_LEFT:
			str="Left";
			break;
		case CL_MOUSE_RIGHT:
			str="Right";
			break;
		case CL_MOUSE_MIDDLE:
			str="Middle";
			break;
		case CL_MOUSE_WHEEL_UP:
			str="Up";
			break;
		case CL_MOUSE_WHEEL_DOWN:
			str="Down";
			break;
		case CL_MOUSE_XBUTTON1:
			str="Button 1";
			break;
		case CL_MOUSE_XBUTTON2:
			str="Button 2";
			break;
		default:
			break;
	}
	create_shooter(key, str, false, true, false);
}

void App::on_joystick_down(const CL_InputEvent &key, const CL_InputState &state, int joystick_number)
{
	CL_String str(cl_format("J%1 B%2", joystick_number, key.id));
	create_shooter(key, str, true, true, false);
}
void App::on_window_close()
{
	quit = true;
}

void App::calculate_matrix(CL_GraphicContext &gc)
{
	float lens_zoom = 3.2f;
	float lens_near = 0.1f;
	float lens_far = 10000.0f;
	float lens_aspect = 1.0f;

	float fov = 2.0f * atan2(1.0f, lens_zoom);
	float aspect = 1.0f;
	float width = (float) gc.get_width();
	float height = (float) gc.get_height();

	if (height)
		aspect = ( width * lens_aspect) / height;

	fov = (fov * 180.0f) / CL_PI;
	projection_matrix = CL_Mat4f::perspective( fov, aspect, lens_near, lens_far);

	modelview_matrix = CL_Mat4f::identity();
	modelview_matrix.matrix[2 + (4*2)] = -1.0f;
}

void App::draw_text_shooter(CL_GraphicContext &gc)
{
	unsigned int current_time = CL_System::get_time();

	std::list<TextShooter>::iterator it;
	for (it = text_shooter.begin(); it != text_shooter.end();)
	{
		if (it->draw(gc, current_time))
		{
			++it;
		}
		else
		{
			it = text_shooter.erase(it);
		}
	}
}

void App::draw_keyboard_state(CL_GraphicContext &gc, int yoffset)
{
	CL_InputDevice &keyboard = window.get_ic().get_keyboard();

	CL_String text(cl_format("Keyboard : %1 : %2 is %3", 
		keyboard.get_name(),
		keyboard.get_key_name(CL_KEY_SPACE),
		keyboard.get_keycode(CL_KEY_SPACE) ? "Pressed" : "Released"));

	font.draw_text(gc, 8, yoffset, text);
}

void App::draw_mouse_state(CL_GraphicContext &gc, int yoffset)
{
	CL_InputDevice &mouse = window.get_ic().get_mouse();

	CL_String text(cl_format("Mouse : %1 : ", 
		mouse.get_name() ));

	text = text + cl_format("Left %1 : ", 
		mouse.get_keycode(CL_MOUSE_LEFT) ? "Pressed" : "Released" );

	text = text + cl_format("Middle %1 : ", 
		mouse.get_keycode(CL_MOUSE_MIDDLE) ? "Pressed" : "Released" );

	text = text + cl_format("Right %1 : ", 
		mouse.get_keycode(CL_MOUSE_RIGHT) ? "Pressed" : "Released" );

	CL_Point position = mouse.get_position();
	text = text + cl_format("Position (%1,%2)", 
		 position.x, position.y);

	font.draw_text(gc, 8, yoffset, text);
}

void App::draw_joystick_state(CL_GraphicContext &gc, int joystick_number, int yoffset)
{
	CL_InputDevice &joystick = window.get_ic().get_joystick(joystick_number);

	CL_String text(cl_format("Joystick (%1) : %2 : ",
		joystick_number,
		joystick.get_name() ));

	int num_axis = joystick.get_axis_count();
	text = text + cl_format("%1 Axis (", 
		num_axis );

	for (int axis_count = 0; axis_count < num_axis; axis_count++)
	{
		// Only display the maximum of 4 axis
		if (axis_count == 4)
		{
			text = text + ", ...";
			break;
		}

		text = text + cl_format("%1 %2", 
			axis_count ? ", " : "",
			joystick.get_axis(axis_count) );
	}

	int num_buttons = joystick.get_button_count();
	text = text + cl_format(") : %1 Buttons ", num_buttons);

	for (int button_count = 0; button_count < num_buttons; button_count++)
	{
		text = text + cl_format("%1", joystick.get_keycode(button_count) ? "X" : "-" );
	}

	font.draw_text(gc, 8, yoffset, text);
}

void App::draw_tablet_state(CL_GraphicContext &gc, int tablet_number, int yoffset)
{
	CL_InputDevice &tablet = window.get_ic().get_tablet(tablet_number);

	CL_String text(cl_format("Tablet (%1) : %2 : ",
		tablet_number,
		tablet.get_name() ));

	int num_axis = tablet.get_axis_count();
	text = text + cl_format("%1 Axis (", 
		num_axis );

	for (int axis_count = 0; axis_count < num_axis; axis_count++)
	{
		// Only display the maximum of 4 axis
		if (axis_count == 4)
		{
			text = text + ", ...";
			break;
		}

		text = text + cl_format("%1 %2", 
			axis_count ? ", " : "",
			tablet.get_axis(axis_count) );
	}

	int num_buttons = tablet.get_button_count();
	text = text + cl_format(") : %1 Buttons ", num_buttons);

	for (int button_count = 0; button_count < num_buttons; button_count++)
	{
		text = text + cl_format("%1", tablet.get_keycode(button_count) ? "X" : "-" );
	}

	font.draw_text(gc, 8, yoffset, text);
}
