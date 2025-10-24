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

#ifndef header_inputbox_generic
#define header_inputbox_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/GUI/inputbox.h"
#include <string>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

// --------------------------------------------------------------------------------- CL_InputUndo
struct OneUndo
{
	std::string str;
	int cursor_pos;
	enum UNDO_TYPE
	{
		UNDO_TEXT_ADDED,
		UNDO_TEXT_REMOVED,
	} undo_type;
	bool redo;

	OneUndo( UNDO_TYPE undo_type, std::string const & str, int cursor_old_pos )
		: undo_type(undo_type), str(str), cursor_pos(cursor_old_pos)
	{}
};

class CL_InputUndo
{
public:
	CL_InputUndo()
		: cur_pos(0)
	{}
	~CL_InputUndo()
	{}

	bool canUndo() { return cur_pos > 0; }
	bool canRedo() { return cur_pos < (int)undo_array.size(); }

	void addString( std::string const & str, int cursor_pos );
	void removeString( std::string const & str, int cursor_pos );

	int doUndo( std::string & text );
	int doRedo( std::string & text );

private:
	void addUndo( std::string const & str, int cursor_pos, OneUndo::UNDO_TYPE undo_type );

private:
	std::vector<OneUndo> undo_array;
	int cur_pos;
};


// -----------------------------------------------------------------------------------
class CL_InputBox_Generic
{
public:
	CL_InputBox_Generic(
		CL_InputBox *self,
		const std::string &text,
		bool password_mode,
		bool readonly,
		int maxlength);

	CL_InputBox *inputbox;

	bool password_mode;
	bool read_only;

	bool edited;

	int cursor_position;

	int max_length;

	std::string text;

	CL_Signal_v1<const std::string &> sig_changed;
	CL_Signal_v0 sig_return_pressed;
	CL_Signal_v0 sig_activity;
	CL_Signal_v2<char &, bool &> sig_validate_character;

	CL_Slot slot_set_options;
	CL_Slot slot_lost_focus;

	void on_lost_focus();
	void on_set_options(const CL_DomElement &options);
	void on_input_down(const CL_InputEvent &key);
	void on_input_up(const CL_InputEvent &key);
	//void on_mouse_move(const CL_InputEvent &key);

	void on_mouse_Ldown( int mouse_pos );
	void on_mouse_Lup( int mouse_pos );
	void on_mouse_move( int mouse_pos );

	void set_text(const std::string &text);
	void insert_text(const std::string &text);
	std::string get_marked_text() const;
	void set_max_length(int length);
	void select_all();
	void deselect();
	void set_selection(int start, int length);
	void set_cursor_position(int pos);
	void backspace();
	void del();
	void cut();
	void move_cursor(int delta, bool mark);
	void move_cursor_word(int delta, bool mark);
	void home(bool mark);
	void end(bool mark);
	int get_selection_start() const;
	int get_selection_length() const;

	void do_undo();
	void do_redo();

private:
	bool selecting;
	bool mouse_selecting;
	int selection_start;
	int selection_end;

	bool ctrl_down;
	bool shift_down;
	bool command_down;

	static std::string break_characters;

private:
	void check_selection();
	bool check_control(const CL_InputEvent &key);

	int find_next_break_character(int search_start) const;
	int find_previous_break_character(int search_start) const;

	// clipboard
	void clipboard_copy();
	void clipboard_paste();
	void clipboard_cut();

	// undo manager
	CL_InputUndo input_undo;
};

#endif
