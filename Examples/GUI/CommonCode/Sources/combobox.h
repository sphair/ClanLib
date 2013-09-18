/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Mark Page
*/

#pragma once

#include "info.h"

class ComboBox : public clan::Window
{
public:
	ComboBox(clan::GUIManager &manager);

private:
	bool on_close();
	void on_dropdown_opened(clan::ComboBox *combobox);
	void on_dropdown_closed(clan::ComboBox *combobox);
	void on_before_edit_changed(clan::ComboBox *combobox);
	void on_after_edit_changed(clan::ComboBox *combobox);
	void on_item_selected(int value, clan::ComboBox *combobox);
	void on_selection_changed(int value, clan::ComboBox *combobox);

	void on_checked_disable(clan::CheckBox *checkbox);
	void on_unchecked_disable(clan::CheckBox *checkbox);
	void on_checked_editable(clan::CheckBox *checkbox);
	void on_unchecked_editable(clan::CheckBox *checkbox);

private:
	clan::ComboBox *combobox1;
	clan::PopupMenu menu;

	clan::Label *label_disable;
	clan::CheckBox *checkbox_disable;

	clan::Label *label_editable;
	clan::CheckBox *checkbox_editable;

	Info *info_dropdown_opened;
	Info *info_dropdown_closed;
	Info *info_before_edit_changed;
	Info *info_after_edit_changed;
	Info *info_item_selected;
	Info *info_selection_changed;
};
