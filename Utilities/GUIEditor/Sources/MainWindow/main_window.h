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
*/


#pragma once

#include "Selection/selection.h"

class Application;
class ViewBorder;
class GridComponent;
class PropertyComponent;
class TextureListWindow;

class MainWindow : public CL_Window
{
public:
	MainWindow(Application *application);
	~MainWindow();

// Attributes:
public:
	Application *get_app() { return application; }
	Selection *get_selection() { return &selection; }
	GridComponent *get_grid_component() { return grid_component; }

	int get_selected_tool() const { return selected_tool; }

// Operations:
public:
	void load(const CL_StringRef &str);

	void use_select_tool();

// Implementation:
private:
	static CL_GUITopLevelDescription get_startup_description();
	void create_components();
	void create_new_document();
	void populate_menubar();
	void populate_main_toolbar();
	void populate_tools_toolbar();
	void update_child_positions();
	bool on_close();
	void on_resized();
	void on_tool_selected(CL_ToolBarItem item);
	void on_main_toolbar_clicked(CL_ToolBarItem item);
	void on_menu_source_generator();
	void on_grid_resized();
	void on_process_messages(CL_GUIMessage &msg);

	CL_String show_open_file_dialog();
	CL_String show_save_file_dialog();

	Application *application;
	Selection selection;
	CL_MenuBar *menubar;
	CL_ToolBar *toolbar_main;
	CL_ToolBar *toolbar_tools;
	CL_StatusBar *statusbar;
	ViewBorder *view_border;
	GridComponent *grid_component;
	PropertyComponent *property_component;
	int selected_tool;

	CL_String loaded_filename;
};
