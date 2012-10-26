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

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gui.h>
using namespace clan;

#include "panel.h"
#include "gui.h"

Panel::Panel(GUI *gui) : gui(gui)
{
	GUITopLevelDescription window_desc;
	window_desc.set_title("Information");
	window_desc.set_position(Rect(10, 20, Size(300, 300)), false);
	window = new Window(&gui->get_gui(), window_desc);
	window->set_draggable(true);

	int yoffset = 30;

	radiobutton_sprite = new RadioButton(window);
	radiobutton_sprite->set_geometry(Rect(15, yoffset, Size(64, 32)));
	radiobutton_sprite->set_text("Sprite");
	radiobutton_sprite->set_selected(true);
	radiobutton_sprite->set_group_name("ImageType");
	radiobutton_sprite->func_selected().set(this, &Panel::on_imagetype_selected, radiobutton_sprite);

	radiobutton_image = new RadioButton(window);
	radiobutton_image->set_geometry(Rect(85, yoffset, Size(64, 32)));
	radiobutton_image->set_text("Image");
	radiobutton_image->set_selected(false);
	radiobutton_image->set_group_name("ImageType");
	radiobutton_image->func_selected().set(this, &Panel::on_imagetype_selected, radiobutton_image);

	radiobutton_texture = new RadioButton(window);
	radiobutton_texture->set_geometry(Rect(155, yoffset, Size(76, 32)));
	radiobutton_texture->set_text("Texture");
	radiobutton_texture->set_selected(false);
	radiobutton_texture->set_group_name("ImageType");
	radiobutton_texture->func_selected().set(this, &Panel::on_imagetype_selected, radiobutton_texture);

	radiobutton_font = new RadioButton(window);
	radiobutton_font->set_geometry(Rect(235, yoffset, Size(64, 32)));
	radiobutton_font->set_text("Font");
	radiobutton_font->set_selected(false);
	radiobutton_font->set_group_name("ImageType");
	radiobutton_font->func_selected().set(this, &Panel::on_imagetype_selected, radiobutton_font);

	yoffset += 30;

	radiobutton_linear = new RadioButton(window);
	radiobutton_linear->set_geometry(Rect(15, yoffset, Size(80, 32)));
	radiobutton_linear->set_text("cl_filter_linear");
	radiobutton_linear->set_selected(true);
	radiobutton_linear->set_group_name("FilterType");

	radiobutton_nearest = new RadioButton(window);
	radiobutton_nearest->set_geometry(Rect(105, yoffset, Size(100, 32)));
	radiobutton_nearest->set_text("cl_filter_nearest");
	radiobutton_nearest->set_selected(false);
	radiobutton_nearest->set_group_name("FilterType");

	yoffset += 40;

	checkbox_outline = new CheckBox(window);
	checkbox_outline->set_geometry(Rect(15, yoffset, Size(150, 15)));
	checkbox_outline->set_text("Outline Source Image");

	checkbox_antialias = new CheckBox(window);
	checkbox_antialias->set_geometry(Rect(155, yoffset, Size(100, 15)));
	checkbox_antialias->set_text("AntiAlias Font");
	checkbox_antialias->set_enabled(false);

	yoffset += 40;

	slider_scale = new Slider(window);
	slider_scale->set_geometry(Rect(15, yoffset, Size(100, 17)));
	slider_scale->set_vertical(false);
	slider_scale->set_horizontal(true);
	slider_scale->set_min(0);
	slider_scale->set_max(1000);
	slider_scale->set_tick_count(2);
	slider_scale->set_page_step(100);
	slider_scale->set_position(50);
	slider_scale->set_lock_to_ticks(false);
	slider_scale->func_value_changed().set(this, &Panel::on_slider_changed, slider_scale);

	spin_scale = new Spin(window);
	spin_scale->set_geometry(Rect(130, yoffset, Size(64, 21)));
	spin_scale->set_number_of_decimal_places(2);
	spin_scale->set_floating_point_mode(true);
	spin_scale->set_value_float(1.0f);
	spin_scale->set_step_size_float(0.1f);
	spin_scale->set_ranges_float(0.1f, 4.0f);
	spin_scale->func_value_changed().set(this, &Panel::on_spin_changed, spin_scale);

	Size label_size(50, 15);
	label_scale = new Label(window);
	label_scale->set_geometry(Rect(200, yoffset, label_size));
	label_scale->set_text("Scale");

	set_slider_to_spin(slider_scale, spin_scale);

	yoffset += 40;

	slider_rotation = new Slider(window);
	slider_rotation->set_geometry(Rect(15, yoffset, Size(100, 17)));
	slider_rotation->set_vertical(false);
	slider_rotation->set_horizontal(true);
	slider_rotation->set_min(0);
	slider_rotation->set_max(1000);
	slider_rotation->set_tick_count(2);
	slider_rotation->set_page_step(10);
	slider_rotation->set_position(50);
	slider_rotation->set_lock_to_ticks(false);
	slider_rotation->func_value_changed().set(this, &Panel::on_slider_changed, slider_rotation);

	spin_rotation = new Spin(window);
	spin_rotation->set_geometry(Rect(130, yoffset, Size(64, 21)));
	spin_rotation->set_number_of_decimal_places(2);
	spin_rotation->set_floating_point_mode(true);
	spin_rotation->set_value_float(0.0f);
	spin_rotation->set_step_size_float(45.0f);
	spin_rotation->set_ranges_float(0.0f, 360.0f);
	spin_rotation->func_value_changed().set(this, &Panel::on_spin_changed, spin_rotation);

	label_rotation = new Label(window);
	label_rotation->set_geometry(Rect(200, yoffset, label_size));
	label_rotation->set_text("Rotation");

	set_slider_to_spin(slider_rotation, spin_rotation);

	yoffset += 40;

	slider_translate = new Slider(window);
	slider_translate->set_geometry(Rect(15, yoffset, Size(100, 17)));
	slider_translate->set_vertical(false);
	slider_translate->set_horizontal(true);
	slider_translate->set_min(0);
	slider_translate->set_max(1000);
	slider_translate->set_tick_count(2);
	slider_translate->set_page_step(100);
	slider_translate->set_position(50);
	slider_translate->set_lock_to_ticks(false);
	slider_translate->func_value_changed().set(this, &Panel::on_slider_changed, slider_translate);

	spin_translate = new Spin(window);
	spin_translate->set_geometry(Rect(130, yoffset, Size(64, 21)));
	spin_translate->set_number_of_decimal_places(2);
	spin_translate->set_floating_point_mode(true);
	spin_translate->set_value_float(0.0f);
	spin_translate->set_step_size_float(0.375f);
	spin_translate->set_ranges_float(-1.5f, 1.5f);
	spin_translate->func_value_changed().set(this, &Panel::on_spin_changed, spin_translate);

	label_translate = new Label(window);
	label_translate->set_geometry(Rect(200, yoffset - 8, label_size));
	label_translate->set_text("Matrix");
	label_translate2 = new Label(window);
	label_translate2->set_geometry(Rect(200, yoffset + 8, label_size));
	label_translate2->set_text("Translate");

	set_slider_to_spin(slider_translate, spin_translate);

	yoffset += 40;

	slider_texture_translate = new Slider(window);
	slider_texture_translate->set_geometry(Rect(15, yoffset, Size(100, 17)));
	slider_texture_translate->set_vertical(false);
	slider_texture_translate->set_horizontal(true);
	slider_texture_translate->set_min(0);
	slider_texture_translate->set_max(1000);
	slider_texture_translate->set_tick_count(2);
	slider_texture_translate->set_page_step(100);
	slider_texture_translate->set_position(50);
	slider_texture_translate->set_lock_to_ticks(false);
	slider_texture_translate->func_value_changed().set(this, &Panel::on_slider_changed, slider_texture_translate);

	spin_texture_translate = new Spin(window);
	spin_texture_translate->set_geometry(Rect(130, yoffset, Size(64, 21)));
	spin_texture_translate->set_number_of_decimal_places(3);
	spin_texture_translate->set_floating_point_mode(true);
	spin_texture_translate->set_value_float(0.0f);
	spin_texture_translate->set_step_size_float(0.375f);
	spin_texture_translate->set_ranges_float(-1.0f, 1.0f);
	spin_texture_translate->func_value_changed().set(this, &Panel::on_spin_changed, spin_texture_translate);

	label_texture_translate = new Label(window);
	label_texture_translate->set_geometry(Rect(200, yoffset - 8, label_size));
	label_texture_translate->set_text("Texture");
	label_texture_translate2 = new Label(window);
	label_texture_translate2->set_geometry(Rect(200, yoffset + 8, label_size));
	label_texture_translate2->set_text("Translate");

	set_slider_to_spin(slider_texture_translate, spin_texture_translate);

	slider_texture_translate->set_enabled(false);;
	spin_texture_translate->set_enabled(false);;

}

