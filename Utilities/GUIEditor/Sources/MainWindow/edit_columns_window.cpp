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
**    Kenneth Gangstoe
*/

#include "precomp.h"
#include "edit_columns_window.h"

EditColumnsWindow::EditColumnsWindow(CL_GUIComponent *owner)
: CL_Window(owner, get_startup_description())
{
	set_id_name("edit_columns_window");

	func_close().set(this, &EditColumnsWindow::on_close);

	load_components();
}

EditColumnsWindow::~EditColumnsWindow()
{
}

CL_GUITopLevelDescription EditColumnsWindow::get_startup_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_title("Edit columns");
	desc.set_allow_resize(false);
	desc.set_visible(false);
	return desc;
}

void EditColumnsWindow::load_components()
{
	CL_GUILayoutCorners layout;
	set_layout(layout);
	create_components("Resources/Dialogs/edit_columns.gui");

	button_ok = CL_PushButton::get_named_item(this, "button_ok");
	button_ok->func_clicked().set(this, &EditColumnsWindow::on_button_ok_clicked);

	button_cancel = CL_PushButton::get_named_item(this, "button_cancel");
	button_cancel->func_clicked().set(this, &EditColumnsWindow::on_button_cancel_clicked);

	button_move_column_up = CL_PushButton::get_named_item(this, "button_move_column_up");
	button_move_column_up->func_clicked().set(this, &EditColumnsWindow::on_button_move_column_up_clicked);

	button_move_column_down = CL_PushButton::get_named_item(this, "button_move_column_down");
	button_move_column_down->func_clicked().set(this, &EditColumnsWindow::on_button_move_column_down_clicked);

	button_add_column = CL_PushButton::get_named_item(this, "button_add_column");
	button_add_column->func_clicked().set(this, &EditColumnsWindow::on_button_add_column_clicked);

	button_remove_column = CL_PushButton::get_named_item(this, "button_remove_column");
	button_remove_column->func_clicked().set(this, &EditColumnsWindow::on_button_remove_column_clicked);

	listview_columns = CL_ListView::get_named_item(this, "listview_columns");
	listview_columns->func_selection_changed().set(this, &EditColumnsWindow::on_listview_selection_changed);

	lineedit_text = CL_LineEdit::get_named_item(this, "lineedit_text");
	lineedit_text->func_after_edit_changed().set(this, &EditColumnsWindow::on_lineedit_text_changed);

	lineedit_id = CL_LineEdit::get_named_item(this, "lineedit_id");
	lineedit_id->func_after_edit_changed().set(this, &EditColumnsWindow::on_lineedit_id_changed);

	lineedit_width = CL_LineEdit::get_named_item(this, "lineedit_width");
	lineedit_width->func_after_edit_changed().set(this, &EditColumnsWindow::on_lineedit_width_changed);

	set_visible(true);
}

void EditColumnsWindow::on_lineedit_text_changed(CL_InputEvent &event)
{
	CL_ListViewItem item = listview_columns->get_selected_item();
	item.set_column_text("text", lineedit_text->get_text());
}

void EditColumnsWindow::on_lineedit_id_changed(CL_InputEvent &event)
{
	CL_ListViewItem item = listview_columns->get_selected_item();
	item.set_column_text("id", lineedit_id->get_text());
}

void EditColumnsWindow::on_lineedit_width_changed(CL_InputEvent &event)
{
	CL_ListViewItem item = listview_columns->get_selected_item();
	item.set_column_text("width", lineedit_width->get_text());
}

void EditColumnsWindow::on_listview_selection_changed(CL_ListViewSelection selection)
{
	CL_ListViewSelectedItem selection_item = selection.get_first();
	CL_ListViewItem item = selection_item.get_item();

	show_item(item);
}

void EditColumnsWindow::on_button_ok_clicked()
{
	exit_with_code(1);
}

void EditColumnsWindow::on_button_cancel_clicked()
{
	exit_with_code(0);
}

bool EditColumnsWindow::on_close()
{
	exit_with_code(0);
	return true;
}

void EditColumnsWindow::on_button_move_column_up_clicked()
{
}

void EditColumnsWindow::on_button_move_column_down_clicked()
{
}

void EditColumnsWindow::on_button_add_column_clicked()
{
	static int count = 1;

	CL_String name = cl_format("New Column %1", count++);

	Column column(name, CL_String(), 100);
	CL_ListViewItem item = add_column(column);
	show_item(item);
}

void EditColumnsWindow::on_button_remove_column_clicked()
{
}

std::vector<EditColumnsWindow::Column> EditColumnsWindow::get_columns() const
{
	CL_ListViewItem item = listview_columns->get_document_item().get_first_child();

	std::vector<Column> columns;
	while(!item.is_null())
	{
		CL_ListViewColumnData column_data_text = item.get_column("text");
		CL_ListViewColumnData column_data_id = item.get_column("id");
		CL_ListViewColumnData column_data_width = item.get_column("width");

		CL_String text = column_data_text.get_text();
		CL_String id = column_data_id.get_text();
		int width = CL_StringHelp::text_to_int(column_data_width.get_text());

		columns.push_back(Column(text, id, width));

		item = item.get_next_sibling();
	}

	return columns;
}

void EditColumnsWindow::show_item(CL_ListViewItem &item)
{
	CL_ListViewColumnData column_data_text = item.get_column("text");
	lineedit_text->set_text(column_data_text.get_text());

	CL_ListViewColumnData column_data_id = item.get_column("id");
	lineedit_id->set_text(column_data_id.get_text());

	CL_ListViewColumnData column_data_width = item.get_column("width");
	lineedit_width->set_text(column_data_width.get_text());

	lineedit_text->set_focus();

	lineedit_text->set_enabled(true);
	lineedit_id->set_enabled(true);
	lineedit_width->set_enabled(true);
}

void EditColumnsWindow::set_columns(const std::vector<Column> &columns)
{
	for(size_t i = 0; i < columns.size(); ++i)
	{
		add_column(columns[i]);
	}
}

CL_ListViewItem EditColumnsWindow::add_column(Column column)
{
	CL_ListViewItem item = listview_columns->create_item();

	item.set_column_text("text", column.text);
	item.set_column_text("id", column.id);
	item.set_column_text("width", CL_StringHelp::int_to_text(column.width));

	listview_columns->get_document_item().append_child(item);
	listview_columns->set_selected(item);

	return item;
}
