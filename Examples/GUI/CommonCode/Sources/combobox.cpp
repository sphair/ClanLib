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

#include "precomp.h"
#include "combobox.h"

ComboBox::ComboBox(clan::GUIManager &manager) :
	clan::Window(&manager, clan::GUITopLevelDescription("ComboBox", clan::Rect(256*3 + 32, 8, clan::Size(256, 256)), false))
{
	func_close().set(this, &ComboBox::on_close);


	clan::Rect client_area = get_client_area();

	menu.insert_item("Item A");
	menu.insert_item("Item B");
	menu.insert_item("Item C");
	menu.insert_item("Item D");

	combobox1 = new clan::ComboBox(this);
	combobox1->set_geometry(clan::Rect(client_area.left + 11, client_area.top + 10, clan::Size(128, 21)));
	combobox1->set_text("Combo Box");
	combobox1->set_editable(false);
	combobox1->set_dropdown_height(128);
	combobox1->set_dropdown_minimum_width(64);
	combobox1->set_popup_menu(menu);
	combobox1->set_selected_item(0);

	combobox1->func_dropdown_opened().set(this, &ComboBox::on_dropdown_opened, combobox1);
	combobox1->func_dropdown_closed().set(this, &ComboBox::on_dropdown_closed, combobox1);
	combobox1->func_before_edit_changed().set(this, &ComboBox::on_before_edit_changed, combobox1);
	combobox1->func_after_edit_changed().set(this, &ComboBox::on_after_edit_changed, combobox1);
	combobox1->func_item_selected().set(this, &ComboBox::on_item_selected, combobox1);
	combobox1->func_selection_changed().set(this, &ComboBox::on_selection_changed, combobox1);

	int xoffset = client_area.left + 36;
	int yoffset = client_area.top + 40;
	const int gap = 16;
	clan::Size label_size(50, 15);

	info_dropdown_opened = new Info(this);
	info_dropdown_opened->set(xoffset, yoffset, "Dropdown Opened");
	yoffset += gap;
	info_dropdown_closed = new Info(this);
	info_dropdown_closed->set(xoffset, yoffset, "Dropdown Closed");
	yoffset += gap;
	info_before_edit_changed = new Info(this);
	info_before_edit_changed->set(xoffset, yoffset, "Before Edit Changed");
	yoffset += gap;
	info_after_edit_changed = new Info(this);
	info_after_edit_changed->set(xoffset, yoffset, "After Edit Changed");
	yoffset += gap;
	info_item_selected = new Info(this);
	info_item_selected->set(xoffset, yoffset, "Item Selected");
	yoffset += gap;
	info_selection_changed = new Info(this);
	info_selection_changed->set(xoffset, yoffset, "Selection Changed");
	yoffset += gap;

	xoffset = client_area.left + 36;
	yoffset = client_area.top + 195 - gap;

	checkbox_editable = new clan::CheckBox(this);
	checkbox_editable->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(100, 15)));
	checkbox_editable->func_checked().set(this, &ComboBox::on_checked_editable, checkbox_editable);
	checkbox_editable->func_unchecked().set(this, &ComboBox::on_unchecked_editable, checkbox_editable);
	checkbox_editable->set_text("Editable");

	yoffset += gap;

	checkbox_disable = new clan::CheckBox(this);
	checkbox_disable->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(100, 15)));
	checkbox_disable->func_checked().set(this, &ComboBox::on_checked_disable, checkbox_disable);
	checkbox_disable->func_unchecked().set(this, &ComboBox::on_unchecked_disable, checkbox_disable);
	checkbox_disable->set_text("Disable");
}

void ComboBox::on_dropdown_opened(clan::ComboBox *combobox)
{
	info_dropdown_opened->activate();
}

void ComboBox::on_dropdown_closed(clan::ComboBox *combobox)
{
	info_dropdown_closed->activate();
}

void ComboBox::on_before_edit_changed(clan::ComboBox *combobox)
{
	info_before_edit_changed->activate();
}

void ComboBox::on_after_edit_changed(clan::ComboBox *combobox)
{
	info_after_edit_changed->activate();
}

void ComboBox::on_item_selected(int value, clan::ComboBox *combobox)
{
	std::string string = clan::string_format(" (%1)", value);
	info_item_selected->set_comment( string );

	info_item_selected->activate();
}

void ComboBox::on_selection_changed(int value, clan::ComboBox *combobox)
{
	std::string string = clan::string_format(" (%1)", value);
	info_selection_changed->set_comment( string );

	info_selection_changed->activate();
}

void ComboBox::on_checked_disable(clan::CheckBox *checkbox)
{
	combobox1->set_enabled(false);
}

void ComboBox::on_unchecked_disable(clan::CheckBox *checkbox)
{
	combobox1->set_enabled(true);
}

void ComboBox::on_checked_editable(clan::CheckBox *checkbox)
{
	combobox1->set_editable(true);
}

void ComboBox::on_unchecked_editable(clan::CheckBox *checkbox)
{
	combobox1->set_editable(false);
}

bool ComboBox::on_close()
{
	exit_with_code(0);
	return true;
}
