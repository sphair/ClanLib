/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

Options::Options(CL_GUIManager &gui, CL_Rect gui_position) : CL_GUIComponent(&gui, CL_GUITopLevelDescription("Options", gui_position, false))
{
	// Note, when changing these, remember to change the popup menu defaults
	selected_light = light_spot;
	max_color_value = 4.0f;
	max_angle_value = 360.0f;
	max_exponent_value = 5.0f;
	max_position_value = 100.0f;

	spot_light_position.x = max_position_value / 2.0f;
	spot_light_position.y = max_position_value / 2.0f;
	spot_light_position.z = max_position_value / 2.0f;
	spot_light_cutoff = 30.0f;
	spot_light_exponent = 0.5f;

	distant_specular_color = CL_Colorf(0.5f, 0.5f, 0.5f, 1.0f);
	distant_diffuse_color = CL_Colorf(0.5f, 0.5f, 0.5f, 1.0f);

	spot_specular_color = CL_Colorf(1.0f, 0.0f, 0.0f, 1.0f);
	spot_diffuse_color = CL_Colorf(1.0f, 0.0f, 0.0f, 1.0f);

	distant_direction_heading = CL_Angle(45.0f, cl_degrees);
	distant_direction_pitch = CL_Angle(35.0f, cl_degrees);
	distant_direction_bank = CL_Angle(0.0f, cl_degrees);

	spot_direction_heading = CL_Angle(129.6f, cl_degrees);
	spot_direction_pitch = CL_Angle(123.8f, cl_degrees);
	spot_direction_bank = CL_Angle(0.0f, cl_degrees);

	make_light_menu(combo_light_menu);
	combo_light = create_light_combo_box(8, 16, combo_light_menu, 1);
	label_light = create_combobox_label(combo_light, "Selected Light");

	int slider_xpos = 8;
	int slider_ypos = 48;
	int slider_gap = 24;
	slider_ypos += 8;
	slider_diffuse_red = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_diffuse_red->func_value_changed().set(this, &Options::slider_diffuse_red_changed);
	slider_diffuse_green = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_diffuse_green->func_value_changed().set(this, &Options::slider_diffuse_green_changed);
	slider_diffuse_blue = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_diffuse_blue->func_value_changed().set(this, &Options::slider_diffuse_blue_changed);
	label_diffuse_red = create_slider_label(slider_diffuse_red);
	label_diffuse_green = create_slider_label(slider_diffuse_green);
	label_diffuse_blue = create_slider_label(slider_diffuse_blue);
	slider_ypos += 8;
	slider_specular_red = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_specular_red->func_value_changed().set(this, &Options::slider_specular_red_changed);
	slider_specular_green = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_specular_green->func_value_changed().set(this, &Options::slider_specular_green_changed);
	slider_specular_blue = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_specular_blue->func_value_changed().set(this, &Options::slider_specular_blue_changed);
	label_specular_red = create_slider_label(slider_specular_red);
	label_specular_green = create_slider_label(slider_specular_green);
	label_specular_blue = create_slider_label(slider_specular_blue);
	slider_ypos += 8;
	slider_direction_heading = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_direction_heading->func_value_changed().set(this, &Options::slider_direction_heading_changed);
	slider_direction_pitch = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_direction_pitch->func_value_changed().set(this, &Options::slider_direction_pitch_changed);
	slider_direction_bank = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_direction_bank->func_value_changed().set(this, &Options::slider_direction_bank_changed);
	label_direction_heading = create_slider_label(slider_direction_heading);
	label_direction_pitch = create_slider_label(slider_direction_pitch);
	label_direction_bank = create_slider_label(slider_direction_bank);

	slider_ypos += 8;
	slider_light_xpos = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_light_xpos->func_value_changed().set(this, &Options::slider_light_xpos_changed);
	slider_light_ypos = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_light_ypos->func_value_changed().set(this, &Options::slider_light_ypos_changed);
	slider_light_zpos = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_light_zpos->func_value_changed().set(this, &Options::slider_light_zpos_changed);
	slider_cutoff = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_cutoff->func_value_changed().set(this, &Options::slider_cutoff_changed);
	slider_exponent = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_exponent->func_value_changed().set(this, &Options::slider_exponent_changed);
	label_light_xpos = create_slider_label(slider_light_xpos);
	label_light_ypos = create_slider_label(slider_light_ypos);
	label_light_zpos = create_slider_label(slider_light_zpos);
	label_cutoff = create_slider_label(slider_cutoff);
	label_exponent = create_slider_label(slider_exponent);

	set_all_sliders();

	func_render().set(this, &Options::on_render);
}

Options::~Options()
{

}

