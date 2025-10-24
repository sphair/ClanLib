/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Harry Storbacka
*/

/// \addtogroup clanGUI_Components clanGUI Components
/// \{


#pragma once


#include "../api_gui.h"
#include "../gui_component.h"
#include "../../Display/Window/input_event.h"
#include "../../Core/Signals/callback_v0.h"
#include "../../Core/Signals/callback_v1.h"
#include "../../Core/Signals/callback_1.h"

class CL_LineEdit_Impl;

/// \brief Single line edit component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_LineEdit : public CL_GUIComponent
{
/// \name Construction
/// \{

public:
	CL_LineEdit(CL_GUIComponent *parent);

	virtual ~CL_LineEdit();


/// \}
/// \name Attributes
/// \{

public:
	enum Alignment
	{
		align_left,
		align_center,
		align_right
	};

	Alignment get_alignment() const;

	bool is_read_only() const;

	bool is_lowercase() const;

	bool is_uppercase() const;

	bool is_password_mode() const;

	int get_max_length() const;

	CL_StringRef get_text() const;

	/// \brief Returns the text converted to integer.
	int get_text_int() const;

	/// \brief Returns the text converted to float.
	float get_text_float() const;

	CL_String get_selection() const;

	int get_selection_start() const;

	int get_selection_length() const;

	int get_cursor_pos() const;

	virtual CL_Size get_preferred_size() const;

	/// \brief Returns the size (pixels) of the text in the lineedit, or of the string given as parameter.
	CL_Size get_text_size() const;
	CL_Size get_text_size(const CL_String &str) const;


/// \}
/// \name Operations
/// \{

public:
	void select_all();

	void set_alignment(Alignment alignment);

	void set_read_only(bool enable = true);

	void set_lowercase(bool enable = true);

	void set_uppercase(bool enable = true);

	void set_password_mode(bool enable = true);

	void set_numeric_mode(bool enable = true, bool decimals = false);

	void set_max_length(int length);

	void set_text(const CL_StringRef &text);

	void set_text(int number);

	void set_text(float number);

	void set_selection(int pos, int length);

	void clear_selection();

	void set_cursor_pos(int pos);

	void delete_selected_text();

	/// \brief Resize the components width so that its whole text becomes visible.
	void resize_to_fit(int max_width);

	/// \brief Use mask to restrict accepted input.
	void set_input_mask(const CL_StringRef &mask);

	void set_decimal_character(const CL_StringRef &decimal_char);

/// \}
/// \name Events
/// \{

public:
	CL_Callback_v1<CL_InputEvent> &func_before_edit_changed();

	CL_Callback_v1<CL_InputEvent> &func_after_edit_changed();

	CL_Callback_v0 &func_selection_changed();

	/// \brief Callback invoked after the lineedit gained focus.
	CL_Callback_v0 &func_focus_gained();

	/// \brief Callback invoked when the lineedit is about to lose focus.
	CL_Callback_v0 &func_focus_lost();

	CL_Callback_v0 &func_enter_pressed();

	/// \brief Callback invoked when a event not processed by lineedit occurs.
	/** This includes keys such as Enter, Escape, UP and DOWN.
      * The callback should return true if the event was processsed, else false. */
	CL_Callback_1<bool, CL_InputEvent> &func_unhandled_event();


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_LineEdit_Impl> impl;
/// \}
};


/// \}