bool Panel::is_sprite()
{
	return radiobutton_sprite->is_selected();
}

bool Panel::is_image()
{
	return radiobutton_image->is_selected();
}

bool Panel::is_font()
{
	return radiobutton_font->is_selected();
}

bool Panel::is_texture()
{
	return radiobutton_texture->is_selected();
}

bool Panel::is_linear()
{
	return radiobutton_linear->is_selected();
}

bool Panel::is_nearest()
{
	return radiobutton_nearest->is_selected();
}

bool Panel::is_outlined()
{
	return checkbox_outline->is_checked();
}

bool Panel::is_antialias()
{
	return checkbox_antialias->is_checked();
}

void Panel::on_slider_changed(Slider *slider)
{
	Spin *spin = NULL;
	if (slider == slider_rotation)
		spin = spin_rotation;
	if (slider == slider_scale)
		spin = spin_scale;
	if (slider == slider_translate)
		spin = spin_translate;
	if (slider == slider_texture_translate)
		spin = spin_texture_translate;
	if (!spin)
		return;

	float position = (float) slider->get_position();

	position -= slider->get_min();
	float max = (float) (slider->get_max() + 1 - slider->get_min());

	float spin_min = spin->get_min_float();
	float spin_max = spin->get_max_float();
	float spin_range = spin_max - spin_min;

	float value = (position * spin_range) / max;
	value += spin_min;
	if (value < spin_min)
		value = spin_min;

	if (value > spin_max)
		value = spin_max;

	spin->set_value_float(value);
}

