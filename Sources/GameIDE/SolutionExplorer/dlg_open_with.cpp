/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "dlg_open_with.h"
#include "API/GameIDE/UIController/document_editor_type.h"

namespace clan
{

DlgOpenWith::DlgOpenWith(GUIComponent *owner, std::vector<DocumentEditorType *> editors)
: GUIComponent(owner, get_toplevel_description())
{
	button_ok = new PushButton(this);
	button_cancel = new PushButton(this);
	list_items = new ListView(this);

	button_ok->set_text("OK");
	button_ok->set_default(true);
	button_cancel->set_text("Cancel");
	button_cancel->set_cancel(true);

	list_items->get_header()->append(list_items->get_header()->create_column("name", "Name")).set_width(110);
	list_items->show_detail_opener(false);
	list_items->show_detail_icon(false);
	list_items->get_header()->set_class("hidden", true);


	func_resized().set(this, &DlgOpenWith::on_resized);
	func_close().set(this, &DlgOpenWith::on_close);
	button_ok->func_clicked().set(this, &DlgOpenWith::on_button_ok_clicked);
	button_cancel->func_clicked().set(this, &DlgOpenWith::on_button_cancel_clicked);
	list_items->func_item_doubleclick().set(this, &DlgOpenWith::on_item_doubleclick);

	populate(editors);
	list_items->set_focus();
	on_resized();
}

DocumentEditorType *DlgOpenWith::get_editor_type()
{
	ListViewItem item = list_items->get_selected_item();
	if (!item.is_null())
		return std::dynamic_pointer_cast<ListViewItemData>(item.get_userdata())->type;
	else
		return 0;
}

void DlgOpenWith::on_resized()
{
	Rect client_box = get_size();
	client_box.shrink(11);

	Rect list_items_area_box(client_box.left, client_box.top, client_box.right, client_box.bottom - 22 - 7);
	Rect button_area_box(client_box.left, client_box.bottom - 22, client_box.right, client_box.bottom);
	Rect cancel_box(button_area_box.right - 75, button_area_box.top, button_area_box.right, button_area_box.bottom);
	Rect ok_box(cancel_box.left - 5 - 75, button_area_box.top, cancel_box.left - 5, button_area_box.bottom);

	list_items->set_geometry(list_items_area_box);
	button_ok->set_geometry(ok_box);
	button_cancel->set_geometry(cancel_box);
}

bool DlgOpenWith::on_close()
{
	on_button_cancel_clicked();
	return true;
}

void DlgOpenWith::on_button_ok_clicked()
{
	exit_with_code(1);
}

void DlgOpenWith::on_button_cancel_clicked()
{
	exit_with_code(0);
}

void DlgOpenWith::on_item_doubleclick(const ListViewItem &item)
{
	exit_with_code(1);
}

void DlgOpenWith::populate(std::vector<DocumentEditorType *> editors)
{
	for (size_t i = 0; i < editors.size(); i++)
	{
		ListViewItem item1 = list_items->create_item();
		item1.set_column_text("name", editors[i]->get_name());
		item1.set_userdata(ListViewItemDataPtr(new ListViewItemData(editors[i])));
		list_items->get_document_item().append_child(item1);
		if (list_items->get_selected_item().is_null())
			list_items->set_selected(item1);
	}
}

GUITopLevelDescription DlgOpenWith::get_toplevel_description()
{
	GUITopLevelDescription desc;
	desc.show_minimize_button(false);
	desc.show_maximize_button(false);
	desc.show_sysmenu(false);
	desc.set_allow_resize(false);
	desc.set_title("Open With");
	desc.set_size(Size(300, 125), true);
	desc.set_dialog_window(true);
	return desc;
}

}
