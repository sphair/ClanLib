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
#include "font.h"

void App::draw_font_info()
{
	CL_GraphicContext gc = window_ptr->get_gc();

	int offset_x = 300;
	int offset_y = 32;
	const int gap = 24;

	small_font.draw_text(gc, offset_x - 10, offset_y, "Font Class :", CL_Colorf::yellow);
	offset_y += gap;
	switch (selected_fontclass)
	{
		case font_freetype:
			small_font.draw_text(gc, offset_x, offset_y, cl_format("Freetype (CL_Texture based)", font_desc.get_typeface_name()),  CL_Colorf::white);
			break;
		case font_system:
			small_font.draw_text(gc, offset_x, offset_y, cl_format("System (CL_Texture based)", font_desc.get_typeface_name()),  CL_Colorf::white);
			break;
		case font_vector:
			small_font.draw_text(gc, offset_x, offset_y, cl_format("Vector (Triangle based)", font_desc.get_typeface_name()),  CL_Colorf::white);
			break;
		case font_sprite:
			small_font.draw_text(gc, offset_x, offset_y, cl_format("Sprite (CL_Sprite based)", font_desc.get_typeface_name()),  CL_Colorf::white);
			break;
	}
	offset_y += gap;

	small_font.draw_text(gc, offset_x - 10, offset_y, "Font Description :", CL_Colorf::yellow);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Name : %1", font_desc.get_typeface_name()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Height : %1", font_desc.get_height()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Average Width : %1", font_desc.get_average_width()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Escapement : %1", font_desc.get_escapement()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Orientation : %1", font_desc.get_orientation()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Weight : %1", font_desc.get_weight()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Italic : %1", font_desc.get_italic()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Underline : %1", font_desc.get_underline()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Strikeout : %1", font_desc.get_strikeout()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Fixed Pitch : %1", font_desc.get_fixed_pitch()),  CL_Colorf::white);
	offset_y += gap;

	if (font_desc.get_anti_alias_set())
	{
		small_font.draw_text(gc, offset_x, offset_y, cl_format("Anti Alias : %1", font_desc.get_anti_alias()),  CL_Colorf::white);
	}else
	{
		small_font.draw_text(gc, offset_x, offset_y, "Anti Alias : Not Applicable",  CL_Colorf::white);
	}
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Anti Alias Set : %1", font_desc.get_anti_alias_set()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x - 10, offset_y, "Example Text Size:", CL_Colorf::yellow);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Width : %1", font_size.width),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Height : %1", font_size.height),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x - 10, offset_y, "Frames per second:", CL_Colorf::yellow);
	CL_String fps = cl_format("%1", last_fps);
	small_font.draw_text(gc, offset_x + 110, offset_y, fps, CL_Colorf::white);
	offset_y += gap;

	offset_x = 620;
	offset_y = 32;

	small_font.draw_text(gc, offset_x - 10, offset_y, "Font Metrics :", CL_Colorf::yellow);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Height : %1", font_metrics.get_height()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Ascender : %1", font_metrics.get_ascent()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Descender : %1", font_metrics.get_descent()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Internal Leading : %1", font_metrics.get_internal_leading()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("External Leading : %1", font_metrics.get_external_leading()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Average Character Width : %1", font_metrics.get_average_character_width()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Maximum Character Width : %1", font_metrics.get_max_character_width()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Weight : %1", font_metrics.get_weight()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Overhang : %1", font_metrics.get_overhang()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Digitized Aspect X : %1", font_metrics.get_digitized_aspect_x()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Digitized Aspect Y : %1", font_metrics.get_digitized_aspect_y()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("First Char : %1", font_metrics.get_first_char()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Last Char : %1", font_metrics.get_last_char()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Default Char : %1", font_metrics.get_default_char()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Word Break Char : %1", font_metrics.get_word_break_char()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Is Italic : %1", font_metrics.is_italic()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Is Underlined : %1", font_metrics.is_underlined()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Is Struck Out : %1", font_metrics.is_struck_out()),  CL_Colorf::white);
	offset_y += gap;

	small_font.draw_text(gc, offset_x, offset_y, cl_format("Is Fixed Pitch : %1", font_metrics.is_fixed_pitch()),  CL_Colorf::white);
	offset_y += gap;
}

void App::on_lineedit_changed(CL_InputEvent &e)
{
	CL_GraphicContext gc = window_ptr->get_gc();
	font_text = lineedit_text_ptr->get_text();
	font_size = selected_font.get_text_size(gc, font_text);
}

void App::on_button_clicked_class_freetype(CL_PushButton *button)
{
	selected_fontclass = font_freetype;
	font_desc.set_typeface_name("../../Game/DiceWar/Resources/bitstream_vera_sans/VeraBd.ttf");
	button_typeface_sans_ptr->set_enabled(false);
	button_typeface_tahoma_ptr->set_enabled(false);
	select_font();
}

void App::on_button_clicked_class_vector(CL_PushButton *button)
{
	selected_fontclass = font_vector;
	font_desc.set_typeface_name("../../Game/DiceWar/Resources/bitstream_vera_sans/VeraBd.ttf");
	button_typeface_sans_ptr->set_enabled(false);
	button_typeface_tahoma_ptr->set_enabled(false);
	select_font();
}

void App::on_button_clicked_class_sprite(CL_PushButton *button)
{
	selected_fontclass = font_sprite;
	font_desc.set_typeface_name("(Resources)");
	button_typeface_sans_ptr->set_enabled(false);
	button_typeface_tahoma_ptr->set_enabled(false);
	select_font();
}
void App::on_button_clicked_class_system(CL_PushButton *button)
{
	if (!button_typeface_sans_ptr->is_enabled())
	{
		font_desc.set_typeface_name("Tahoma");
		button_typeface_sans_ptr->set_enabled();
		button_typeface_tahoma_ptr->set_enabled();
	}
	selected_fontclass = font_system;
	select_font();
}

void App::on_button_clicked_typeface_tahoma(CL_PushButton *button)
{
	font_desc.set_typeface_name("Tahoma");
	select_font();
}

void App::on_button_clicked_typeface_sans(CL_PushButton *button)
{
	font_desc.set_typeface_name("Microsoft Sans Serif");
	select_font();
}

void App::on_checkbox_state_underline(CL_CheckBox *checkbox)
{
	font_desc.set_underline(checkbox->is_checked());
	select_font();
}

void App::on_checkbox_state_italic(CL_CheckBox *checkbox)
{
	font_desc.set_italic(checkbox->is_checked());
	select_font();
}

void App::on_checkbox_state_strikeout(CL_CheckBox *checkbox)
{
	font_desc.set_strikeout(checkbox->is_checked());
	select_font();
}

void App::on_checkbox_state_antialias(CL_CheckBox *checkbox)
{
	font_desc.set_anti_alias(checkbox->is_checked());
	select_font();
}

void App::on_button_clicked_weight_light(CL_PushButton *button)
{
	font_desc.set_weight(300);
	select_font();
}

void App::on_button_clicked_weight_normal(CL_PushButton *button)
{
	font_desc.set_weight(400);
	select_font();
}

void App::on_button_clicked_weight_bold(CL_PushButton *button)
{
	font_desc.set_weight(700);
	select_font();
}

void App::on_button_clicked_size_16(CL_PushButton *button)
{
	font_desc.set_height(16);
	select_font();
}

void App::on_button_clicked_size_32(CL_PushButton *button)
{
	font_desc.set_height(32);
	select_font();
}

void App::on_button_clicked_size_64(CL_PushButton *button)
{
	font_desc.set_height(64);
	select_font();
}
