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

#include "precomp.h"
#include "app.h"
#include "colorwheel.h"
#include <cstdlib>

clan::ApplicationInstance<App> clanapp;

App::App()
{
	// We support all display targets, in order listed here
#ifdef WIN32
	clan::D3DTarget::enable();
#endif
	clan::OpenGLTarget::enable();
	
	clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("ColorWheel Example");
	win_desc.set_size(clan::Size( 800, 600 ), false);

	window = clan::DisplayWindow(win_desc);
	sc.connect(window.sig_window_close(), clan::bind_member(this, &App::on_window_close));
	sc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_input_up));

	canvas = clan::Canvas(window);

	// Deleted automatically by the GUI
//	new ColorWheel(canvas, gui, clan::Rect(32, 32, clan::Size(512, 512)));
}

bool App::update()
{
	canvas.clear(clan::Colorf(0.0f,0.0f,0.0f));

	clan::Pointf center((float)canvas.get_width() / 2.0f, (float)canvas.get_height() / 2.0f);
	float radius = 200.0f;
	draw(canvas, center, radius);

	draw_labels(canvas);

	window.flip(1);

	return !quit;
}

void App::draw_labels(clan::Canvas &canvas)
{
	/*
	std::string text;
	text = std::string(clan::string_format("Saturation Outer = %1", saturation_outer));
	font.draw_text(canvas, slider_saturation_outer->get_geometry().right + 8, slider_saturation_outer->get_geometry().bottom - 4, text);
	text = std::string(clan::string_format("Saturation Inner = %1", saturation_inner));
	font.draw_text(canvas, slider_saturation_inner->get_geometry().right + 8, slider_saturation_inner->get_geometry().bottom - 4, text);

	if (is_hsl)
	{
		text = std::string(clan::string_format("Lightness Outer = %1", value_outer));
	}
	else
	{
		text = std::string(clan::string_format("Value Outer = %1", value_outer));
	}
	font.draw_text(canvas, slider_value_outer->get_geometry().right + 8, slider_value_outer->get_geometry().bottom - 4, text);

	if (is_hsl)
	{
		text = std::string(clan::string_format("Lightness Inner = %1", value_inner));
	}
	else
	{
		text = std::string(clan::string_format("Value Inner = %1", value_inner));
	}
	font.draw_text(canvas, slider_value_inner->get_geometry().right + 8, slider_value_inner->get_geometry().bottom - 4, text);

	font.draw_text(canvas, radiobutton_HSV->get_geometry().left + 16, radiobutton_HSV->get_geometry().bottom - 2, "HSV");
	font.draw_text(canvas, radiobutton_HSL->get_geometry().left + 16, radiobutton_HSL->get_geometry().bottom - 2, "HSL");
	*/
}

void App::draw(clan::Canvas &canvas, const clan::Pointf &center, float radius)
{
	const int colorwheel_segments = 360 / 2;
	clan::Vec2f colorwheel_positions[colorwheel_segments * 3];
	clan::Colorf colorwheel_colors[colorwheel_segments * 3];

	for (int segment = 0; segment < colorwheel_segments; segment++)
	{
		int source_segment = segment;
		int next_segment = segment + 1;

		float src_angle = (source_segment * clan::PI*2.0f) / colorwheel_segments;
		float dest_angle = (next_segment * clan::PI*2.0f) / colorwheel_segments;
		src_angle -= clan::PI / 2.0f;	// So red is at the top
		dest_angle -= clan::PI / 2.0f;
		float src_x = cos(src_angle);
		float src_y = sin(src_angle);
		float dest_x = cos(dest_angle);
		float dest_y = sin(dest_angle);

		int triangle_offset = segment * 3;
		colorwheel_positions[triangle_offset + 0].x = center.x;
		colorwheel_positions[triangle_offset + 0].y = center.y;

		colorwheel_positions[triangle_offset + 1].x = (src_x * radius) + center.x;
		colorwheel_positions[triangle_offset + 1].y = (src_y * radius) + center.y;

		colorwheel_positions[triangle_offset + 2].x = (dest_x * radius) + center.x;
		colorwheel_positions[triangle_offset + 2].y = (dest_y * radius) + center.y;

		clan::Colorf work_color_src;
		clan::Colorf work_color_dest;
		clan::Colorf work_color_center;

		if (is_hsl)
		{
			clan::ColorHSLf color_src_hsv(source_segment * 360.0f / colorwheel_segments, saturation_outer, value_outer, 1.0f);
			work_color_src = clan::Colorf(color_src_hsv);

			clan::ColorHSLf color_dest_hsv(next_segment * 360.0f / colorwheel_segments, saturation_outer, value_outer, 1.0f);
			work_color_dest = clan::Colorf(color_dest_hsv);

			clan::ColorHSLf color_center_hsv(((source_segment + next_segment) / 2.0f) * 360.0f / colorwheel_segments, saturation_inner, value_inner, 1.0f);
			work_color_center = clan::Colorf(color_center_hsv);
		}
		else
		{
			clan::ColorHSVf color_src_hsv(source_segment * 360.0f / colorwheel_segments, saturation_outer, value_outer, 1.0f);
			work_color_src = clan::Colorf(color_src_hsv);

			clan::ColorHSVf color_dest_hsv(next_segment * 360.0f / colorwheel_segments, saturation_outer, value_outer, 1.0f);
			work_color_dest = clan::Colorf(color_dest_hsv);

			clan::ColorHSVf color_center_hsv(((source_segment + next_segment) / 2.0f) * 360.0f / colorwheel_segments, saturation_inner, value_inner, 1.0f);
			work_color_center = clan::Colorf(color_center_hsv);
		}

		colorwheel_colors[triangle_offset + 0] = work_color_center;
		colorwheel_colors[triangle_offset + 1] = work_color_src;
		colorwheel_colors[triangle_offset + 2] = work_color_dest;

	}
	canvas.fill_triangles(colorwheel_positions, colorwheel_colors, colorwheel_segments * 3);

}


// A key was pressed
void App::on_input_up(const clan::InputEvent &key)
{
	switch (key.id)
	{
		case clan::keycode_escape:
			quit = true;
			break;
	}

}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

