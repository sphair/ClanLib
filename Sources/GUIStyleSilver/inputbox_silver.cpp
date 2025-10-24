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
#include "API/Display/keys.h"
#include "API/Core/System/system.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/inputbox_stylebase.h"
#include "API/GUIStyleSilver/inputbox_silver.h"
#include "API/GUIStyleSilver/stylemanager_silver.h"
#include <cstring>

#pragma warning ( push, 1 )
#pragma warning ( disable: 4244 ) // conversion from 'int' to 'float', possible loss of data


/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_InputBox_Silver::CL_InputBox_Silver(
	CL_InputBox *inputbox,
	CL_StyleManager_Silver *style)
: 
	CL_ComponentStyle(inputbox),
	CL_InputBox_StyleBase(inputbox, style->get_resources(), "InputBox/font", 3)
{
	font_disabled = new CL_Font("InputBox/font_disabled", style->get_resources());
	if (font_disabled)
		font_disabled->set_color( CL_Color::gray );

	slot_paint = inputbox->sig_paint().connect(
		this, &CL_InputBox_Silver::on_paint);
	slot_activity = inputbox->sig_activity().connect(
		this, &CL_InputBox_Silver::on_activity);
	slot_get_preferred_size = inputbox->sig_get_preferred_size().connect(
		this, &CL_InputBox_Silver::on_get_preferred_size);
}

CL_InputBox_Silver::~CL_InputBox_Silver()
{
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
	std::string const & text = inputbox->get_text();

	// Calculate offset for vertical centering
	int vert_center = (inputbox->get_height () - font->get_height ()) / 2;

	// Calculate scroll offset
	int drawing_text_width = 0;
	int draw_to = draw_from;

	while (draw_to < text.length())
	{
		int w = get_chars_width( draw_to, draw_to );
		if (drawing_text_width + w > width - border_size * 2)
			break;

		drawing_text_width += w;
		draw_to ++;
	}

	// Display marked text, if any
	if(has_focus) 
	{
		if (inputbox->has_marked_text())
		{
			int start = inputbox->get_selection_start();
			int end = start + inputbox->get_selection_length();

			if (start < draw_from) start = draw_from;
			if (end   > draw_to  ) end   = draw_to;

			int mark_x1 = get_chars_width( draw_from, start-1 );
			int mark_x2 = get_chars_width( draw_from, end-1 );

			CL_Display::fill_rect(
				CL_Rect(
					inputbox->get_screen_x()+border_size + mark_x1 - 1,
					inputbox->get_screen_y() + vert_center,
					inputbox->get_screen_x()+border_size + mark_x2,
					inputbox->get_screen_y() + font->get_height() + vert_center),
				CL_Color(204, 208, 232));
		}
	}

	// Display text
	int xx = inputbox->get_screen_x()+border_size;
	int yy = inputbox->get_screen_y()+vert_center;

	int draw_count = draw_to - draw_from;

	if (draw_from >= 0 && draw_count > 0)
	{
		if(inputbox->is_enabled() == false)
		{
			if (inputbox->in_password_mode())
				font_disabled->draw(xx, yy, std::string( draw_count, '*' ) );
			else
				font_disabled->draw(xx, yy, text.substr( draw_from, draw_count ) );
		}
		else
		{
			if (inputbox->in_password_mode())
				font->draw(xx, yy, std::string( draw_count, '*' ) );
			else
				font->draw(xx, yy, text.substr( draw_from, draw_count ) );
		}
	}

	// Show blinking cursor
	if(has_focus) 
	{
		if (show_cursor)
		{
			int cursor_pos = inputbox->get_cursor_position();
			int cursor_x = border_size + get_chars_width( draw_from, cursor_pos - 1 );

			int xx = inputbox->get_screen_x() + cursor_x;
			int y1 = inputbox->get_screen_y() + vert_center;
			int y2 = inputbox->get_screen_y() + vert_center + font->get_height() - 1;
			CL_Display::draw_line(
				xx-1, y1, xx-1, y2,
				CL_Color::black );
			CL_Display::draw_line(
				xx-2, y1-1, xx+1, y1-1,
				CL_Color::black );
			CL_Display::draw_line(
				xx-2, y2, xx+1, y2,
				CL_Color::black );

			//char buf[400];
			//sprintf( buf, "%d", cursor_pos );
			//font->draw( CL_Rect( xx, y1-15, xx+100, y1 ), buf );
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

#pragma warning ( pop )

