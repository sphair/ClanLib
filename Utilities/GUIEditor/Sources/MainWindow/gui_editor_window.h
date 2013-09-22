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


#pragma once

#include "Selection/selection.h"

class ViewBorder;
class GridComponent;
class PropertyComponent;
class TextureListWindow;

class GuiEditorWindow : public clan::Window
{
public:
	GuiEditorWindow(clan::GUIManager *gui_manager);
	~GuiEditorWindow();

// Attributes:
public:
	Selection *get_selection() { return &selection; }
	GridComponent *get_grid_component() { return grid_component; }

	int get_selected_tool() const { return selected_tool; }

// Operations:
public:
	void load(const std::string &str);

	void use_select_tool();

// Implementation:
private:
	static clan::GUITopLevelDescription get_startup_description();
	void create_components();
	void create_new_document();
	void populate_menubar();
	void populate_main_toolbar(clan::ResourceManager resources);
	void populate_tools_toolbar(clan::ResourceManager resources);
	void update_child_positions();
	bool on_close();
	void on_resized();
	void on_tool_selected(clan::ToolBarItem item);
	void on_main_toolbar_clicked(clan::ToolBarItem item);
	void on_menu_source_generator();
	void on_grid_resized();
	void on_process_messages(std::shared_ptr<clan::GUIMessage> &msg);

	std::string show_open_file_dialog();
	std::string show_save_file_dialog();

	Selection selection;
	clan::MenuBar *menubar;
	clan::ToolBar *toolbar_main;
	clan::ToolBar *toolbar_tools;
	clan::StatusBar *statusbar;
	ViewBorder *view_border;
	GridComponent *grid_component;
	PropertyComponent *property_component;
	int selected_tool;

	std::string loaded_filename;
};
