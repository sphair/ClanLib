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
#include "API/Core/System/clipboard.h"

// --------------------------------------------------------------------------------- CL_InputUndo
void CL_InputUndo::addString( std::string const & str, int cursor_pos )
{
	addUndo( str, cursor_pos, OneUndo::UNDO_TEXT_ADDED );
}

void CL_InputUndo::removeString( std::string const & str, int cursor_pos )
{
	addUndo( str, cursor_pos, OneUndo::UNDO_TEXT_REMOVED );
}

void CL_InputUndo::addUndo( std::string const & str, int cursor_pos, OneUndo::UNDO_TYPE undo_type )
{
	if (str.length() > 100000) // no undo for such a big strings
	{
		undo_array.clear();
		cur_pos = 0;
		return;
	}

	if (cur_pos < (int)undo_array.size())
		undo_array.erase( undo_array.begin() + cur_pos, undo_array.end() );

	undo_array.push_back( OneUndo( undo_type, str, cursor_pos ) );
	cur_pos ++;
}

int CL_InputUndo::doUndo( std::string & text )
{
	if (!canUndo()) return -1;

	cur_pos --;
	OneUndo const & u = undo_array[cur_pos];

	switch (u.undo_type)
	{
	case OneUndo::UNDO_TEXT_ADDED:
		text.erase( u.cursor_pos, u.str.length() );
		break;
	case OneUndo::UNDO_TEXT_REMOVED:
		text.insert( u.cursor_pos, u.str );
		break;
	}

	return u.cursor_pos;
}

int CL_InputUndo::doRedo( std::string & text )
{
	if (!canRedo()) return NULL;

	OneUndo const & u = undo_array[cur_pos];

	switch (u.undo_type)
	{
	case OneUndo::UNDO_TEXT_ADDED:
		text.insert( u.cursor_pos, u.str );
		break;
	case OneUndo::UNDO_TEXT_REMOVED:
		text.erase( u.cursor_pos, u.str.length() );
		break;
	}

	cur_pos ++;
	return u.cursor_pos;
}


// --------------------------------------------------------------------------------- 
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
	command_down = false; //mac thing

	selecting = false;
	mouse_selecting = false;
	selection_start = 0;
	selection_end = 0;

	cursor_position = 0;

	edited = false;

	slot_set_options = inputbox->sig_set_options().connect(
		this, &CL_InputBox_Generic::on_set_options);
	slot_lost_focus = inputbox->sig_lost_focus().connect(
		this, &CL_InputBox_Generic::on_lost_focus);
}

void CL_InputBox_Generic::on_lost_focus()
{
	ctrl_down = false;
	shift_down = false;
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
	// Keyboard?
	if(key.device.get_type() != CL_InputDevice::keyboard)
		return;

	if (check_control(key))
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
		if (ctrl_down)
		{
			int steps = find_previous_break_character(cursor_position - 1) - cursor_position;
			move_cursor( steps, selecting );
		}
		else
			move_cursor(-1, selecting);
		break;
	case CL_KEY_RIGHT:
		check_selection();	
		if (ctrl_down)
		{
			int steps = find_next_break_character(cursor_position) - cursor_position;
			move_cursor( steps, selecting );
		}
		else
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
#ifdef __APPLE__
	case CL_KEY_COMMAND:
		command_down = true;
		break;
#endif

	default:
		if (!read_only)
			insert_text(key.str);
		break;
	}
}

bool CL_InputBox_Generic::check_control(const CL_InputEvent &key)
{
	if (shift_down)
	{
		switch (key.id)
		{
		case CL_KEY_INSERT:
			clipboard_paste();
			return true;
		case CL_KEY_DELETE:
			clipboard_cut();
			return true;
		}
	}

	if (ctrl_down || command_down)
	{
		switch (key.id)
		{
		case CL_KEY_C:
		case CL_KEY_INSERT:
			clipboard_copy();
			return true;
		case CL_KEY_V:
			if (!read_only)
				clipboard_paste();
			return true;
		case CL_KEY_X:
			if (!read_only)
				clipboard_cut();
			return true;
		case CL_KEY_A:
			select_all();
			return true;
		case CL_KEY_Z:
			do_undo();
			return true;
		case CL_KEY_Y:
			do_redo();
			return true;
		}
	}

	return false;
}

void CL_InputBox_Generic::on_input_up(const CL_InputEvent &key)
{
	// Keyboard?
	if(key.device.get_type() != CL_InputDevice::keyboard)
		return;

	switch (key.id)
	{
	case CL_KEY_CONTROL:
		ctrl_down = false;
		break;
	case CL_KEY_SHIFT:
		shift_down = false;
		break;
#ifdef __APPLE__
	case CL_KEY_COMMAND:
		command_down = false;
		break;
#endif
	}
}

std::string CL_InputBox_Generic::break_characters = " :;,.-!?'`\"";

int CL_InputBox_Generic::find_next_break_character(int search_start) const
{
	// skip all breaks
	size_t i = search_start;
	for (; i<text.length(); ++i)
	{
		if ( break_characters.find( text[i] ) == -1 )
			break;
	}
	// find the first non-break symbol
	for (; i<text.length(); ++i)
	{
		if ( break_characters.find( text[i] ) != -1 )
			return i;
	}
	return text.length();
}

int CL_InputBox_Generic::find_previous_break_character(int search_start) const
{
	if (search_start <= 1)
		return 0;

	// skip all breaks
	size_t i = search_start;
	for (; i>=0; --i)
	{
		if ( break_characters.find( text[i] ) == -1 )
			break;
	}
	// find the first non-break symbol
	for (; i>=0; --i)
	{
		if ( break_characters.find( text[i] ) != -1 )
			return i + 1;
	}
	return 0;
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
	input_undo.removeString( text, 0 );
	input_undo.addString( new_text, 0 );

	text = new_text;
	selecting = false;
	selection_start = -1;
	selection_end = -1;
	cursor_position = text.length();

	set_max_length( max_length ); // Check against max_length
}

