
#pragma once

#include "selection.h"

class Application;
class DialogDocument;
class ViewBorder;
class GridComponent;
class PropertyComponent;
class TextureListWindow;

enum EditorMode { edit_mode_position };

class MainWindow : public CL_Window
{
public:
	MainWindow(Application *application);
	~MainWindow();

// Attributes:
public:
	Application *get_app() { return application; }
	DialogDocument *get_document() { return document; }
	Selection *get_selection() { return &selection; }
	EditorMode get_editor_mode() { return editor_mode; }
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
	void on_mode_toolbar_clicked(CL_ToolBarItem item);
	void on_menu_source_generator();
	void on_grid_resized();
	void on_process_messages(CL_GUIMessage &msg);

	CL_String show_open_file_dialog();
	CL_String show_save_file_dialog();

	Application *application;
	DialogDocument *document;
	Selection selection;
	CL_MenuBar *menubar;
	CL_ToolBar *toolbar_main;
	CL_ToolBar *toolbar_tools;
	CL_StatusBar *statusbar;
	ViewBorder *view_border;
	GridComponent *grid_component;
	PropertyComponent *property_component;
	EditorMode editor_mode;
	int selected_tool;
};
