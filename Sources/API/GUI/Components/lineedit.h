/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

	/// \brief Constructs a LineEdit
	///
	/// \param parent = GUIComponent
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

	using CL_GUIComponent::get_named_item;

	/// \brief Find the child CL_LineEdit with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static CL_LineEdit *get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id);

	/// \brief Get Alignment
	///
	/// \return alignment
	Alignment get_alignment() const;

	/// \brief Is Read only
	///
	/// \return true = read_only
	bool is_read_only() const;

	/// \brief Is Lowercase
	///
	/// \return true = lowercase
	bool is_lowercase() const;

	/// \brief Is Uppercase
	///
	/// \return true = uppercase
	bool is_uppercase() const;

	/// \brief Is Password mode
	///
	/// \return true = password_mode
	bool is_password_mode() const;

	/// \brief Get Max length
	///
	/// \return max_length
	int get_max_length() const;

	/// \brief Get Text
	///
	/// \return text
	CL_StringRef get_text() const;

	/// \brief Returns the text converted to integer.
	int get_text_int() const;

	/// \brief Returns the text converted to float.
	float get_text_float() const;

	/// \brief Get Selection
	///
	/// \return selection
	CL_String get_selection() const;

	/// \brief Get Selection start
	///
	/// \return selection_start
	int get_selection_start() const;

	/// \brief Get Selection length
	///
	/// \return selection_length
	int get_selection_length() const;

	/// \brief Get Cursor pos
	///
	/// \return cursor_pos
	int get_cursor_pos() const;

	/// \brief Get Preferred size
	///
	/// \return preferred_size
	virtual CL_Size get_preferred_size() const;

	/// \brief Returns the size (pixels) of the text in the lineedit, or of the string given as parameter.
	CL_Size get_text_size();

	/// \brief Get text size
	///
	/// \param str = String
	///
	/// \return Size
	CL_Size get_text_size(const CL_String &str);

/// \}
/// \name Operations
/// \{

public:
	void set_select_all_on_focus_gain(bool enable);

	/// \brief Select all
	void select_all();

	/// \brief Set alignment
	///
	/// \param alignment = Alignment
	void set_alignment(Alignment alignment);

	/// \brief Set read only
	///
	/// \param enable = bool
	void set_read_only(bool enable = true);

	/// \brief Set lowercase
	///
	/// \param enable = bool
	void set_lowercase(bool enable = true);

	/// \brief Set uppercase
	///
	/// \param enable = bool
	void set_uppercase(bool enable = true);

	/// \brief Set password mode
	///
	/// \param enable = bool
	void set_password_mode(bool enable = true);

	/// \brief Set numeric mode
	///
	/// \param enable = bool
	/// \param decimals = bool
	void set_numeric_mode(bool enable = true, bool decimals = false);

	/// \brief Set max length
	///
	/// \param length = value
	void set_max_length(int length);

	/// \brief Set text
	///
	/// \param text = String Ref
	void set_text(const CL_StringRef &text);

	/// \brief Set text
	///
	/// \param number = value
	void set_text(int number);

	/// \brief Set text
	///
	/// \param number = value
	/// \param num_decimal_places = number of decimal places
	void set_text(float number, int num_decimal_places = 6);

	/// \brief Set selection
	///
	/// \param pos = value
	/// \param length = value
	void set_selection(int pos, int length);

	/// \brief Clear selection
	void clear_selection();

	/// \brief Set cursor pos
	///
	/// \param pos = value
	void set_cursor_pos(int pos);

	/// \brief Delete selected text
	void delete_selected_text();

	/// \brief Resize the components width so that its whole text becomes visible.
	void resize_to_fit(int max_width);

	/// \brief Use mask to restrict accepted input.
	void set_input_mask(const CL_StringRef &mask);

	/// \brief Set decimal character
	///
	/// \param decimal_char = String Ref
	void set_decimal_character(const CL_StringRef &decimal_char);

	/// \brief Set cursor drawing enabled/disabled.
	void set_cursor_drawing_enabled(bool enable);

/// \}
/// \name Events
/// \{

public:
	CL_Callback_v1<CL_InputEvent &> &func_before_edit_changed();

	CL_Callback_v1<CL_InputEvent &> &func_after_edit_changed();

	/// \brief Func selection changed
	///
	/// \return Callback_v0
	CL_Callback_v0 &func_selection_changed();

	/// \brief Callback invoked after the lineedit gained focus.
	CL_Callback_v0 &func_focus_gained();

	/// \brief Callback invoked when the lineedit is about to lose focus.
	CL_Callback_v0 &func_focus_lost();

	/// \brief Func enter pressed
	///
	/// \return Callback_v0
	CL_Callback_v0 &func_enter_pressed();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_LineEdit_Impl> impl;
/// \}
};

/// \}