float Panel::get_scale()
{
	return spin_scale->get_value_float();
}

float Panel::get_rotation()
{
	return spin_rotation->get_value_float();
}

float Panel::get_translate()
{
	return spin_translate->get_value_float();
}

float Panel::get_texture_translate()
{
	return spin_texture_translate->get_value_float();
}

void Panel::set_slider_to_spin(Slider *slider, Spin *spin)
{
	float value = spin->get_value_float();
	float spin_min = spin->get_min_float();
	float spin_max = spin->get_max_float();
	float spin_range = spin_max - spin_min;

	int slider_max = slider->get_max();
	int slider_min = slider->get_min();

	float slider_range = (float) (slider_max + 1 - slider_min);

	int new_value = (int) (((value - spin_min) * slider_range) / spin_range);
	if (new_value < slider_min)
		new_value = slider_min;

	if (new_value > slider_max)
		new_value = slider_max;

	slider->set_position(new_value);

}

void Panel::on_spin_changed(Spin *spin)
{
	Slider *slider = NULL;
	if (spin == spin_rotation)
		slider = slider_rotation;
	if (spin == spin_scale)
		slider = slider_scale;
	if (spin == spin_translate)
		slider = slider_translate;
	if (spin == spin_texture_translate)
		slider = slider_texture_translate;
	if (!slider)
		return;

	set_slider_to_spin(slider, spin);
}

void Panel::on_imagetype_selected(RadioButton *radiobutton)
{
	if (radiobutton == radiobutton_sprite)
	{
		radiobutton_linear->set_enabled(true);
		radiobutton_nearest->set_enabled(true);
		slider_texture_translate->set_enabled(false);;
		spin_texture_translate->set_enabled(false);;
		checkbox_outline->set_enabled(true);
		checkbox_antialias->set_enabled(false);
	}

	if (radiobutton == radiobutton_texture)
	{
		radiobutton_linear->set_enabled(true);
		radiobutton_nearest->set_enabled(true);
		slider_texture_translate->set_enabled(true);
		spin_texture_translate->set_enabled(true);
		checkbox_outline->set_enabled(true);
		checkbox_antialias->set_enabled(false);
	}

	if (radiobutton == radiobutton_image)
	{
		radiobutton_linear->set_enabled(false);
		radiobutton_nearest->set_enabled(false);
		slider_texture_translate->set_enabled(false);;
		spin_texture_translate->set_enabled(false);;
		checkbox_outline->set_enabled(true);
		checkbox_antialias->set_enabled(false);
	}

	if (radiobutton == radiobutton_font)
	{
		radiobutton_linear->set_enabled(false);
		radiobutton_nearest->set_enabled(false);
		slider_texture_translate->set_enabled(false);;
		spin_texture_translate->set_enabled(false);;
		checkbox_outline->set_enabled(false);
		checkbox_antialias->set_enabled(true);
	}
}