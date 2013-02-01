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

#include "colorwheel.h"

ColorWheel::ColorWheel(clan::Canvas &canvas, clan::GUIManager &gui, clan::Rect gui_position) : clan::GUIComponent(&gui, clan::GUITopLevelDescription("ColorWheel", gui_position, false))
{
	saturation_outer = 1.0f;
	saturation_inner = 0.0f;
	value_outer = 1.0f;
	value_inner = 0.0f;
	is_hsl = false;

	slider_saturation_outer = create_slider(16, 16);
	slider_saturation_inner = create_slider(16, 48);
	slider_value_outer = create_slider(16, 80);
	slider_value_inner = create_slider(16, 112);

	slider_saturation_outer->set_position(slider_saturation_outer->get_max());
	slider_saturation_inner->set_position(0);
	slider_value_outer->set_position(slider_value_outer->get_max());
	slider_value_inner->set_position(0);

	radiobutton_HSV = new clan::RadioButton(this);
	radiobutton_HSV->set_geometry(clan::Rect(16, 0 , clan::Size(64, 16)));
	radiobutton_HSV->set_text("         ");		// ** I'm got lazy to set the text color to white, so I draw it elsewhere
	radiobutton_HSV->set_selected(true);
	radiobutton_HSV->set_group_name("Main Group");
	radiobutton_HSV->func_selected().set(this, &ColorWheel::on_selected, radiobutton_HSV);

	radiobutton_HSL = new clan::RadioButton(this);
	radiobutton_HSL->set_geometry(clan::Rect(100, 0, clan::Size(64, 16)));
	radiobutton_HSL->set_text("         ");
	radiobutton_HSL->set_group_name("Main Group");
	radiobutton_HSL->func_selected().set(this, &ColorWheel::on_selected, radiobutton_HSL);

	radiobutton_HSV->set_selected(true);

	func_render().set(this, &ColorWheel::on_render);

	font = clan::Font(canvas, "tahoma", 16);

}

ColorWheel::~ColorWheel()
{

}

void ColorWheel::on_render(clan::Canvas &canvas, const clan::Rect &update_rect)
{
	clan::Rect rect = get_geometry();
	canvas.clear(clan::Colorf(0.0f, 0.0f, 0.0f, 0.0f));

	get_options();

	clan::Pointf center( (float) canvas.get_width()/2.0f, (float) canvas.get_height()/2.0f);
	float radius = 200.0f;
	create_colorwheel(center, radius);

	canvas.fill_triangles(colorwheel_positions, colorwheel_colors, colorwheel_segments * 3);

	draw_labels(canvas);
}

void ColorWheel::get_options()
{
	saturation_outer = get_value(slider_saturation_outer);
	saturation_inner = get_value(slider_saturation_inner);
	value_outer = get_value(slider_value_outer);
	value_inner = get_value(slider_value_inner);
}

float ColorWheel::get_value(clan::Slider *slider)
{
	float value = (float) slider->get_position();
	value /= (float) slider->get_max();
	return value;
}

void ColorWheel::draw_labels(clan::Canvas &canvas)
{
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

	font.draw_text(canvas, radiobutton_HSV->get_geometry().left + 16, radiobutton_HSV->get_geometry().bottom-2, "HSV");
	font.draw_text(canvas, radiobutton_HSL->get_geometry().left + 16, radiobutton_HSL->get_geometry().bottom-2, "HSL");
}

void ColorWheel::create_colorwheel(const clan::Pointf &center, float radius)
{
	for (int segment = 0; segment < colorwheel_segments; segment++)
	{
		int source_segment = segment;
		int next_segment = segment + 1;

		float src_angle = (source_segment * clan::PI*2.0f) / colorwheel_segments;
		float dest_angle = (next_segment * clan::PI*2.0f) / colorwheel_segments;
		src_angle -= clan::PI/2.0f;	// So red is at the top
		dest_angle -= clan::PI/2.0f;
		float src_x = cos( src_angle );
		float src_y = sin( src_angle );
		float dest_x = cos( dest_angle );
		float dest_y = sin( dest_angle );

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
			clan::ColorHSLf color_src_hsv( source_segment * 360.0f / colorwheel_segments, saturation_outer, value_outer, 1.0f );
			work_color_src = clan::Colorf(color_src_hsv);

			clan::ColorHSLf color_dest_hsv( next_segment * 360.0f / colorwheel_segments, saturation_outer, value_outer, 1.0f );
			work_color_dest = clan::Colorf(color_dest_hsv);

			clan::ColorHSLf color_center_hsv( ( ( source_segment + next_segment) /2.0f ) * 360.0f / colorwheel_segments, saturation_inner, value_inner, 1.0f );
			work_color_center = clan::Colorf(color_center_hsv);
		}
		else
		{
			clan::ColorHSVf color_src_hsv( source_segment * 360.0f / colorwheel_segments, saturation_outer, value_outer, 1.0f );
			work_color_src = clan::Colorf(color_src_hsv);

			clan::ColorHSVf color_dest_hsv( next_segment * 360.0f / colorwheel_segments, saturation_outer, value_outer, 1.0f );
			work_color_dest = clan::Colorf(color_dest_hsv);

			clan::ColorHSVf color_center_hsv( ( ( source_segment + next_segment) /2.0f ) * 360.0f / colorwheel_segments, saturation_inner, value_inner, 1.0f );
			work_color_center = clan::Colorf(color_center_hsv);
		}

		colorwheel_colors[triangle_offset + 0] = work_color_center;
		colorwheel_colors[triangle_offset + 1] = work_color_src;
		colorwheel_colors[triangle_offset + 2] = work_color_dest;

	}
}

clan::Slider *ColorWheel::create_slider(int xpos, int ypos)
{
	clan::Slider *component = new clan::Slider(this);
	component->set_geometry(clan::Rect(xpos, ypos, clan::Size(256, 17)));
	component->set_vertical(false);
	component->set_horizontal(true);
	component->set_min(0);
	component->set_max(1000);
	component->set_tick_count(100);
	component->set_page_step(100);
	component->set_position(0);
	component->set_lock_to_ticks(false);

	component->func_value_changed().set(this, &ColorWheel::option_changed);

	return component;

}

void ColorWheel::option_changed()
{
	request_repaint();
}

void ColorWheel::on_selected(clan::RadioButton *radiobutton)
{
	if (radiobutton == radiobutton_HSL)
	{
		is_hsl = true;
	}
	else
	{
		is_hsl = false;
	}
	request_repaint();

}