void CL_InputBox_Generic::insert_text( const std::string &str )
{
	if (max_length != 0  &&  (int)text.length() >= max_length  &&  !selecting)
		return;

	if (str.length() == 0)
		return;

	bool changed = false;
	std::string str_to_insert;

	for(unsigned int i=0; i<str.length(); ++i)
	{
		unsigned char ch = str[i];
		if (ch > 31)
		{
			bool accept = true;
			sig_validate_character((char&)ch, accept);
			if (accept)
			{
				if (changed == false)
				{
					cut();
					changed = true;
				}
			
				str_to_insert += ch;
				if (max_length != 0  &&  (int)text.length() + (int)str_to_insert.length() >= max_length)
					break;
			}
		}
	}
	
	if(changed)
	{
		input_undo.addString( str_to_insert, cursor_position );

		text.insert( cursor_position, str_to_insert );
		cursor_position += str_to_insert.length();

		sig_changed(text);
		sig_activity();
	}
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
		return text.substr( get_selection_start(), get_selection_length() );
	}
}

void CL_InputBox_Generic::set_max_length(int length)
{
	if (length < 0)
		return;

	max_length = length;

	if (max_length != 0  &&  (int)text.length() > max_length)
	{
		// do NOT undo in this case!
		text.erase(max_length, text.length());
		if (cursor_position > (int)text.length())
			cursor_position = text.length();
	}
}

void CL_InputBox_Generic::select_all()
{
	// Added by E.R. Ylvisaker on 11/19/05
	if (text.length() > 0)
	{
		selecting = true;
		selection_start = 0;
		selection_end = text.length();
		cursor_position = text.length();
	}
}

void CL_InputBox_Generic::deselect()
{
	// Added by E.R. Ylvisaker on 11/19/05
	selecting = false;
	selection_start = -1;
	selection_end = -1;
}

void CL_InputBox_Generic::set_selection(int start, int length)
{
	// Added by E.R. Ylvisaker on 11/19/05
	if (start < 0) start = 0;
	if (start + length > (int)text.length())
		length = text.length() - start;

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
}

void CL_InputBox_Generic::set_cursor_position(int pos)
{
	cursor_position = pos;
	if(cursor_position < 0)
		cursor_position = 0;
	if(cursor_position > max_length - 1 && max_length != 0)
		cursor_position = max_length - 1;
	if(cursor_position > (int)text.length())
		cursor_position = text.length();
}

void CL_InputBox_Generic::del()
{
	if (selecting)
		cut();
	else if (cursor_position < (int)text.length())
	{
		input_undo.removeString( text.substr( cursor_position, 1 ), cursor_position );

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
		cursor_position--;
		input_undo.removeString( text.substr( cursor_position, 1 ), cursor_position );

		text.erase(cursor_position, 1);
		sig_changed(text);
	}

	sig_activity();
}

void CL_InputBox_Generic::home(bool mark)
{
	if(mark)
	{
		selecting = true;
		//selection_start = cursor_position;
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
		//selection_start = cursor_position;
		selection_end = text.length();
	}

	cursor_position = text.length();

	sig_activity();
}

void CL_InputBox_Generic::move_cursor(int delta, bool mark)
{
	cursor_position += delta;

	// Check bounds
	if (cursor_position < 0) cursor_position = 0;
	if (cursor_position > (int)text.length()) cursor_position = (int)text.length();

	if (mark)
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
	if (selecting)
	{
		int start = get_selection_start();
		int end = get_selection_length();

		input_undo.removeString( text.substr( start, end ), start );

		text.erase(start, end);
		cursor_position = start;

		selecting = false;
		selection_start = selection_end = -1;

		if (cursor_position > (int)text.length())
			cursor_position = (int)text.length();

		sig_changed(text);
	}
}

int CL_InputBox_Generic::get_selection_start() const
{
	return (selection_start < selection_end ? selection_start : selection_end);
}

int CL_InputBox_Generic::get_selection_length() const
{
	return abs(selection_start - selection_end);
}

void CL_InputBox_Generic::on_mouse_Ldown( int mouse_pos )
{
	cursor_position = mouse_pos;
	mouse_selecting = true;
	selecting = true;
	selection_start = mouse_pos;
	selection_end = mouse_pos;
	inputbox->capture_mouse();
}

void CL_InputBox_Generic::on_mouse_Lup( int mouse_pos )
{
	if (mouse_selecting && mouse_pos == selection_start)
		selecting = false;
	mouse_selecting = false;
	inputbox->release_mouse();
}

void CL_InputBox_Generic::on_mouse_move( int mouse_pos )
{
	if (mouse_selecting)
	{
		set_cursor_position( mouse_pos );
		selection_end = mouse_pos;
	}
}

// --------------------------------------------------------------------------- clipboard
void CL_InputBox_Generic::clipboard_copy()
{
	// prepare the text to copy
	std::string str = (get_selection_length() == 0) 
		?	text
		:	get_marked_text();

	CL_Clipboard::set_text(str);
}

void CL_InputBox_Generic::clipboard_paste()
{
	cut();

	std::string s = CL_Clipboard::get_text();
	insert_text(s);
}

void CL_InputBox_Generic::clipboard_cut()
{
	clipboard_copy();
	cut();
}

void CL_InputBox_Generic::do_undo()
{
	if ( input_undo.canUndo() )
		cursor_position = input_undo.doUndo( text );
}

void CL_InputBox_Generic::do_redo()
{
	if ( input_undo.canRedo() )
		cursor_position = input_undo.doRedo( text );
}
