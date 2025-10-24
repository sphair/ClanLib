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

#include "precomp.h"
#include "inputbox_generic.h"
#include "API/Core/XML/dom_element.h"
#include "API/Display/keys.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/System/error.h"

CL_InputBox_Generic::CL_InputBox_Generic(
	CL_InputBox *self, 
	const std::string &text,
	bool password_mode,
	bool readonly,
	int maxlength)
: inputbox(self), password_mode(password_mode), read_only(readonly),
  max_length(maxlength), text(text)
{
	ctrl_down = false;
	shift_down = false;

	selecting = false;
	mouse_selecting = false;
	selection_start = 0;
	selection_end = 0;

	cursor_position = 0;

	edited = false;

	slot_set_options = inputbox->sig_set_options().connect(
		this, &CL_InputBox_Generic::on_set_options);
	slot_input_down = inputbox->sig_input_down().connect(
		this, &CL_InputBox_Generic::on_input_down);
	slot_input_up = inputbox->sig_input_up().connect(
		this, &CL_InputBox_Generic::on_input_up);
	slot_mouse_move = inputbox->sig_mouse_move().connect(
		this, &CL_InputBox_Generic::on_mouse_move);
}

void CL_InputBox_Generic::on_set_options(const CL_DomElement &options)
{
	if (options.has_attribute("text"))
		set_text(options.get_attribute("text"));
	if (options.has_attribute("passwordmode"))
		password_mode = CL_String::to_bool(options.get_attribute("passwordmode"));
	if (options.has_attribute("read_only"))
		read_only = CL_String::to_bool(options.get_attribute("read_only"));
	if (options.has_attribute("max_length"))
		set_max_length(CL_String::to_int(options.get_attribute("max_length")));
}

void CL_InputBox_Generic::on_input_down(const CL_InputEvent &key)
{
	// Start selecting with mouse?
	if(key.id == CL_MOUSE_LEFT)
	{
		cursor_position = text.size();
		selecting = false;
		selection_start = -1;
		selection_end = -1;
//		cursor_position = get_mouse_position(key.x, key.y);
/*		mouse_selecting = true;
		selecting = true;
		selection_start = cursor_position;
		inputbox->capture_mouse();
*/		return;
	}

	// Keyboard?
	if(key.device.get_type() != CL_InputDevice::keyboard)
		return;

	switch (key.id)
	{
	case CL_KEY_DELETE:
		if (!read_only)del();
		break;
	case CL_KEY_BACKSPACE:
		if (!read_only)backspace();
		break;
	case CL_KEY_END:
		check_selection();	
		end(selecting);
		break;
	case CL_KEY_HOME:
		check_selection();	
		home(selecting);
		break;
	case CL_KEY_LEFT:
		check_selection();	
//		if (ctrl_down)
//			word_left();
//		else
			move_cursor(-1, selecting);
		break;
	case CL_KEY_RIGHT:
		check_selection();	
//		if (ctrl_down)
//			word_right();
//		else
			move_cursor(1, selecting);
		break;
	case CL_KEY_RETURN:
		if (!read_only)sig_return_pressed();
//		inputbox->clear_focus();
		break;
	case CL_KEY_CONTROL:
		ctrl_down = true;
		break;
	case CL_KEY_SHIFT:
		shift_down = true;
		break;
	default:
		if (!read_only)update_text(key);
		break;
	}
}

void CL_InputBox_Generic::on_input_up(const CL_InputEvent &key)
{
	// End mouse-selecting
	if(key.id == CL_MOUSE_LEFT)
	{
/*		cursor_position = get_mouse_position(key.x, key.y);
		if (mouse_selecting && cursor_position == selection_start)
			selecting = false;
		mouse_selecting = false;
		inputbox->release_mouse();
*/		return;
	}

	switch (key.id)
	{
	case CL_KEY_CONTROL:
		ctrl_down = false;
		break;
	case CL_KEY_SHIFT:
		shift_down = false;
		break;
	}
}

void CL_InputBox_Generic::on_mouse_move(const CL_InputEvent &key)
{
	if (mouse_selecting)
		set_cursor_position(get_mouse_position(key.mouse_pos.x, key.mouse_pos.y));
}

void CL_InputBox_Generic::check_selection()
{
	if(shift_down)
	{
		if(selecting == false)
		{
			selecting = true;
			selection_start = cursor_position;
		}
	}
	else
	{
		selecting = false;
		selection_start = -1;
		selection_end = -1;
	}
}

void CL_InputBox_Generic::set_text(const std::string &new_text)
{
	// TODO: Check against max_length

	text = new_text;
	selecting = false;
	selection_start = -1;
	selection_end = -1;
	cursor_position = text.size();
}

std::string CL_InputBox_Generic::get_marked_text() const
{
	// Added by E.R. Ylvisaker on 11/19/05
	if (!selecting)
	{
		return "";
	}
	else
	{
		return text.substr(selection_start, selection_end - selection_start);
	}

	//throw CL_Error("CL_InputBox_Generic::get_marked_text() is not implemented");
}

void CL_InputBox_Generic::set_max_length(int length)
{
	max_length = length;
	int text_size = text.size();

	if (text_size > max_length && max_length != 0)
		text.erase(max_length, text.size());
}

