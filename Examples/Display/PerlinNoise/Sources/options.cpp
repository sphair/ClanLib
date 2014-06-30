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

// NOTE: These limits are only to make the example easier to use
#define MAX_AMPLITUDE	2.0f	
#define MAX_POSITION	255.0f
#define MAX_LENGTH		64.0f

Options::Options(clan::GUIManager &gui, clan::Rect gui_position) : clan::GUIComponent(&gui, clan::GUITopLevelDescription("Options", gui_position, false))
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

	make_format_menu(combo_format_menu);
	combo_format = create_format_combo_box(450, 30, combo_format_menu, 0);
	label_format = create_combobox_label(combo_format, "Pixel Format");

	checkbox_normals = create_checkbox(640, 35, "Draw Normals (rgb only)", is_normals_set);
	checkbox_normals->func_state_changed() = bind_member(this, &Options::checkbox_normals_changed);

	make_dimension_menu(combo_dimension_menu);
	combo_dimension = create_dimension_combo_box(450, 80, combo_dimension_menu, 0);
	combo_dimension->set_selected_item(1);
	label_dimension = create_combobox_label(combo_dimension, "Dimension");

	int slider_xpos = 450;
	int slider_ypos = 120;
	int slider_gap = 20;

	slider_width = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_ypos += 8;
	slider_width->set_min(32);
	slider_width->set_max(382);
	slider_width->set_position(width);
	slider_width->func_value_changed() = bind_member(this, &Options::slider_width_changed);
	slider_ypos += 8;
	label_width = create_slider_label(slider_width);

	slider_height = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_ypos += 8;
	slider_height->set_min(32);
	slider_height->set_max(382);
	slider_height->set_position(height);
	slider_height->func_value_changed() = bind_member(this, &Options::slider_height_changed);
	slider_ypos += 8;
	label_height = create_slider_label(slider_height);

	slider_octaves = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_ypos += 8;
	slider_octaves->set_min(1);
	slider_octaves->set_max(9);
	slider_octaves->set_page_step(1);
	slider_octaves->set_position(octaves);
	slider_octaves->func_value_changed() = bind_member(this, &Options::slider_octaves_changed);
	slider_ypos += 8;
	label_octaves = create_slider_label(slider_octaves);

	slider_amplitude = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_ypos += 8;
	set_value(slider_amplitude, amplitude, MAX_AMPLITUDE);
	slider_amplitude->func_value_changed() = bind_member(this, &Options::slider_amplitude_changed);
	slider_ypos += 8;
	label_amplitude = create_slider_label(slider_amplitude);

	slider_start_x = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_ypos += 8;
	set_value(slider_start_x, start_x, MAX_POSITION);
	slider_start_x->func_value_changed() = bind_member(this, &Options::slider_start_x_changed);
	slider_ypos += 8;
	label_start_x = create_slider_label(slider_start_x);

	slider_length_x = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_ypos += 8;
	set_value(slider_length_x, length_x, MAX_LENGTH);
	slider_length_x->func_value_changed() = bind_member(this, &Options::slider_length_x_changed);
	slider_ypos += 8;
	label_length_x = create_slider_label(slider_length_x);

	slider_start_y = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_ypos += 8;
	set_value(slider_start_y, start_y, MAX_POSITION);
	slider_start_y->func_value_changed() = bind_member(this, &Options::slider_start_y_changed);
	slider_ypos += 8;
	label_start_y = create_slider_label(slider_start_y);

	slider_length_y = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_ypos += 8;
	set_value(slider_length_y, length_y, MAX_LENGTH);
	slider_length_y->func_value_changed() = bind_member(this, &Options::slider_length_y_changed);
	slider_ypos += 8;
	label_length_y = create_slider_label(slider_length_y);

	slider_position_z = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_ypos += 8;
	set_value(slider_position_z, position_z, MAX_POSITION);
	slider_position_z->func_value_changed() = bind_member(this, &Options::slider_position_z_changed);
	slider_ypos += 8;
	label_position_z = create_slider_label(slider_position_z);

	slider_position_w = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_ypos += 8;
	set_value(slider_position_w, position_w, MAX_POSITION);
	slider_position_w->func_value_changed() = bind_member(this, &Options::slider_position_w_changed);
	slider_ypos += 8;
	label_position_w = create_slider_label(slider_position_w);


	update_all_slider_text();

	func_render() = bind_member(this, &Options::on_render);
}

