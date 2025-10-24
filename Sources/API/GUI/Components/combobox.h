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
#include "../../Core/Signals/callback_v0.h"
#include "../../Core/Signals/callback_v1.h"
#include "../../Core/Signals/callback_1.h"

class CL_Sprite;
class CL_ComboBox_Impl;
class CL_PopupMenu;

/// \brief Combo box component.
///
/// \xmlonly !group=GUI/Components! !header=gui.h! \endxmlonly
class CL_API_GUI CL_ComboBox : public CL_GUIComponent
{
/// \name Construction
/// \{

public:
	CL_ComboBox(CL_GUIComponent *parent);

	virtual ~CL_ComboBox();


/// \}
/// \name Attributes
/// \{

public:
	bool is_editable() const;

	int get_dropdown_height() const;

	int get_selected_item() const;

	CL_StringRef get_text() const;

	CL_StringRef get_item(int index) const;


/// \}
/// \name Operations
/// \{

public:
	void set_editable(bool enable = true);

	void set_dropdown_height(int height);

	void set_dropdown_minimum_width(int min_width);

	void set_text(const CL_StringRef &text);

	void set_selected_item(int index);

	void set_popup_menu(CL_PopupMenu &menu);


/// \}
/// \name Events
/// \{

public:
	CL_Callback_v0 &func_dropdown_opened();

	CL_Callback_v0 &func_dropdown_closed();

	CL_Callback_v0 &func_before_edit_changed();

	CL_Callback_v0 &func_after_edit_changed();

	/// \brief Callback invoked when an event not processed by the contained lineedit nor the combobox occurs.
	/** This includes keys such as Enter and Escape.
	* The callback should return true if the event was processsed, else false. */
	CL_Callback_1<bool, CL_InputEvent> &func_lineedit_unhandled_event();

	CL_Callback_v1<CL_Rect> &func_display_popup();

	CL_Callback_v1<int> &func_item_selected();

	CL_Callback_v1<int> &func_selection_changed();


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_ComboBox_Impl> impl;
/// \}
};


/// \}
