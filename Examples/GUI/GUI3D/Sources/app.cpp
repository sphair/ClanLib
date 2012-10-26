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
	DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("3D GUI Example");
	win_desc.set_size(Size( 700, 700 ), false);
	window = DisplayWindow(win_desc);

	// Connect the slots that we require
	Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
	Slot slot_input_down = (window.get_ic().get_keyboard()).sig_key_down().connect(this, &App::on_input_down);

	GraphicContext gc = window.get_gc();

	Font font = Font(gc, "tahoma", 16);

	// Initialise the GUI system
	GUI gui(this);

	// NOTE: The GUI component positions are still in 2D world, therefore
	// be careful not to overlap windows, else unpredicted results may occur!

	window1 = new Window1(gui, Rect(0,0, Size(256, 256)));

	slider_1_xrotation = new Slider(gui, Rect(0, 512, Size(200, 17)));
	slider_1_xrotation->object_matrix.translate_self(0.0f, 0.8f, 3.0f);
	slider_1_xrotation->object_matrix = slider_1_xrotation->object_matrix * Mat4f::rotate(Angle(10, cl_degrees), 0.0f, 0.0f, 1.0f);

	slider_1_yrotation = new Slider(gui, Rect(256*1, 512, Size(200, 17)));
	slider_1_yrotation->object_matrix.translate_self(0.0f, 0.7f, 3.0f);
	slider_1_yrotation->object_matrix = slider_1_yrotation->object_matrix * Mat4f::rotate(Angle(10, cl_degrees), 0.0f, 0.0f, 1.0f);

	slider_1_zrotation = new Slider(gui, Rect(256*2, 512, Size(200, 17)));
	slider_1_zrotation->object_matrix.translate_self(0.0f, 0.6f, 3.0f);
	slider_1_zrotation->object_matrix = slider_1_zrotation->object_matrix * Mat4f::rotate(Angle(10, cl_degrees), 0.0f, 0.0f, 1.0f);

	slider_1_xtranslation = new Slider(gui, Rect(256*3, 512, Size(200, 17)));
	slider_1_xtranslation->object_matrix.translate_self(0.0f, 0.5f, 3.0f);
	slider_1_xtranslation->object_matrix = slider_1_xtranslation->object_matrix * Mat4f::rotate(Angle(10, cl_degrees), 0.0f, 0.0f, 1.0f);
	slider_1_xtranslation->component->set_position(500);

	slider_1_ytranslation = new Slider(gui, Rect(256*4, 512, Size(200, 17)));
	slider_1_ytranslation->object_matrix.translate_self(0.0f, 0.4f, 3.0f);
	slider_1_ytranslation->object_matrix = slider_1_ytranslation->object_matrix * Mat4f::rotate(Angle(10, cl_degrees), 0.0f, 0.0f, 1.0f);
	slider_1_ytranslation->component->set_position(500);

	slider_1_ztranslation = new Slider(gui, Rect(256*5, 512, Size(200, 17)));
	slider_1_ztranslation->object_matrix.translate_self(0.0f, 0.3f, 3.0f);
	slider_1_ztranslation->object_matrix = slider_1_ztranslation->object_matrix * Mat4f::rotate(Angle(10, cl_degrees), 0.0f, 0.0f, 1.0f);
	slider_1_ztranslation->component->set_position(500);

	while(!quit)
	{
		calculate_matrix();

		gc.set_modelview(Mat4f::identity());
		gc.set_map_mode(MapMode(cl_map_2d_upper_left));

		// Draw the gradient
		Draw::gradient_fill(gc, Rect(0, 0, gc.get_width(), gc.get_height()/2), Gradient(Colorf(0.2f, 0.2f, 0.8f, 1.0f), Colorf(0.0f, 0.0f, 0.2f, 1.0f)));
		Draw::gradient_fill(gc, Rect(0, gc.get_height()/2, gc.get_width(), gc.get_height()), Gradient(Colorf(0.0f, 0.0f, 0.2f, 1.0f), Colorf(0.2f, 0.2f, 0.8f, 1.0f)));

		font.draw_text(gc, 8, 20, "GUI3D");

		int xoffset = 160;
		int yoffset = 70;
		const int ygap = 35;
		font.draw_text(gc, xoffset, yoffset, "X Rotation");
		yoffset += ygap;
		font.draw_text(gc, xoffset, yoffset, "Y Rotation");
		yoffset += ygap;
		font.draw_text(gc, xoffset, yoffset, "Z Rotation");
		yoffset += ygap;
		font.draw_text(gc, xoffset, yoffset, "X Translation");
		yoffset += ygap;
		font.draw_text(gc, xoffset, yoffset, "Y Translation");
		yoffset += ygap;
		font.draw_text(gc, xoffset, yoffset, "Z Translation");
		yoffset += ygap;

		if (!gui.run())
			break;

		gc.set_map_mode(cl_user_projection);
		gc.set_projection(projection_matrix);
		gc.set_modelview(modelview_matrix);

		control_window();

		gui.draw();

		window.flip(1);

		KeepAlive::process();
	}

	return 0;
}

void App::on_input_down(const InputEvent &key)
{
	if(key.id == KEY_ESCAPE)
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
	GraphicContext gc = window.get_gc();

	const float lens_zoom = 3.2f;
	const float lens_near = 0.1f;
	const float lens_far = 10000.0f;
	const float lens_aspect = 1.0f;

	float fov = 2.0f * atan2(1.0f, lens_zoom);
	float aspect = 1.0f;
	float width = (float) gc.get_width();
	float height = (float) gc.get_height();

	if (height)
		aspect = ( width * lens_aspect) / height;

	fov = (fov * 180.0f) / PI;
	projection_matrix = Mat4f::perspective( fov, aspect, lens_near, lens_far);

	modelview_matrix = Mat4f::identity();
	modelview_matrix.matrix[2 + (4*2)] = -1.0f;
}

void App::control_window()
{
	window1->object_matrix = Mat4f::identity();

	float x_trans = ( (float) slider_1_xtranslation->component->get_position() * 1.0f) / (float) slider_1_xrotation->component->get_max();
	float y_trans = ( (float) slider_1_ytranslation->component->get_position() * 1.0f) / (float) slider_1_xrotation->component->get_max();
	float z_trans = ( (float) slider_1_ztranslation->component->get_position() * 6.0f) / (float) slider_1_xrotation->component->get_max();

	window1->object_matrix.translate_self(x_trans - 0.5f, y_trans - 1.0f, z_trans);

	float value = ( (float) slider_1_xrotation->component->get_position() * 360.0f) / (float) slider_1_xrotation->component->get_max();
	window1->object_matrix = window1->object_matrix * Mat4f::rotate(Angle( value, cl_degrees), 1.0f, 0.0f, 0.0f);

	value = ( (float) slider_1_yrotation->component->get_position() * 360.0f) / (float) slider_1_xrotation->component->get_max();
	window1->object_matrix = window1->object_matrix * Mat4f::rotate(Angle( value, cl_degrees), 0.0f, 1.0f, 0.0f);

	value = ( (float) slider_1_zrotation->component->get_position() * 360.0f) / (float) slider_1_xrotation->component->get_max();
	window1->object_matrix = window1->object_matrix * Mat4f::rotate(Angle( value, cl_degrees), 0.0f, 0.0f, 1.0f);
}