Options::~Options()
{

}

clan::CheckBox *Options::create_checkbox(int xpos, int ypos, const char *name, bool state)
{
	clan::CheckBox *checkbox = new clan::CheckBox(this);
	checkbox->set_geometry(clan::Rect(xpos, ypos , clan::Size(140, 16)));
	checkbox->set_text(name);
	checkbox->set_checked(state);
	return checkbox;
}

void Options::checkbox_normals_changed()
{
	is_normals_set = checkbox_normals->is_checked();
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
	slider->set_position(value);
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

void Options::on_format_selected(int value, clan::ComboBox *combo)
{
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

void Options::on_dimension_selected(int value, clan::ComboBox *combo)
{
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
clan::Label *Options::create_slider_label(clan::Slider *slider)
{
	clan::Label *component = new clan::Label(this);
	clan::Rect slider_geometry = slider->get_geometry();
	component->set_geometry(clan::Rect(slider_geometry.right + 4, slider_geometry.top - 2, clan::Size(256, 17)));
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
	width = slider_width->get_position();
	std::string text(clan::string_format("Width : %1", width));
	label_width->set_text(text);
}
void Options::slider_height_changed()
{
	height = slider_height->get_position();
	std::string text(clan::string_format("Height : %1", height));
	label_height->set_text(text);
}
void Options::slider_octaves_changed()
{
	octaves = slider_octaves->get_position();
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

clan::ComboBox *Options::create_format_combo_box(int xpos, int ypos, clan::PopupMenu &menu, int selected_item)
{
	clan::ComboBox *combo = new clan::ComboBox(this);
	combo->set_geometry(clan::Rect(xpos, ypos, clan::Size(180, 21)));
	combo->set_editable(false);
	combo->set_dropdown_height(128);
	combo->set_dropdown_minimum_width(64);
	combo->set_popup_menu(menu);
	combo->set_selected_item(selected_item);
	combo->func_item_selected() = bind_member(this, &Options::on_format_selected, combo);

	return combo;
}

void Options::make_format_menu(clan::PopupMenu &menu)
{
	menu.insert_item("clan::tf_rgb8");
	menu.insert_item("clan::tf_rgba8");
	menu.insert_item("clan::tf_r8");
	menu.insert_item("clan::tf_r32f");
}

clan::ComboBox *Options::create_dimension_combo_box(int xpos, int ypos, clan::PopupMenu &menu, int selected_item)
{
	clan::ComboBox *combo = new clan::ComboBox(this);
	combo->set_geometry(clan::Rect(xpos, ypos, clan::Size(180, 21)));
	combo->set_editable(false);
	combo->set_dropdown_height(128);
	combo->set_dropdown_minimum_width(64);
	combo->set_popup_menu(menu);
	combo->set_selected_item(selected_item);
	combo->func_item_selected() = bind_member(this, &Options::on_dimension_selected, combo);

	return combo;
}

void Options::make_dimension_menu(clan::PopupMenu &menu)
{
	menu.insert_item("1D");
	menu.insert_item("2D");
	menu.insert_item("3D");
	menu.insert_item("4D");
}

clan::Label *Options::create_combobox_label(clan::ComboBox *combo, const char *text)
{
	clan::Label *component = new clan::Label(this);
	clan::Rect combo_geometry = combo->get_geometry();
	component->set_geometry(clan::Rect(combo_geometry.left, combo_geometry.top - 20, clan::Size(256, 17)));
	component->set_text(text);
	return component;
}
