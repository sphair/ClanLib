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

#include "colorwheel.h"

ColorWheel::ColorWheel()
{
	style()->set("position: absolute; left: 0; top: 0; width:200px; height:200px;");
	//style()->set("flex: 1 1 main-size");

	add_subview(slider_saturation_outer);
	add_subview(slider_saturation_inner);
	add_subview(slider_value_outer);
	add_subview(slider_value_inner);
	add_subview(radiobutton_HSV);
	add_subview(radiobutton_HSL);

	set_slider(slider_saturation_outer.get(), 16, 16);
	set_slider(slider_saturation_inner.get(), 16, 48);
	set_slider(slider_value_outer.get(), 16, 80);
	set_slider(slider_value_inner.get(), 16, 112);

	slider_saturation_outer->set_position(slider_saturation_outer->max_position());
	slider_saturation_inner->set_position(0);
	slider_value_outer->set_position(slider_value_outer->max_position());
	slider_value_inner->set_position(0);

	radiobutton_HSV->style()->set("position: absolute; left: 16px; top: 0px; width:64px; height:16px");
	radiobutton_HSV->set_selected(true);
	radiobutton_HSV->func_selected() = [&]() {is_hsl = false; set_needs_render(); };
	radiobutton_HSL->style()->set("position: absolute; left: 100px; top: 0px; width:64px; height:16px");
	radiobutton_HSL->func_selected() = [&]() {is_hsl = true; set_needs_render(); };

	radiobutton_HSV->set_selected(true);

	font = clan::Font( "tahoma", 16);

}

ColorWheel::~ColorWheel()
{

}

void ColorWheel::render_content(clan::Canvas &canvas)
{
	get_options();

	clan::Pointf center = geometry().content_box().get_center();
	float radius = std::min(geometry().content_box().get_width(), geometry().content_box().get_height()) * 0.5f;
	draw(canvas, center, radius);

	draw_labels(canvas);

}

void ColorWheel::get_options()
{
	saturation_outer = get_value(slider_saturation_outer.get());
	saturation_inner = get_value(slider_saturation_inner.get());
	value_outer = get_value(slider_value_outer.get());
	value_inner = get_value(slider_value_inner.get());
}

float ColorWheel::get_value(clan::SliderView *slider)
{
	float value = (float) slider->position();
	value /= (float) slider->max_position();
	return value;
}

void ColorWheel::set_slider(clan::SliderView *component, int xpos, int ypos)
{
	component->style()->set("position: absolute; left: %1px; top: %2px; width:256px; height:17px", xpos, ypos);

	component->set_horizontal();
	component->set_min_position(0);
	component->set_max_position(1000);
	component->set_tick_count(100);
	component->set_page_step(100);
	component->set_position(0);
	component->set_lock_to_ticks(false);

	component->func_value_changed() = bind_member(this, &ColorWheel::option_changed);
}

void ColorWheel::option_changed()
{
	set_needs_render();
}


void ColorWheel::draw(clan::Canvas &canvas, const clan::Pointf &center, float radius)
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

void ColorWheel::draw_labels(clan::Canvas &canvas)
{

	std::string text;
	text = std::string(clan::string_format("Saturation Outer = %1", saturation_outer));
	font.draw_text(canvas, slider_saturation_outer->geometry().content_box().right + 8, slider_saturation_outer->geometry().content_box().bottom - 4, text);
	text = std::string(clan::string_format("Saturation Inner = %1", saturation_inner));
	font.draw_text(canvas, slider_saturation_inner->geometry().content_box().right + 8, slider_saturation_inner->geometry().content_box().bottom - 4, text);

	if (is_hsl)
	{
	text = std::string(clan::string_format("Lightness Outer = %1", value_outer));
	}
	else
	{
	text = std::string(clan::string_format("Value Outer = %1", value_outer));
	}
	font.draw_text(canvas, slider_value_outer->geometry().content_box().right + 8, slider_value_outer->geometry().content_box().bottom - 4, text);

	if (is_hsl)
	{
	text = std::string(clan::string_format("Lightness Inner = %1", value_inner));
	}
	else
	{
	text = std::string(clan::string_format("Value Inner = %1", value_inner));
	}
	font.draw_text(canvas, slider_value_inner->geometry().content_box().right + 8, slider_value_inner->geometry().content_box().bottom - 4, text);

	font.draw_text(canvas, radiobutton_HSV->geometry().content_box().left + 16, radiobutton_HSV->geometry().content_box().bottom - 2, "HSV");
	font.draw_text(canvas, radiobutton_HSL->geometry().content_box().left + 16, radiobutton_HSL->geometry().content_box().bottom - 2, "HSL");
}


