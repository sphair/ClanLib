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
#include "gui.h"
#include "panel.h"
#include "window_1.h"
#include "slider.h"

App::App() : quit(false)
{
}

// The start of the Application
int App::start(const std::vector<std::string> &args)
{
	// Setup the window
	clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("3D GUI Example");
	win_desc.set_size(clan::Size( 700, 700 ), false);
	window = clan::DisplayWindow(win_desc);

	// Connect the slots that we require
	clan::Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
	clan::Slot slot_input_down = (window.get_ic().get_keyboard()).sig_key_down().connect(this, &App::on_input_down);

	canvas = clan::Canvas(window);

	clan::Font font = clan::Font(canvas, "tahoma", 16);

	// Initialise the GUI system
	GUI gui(this);

	// NOTE: The GUI component positions are still in 2D world, therefore
	// be careful not to overlap windows, else unpredicted results may occur!

	window1 = new Window1(gui, clan::Rect(0,0, clan::Size(256, 256)));

	slider_1_xrotation = new Slider(gui, clan::Rect(0, 512, clan::Size(200, 17)));
	slider_1_xrotation->object_matrix.translate_self(0.0f, 0.8f, 3.0f);
	slider_1_xrotation->object_matrix = slider_1_xrotation->object_matrix * clan::Mat4f::rotate(clan::Angle(10, clan::angle_degrees), 0.0f, 0.0f, 1.0f);

	slider_1_yrotation = new Slider(gui, clan::Rect(256*1, 512, clan::Size(200, 17)));
	slider_1_yrotation->object_matrix.translate_self(0.0f, 0.7f, 3.0f);
	slider_1_yrotation->object_matrix = slider_1_yrotation->object_matrix * clan::Mat4f::rotate(clan::Angle(10, clan::angle_degrees), 0.0f, 0.0f, 1.0f);

	slider_1_zrotation = new Slider(gui, clan::Rect(256*2, 512, clan::Size(200, 17)));
	slider_1_zrotation->object_matrix.translate_self(0.0f, 0.6f, 3.0f);
	slider_1_zrotation->object_matrix = slider_1_zrotation->object_matrix * clan::Mat4f::rotate(clan::Angle(10, clan::angle_degrees), 0.0f, 0.0f, 1.0f);

	slider_1_xtranslation = new Slider(gui, clan::Rect(256*3, 512, clan::Size(200, 17)));
	slider_1_xtranslation->object_matrix.translate_self(0.0f, 0.5f, 3.0f);
	slider_1_xtranslation->object_matrix = slider_1_xtranslation->object_matrix * clan::Mat4f::rotate(clan::Angle(10, clan::angle_degrees), 0.0f, 0.0f, 1.0f);
	slider_1_xtranslation->component->set_position(500);

	slider_1_ytranslation = new Slider(gui, clan::Rect(256*4, 512, clan::Size(200, 17)));
	slider_1_ytranslation->object_matrix.translate_self(0.0f, 0.4f, 3.0f);
	slider_1_ytranslation->object_matrix = slider_1_ytranslation->object_matrix * clan::Mat4f::rotate(clan::Angle(10, clan::angle_degrees), 0.0f, 0.0f, 1.0f);
	slider_1_ytranslation->component->set_position(500);

	slider_1_ztranslation = new Slider(gui, clan::Rect(256*5, 512, clan::Size(200, 17)));
	slider_1_ztranslation->object_matrix.translate_self(0.0f, 0.3f, 3.0f);
	slider_1_ztranslation->object_matrix = slider_1_ztranslation->object_matrix * clan::Mat4f::rotate(clan::Angle(10, clan::angle_degrees), 0.0f, 0.0f, 1.0f);
	slider_1_ztranslation->component->set_position(500);

	while(!quit)
	{
		calculate_matrix();

		canvas.set_modelview(clan::Mat4f::identity());
		canvas.set_map_mode(clan::map_2d_upper_left);

		// Draw the gradient
		canvas.fill_rect(clan::Rect(0, 0, canvas.get_width(), canvas.get_height()/2), clan::Gradient(clan::Colorf(0.2f, 0.2f, 0.8f, 1.0f), clan::Colorf(0.0f, 0.0f, 0.2f, 1.0f)));
		canvas.fill_rect(clan::Rect(0, canvas.get_height()/2, canvas.get_width(), canvas.get_height()), clan::Gradient(clan::Colorf(0.0f, 0.0f, 0.2f, 1.0f), clan::Colorf(0.2f, 0.2f, 0.8f, 1.0f)));

		font.draw_text(canvas, 8, 20, "GUI3D");

		int xoffset = 160;
		int yoffset = 70;
		const int ygap = 35;
		font.draw_text(canvas, xoffset, yoffset, "X Rotation");
		yoffset += ygap;
		font.draw_text(canvas, xoffset, yoffset, "Y Rotation");
		yoffset += ygap;
		font.draw_text(canvas, xoffset, yoffset, "Z Rotation");
		yoffset += ygap;
		font.draw_text(canvas, xoffset, yoffset, "X Translation");
		yoffset += ygap;
		font.draw_text(canvas, xoffset, yoffset, "Y Translation");
		yoffset += ygap;
		font.draw_text(canvas, xoffset, yoffset, "Z Translation");
		yoffset += ygap;

		if (!gui.run())
			break;

		canvas.set_map_mode(clan::map_user_projection);
		canvas.set_projection(projection_matrix);
		canvas.set_modelview(modelview_matrix);

		control_window();

		gui.draw();

		canvas.flip(1);

		clan::KeepAlive::process();
	}

	return 0;
}

