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

#include "options.h"

Options::Options(clan::Canvas &canvas) : clan::TextureWindow(canvas)
{
	// Note, when changing these, remember to change the popup menu defaults
	num_particles = 64;
	point_size = 64;

	int slider_xpos = 450;
	int slider_label_xpos = slider_xpos + 200;
	int slider_ypos = 100;
	int slider_gap = 24;
	slider_num_particles = create_slider(slider_xpos, slider_ypos);
	slider_num_particles->set_max_position(1000);
	slider_num_particles->set_position(num_particles);
	slider_num_particles->func_value_changed() = bind_member(this, &Options::slider_num_particles_changed);
	label_num_particles = create_slider_label(slider_label_xpos, slider_ypos);
	slider_ypos += slider_gap;

	slider_point_size = create_slider(slider_xpos, slider_ypos);
	slider_point_size->set_min_position(1);
	slider_point_size->set_max_position(256);
	slider_point_size->set_position(point_size);
	slider_point_size->func_value_changed() = bind_member(this, &Options::slider_point_size_changed);
	label_point_size = create_slider_label(slider_label_xpos, slider_ypos);
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

std::shared_ptr<clan::SliderView> Options::create_slider(int xpos, int ypos)
{
	std::shared_ptr<clan::SliderView> component = Theme::create_slider();
	add_subview(component);

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
	add_subview(component);
	component->style()->set("position: absolute; left:%1px; top:%2px", xpos, ypos);
	component->set_text("##################");
	return component;
}


void Options::slider_num_particles_changed()
{
	num_particles = slider_num_particles->position();
	std::string text(clan::string_format("Number of Particles : %1", num_particles));
	label_num_particles->set_text(text);
}

void Options::slider_point_size_changed()
{
	point_size = slider_point_size->position();
	std::string text(clan::string_format("set_point_size: %1", point_size));
	label_point_size->set_text(text);
}

void Options::update_all_slider_text()
{
	slider_num_particles_changed();
	slider_point_size_changed();
}
