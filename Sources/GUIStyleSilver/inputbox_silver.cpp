/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include "API/Display/font.h"
#include "API/Display/display.h"
#include "API/Core/System/system.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/GUI/gui_manager.h"
#include "API/GUIStyleSilver/inputbox_silver.h"
#include "API/GUIStyleSilver/stylemanager_silver.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_InputBox_Silver::CL_InputBox_Silver(
	CL_InputBox *inputbox,
	CL_StyleManager_Silver *style)
: 
	CL_ComponentStyle(inputbox),
	inputbox(inputbox),
	style(style)
{
	resources = style->get_resources();
	
	font = new CL_Font("InputBox/font", resources);
	font_disabled = new CL_Font("InputBox/font_disabled", resources);

	show_cursor = false;
	character_offset = 0;
	cursor_blink_time = CL_System::get_time();

	set_border_size(3);

	slot_paint = inputbox->sig_paint().connect(
		this, &CL_InputBox_Silver::on_paint);
	slot_activity = inputbox->sig_activity().connect(
		this, &CL_InputBox_Silver::on_activity);
	slot_get_preferred_size = inputbox->sig_get_preferred_size().connect(
		this, &CL_InputBox_Silver::on_get_preferred_size);
}

CL_InputBox_Silver::~CL_InputBox_Silver()
{
	delete font;
	delete font_disabled;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_InputBox_Silver::set_border_size(int size)
{
	border_size = size;

	// Override height for font-height
	CL_Rect position = inputbox->get_position();
	position.bottom = position.top + font->get_height() + border_size * 2;
	inputbox->set_position(position);
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_InputBox_Silver::on_paint()
{
	bool has_focus = inputbox->is_enabled() && inputbox->has_focus();

	CL_Rectf rect = inputbox->get_screen_rect();
	int width = inputbox->get_width();

	// Outline
	if(has_focus)
	{
		// Fill
		CL_Display::fill_rect(
			CL_Rectf(rect.left + 1, rect.top + 1, rect.right - 1, rect.bottom - 1),
			CL_Color(245, 250, 255));

		// Main border
		CL_Display::draw_rect(
			rect,
			CL_Color(90, 118, 149));

		// Highlight
		CL_Display::draw_rect(
			CL_Rectf(rect.left - 1, rect.top - 1, rect.right + 1, rect.bottom + 1),
			CL_Color(206, 220, 233));
	}
	else
	{
		// Fill
		CL_Display::fill_rect(
			CL_Rectf(rect.left + 1, rect.top + 1, rect.right - 1, rect.bottom - 1),
			CL_Color::white);

		// Main border
		CL_Display::draw_rect(
			rect,
			CL_Color(128, 142, 159));
	}

	// For easy reference, put inputfield-text into local variable
	const char *text = inputbox->get_text().c_str();

	// Calculate offset for vertical centering
	int vert_center = (inputbox->get_height () - font->get_height ()) / 2;

	// Calculate scroll offset
	int character_offset = 0;
	int pixel_offset = 0;
	int text_width = (int) inputbox->in_password_mode() ? font->get_width('*')*strlen(text) : font->get_width(text);
	if (text_width > width - border_size * 2)
	{
		while (
			text_width >= width - border_size * 2 &&
			character_offset + 1 < inputbox->get_cursor_position())
		{
			int w = inputbox->in_password_mode() ? font->get_width('*') : font->get_width(text[character_offset]);
			character_offset++;
			pixel_offset += w;
			text_width -= w;
		}
	}

	// Display marked text, if any
	if(has_focus) 
	{
		if (inputbox->has_marked_text())
		{
			int start = inputbox->get_selection_start();
			int end = start + inputbox->get_selection_length();

			int mark_x1 = 0, mark_x2 = 0;

			int i;
			if (inputbox->in_password_mode())
			{
				int w = font->get_width('*');

				if (start > 0)
					mark_x1 += start*w;

				if (end > start)
					mark_x2 += (end-start)*w;
			}
			else
			{
				for(i = 0; i < start; i++)
					mark_x1 += font->get_width(text[i]);

				for(i = start; i < end; i++)
					mark_x2 += font->get_width(text[i]);
			}

			CL_Display::fill_rect(
				CL_Rect(
					inputbox->get_screen_x()+border_size + mark_x1 - 1,
					inputbox->get_screen_y() + vert_center,
					inputbox->get_screen_x()+border_size + mark_x1 + mark_x2,
					inputbox->get_screen_y() + font->get_height() + vert_center),
				CL_Color(204, 208, 232));
		}
	}

	// Display text
	if(inputbox->is_enabled() == false)
	{
		if (inputbox->in_password_mode())
			font_disabled->draw(inputbox->get_screen_x()+border_size, inputbox->get_screen_y()+vert_center, std::string(strlen(text+character_offset), '*'));
		else
			font_disabled->draw(inputbox->get_screen_x()+border_size, inputbox->get_screen_y()+vert_center, &text[character_offset]);
	}
	else
	{
		if (inputbox->in_password_mode())
			font->draw(inputbox->get_screen_x()+border_size, inputbox->get_screen_y()+vert_center, std::string(strlen(text+character_offset), '*'));
		else
			font->draw(inputbox->get_screen_x()+border_size, inputbox->get_screen_y()+vert_center, &text[character_offset]);
	}

	// Show blinking cursor
	if(has_focus) 
	{
		if (show_cursor)
		{
			int cursor_x = border_size - pixel_offset;

			if (inputbox->in_password_mode())
				cursor_x += inputbox->get_cursor_position() * font->get_width('*');
			else
				for(int i = 0; i < inputbox->get_cursor_position(); i++)
					cursor_x += font->get_width(text[i]);
			
			CL_Display::draw_line(
				inputbox->get_screen_x() + cursor_x,
				inputbox->get_screen_y() + vert_center,
				inputbox->get_screen_x() + cursor_x,
				inputbox->get_screen_y() + vert_center + font->get_height() - 1,
				CL_Color::black);
		}

		unsigned int cur_time = CL_System::get_time();
		if (cur_time >= cursor_blink_time)
		{
			cursor_blink_time = cur_time + 400;
			show_cursor = !show_cursor;
		}
	}
}

void CL_InputBox_Silver::on_activity()
{
	cursor_blink_time = CL_System::get_time() + 400;
	show_cursor = true;
}

void CL_InputBox_Silver::on_get_preferred_size(CL_Size &size)
{
	border_size = 3;
	
	// Override height for font-height
	CL_Rect position = inputbox->get_position();
	position.bottom = position.top + font->get_height() + border_size * 2;

	size.width = position.get_width();
	size.height = position.get_height();
}
