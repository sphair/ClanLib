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

// NOTE: These limits are only to make the example easier to use
#define MAX_AMPLITUDE	2.0f	
#define MAX_POSITION	255.0f
#define MAX_LENGTH		64.0f

Options::Options(clan::Canvas &canvas) : clan::TextureWindow(canvas)
{
	is_normals_set = false;
	sized_format = clan::tf_rgb8;
	dimension = perlin_2d;
	amplitude = 1.0;
	width = 256;
	height = 256;
	octaves = 1;

	start_x = 0.0f;
	length_x = 32.0f;
	start_y = 0.0f;
	length_y = 32.0f;
	position_z = 0.0f;
	position_w = 0.0f;

	std::shared_ptr<clan::ListBoxView> listbox;
	listbox = create_listbox(450, 30, "Pixel Format");
	listbox->set_items<std::string>( { "tf_rgb8", "tf_rgba8", "tf_r8", "tf_r32f" }, Theme::create_listbox_label);
	listbox->func_selection_changed() = [=](){on_format_selected(listbox); };
	listbox->set_selected_item(0);

	checkbox_normals = create_checkbox(670, 35, "Draw Normals (rgb only)", is_normals_set);
	checkbox_normals->func_state_changed() = bind_member(this, &Options::checkbox_normals_changed);

	listbox = create_listbox(450, 180, "Dimension");
	listbox->set_items<std::string>({ "1D", "2D", "3D", "4D" }, Theme::create_listbox_label);
	listbox->func_selection_changed() = [=](){on_dimension_selected(listbox); };
	listbox->set_selected_item(1);

	int slider_xpos = 450;
	int slider_label_xpos = slider_xpos + 200;
	int slider_ypos = 320;
	int slider_gap = 36;

	slider_width = create_slider(slider_xpos, slider_ypos);
	slider_width->set_min_position(32);
	slider_width->set_max_position(382);
	slider_width->set_position(width);
	slider_width->func_value_changed() = bind_member(this, &Options::slider_width_changed);
	label_width = create_slider_label(slider_label_xpos, slider_ypos);
	slider_ypos += slider_gap;

	slider_height = create_slider(slider_xpos, slider_ypos);
	slider_height->set_min_position(32);
	slider_height->set_max_position(382);
	slider_height->set_position(height);
	slider_height->func_value_changed() = bind_member(this, &Options::slider_height_changed);
	label_height = create_slider_label(slider_label_xpos, slider_ypos);
	slider_ypos += slider_gap;

	slider_octaves = create_slider(slider_xpos, slider_ypos);
	slider_octaves->set_min_position(1);
	slider_octaves->set_max_position(9);
	slider_octaves->set_page_step(1);
	slider_octaves->set_position(octaves);
	slider_octaves->func_value_changed() = bind_member(this, &Options::slider_octaves_changed);
	label_octaves = create_slider_label(slider_xpos + 200, slider_ypos);
	slider_ypos += slider_gap;

	slider_amplitude = create_slider(slider_xpos, slider_ypos);
	set_value(slider_amplitude, amplitude, MAX_AMPLITUDE);
	slider_amplitude->func_value_changed() = bind_member(this, &Options::slider_amplitude_changed);
	label_amplitude = create_slider_label(slider_xpos + 200, slider_ypos);
	slider_ypos += slider_gap;

	slider_start_x = create_slider(slider_xpos, slider_ypos);
	set_value(slider_start_x, start_x, MAX_POSITION);
	slider_start_x->func_value_changed() = bind_member(this, &Options::slider_start_x_changed);
	label_start_x = create_slider_label(slider_xpos + 200, slider_ypos);
	slider_ypos += slider_gap;

	slider_length_x = create_slider(slider_xpos, slider_ypos);
	set_value(slider_length_x, length_x, MAX_LENGTH);
	slider_length_x->func_value_changed() = bind_member(this, &Options::slider_length_x_changed);
	label_length_x = create_slider_label(slider_xpos + 200, slider_ypos);
	slider_ypos += slider_gap;

	slider_start_y = create_slider(slider_xpos, slider_ypos);
	set_value(slider_start_y, start_y, MAX_POSITION);
	slider_start_y->func_value_changed() = bind_member(this, &Options::slider_start_y_changed);
	label_start_y = create_slider_label(slider_xpos + 200, slider_ypos);
	slider_ypos += slider_gap;

	slider_length_y = create_slider(slider_xpos, slider_ypos);
	set_value(slider_length_y, length_y, MAX_LENGTH);
	slider_length_y->func_value_changed() = bind_member(this, &Options::slider_length_y_changed);
	label_length_y = create_slider_label(slider_xpos + 200, slider_ypos);
	slider_ypos += slider_gap;

	slider_position_z = create_slider(slider_xpos, slider_ypos);
	set_value(slider_position_z, position_z, MAX_POSITION);
	slider_position_z->func_value_changed() = bind_member(this, &Options::slider_position_z_changed);
	label_position_z = create_slider_label(slider_xpos + 200, slider_ypos);
	slider_ypos += slider_gap;

	slider_position_w = create_slider(slider_xpos, slider_ypos);
	set_value(slider_position_w, position_w, MAX_POSITION);
	slider_position_w->func_value_changed() = bind_member(this, &Options::slider_position_w_changed);
	label_position_w = create_slider_label(slider_xpos + 200, slider_ypos);
	slider_ypos += slider_gap;

	update_all_slider_text();
}

