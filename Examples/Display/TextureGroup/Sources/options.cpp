/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

Options::Options(clan::Canvas &canvas) : clan::TextureWindow(canvas)
{
	int slider_xpos = 400;
	int slider_label_xpos = slider_xpos + 200;
	int slider_ypos = 200;
	int slider_gap = 32;
	slider_outline_size = create_slider(slider_xpos, slider_ypos);
	slider_outline_size->set_max_position(32);
	slider_outline_size->set_position(outline_size);
	slider_outline_size->func_value_changed() = bind_member(this, &Options::slider_outline_size_changed);
	label_outline_size = create_slider_label(slider_label_xpos, slider_ypos);
	slider_ypos += slider_gap;

	slider_group_size = create_slider(slider_xpos, slider_ypos);
	slider_group_size->set_max_position(1024);
	slider_group_size->set_position(group_size);
	slider_group_size->func_value_changed() = bind_member(this, &Options::slider_group_size_changed);
	label_group_size = create_slider_label(slider_label_xpos, slider_ypos);
	slider_ypos += slider_gap;

	update_all_slider_text();

}

Options::~Options()
{

}
float Options::get_value(std::shared_ptr<clan::SliderView> slider)
{
	float value = (float)slider->position();
	value /= (float)slider->max_position();
	return value;
}

void Options::set_value(std::shared_ptr<clan::SliderView> slider, float value, float max_value)
{
	value /= max_value;
	value *= (float)slider->max_position();
	slider->set_position(value);
}

std::shared_ptr<clan::SliderView> Options::create_slider(int xpos, int ypos)
{
	std::shared_ptr<clan::SliderView> component = Theme::create_slider();
	add_child(component);

	component->style()->set("position: absolute; left:%1px; top:%2px; width:%3px; height:auto;", xpos, ypos, 192);
	component->set_horizontal();
	component->set_min_position(0);
	component->set_max_position(1000);
	component->set_tick_count(100);
	component->set_page_step(100);
	component->set_lock_to_ticks(false);
	component->set_position(component->max_position());

	return component;

}

std::shared_ptr<clan::LabelView> Options::create_slider_label(int xpos, int ypos)
{
	std::shared_ptr<clan::LabelView> component = Theme::create_label(true);
	add_child(component);
	component->style()->set("position: absolute; left:%1px; top:%2px", xpos, ypos);
	component->set_text("##################");
	return component;
}

void Options::slider_outline_size_changed()
{
	outline_size = slider_outline_size->position();
	std::string text(clan::string_format("Outline Size : %1", outline_size));
	label_outline_size->set_text(text);
	options_changed = true;
}

void Options::slider_group_size_changed()
{
	group_size = slider_group_size->position();
	std::string text(clan::string_format("Group Page Size : %1", group_size));
	label_group_size->set_text(text);
	options_changed = true;
}

void Options::update_all_slider_text()
{
	slider_outline_size_changed();
	slider_group_size_changed();
}
