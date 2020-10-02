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
		case font_ttf:
			small_font.draw_text(canvas, offset_x, offset_y, string_format("TTF", font_typeface), Colorf::white);
			break;
		case font_sprite:
			small_font.draw_text(canvas, offset_x, offset_y, string_format("Sprite", font_typeface), Colorf::white);
			break;
	}
	offset_y += gap;

	small_font.draw_text(canvas, offset_x - 10, offset_y, "Font Description :", Colorf::yellow);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Name : %1", font_typeface), Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Height : %1", font_desc.get_height()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Average Width : %1", font_desc.get_average_width()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Escapement : %1", font_desc.get_escapement()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Orientation : %1", font_desc.get_orientation()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Weight : %1", (int) font_desc.get_weight()),  Colorf::white);
	offset_y += gap;

	small_font.draw_text(canvas, offset_x, offset_y, string_format("Italic : %1", font_desc.get_style() == FontStyle::italic ? "true" : "false"), Colorf::white);
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
}

void App::on_lineedit_changed()
{
	font_text = lineedit_text->text();
	font_size = selected_font.measure_text(canvas, font_text).bbox_size;
}

void App::on_button_clicked_class_sprite()
{
	selected_fontclass = font_sprite;
	font_typeface = "ClanFont";
	font_filename = "";
	button_typeface_sans->set_disabled();
	button_typeface_tahoma->set_disabled();
	button_typeface_bitstream->set_disabled();
	select_font();
}
void App::on_button_clicked_class_system()
{
	selected_fontclass = font_ttf;
	button_typeface_tahoma->set_enabled();
	button_typeface_sans->set_enabled();
	button_typeface_bitstream->set_enabled();
	on_button_clicked_typeface_tahoma();
}

void App::on_button_clicked_typeface_tahoma()
{
	font_typeface = "Tahoma";
	font_filename = "";
	select_font();
}

void App::on_button_clicked_typeface_sans()
{
	font_typeface = "Microsoft Sans Serif";
	font_filename = "";
	select_font();
}

void App::on_button_clicked_typeface_bitstream()
{
	font_typeface = "Bitstream Vera Sans";
	font_filename = "../../Display_Text/Font/Resources/bitstream_vera_sans/VeraBd.ttf";
	select_font();
}

void App::on_checkbox_state_italic()
{
	font_desc.set_style(checkbox_italic->checked() ? clan::FontStyle::italic : clan::FontStyle::normal);
	select_font();
}

void App::on_checkbox_state_subpixel()
{
	font_desc.set_subpixel(checkbox_subpixel->checked());
	select_font();
}

void App::on_checkbox_state_antialias()
{
	font_desc.set_anti_alias(checkbox_antialias->checked());
	select_font();
}

void App::on_button_clicked_weight_light()
{
	font_desc.set_weight(FontWeight::light);
	select_font();
}

void App::on_button_clicked_weight_normal()
{
	font_desc.set_weight(FontWeight::normal);
	select_font();
}

void App::on_button_clicked_weight_bold()
{
	font_desc.set_weight(FontWeight::bold);
	select_font();
}

void App::on_button_clicked_size_16()
{
	font_desc.set_height(16);
	select_font();
}

void App::on_button_clicked_size_32()
{
	font_desc.set_height(32);
	select_font();
}

void App::on_button_clicked_size_64()
{
	font_desc.set_height(64);
	select_font();
}