Options::~Options()
{

}

std::shared_ptr<clan::CheckBoxView> Options::create_checkbox(int xpos, int ypos, const std::string &name, bool state)
{
	std::shared_ptr<clan::CheckBoxView> checkbox = Theme::create_checkbox();
	add_child(checkbox);
	checkbox->style()->set("position: absolute; left:%1px; top:%2px", xpos, ypos);
	checkbox->set_check(state);

	auto label = Theme::create_label(true);
	label->set_text(name);
	label->style()->set("position: absolute; left:%1px; top:%2px", xpos + 16, ypos - 3);
	add_child(label);

	return checkbox;
}

void Options::checkbox_normals_changed()
{
	is_normals_set = checkbox_normals->checked();
}

float Options::get_value(std::shared_ptr<clan::SliderView> slider, float max_value)
{
	float value = (float) slider->position();
	value /= (float) slider->max_position();
	return value * max_value;
}

void Options::set_value(std::shared_ptr<clan::SliderView> slider, float value, float max_value)
{
	value /= max_value;
	value *= (float) slider->max_position();
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


std::shared_ptr<clan::ListBoxView> Options::create_listbox(int xpos, int ypos, const std::string &title)
{
	auto listbox = Theme::create_listbox();
	add_child(listbox);
	listbox->style()->set("position: absolute; left:%1px; top:%2px; width:%3px; height:%4px;", xpos, ypos, 200, 100);
	auto label_logic = create_slider_label(xpos, ypos - 30);
	label_logic->set_text(title);
	return listbox;
}

void Options::on_format_selected(std::shared_ptr<clan::ListBoxView> listbox)
{
	int value = listbox->selected_item();
	switch (value)
	{
		case 0:
			sized_format = clan::tf_rgb8;
			break;
		case 1:
			sized_format = clan::tf_rgba8;
			break;
		case 2:
			sized_format = clan::tf_r8;
			break;
		case 3:
			sized_format = clan::tf_r32f;
			break;
	}
}

void Options::on_dimension_selected(std::shared_ptr<clan::ListBoxView> listbox)
{
	int value = listbox->selected_item();
	switch (value)
	{
		case 0:
			dimension = perlin_1d;
			break;
		case 1:
			dimension = perlin_2d;
			break;
		case 2:
			dimension = perlin_3d;
			break;
		case 3:
			dimension = perlin_4d;
			break;
	}
}

std::shared_ptr<clan::LabelView> Options::create_slider_label(int xpos, int ypos)
{
	std::shared_ptr<clan::LabelView> component = Theme::create_label(true);
	add_child(component);
	component->style()->set("position: absolute; left:%1px; top:%2px", xpos , ypos);
	component->set_text("##################");
	return component;
}

void Options::slider_amplitude_changed()
{
	amplitude = get_value(slider_amplitude, MAX_AMPLITUDE);
	std::string text(clan::string_format("Amplitude: %1", amplitude));
	label_amplitude->set_text(text);
}
void Options::slider_width_changed()
{
	width = slider_width->position();
	std::string text(clan::string_format("Width : %1", width));
	label_width->set_text(text);
}
void Options::slider_height_changed()
{
	height = slider_height->position();
	std::string text(clan::string_format("Height : %1", height));
	label_height->set_text(text);
}
void Options::slider_octaves_changed()
{
	octaves = slider_octaves->position();
	std::string text(clan::string_format("Octaves : %1", octaves));
	label_octaves->set_text(text);
}
void Options::slider_start_x_changed()
{
	start_x = get_value(slider_start_x, MAX_POSITION);
	std::string text(clan::string_format("Start X : %1", start_x));
	label_start_x->set_text(text);
}
void Options::slider_length_x_changed()
{
	length_x = get_value(slider_length_x, MAX_LENGTH);
	std::string text(clan::string_format("Length X : %1", length_x));
	label_length_x->set_text(text);
}

void Options::slider_start_y_changed()
{
	start_y = get_value(slider_start_y, MAX_POSITION);
	std::string text(clan::string_format("Start Y : %1", start_y));
	label_start_y->set_text(text);
}
void Options::slider_length_y_changed()
{
	length_y = get_value(slider_length_y, MAX_LENGTH);
	std::string text(clan::string_format("Length Y : %1", length_y));
	label_length_y->set_text(text);
}
void Options::slider_position_z_changed()
{
	position_z = get_value(slider_position_z, MAX_POSITION);
	std::string text(clan::string_format("Position Z : %1", position_z));
	label_position_z->set_text(text);
}
void Options::slider_position_w_changed()
{
	position_w = get_value(slider_position_w, MAX_POSITION);
	std::string text(clan::string_format("Position W : %1", position_w));
	label_position_w->set_text(text);
}

void Options::update_all_slider_text()
{
	slider_amplitude_changed();
	slider_width_changed();
	slider_height_changed();
	slider_octaves_changed();
	slider_start_x_changed();
	slider_length_x_changed();
	slider_start_y_changed();
	slider_length_y_changed();
	slider_position_z_changed();
	slider_position_w_changed();
}
