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
**    Kenneth Gangstoe
*/

#pragma once

class EditColumnsWindow : public GUIComponent
{
public:
	struct Column
	{
		Column(const std::string &text, const std::string &id, int width) : text(text), id(id), width(width) {}

		std::string text;
		std::string id;
		int width;
	};

public:
	EditColumnsWindow(GUIComponent *owner);
	~EditColumnsWindow();

// Attributes:
public:
	std::vector<Column> get_columns() const;

// Operations:
public:
	void set_columns(const std::vector<Column> &columns);

// Implementation:
private:
	GUITopLevelDescription get_startup_description();

	void load_components();
	ListViewItem add_column(Column column);
	void show_item(ListViewItem &item);

	void on_button_ok_clicked();
	void on_button_cancel_clicked();
	void on_button_move_column_up_clicked();
	void on_button_move_column_down_clicked();
	void on_button_add_column_clicked();
	void on_button_remove_column_clicked();
	void on_listview_selection_changed(ListViewSelection selection);
	bool on_close();
	void on_lineedit_text_changed(InputEvent &event);
	void on_lineedit_id_changed(InputEvent &event);
	void on_lineedit_width_changed(InputEvent &event);

	PushButton *button_ok;
	PushButton *button_cancel;
	PushButton *button_move_column_up;
	PushButton *button_move_column_down;
	PushButton *button_add_column;
	PushButton *button_remove_column;
	ListView *listview_columns;
	LineEdit *lineedit_text;
	LineEdit *lineedit_id;
	LineEdit *lineedit_width;
};
