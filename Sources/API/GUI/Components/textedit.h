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

class CL_TextEdit_Impl;

/// \brief Text edit component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_TextEdit : public CL_GUIComponent
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a LineEdit
	///
	/// \param parent = GUIComponent
	CL_TextEdit(CL_GUIComponent *parent);

	virtual ~CL_TextEdit();

/// \}
/// \name Attributes
/// \{

public:
	using CL_GUIComponent::get_named_item;

	/// \brief Find the child CL_TextEdit with the specified component ID name.
	///
	/// If it was not found, an exception is thrown.
	static CL_TextEdit *get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id);

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

	/// \brief Get Max length
	///
	/// \return max_length
	int get_max_length() const;

	/// \brief Get Text
	///
	/// \return text
	CL_String get_text() const;

	/// \brief Get line count
	///
	/// \return line count
	int get_line_count() const;

	/// \brief Get line text
	///
	/// \return text
	CL_String get_line_text(int line) const;

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

	/// \brief Get Cursor line number
	///
	/// \return cursor line number
	int get_cursor_line_number() const;

	/// \brief Get Preferred size
	///
	/// \return preferred_size
	virtual CL_Size get_preferred_size() const;

	/// \brief Returns the height required to display all lines with the current width of the control
	int get_total_height();

/// \}
/// \name Operations
/// \{

public:
	void set_select_all_on_focus_gain(bool enable);

	/// \brief Select all
	void select_all();

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

	/// \brief Set max length
	///
	/// \param length = value
	void set_max_length(int length);

	/// \brief Set text
	///
	/// \param text = String Ref
	void set_text(const CL_StringRef &text);

	/// \brief Add text to end 
	///
	/// \param text = String Ref
	void add_text(const CL_StringRef &text);

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

	/// \brief Use mask to restrict accepted input.
	void set_input_mask(const CL_StringRef &mask);

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
	CL_SharedPtr<CL_TextEdit_Impl> impl;
/// \}
};

/// \}
