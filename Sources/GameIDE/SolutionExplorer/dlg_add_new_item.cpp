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
**    Magnus Norddahl
*/

#include "GameIDE/precomp.h"
#include "dlg_add_new_item.h"
#include "API/GameIDE/SolutionModel/solution_model.h"
#include "API/GameIDE/FileItemType/file_item_type.h"

namespace clan
{

DlgAddNewItem::DlgAddNewItem(GUIComponent *owner, FileItemTypeFactory &factory, const std::string &default_location)
: GUIComponent(owner, get_toplevel_description())
{
	label_name = new Label(this);
	lineedit_name = new LineEdit(this);
	label_location = new Label(this);
	lineedit_location = new LineEdit(this);
	button_browse_location = new PushButton(this);
	button_ok = new PushButton(this);
	button_cancel = new PushButton(this);
	list_items = new ListView(this);
	label_description_header = new Label(this);
	label_description = new Label(this);

	label_name->set_text("Name:");
	label_location->set_text("Location:");
	button_browse_location->set_text("Browse...");
	button_ok->set_text("OK");
	button_ok->set_default(true);
	button_cancel->set_text("Cancel");
	button_cancel->set_cancel(true);
	lineedit_name->set_focus();

	lineedit_name->set_text("New item");
	lineedit_location->set_text(default_location);

	list_items->get_header()->append(list_items->get_header()->create_column("name", "Name")).set_width(110);
	list_items->show_detail_opener(false);
	//	list_items->get_header()->set_visible(false);
	list_items->get_header()->set_class("hidden", true);

	func_resized().set(this, &DlgAddNewItem::on_resized);
	func_close().set(this, &DlgAddNewItem::on_close);
	button_ok->func_clicked().set(this, &DlgAddNewItem::on_button_ok_clicked);
	button_cancel->func_clicked().set(this, &DlgAddNewItem::on_button_cancel_clicked);
	button_browse_location->func_clicked().set(this, &DlgAddNewItem::on_button_browse_location_clicked);

	list_items->func_selection_changed().set(this, &DlgAddNewItem::on_list_items_selection_changed);

	on_resized();

	populate(factory);
}

void DlgAddNewItem::populate(FileItemTypeFactory &factory)
{
	int icon_index = 1;

	Canvas canvas = get_canvas();
	ResourceManager resources = get_resources();

	const std::vector<FileItemType*> &types = factory.types();
	for(size_t i = 0; i < types.size(); ++i)
	{
		ListViewIcon icon;
		Sprite sprite = types[i]->get_icon(canvas, &resources);
		icon.set_sprite(sprite, listview_mode_details);
		list_items->get_icon_list().set_icon(icon_index, icon);

		ListViewItem item1 = list_items->create_item();
		item1.set_column_text("name", types[i]->get_name());
		item1.set_icon(icon_index);
		item1.set_userdata(ListViewItemDataPtr(new ListViewItemData(types[i])));

		list_items->get_document_item().append_child(item1);

		icon_index++;

		if(list_items->get_selected_item().is_null())
			list_items->set_selected(item1);
	}
}

void DlgAddNewItem::on_resized()
{
	Rect client_box = get_size();
	client_box.shrink(11);

	Rect list_items_area_box(client_box.left, client_box.top, client_box.right - 300, client_box.bottom - 100);
	Rect label_description_header_box(client_box.right - 290, client_box.top, client_box.right, client_box.top + 20);
	Rect label_description_box(client_box.right - 290, client_box.top + 20, client_box.right, client_box.bottom - 100);
	Rect button_area_box(client_box.left, client_box.bottom - 22, client_box.right, client_box.bottom);
	Rect cancel_box(button_area_box.right - 75, button_area_box.top, button_area_box.right, button_area_box.bottom);
	Rect ok_box(cancel_box.left - 5 - 75, button_area_box.top, cancel_box.left - 5, button_area_box.bottom);

	int label_width = 75;
	int browse_width = 75;
	int edit_width = client_box.get_width() - label_width - browse_width - 5;

	int y = client_box.bottom - 90;

	Rect label_name_box(client_box.left, y + 2, client_box.left + label_width, y + 22);
	Rect lineedit_name_box(client_box.left + label_width, y, client_box.left + label_width + edit_width, y + 22);

	y += 22 + 5;

	Rect label_location_box(client_box.left, y + 2, client_box.left + label_width, y + 22);
	Rect lineedit_location_box(client_box.left + label_width, y, client_box.left + label_width + edit_width, y + 22);
	Rect browse_box(client_box.left + label_width + edit_width + 5, y, client_box.left + label_width + edit_width + 5 + browse_width, y + 22);

	list_items->set_geometry(list_items_area_box);
	label_description->set_geometry(label_description_box);
	label_description_header->set_geometry(label_description_header_box);

	button_ok->set_geometry(ok_box);
	button_cancel->set_geometry(cancel_box);

	label_name->set_geometry(label_name_box);
	lineedit_name->set_geometry(lineedit_name_box);

	label_location->set_geometry(label_location_box);
	lineedit_location->set_geometry(lineedit_location_box);
	button_browse_location->set_geometry(browse_box);
}

bool DlgAddNewItem::on_close()
{
	on_button_cancel_clicked();
	return true;
}

void DlgAddNewItem::on_button_ok_clicked()
{
	std::string name = lineedit_name->get_text();
	std::string folder = lineedit_location->get_text();

	if(StringHelp::trim(name).length() == 0)
	{
		message_box(this, "Please fill in name", "Please enter a name for the new item.", mb_buttons_ok, mb_icon_error);
		return;
	}
	if(StringHelp::trim(folder).length() == 0)
	{
		message_box(this, "Please fill in folder", "Please enter a location for the new item.", mb_buttons_ok, mb_icon_error);
		return;
	}
	if(list_items->get_selected_item().is_null())
	{
		message_box(this, "Please select a type", "Please select a type for the new item.", mb_buttons_ok, mb_icon_error);
		return;
	}
	exit_with_code(1);
}

void DlgAddNewItem::on_button_cancel_clicked()
{
	exit_with_code(0);
}

void DlgAddNewItem::on_button_browse_location_clicked()
{
	BrowseFolderDialog dlg(this);
	dlg.set_title("Select Location Folder");
	if (dlg.show())
	{
		lineedit_location->set_text(dlg.get_selected_path());
	}
}

void DlgAddNewItem::on_list_items_selection_changed(ListViewSelection selection)
{
	ListViewSelectedItem selected_item = selection.get_first();
	if(!selected_item.is_null())
	{
		label_description_header->set_text("Description:");
		label_description->set_text(get_fileitemtype()->get_description());
	}
}

GUITopLevelDescription DlgAddNewItem::get_toplevel_description()
{
	GUITopLevelDescription desc;
	desc.show_minimize_button(false);
	desc.show_maximize_button(false);
	desc.show_sysmenu(false);
	desc.set_allow_resize(true);
	desc.set_title("Add New Item");
	desc.set_size(Size(680, 350), true);
	return desc;
}

std::string DlgAddNewItem::get_name() const
{
	return lineedit_name->get_text();
}

std::string DlgAddNewItem::get_location() const
{
	return lineedit_location->get_text();
}

FileItemType *DlgAddNewItem::get_fileitemtype() const
{
	ListViewItemDataPtr data = std::dynamic_pointer_cast<ListViewItemData>(list_items->get_selected_item().get_userdata());
	return data->type;
}

}
