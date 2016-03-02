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
#include "colorwheel.h"

ColorWheel::ColorWheel()
{
	add_child(overlay);

	style()->set("position: absolute; left: 0; top: 0; width:400px; height:400px;");

	overlay->saturation_outer->slider->func_value_changed() = bind_member(this, &ColorWheel::option_changed);
	overlay->saturation_inner->slider->func_value_changed() = bind_member(this, &ColorWheel::option_changed);
	overlay->value_outer->slider->func_value_changed() = bind_member(this, &ColorWheel::option_changed);
	overlay->value_inner->slider->func_value_changed() = bind_member(this, &ColorWheel::option_changed);
	overlay->radio_row->radiobutton_HSV->func_selected() = bind_member(this, &ColorWheel::option_changed);
	overlay->radio_row->radiobutton_HSL->func_selected() = bind_member(this, &ColorWheel::option_changed);

	overlay->saturation_outer->slider->set_position(overlay->saturation_outer->slider->max_position());
	overlay->saturation_inner->slider->set_position(0);
	overlay->value_outer->slider->set_position(overlay->value_outer->slider->max_position());
	overlay->value_inner->slider->set_position(0);

	overlay->radio_row->radiobutton_HSV->set_selected(true);
	overlay->radio_row->radiobutton_HSV->set_selected(true);

	update_labels();
}

void ColorWheel::render_content(clan::Canvas &canvas)
{
	get_options();

	clan::Pointf center = geometry().content_box().get_center();
	float radius = std::min(geometry().content_box().get_width(), geometry().content_box().get_height()) * 0.5f;
	draw(canvas, center, radius);
}

void ColorWheel::get_options()
{
	is_hsl = overlay->radio_row->radiobutton_HSL->selected();
	saturation_outer = get_value(overlay->saturation_outer->slider);
	saturation_inner = get_value(overlay->saturation_inner->slider);
	value_outer = get_value(overlay->value_outer->slider);
	value_inner = get_value(overlay->value_inner->slider);
}

float ColorWheel::get_value(const std::shared_ptr<clan::SliderView> &slider)
{
	float value = (float) slider->position();
	value /= (float) slider->max_position();
	return value;
}

void ColorWheel::option_changed()
{
	set_needs_render();
	update_labels();
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

void ColorWheel::update_labels()
{
	get_options();

	overlay->saturation_outer->label->set_text(clan::string_format("Saturation Outer = %1", saturation_outer));
	overlay->saturation_inner->label->set_text(clan::string_format("Saturation Inner = %1", saturation_inner));

	if (is_hsl)
	{
		overlay->value_outer->label->set_text(clan::string_format("Lightness Outer = %1", value_outer));
	}
	else
	{
		overlay->value_outer->label->set_text(clan::string_format("Value Outer = %1", value_outer));
	}

	if (is_hsl)
	{
		overlay->value_inner->label->set_text(clan::string_format("Lightness Inner = %1", value_inner));
	}
	else
	{
		overlay->value_inner->label->set_text(clan::string_format("Value Inner = %1", value_inner));
	}
}
