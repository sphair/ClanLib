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
	// Note, when changing these, remember to change the popup menu defaults
	num_particles = 64;
	point_size = 64;

	int slider_xpos = 450;
	int slider_ypos = 100;
	int slider_gap = 24;
	slider_num_particles = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_ypos += 8;
	slider_num_particles->set_max(1000);
	slider_num_particles->set_position(num_particles);
	slider_num_particles->func_value_changed().set(this, &Options::slider_num_particles_changed);
	slider_ypos += 8;
	label_num_particles = create_slider_label(slider_num_particles);

	slider_point_size = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_ypos += 8;
	slider_point_size->set_max(256);
	slider_point_size->set_position(point_size);
	slider_point_size->func_value_changed().set(this, &Options::slider_point_size_changed);
	slider_ypos += 8;
	label_point_size = create_slider_label(slider_point_size);

	update_all_slider_text();

	func_render().set(this, &Options::on_render);
}

Options::~Options()
{

}

void Options::on_render(clan::Canvas &canvas, const clan::Rect &update_rect)
{
	clan::Rect rect = get_geometry();
	canvas.fill_rect(update_rect, clan::Colorf(0.6f, 0.6f, 0.2f, 1.0f));
}

float Options::get_value(clan::Slider *slider)
{
	float value = (float) slider->get_position();
	value /= (float) slider->get_max();
	return value;
}

clan::Slider *Options::create_slider(int xpos, int ypos)
{
	clan::Slider *component = new clan::Slider(this);
	component->set_geometry(clan::Rect(xpos, ypos, clan::Size(128, 17)));
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

void Options::slider_num_particles_changed()
{
	num_particles = slider_num_particles->get_position();
	std::string text(clan::string_format("Number of Particles : %1", num_particles));
	label_num_particles->set_text(text);
}

void Options::slider_point_size_changed()
{
	point_size = slider_point_size->get_position();
	std::string text(clan::string_format("set_point_size: %1", point_size));
	label_point_size->set_text(text);
}

void Options::update_all_slider_text()
{
	slider_num_particles_changed();
	slider_point_size_changed();
}