void CL_InputBox_Generic::select_all()
{
	// Added by E.R. Ylvisaker on 11/19/05
	if (text.size() > 0)
	{
		selecting = true;
		selection_start = 0;
		selection_end = text.size();
	}
	
	//throw CL_Error("CL_InputBox_Generic::select_all() is not implemented");
}

void CL_InputBox_Generic::deselect()
{
	// Added by E.R. Ylvisaker on 11/19/05
	selecting = false;
	selection_start = -1;
	selection_end = -1;

//	throw CL_Error("CL_InputBox_Generic::deselect() is not implemented");
}

void CL_InputBox_Generic::set_selection(int start, int length)
{
	// Added by E.R. Ylvisaker on 11/19/05
	if (start < 0) start = 0;
	if (start + length > (signed) text.size()) length = text.size() - start;

	if (length > 0)
	{
		selecting = true;
		selection_start = start;
		selection_end = start + length;
	}
	else if (length < 0)
	{
		selecting = true;
		selection_start = start - length;
		selection_end = start;

		if (selection_start < 0)
			selection_start = 0;

	}
	else
	{
		selecting = false;
	}

	//throw CL_Error("CL_InputBox_Generic::set_selection() is not implemented");
}

void CL_InputBox_Generic::set_cursor_position(int pos)
{
	cursor_position = pos;
	if(cursor_position < 0)
		cursor_position = 0;
	if(cursor_position > max_length - 1 && max_length != 0)
		cursor_position = max_length - 1;
	if(cursor_position > (signed) text.size())
		cursor_position = text.size();
}

void CL_InputBox_Generic::del()
{
	if (selecting)
		cut();
	else if (cursor_position < (int)text.size())
	{
		text.erase(cursor_position, 1);
		sig_changed(text);
	}

	sig_activity();
}

void CL_InputBox_Generic::backspace()
{
	if (selecting)
		cut();
	else if (cursor_position > 0)
	{
		text.erase(cursor_position - 1, 1);
		cursor_position--;
		sig_changed(text);
	}

	sig_activity();
}

void CL_InputBox_Generic::home(bool mark)
{
	if(mark)
	{
		selecting = true;
		selection_start = cursor_position;
		selection_end = 0;
	}

	cursor_position = 0;

	sig_activity();
}

void CL_InputBox_Generic::end(bool mark)
{
	if(mark)
	{
		selecting = true;
		selection_start = cursor_position;
		selection_end = text.size();
	}

	cursor_position = text.size();

	sig_activity();
}

void CL_InputBox_Generic::move_cursor(int delta, bool mark)
{
	cursor_position += delta;

	// Check bounds
	if (cursor_position < 0) cursor_position = 0;
	if (cursor_position > (int)text.size()) cursor_position = (int)text.size();

	if(mark)
		selection_end = cursor_position;

	sig_activity();
}

void CL_InputBox_Generic::move_cursor_word(int delta, bool mark)
{
	throw CL_Error("CL_InputBox_Generic::move_cursor_word() is not implemented");
/*
		if (ctrl_down)
		{
			while (caret_position > 0 && text[caret_position] == ' ') caret_position--;
			while (caret_position > 0 && text[caret_position] != ' ') caret_position--;
		}
		if (ctrl_down)
		{
			while (caret_position < (int)text.size() && text[caret_position] == ' ') caret_position++;
			while (caret_position < (int)text.size() && text[caret_position] != ' ') caret_position++;
		}
		sig_activity();
*/
}

void CL_InputBox_Generic::cut()
{
	if(selecting) 
	{
		int start = get_selection_start();
		int end = get_selection_length();
		text.erase(start, end);
		cursor_position = start;

		selecting = false;
		selection_start = selection_end = -1;

		if (cursor_position > (int)text.size())
			cursor_position = (int)text.size();

		sig_changed(text);
	}
}

int CL_InputBox_Generic::get_selection_start()
{
	if(selection_start < selection_end)
		return selection_start;
	else
		return selection_end;
}

int CL_InputBox_Generic::get_selection_length()
{
	return abs(selection_start - selection_end);
}

void CL_InputBox_Generic::update_text(CL_InputEvent key)
{
	if (!key.str.empty())
	{
		bool changed = false;
		for(unsigned int i=0; i<key.str.length(); ++i)
		{
			if (key.str[i] > 31)
			{
				char character = key.str[i];
				bool accept = true;
				sig_validate_character(character, accept);
				if (accept)
				{
					if(changed == false)
					{
						cut();
						changed = true;
					}
				
					text.insert(text.begin() + cursor_position, character);
					cursor_position++;
				}
			}
		}
		
		if(changed)
		{
			sig_changed(text);
			sig_activity();
		}
	}
}

int CL_InputBox_Generic::get_mouse_position(int x, int y)
{
/*	int delta_x = x - X_TEXTOFFSET;
	unsigned int i;
	for (i = 0; i < text.size(); i++)
	{
		char buf[2];
		buf[0] = text[i];
		buf[1] = 0;
		int w = fnt_inputbox->get_text_width(buf);
		delta_x -= w;
		if (delta_x <= -w/2)
		{
				return i + last_offset;
		}
	}
	return i;
*/
	return text.size() - 1;
}
