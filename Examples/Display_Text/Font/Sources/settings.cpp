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

#include "precomp.h"
#include "font.h"

void App::draw_font_info()
{
	int offset_x = 300;
	int offset_y = 32;
	const int gap = 24;

	small_font.draw_text(canvas, offset_x - 10, offset_y, "Font Class :", Colorf::yellow);
	offset_y += gap;
	switch (selected_fontclass)
	{
		case font_system:
			small_font.draw_text(canvas, offset_x, offset_y, string_format("System (Texture based)", font_desc.get_typeface_name()),  Colorf::white);
			break;
		case font_vector:
			small_font.draw_text(canvas, offset_x, offset_y, string_format("Vector (Triangle based)", font_desc.get_typeface_name()),  Colorf::white);
			break;
		case font_sprite:
			small_font.draw_text(canvas, offset_x, offset_y, string_format("Sprite (Sprite based)", font_desc.get_typeface_name()),  Colorf::white);
			break;
	}
	offset_y += gap;

	small_font.draw_text(canvas, offset_x - 10, offset_y, "Font Description :", Colorf::yellow);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Name : %1", font_desc.get_typeface_name()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Height : %1", font_desc.get_height()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Average Width : %1", font_desc.get_average_width()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Escapement : %1", font_desc.get_escapement()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Orientation : %1", font_desc.get_orientation()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Weight : %1", font_desc.get_weight()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Italic : %1", font_desc.get_italic() ? "true" : "false"),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Underline : %1", font_desc.get_underline() ? "true" : "false"),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Strikeout : %1", font_desc.get_strikeout() ? "true" : "false"),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Fixed Pitch : %1", font_desc.get_fixed_pitch() ? "true" : "false"),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("SubPixel Rendering : %1", font_desc.get_subpixel() ? "true" : "false"),  Colorf::white);
	offset_y += gap;

	if (font_desc.get_subpixel())
	{
		small_font.draw_text(canvas, offset_x, offset_y, "Anti Alias : Not Applicable (Subpixel set)",  Colorf::white);
	}
	else
	{
		small_font.draw_text(canvas, offset_x, offset_y, string_format("Anti Alias : %1", font_desc.get_anti_alias() ? "true" : "false"),  Colorf::white);
	}
	offset_y += gap;

	small_font.draw_text(canvas, offset_x - 10, offset_y, "Example Text Size:", Colorf::yellow);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Width : %1", font_size.width),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Height : %1", font_size.height),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x - 10, offset_y, "Frames per second:", Colorf::yellow);
	offset_y += gap;
	std::string fps = string_format("%1", last_fps);
	small_font.draw_text(canvas, offset_x, offset_y, fps, Colorf::white);
	offset_y += gap;

	offset_x = 620;
	offset_y = 32;

	small_font.draw_text(canvas, offset_x - 10, offset_y, "Font Metrics :", Colorf::yellow);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Height : %1", font_metrics.get_height()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Ascender : %1", font_metrics.get_ascent()),  Colorf::white);
	offset_y += gap;
	offset_y += gap;	// Allow long font names

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Descender : %1", font_metrics.get_descent()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Internal Leading : %1", font_metrics.get_internal_leading()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("External Leading : %1", font_metrics.get_external_leading()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Average Character Width : %1", font_metrics.get_average_character_width()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Maximum Character Width : %1", font_metrics.get_max_character_width()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Weight : %1", font_metrics.get_weight()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Overhang : %1", font_metrics.get_overhang()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Digitized Aspect X : %1", font_metrics.get_digitized_aspect_x()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Digitized Aspect Y : %1", font_metrics.get_digitized_aspect_y()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("First Char : %1", font_metrics.get_first_char()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Last Char : %1", font_metrics.get_last_char()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Default Char : %1", font_metrics.get_default_char()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Word Break Char : %1", font_metrics.get_word_break_char()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Is Italic : %1", font_metrics.is_italic() ? "true" : "false"),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Is Underlined : %1", font_metrics.is_underlined() ? "true" : "false"),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Is Struck Out : %1", font_metrics.is_struck_out() ? "true" : "false"),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Is Fixed Pitch : %1", font_metrics.is_fixed_pitch() ? "true" : "false"),  Colorf::white);
	offset_y += gap;
}

void App::on_lineedit_changed(InputEvent &e)
{
	font_text = lineedit_text_ptr->get_text();
	font_size = selected_font.get_text_size(canvas, font_text);
}

void App::on_button_clicked_class_vector(PushButton *button)
{
	selected_fontclass = font_vector;
	font_desc.set_typeface_name("Bitstream Vera Sans");
	font_desc.set_filename("../../Game/DiceWar/Resources/bitstream_vera_sans/VeraBd.ttf");
	button_typeface_sans_ptr->set_enabled(false);
	button_typeface_tahoma_ptr->set_enabled(false);
	select_font();
}

void App::on_button_clicked_class_sprite(PushButton *button)
{
	selected_fontclass = font_sprite;
	font_desc.set_typeface_name("(Resources)");
	font_desc.set_filename("");
	button_typeface_sans_ptr->set_enabled(false);
	button_typeface_tahoma_ptr->set_enabled(false);
	select_font();
}
void App::on_button_clicked_class_system(PushButton *button)
{
	if (!button_typeface_sans_ptr->is_enabled())
	{
		font_desc.set_typeface_name("Tahoma");
		font_desc.set_filename("");
		button_typeface_sans_ptr->set_enabled();
		button_typeface_tahoma_ptr->set_enabled();
	}
	selected_fontclass = font_system;
	select_font();
}

void App::on_button_clicked_typeface_tahoma(PushButton *button)
{
	font_desc.set_typeface_name("Tahoma");
	font_desc.set_filename("");
	select_font();
}

void App::on_button_clicked_typeface_sans(PushButton *button)
{
	font_desc.set_typeface_name("Microsoft Sans Serif");
	font_desc.set_filename("");
	select_font();
}

void App::on_checkbox_state_underline(CheckBox *checkbox)
{
	font_desc.set_underline(checkbox->is_checked());
	select_font();
}

void App::on_checkbox_state_italic(CheckBox *checkbox)
{
	font_desc.set_italic(checkbox->is_checked());
	select_font();
}

void App::on_checkbox_state_subpixel(CheckBox *checkbox)
{
	font_desc.set_subpixel(checkbox->is_checked());
	select_font();
}

void App::on_checkbox_state_strikeout(CheckBox *checkbox)
{
	font_desc.set_strikeout(checkbox->is_checked());
	select_font();
}

void App::on_checkbox_state_antialias(CheckBox *checkbox)
{
	font_desc.set_anti_alias(checkbox->is_checked());
	select_font();
}

void App::on_button_clicked_weight_light(PushButton *button)
{
	font_desc.set_weight(300);
	select_font();
}

void App::on_button_clicked_weight_normal(PushButton *button)
{
	font_desc.set_weight(400);
	select_font();
}

void App::on_button_clicked_weight_bold(PushButton *button)
{
	font_desc.set_weight(700);
	select_font();
}

void App::on_button_clicked_size_16(PushButton *button)
{
	font_desc.set_height(16);
	select_font();
}

void App::on_button_clicked_size_32(PushButton *button)
{
	font_desc.set_height(32);
	select_font();
}

void App::on_button_clicked_size_64(PushButton *button)
{
	font_desc.set_height(64);
	select_font();
}
