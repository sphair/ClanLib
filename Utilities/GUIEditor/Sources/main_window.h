
#ifndef header_my_main_window
#define header_my_main_window

#include "selection.h"

class Application;
class DialogDocument;
class ViewBorder;
class GridComponent;
class PropertyComponent;
class PropertyComponentTabOrder;
class TextureListWindow;

enum EditorMode { edit_mode_position, edit_mode_taborder };

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

// Operations:
public:
	void load(const CL_StringRef &str);

// Implementation:
private:
	static CL_GUITopLevelDescription get_startup_description();
	void create_components();
	void populate_menubar();
	void populate_main_toolbar();
	void populate_tools_toolbar();
	void populate_mode_toolbar();
	void update_child_positions();
	void on_close();
	void on_resized();
	void on_tool_selected(CL_ToolBarItem item);
	void on_main_toolbar_clicked(CL_ToolBarItem item);
	void on_mode_toolbar_clicked(CL_ToolBarItem item);
	void on_menu_source_generator();
	Application *application;
	DialogDocument *document;
	Selection selection;
	CL_MenuBar *menubar;
	CL_ToolBar *toolbar_main;
	CL_ToolBar *toolbar_tools;
	CL_ToolBar *toolbar_mode;
	CL_StatusBar *statusbar;
	ViewBorder *view_border;
	GridComponent *grid_component;
	PropertyComponent *property_component;
	PropertyComponentTabOrder *property_component_tab_order;
	EditorMode editor_mode;
};

#endif
