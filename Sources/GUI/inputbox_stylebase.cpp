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
**    AlexeyKharkov
**    (if your name is missing here, please add it)
*/

#include "precomp.h"
#include "API/Display/font.h"
#include "API/Display/display.h"
#include "API/Display/keys.h"
#include "API/Core/System/system.h"
#include "API/GUI/inputbox_stylebase.h"


// Constructor
CL_InputBox_StyleBase::CL_InputBox_StyleBase(CL_InputBox *inputbox, CL_ResourceManager *resources, std::string const & font_name, int border_size)
: 
	inputbox(inputbox),
	border_size(border_size),
	show_cursor(false),
	draw_from(0)
{
	font = new CL_Font(font_name, resources);
	if (font)
		font->set_color( CL_Color::black );

	cursor_blink_time = CL_System::get_time();

	slot_input_down = inputbox->sig_input_down().connect(
		this, &CL_InputBox_StyleBase::on_input_down);
	slot_input_up = inputbox->sig_input_up().connect(
		this, &CL_InputBox_StyleBase::on_input_up);
	slot_mouse_move = inputbox->sig_mouse_move().connect(
		this, &CL_InputBox_StyleBase::on_mouse_move);
}

// Destructor
CL_InputBox_StyleBase::~CL_InputBox_StyleBase()
{
	delete font;
}

// -------------------------------------------------------------------------------------
void CL_InputBox_StyleBase::on_input_down(const CL_InputEvent &key)
{
	// Start selecting with mouse?
	if(key.id == CL_MOUSE_LEFT)
	{
		inputbox->on_mouse_Ldown( get_mouse_position(key.mouse_pos.x, key.mouse_pos.y) );
		return;
	}

	int old_cur_pos = inputbox->get_cursor_position();
	inputbox->on_input_down(key);
	int new_cur_pos = inputbox->get_cursor_position();

	if (new_cur_pos != old_cur_pos)
	{
		//I redid this part as scrolling around an edit box via cursor keys was behaving strangely, and the full size
		//of edit box was not usable. -SAR/mrfun 1-29-2008

		std::string const & text = inputbox->get_text();
		
		
		int draw_start_index = draw_from; //default view
		int draw_finish_index = text.size()-1; //the default

		int text_width = 0;
		int input_area_width = inputbox->get_width()- ( border_size*2);
		int scrolled_from_left_x = 0;
		int cur_pos_start_offsetx = 0;

		//calculate what is going to fit in the input box

		int cur_letter_width;

		int kerning = font->get_width_offset();

		if (draw_start_index != 0 && input_area_width > get_chars_width(0, text.size()))
		{
			//maybe some text was deleted with cut or del.
			draw_start_index = 0;
		}
		
		//make sure our view isn't pass the user cursor
		if (draw_start_index > inputbox->get_cursor_position()) draw_start_index = inputbox->get_cursor_position();

		for (unsigned int i=draw_start_index; i < text.size(); i++)
		{
			cur_letter_width = get_chars_width(i,i) + kerning;

			if (int(i) < inputbox->get_cursor_position())
			{
				cur_pos_start_offsetx += cur_letter_width; 
			}
			text_width += cur_letter_width;

			while (cur_pos_start_offsetx-scrolled_from_left_x >= input_area_width)
			{
				//we need to start scrolling to the right now, 
				scrolled_from_left_x += get_chars_width(draw_start_index,draw_start_index)+kerning;
				text_width -= get_chars_width(draw_start_index, draw_start_index)+kerning;
				draw_start_index++;
			}

			if  (text_width >= input_area_width)
			{
				//we've gone too far.
				text_width -= cur_letter_width;
				draw_finish_index = i-1; //don't include this character
				break;
			}

			draw_from = draw_start_index;
		}
	}
}

void CL_InputBox_StyleBase::on_input_up(const CL_InputEvent &key)
{
	// End mouse-selecting
	if(key.id == CL_MOUSE_LEFT)
	{
		inputbox->on_mouse_Lup( get_mouse_position(key.mouse_pos.x, key.mouse_pos.y) );
		return;
	}
	inputbox->on_input_up(key);
}

void CL_InputBox_StyleBase::on_mouse_move(const CL_InputEvent &key)
{
	inputbox->on_mouse_move( get_mouse_position(key.mouse_pos.x, key.mouse_pos.y) );
}


int CL_InputBox_StyleBase::get_mouse_position(int x, int y)
{
	int delta_x = x;
	unsigned int i;
	std::string const & text = inputbox->get_text();

	for (i = draw_from; i < text.length(); i++)
	{
		int w = font->get_width( (unsigned char)text[i] );;
		delta_x -= w;
		if (delta_x <= -w/2)
		{
			return i;// + last_offset;
		}
	}
	return i;
}

int CL_InputBox_StyleBase::get_chars_width( int from, int to ) const
{
	std::string const & text = inputbox->get_text();

	if ( from < 0 ) from = 0;
	if ( to >= (int)text.length() ) to = text.length() - 1;
	if ( to < from ) return 0;

	if (inputbox->in_password_mode())
		return (to - from + 1) * font->get_width('*');

	int res = 0;
	for (int i=from; i<=to; ++i)
		res += font->get_width( (unsigned char)text[i] );

	return res;
}

