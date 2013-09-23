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

#include "options.h"

Options::Options(clan::GUIManager &gui, clan::Rect gui_position) : clan::GUIComponent(&gui, clan::GUITopLevelDescription("Options", gui_position, false))
{
	width = 256;

	int base_xpos = 20;
	int base_ypos = 20;

	int slider_gap = 20;

	slider_width = create_slider(base_xpos, base_ypos); base_ypos += slider_gap;
	slider_width->set_min(32);
	slider_width->set_max(382);
	slider_width->set_position(width);
	slider_width->func_value_changed().set(this, &Options::slider_width_changed);
	label_width = create_slider_label(slider_width);

	update_all_slider_text();

	func_render().set(this, &Options::on_render);
}

Options::~Options()
{

}

void Options::on_render(clan::Canvas &canvas, const clan::Rect &update_rect)
{
	clan::Rect rect = get_geometry();
	canvas.fill_rect(rect, clan::Colorf(0.6f, 0.6f, 0.2f, 1.0f));
}

float Options::get_value(clan::Slider *slider, float max_value)
{
	float value = (float) slider->get_position();
	value /= (float) slider->get_max();
	return value * max_value;
}

void Options::set_value(clan::Slider *slider, float value, float max_value)
{
	value /= max_value;
	value *= (float) slider->get_max();
	slider->set_position((int) (value+0.5f));
}

clan::Slider *Options::create_slider(int xpos, int ypos)
{
	clan::Slider *component = new clan::Slider(this);
	component->set_geometry(clan::Rect(xpos, ypos, clan::Size(192, 17)));
	component->set_vertical(false);
	component->set_horizontal(true);
	component->set_min(0);
	component->set_max(1000);
	component->set_tick_count(100);
	component->set_page_step(100);
	component->set_lock_to_ticks(false);
	component->set_position(component->get_max());

	return component;

}

clan::Label *Options::create_slider_label(clan::Slider *slider)
{
	clan::Label *component = new clan::Label(this);
	clan::Rect slider_geometry = slider->get_geometry();
	component->set_geometry(clan::Rect(slider_geometry.right + 4, slider_geometry.top - 2, clan::Size(256, 17)));
	component->set_text("##################");
	return component;
}

void Options::slider_width_changed()
{
	width = slider_width->get_position();
	std::string text(clan::string_format("Width : %1", width));
	label_width->set_text(text);
}

void Options::update_all_slider_text()
{
	slider_width_changed();
}

