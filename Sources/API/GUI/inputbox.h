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

//! clanGUI="Controls"
//! header=gui.h

#ifndef header_inputbox
#define header_inputbox

#ifdef CL_API_DLL
#ifdef CL_GUI_EXPORT
#define CL_API_GUI __declspec(dllexport)
#else
#define CL_API_GUI __declspec(dllimport)
#endif
#else
#define CL_API_GUI
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "component.h"

class CL_InputBox_Generic;

//: Inputbox component
//- !group=GUI/Controls!
//- !header=gui.h!
class CL_API_GUI CL_InputBox : public CL_Component
{
//! Construction:
public:
	//: InputBox Constructor
	CL_InputBox(
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: InputBox Constructor
	CL_InputBox(
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: InputBox Constructor
	CL_InputBox(
		const CL_Rect &pos,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: InputBox Constructor
	CL_InputBox(
		const CL_Rect &pos,
		const std::string &text,
		CL_Component *parent,
		CL_StyleManager *style = NULL);

	//: InputBox Destructor
	virtual ~CL_InputBox();

	// added by AlexeyKharkov
	void on_input_down(const CL_InputEvent &key);
	void on_input_up(const CL_InputEvent &key);

	void on_mouse_Ldown( int mouse_pos );
	void on_mouse_Lup( int mouse_pos );
	void on_mouse_move( int mouse_pos );

	void on_lost_focus();

//! Attributes:
public:
	//: Returns the inputfield text.
	const std::string &get_text() const;

	//: Returns the length of the inputfield text.
	int get_length() const;

	//: Returns the marked text from the inputfield.
	std::string get_marked_text() const;

	//: Returns true if part of the text has been marked by the user.
	bool has_marked_text() const;

	//: Returns the position where the selection starts.
	int get_selection_start() const;

	//: Returns the length of current selection.
	int get_selection_length() const;

	//: Returns the maximum permitted length of the text.
	//- <p>0 means it has no maximum length requirement.</p>
	int get_max_length() const;

	//: Returns true if password mode is enabled, false othewise.
	bool in_password_mode() const;

	//: Returns whether the inputfield is read-only. 
	bool is_read_only() const;

	//: Returns the current cursor position.
	int get_cursor_position() const;

	//: Returns true if the contents has been edited.
	bool is_edited() const;

//! Operations:
public:
	//: Sets the line edit text to text, clears the selection and moves the cursor to the end of the line.
	//- <p>If necessary the text is truncated to get_max_length().</p>
	void set_text(const std::string &text);

	//: Sets the line edit contents to a plain text containing the printed value of num.
	void set_text(int number);

	//: Sets the line edit contents to a plain text containing the printed value of num.
	void set_text(double number);

	//: Set the maximum length of the text in the editor.
	//- <p>Use 0 to have no maximum length requirement.</p>
	void set_max_length(int length);

	//: If enable is true, display asterisks instead of the characters actually entered.
	void set_password_mode(bool enable = true);

	//: Enables or disables read-only mode.
	void set_read_only(bool enable = true);
	
	//: Selects all text, and moves the cursor to the end.
	void select_all();

	//: Deselects all text (i.e. removes marking) and leaves the cursor at the current position. 
	void deselect();

	//: Sets the marked area at start and be length characters long. 
	void set_selection(int start, int length);

	//: Sets the cursor position to pos. 
	void set_cursor_position(int pos);

	//: Syntactic sugar for set_text("").
	void clear();

	//: Deletes the character to the left of the text cursor and moves the cursor one position to the left.
	//- <p>If a text has been marked, the cursor will be put at the beginning of the marked text
	//- and the marked text will be removed.</p>
	void backspace();

	//: Deletes the character on the right side of the text cursor.
	//- <p>If a text has been marked, the cursor will be put at the beginning of the marked text and the marked
	//- text will be removed.</p>
	void del();

	//: Cut the selected text if any.
	void cut();

	//: Moves the cursor position.
	//- <p>If delta is positive, the cursor is moved forward.
	//- If delta is negative, the cursor is moved backward.
	//- If mark is true, the text is marked.</p>
	void move_cursor(int delta, bool mark = false);

	//: Moves the cursor by word.
	//- <p>If delta is positive, the cursor is moved forward.
	//- If delta is negative, the cursor is moved backward.
	//- If mark is true, the text is marked.</p>
	void move_cursor_word(int delta, bool mark = false);

	//: Moves the text cursor to the left end of the line.
	//- <p>If mark is true, text will be marked towards the first position
	//- If mark is false, any marked text will be unmarked if the cursor is moved.</p>
	void home(bool mark = false);

	//: Moves the text cursor to the right end of the line.
	//- <p>If mark is true, text will be marked towards the last position
	//- If mark is false, any marked text will be unmarked if the cursor is moved. </p>
	void end(bool mark = false);

	//: Sets the edited flag to on. 
	//- <p>The edited flag is changed to true whenever the user changes its contents. </p>
	void set_edited(bool on = true);

//! Signals:
public:
	//: This signal is emitted whenever text is changed in the inputfield.
	//param const std::string &: Contains the new string.
	CL_Signal_v1<const std::string &> &sig_changed();

	//: This signal is emitted when enter is pressed in the inputfield.
	CL_Signal_v0 &sig_return_pressed();

	//: This signal is emitted whenever some character is changed/added/removed, cursor moved etc.
	CL_Signal_v0 &sig_activity();

	//: This signal is emitted after user enter some character, but before it added, to allow user to accept/decline it.
	CL_Signal_v2<char &, bool &> &sig_validate_character();

//! Implementation:
private:
	CL_InputBox(const CL_InputBox &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.
	CL_InputBox_Generic *impl;
};

#endif