void Options::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_BlendMode blendmode;
	blendmode.enable_blending(false);
	gc.set_blend_mode(blendmode);
	CL_Draw::fill(gc, update_rect, CL_Colorf(0.4f, 0.4f, 0.6f, 0.0f));
	gc.reset_blend_mode();
}

float Options::get_value(CL_Slider *slider, float max_value)
{
	float value = (float) slider->get_position();
	value /= (float) slider->get_max();
	return value * max_value;
}

void Options::set_value(CL_Slider *slider, float value, float max_value)
{
	value /= max_value;
	value *= (float) slider->get_max();
	slider->set_position(value);
}


CL_Slider *Options::create_slider(int xpos, int ypos)
{
	CL_Slider *component = new CL_Slider(this);
	component->set_geometry(CL_Rect(xpos, ypos, CL_Size(128, 17)));
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

CL_CheckBox *Options::create_checkbox(int xpos, int ypos, const char *name, bool state)
{
	CL_CheckBox *checkbox = new CL_CheckBox(this);
	checkbox->set_geometry(CL_Rect(xpos, ypos , CL_Size(300, 16)));
	checkbox->set_text(name);
	checkbox->set_checked(state);
	return checkbox;
}

void Options::on_light_selected(int value, CL_ComboBox *combo)
{
	switch (value)
	{
		case 0:
			selected_light = light_distant;
			break;
		case 1:
			selected_light = light_spot;
			break;
	}
	set_all_sliders();
}

CL_Label *Options::create_slider_label(CL_Slider *slider)
{
	CL_Label *component = new CL_Label(this);
	CL_Rect slider_geometry = slider->get_geometry();
	component->set_geometry(CL_Rect(slider_geometry.right + 4, slider_geometry.top - 2, CL_Size(256, 17)));
	component->set_text("##################");
	return component;
}

void Options::update_all_slider_text()
{
	slider_specular_red_changed();
	slider_specular_green_changed();
	slider_specular_blue_changed();
	slider_diffuse_red_changed();
	slider_diffuse_green_changed();
	slider_diffuse_blue_changed();
	slider_direction_heading_changed();
	slider_direction_pitch_changed();
	slider_direction_bank_changed();

	slider_light_xpos_changed();
	slider_light_ypos_changed();
	slider_light_zpos_changed();
	slider_cutoff_changed();
	slider_exponent_changed();
}

CL_ComboBox *Options::create_light_combo_box(int xpos, int ypos, CL_PopupMenu &menu, int selected_item)
{
	CL_ComboBox *combo = new CL_ComboBox(this);
	combo->set_geometry(CL_Rect(xpos, ypos, CL_Size(180, 21)));
	combo->set_editable(false);
	combo->set_dropdown_height(128);
	combo->set_dropdown_minimum_width(64);
	combo->set_popup_menu(menu);
	combo->set_selected_item(selected_item);
	combo->func_item_selected().set(this, &Options::on_light_selected, combo);

	return combo;
}

void Options::make_light_menu(CL_PopupMenu &menu)
{
	menu.insert_item("Distant Light");
	menu.insert_item("Spot Light");
}

CL_Label *Options::create_combobox_label(CL_ComboBox *combo, const char *text)
{
	CL_Label *component = new CL_Label(this);
	CL_Rect combo_geometry = combo->get_geometry();
	component->set_geometry(CL_Rect(combo_geometry.left, combo_geometry.top - 20, CL_Size(256, 17)));
	component->set_text(text);
	return component;
}

void Options::slider_specular_red_changed()
{
	float value = get_value(slider_specular_red, max_color_value);
	CL_String text(cl_format("Specular Red : %1", value));
	label_specular_red->set_text(text);
	get_selected_specular().r = value;
}

void Options::slider_specular_green_changed()
{
	float value = get_value(slider_specular_green, max_color_value);
	CL_String text(cl_format("Specular Green : %1", value));
	label_specular_green->set_text(text);
	get_selected_specular().g = value;
}

void Options::slider_specular_blue_changed()
{
	float value = get_value(slider_specular_blue, max_color_value);
	CL_String text(cl_format("Specular Blue : %1", value));
	label_specular_blue->set_text(text);
	get_selected_specular().b = value;
}

void Options::slider_diffuse_red_changed()
{
	float value = get_value(slider_diffuse_red, max_color_value);
	CL_String text(cl_format("Diffuse Red : %1", value));
	label_diffuse_red->set_text(text);
	get_selected_diffuse().r = value;
}

void Options::slider_diffuse_green_changed()
{
	float value = get_value(slider_diffuse_green, max_color_value);
	CL_String text(cl_format("Diffuse Green : %1", value));
	label_diffuse_green->set_text(text);
	get_selected_diffuse().g = value;
}

void Options::slider_diffuse_blue_changed()
{
	float value = get_value(slider_diffuse_blue, max_color_value);
	CL_String text(cl_format("Diffuse Blue : %1", value));
	label_diffuse_blue->set_text(text);
	get_selected_diffuse().b = value;
}

void Options::slider_direction_heading_changed()
{
	float value = get_value(slider_direction_heading, max_angle_value);
	CL_String text(cl_format("Heading : %1", value));
	label_direction_heading->set_text(text);

	if (selected_light == light_distant)
		distant_direction_heading.set_degrees(value);
	else
		spot_direction_heading.set_degrees(value);
}

void Options::slider_direction_pitch_changed()
{
	float value = get_value(slider_direction_pitch, max_angle_value);
	CL_String text(cl_format("Pitch : %1", value));
	label_direction_pitch->set_text(text);
	if (selected_light == light_distant)
		distant_direction_pitch.set_degrees(value);
	else
		spot_direction_pitch.set_degrees(value);
}

void Options::slider_direction_bank_changed()
{
	float value = get_value(slider_direction_bank, max_angle_value);
	CL_String text(cl_format("Bank : %1", value));
	label_direction_bank->set_text(text);
	if (selected_light == light_distant)
		distant_direction_bank.set_degrees(value);
	else
		spot_direction_bank.set_degrees(value);
}

void Options::slider_light_xpos_changed()
{
	float value = get_value(slider_light_xpos, max_position_value);
	CL_String text(cl_format("Offset X pos : %1", value));
	label_light_xpos->set_text(text);
	spot_light_position.x = value;
}

void Options::slider_light_ypos_changed()
{
	float value = get_value(slider_light_ypos, max_position_value);
	CL_String text(cl_format("Offset Y pos : %1", value));
	label_light_ypos->set_text(text);
	spot_light_position.y = value;
}

void Options::slider_light_zpos_changed()
{
	float value = get_value(slider_light_zpos, max_position_value);
	CL_String text(cl_format("Offset Z pos : %1", value));
	label_light_zpos->set_text(text);
	spot_light_position.z = value;
}

void Options::slider_cutoff_changed()
{
	float value = get_value(slider_cutoff, max_angle_value);
	CL_String text(cl_format("Cutoff : %1", value));
	label_cutoff->set_text(text);
	spot_light_cutoff = value;
}

void Options::slider_exponent_changed()
{
	float value = get_value(slider_exponent, max_exponent_value);
	CL_String text(cl_format("Exponent : %1", value));
	label_exponent->set_text(text);
	spot_light_exponent = value;
}


CL_Colorf &Options::get_selected_specular()
{
	if (selected_light == light_distant)
		return distant_specular_color;
	return spot_specular_color;
}

CL_Colorf &Options::get_selected_diffuse()
{
	if (selected_light == light_distant)
		return distant_diffuse_color;
	return spot_diffuse_color;
}

void Options::set_all_sliders()
{
	set_value(slider_specular_red, get_selected_specular().r, max_color_value);
	set_value(slider_specular_green, get_selected_specular().g, max_color_value);
	set_value(slider_specular_blue, get_selected_specular().b, max_color_value);
	set_value(slider_diffuse_red, get_selected_diffuse().r, max_color_value);
	set_value(slider_diffuse_green, get_selected_diffuse().g, max_color_value);
	set_value(slider_diffuse_blue, get_selected_diffuse().b, max_color_value);

	set_value(slider_exponent, spot_light_exponent, max_exponent_value);
	set_value(slider_cutoff, spot_light_cutoff, max_angle_value);
	set_value(slider_light_xpos, spot_light_position.x, max_position_value);
	set_value(slider_light_ypos, spot_light_position.y, max_position_value);
	set_value(slider_light_zpos, spot_light_position.z, max_position_value);

	if (selected_light == light_distant)
	{
		set_value(slider_direction_heading, distant_direction_heading.to_degrees(), max_angle_value);
		set_value(slider_direction_pitch, distant_direction_pitch.to_degrees(), max_angle_value);
		set_value(slider_direction_bank, distant_direction_bank.to_degrees(), max_angle_value);

		slider_exponent->set_enabled(false);
		slider_light_xpos->set_enabled(false);
		slider_light_ypos->set_enabled(false);
		slider_light_zpos->set_enabled(false);
		slider_cutoff->set_enabled(false);

	}
	else
	{
		set_value(slider_direction_heading, spot_direction_heading.to_degrees(), max_angle_value);
		set_value(slider_direction_pitch, spot_direction_pitch.to_degrees(), max_angle_value);
		set_value(slider_direction_bank, spot_direction_bank.to_degrees(), max_angle_value);

		slider_exponent->set_enabled(true);
		slider_light_xpos->set_enabled(true);
		slider_light_ypos->set_enabled(true);
		slider_light_zpos->set_enabled(true);
		slider_cutoff->set_enabled(true);
	}

	update_all_slider_text();

}