void App::on_input_down(const clan::InputEvent &key)
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

void App::calculate_matrix()
{
	const float lens_zoom = 3.2f;
	const float lens_near = 0.1f;
	const float lens_far = 10000.0f;
	const float lens_aspect = 1.0f;

	float fov = 2.0f * atan2(1.0f, lens_zoom);
	float aspect = 1.0f;
	float width = (float) canvas.get_width();
	float height = (float) canvas.get_height();

	if (height)
		aspect = ( width * lens_aspect) / height;

	fov = (fov * 180.0f) / clan::PI;
	projection_matrix = clan::Mat4f::perspective( fov, aspect, lens_near, lens_far, clan::handed_left, clan::clip_negative_positive_w);
	modelview_matrix = clan::Mat4f::identity();
	
}

void App::control_window()
{
	window1->object_matrix = clan::Mat4f::identity();

	float x_trans = ( (float) slider_1_xtranslation->component->get_position() * 1.0f) / (float) slider_1_xrotation->component->get_max();
	float y_trans = ( (float) slider_1_ytranslation->component->get_position() * 1.0f) / (float) slider_1_xrotation->component->get_max();
	float z_trans = ( (float) slider_1_ztranslation->component->get_position() * 6.0f) / (float) slider_1_xrotation->component->get_max();

	window1->object_matrix.translate_self(x_trans - 0.5f, y_trans - 1.0f, z_trans);

	float value = ( (float) slider_1_xrotation->component->get_position() * 360.0f) / (float) slider_1_xrotation->component->get_max();
	window1->object_matrix = window1->object_matrix * clan::Mat4f::rotate(clan::Angle( value, clan::angle_degrees), 1.0f, 0.0f, 0.0f);

	value = ( (float) slider_1_yrotation->component->get_position() * 360.0f) / (float) slider_1_xrotation->component->get_max();
	window1->object_matrix = window1->object_matrix * clan::Mat4f::rotate(clan::Angle( value, clan::angle_degrees), 0.0f, 1.0f, 0.0f);

	value = ( (float) slider_1_zrotation->component->get_position() * 360.0f) / (float) slider_1_xrotation->component->get_max();
	window1->object_matrix = window1->object_matrix * clan::Mat4f::rotate(clan::Angle( value, clan::angle_degrees), 0.0f, 0.0f, 1.0f);
}
