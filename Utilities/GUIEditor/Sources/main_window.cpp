
#include "precomp.h"
#include "main_window.h"
#include "application.h"
#include "grid_component.h"
#include "property_component.h"
#include "property_component_tab_order.h"
#include "view_border.h"
#include "system_dialog.h"
#include "component_type.h"
#include "dialog_document.h"
#include "source_generator_dialog.h"

enum MainToolbarID { main_toolbar_new, main_toolbar_open, main_toolbar_save };

MainWindow::MainWindow(Application *application)
: CL_Window(get_startup_geometry(), application->get_gui(), get_startup_description()),
  application(application), document(0), grid_component(0), property_component(0)
{
	set_id_name("editor");
	func_close().set(this, &MainWindow::on_close);
	func_resized().set(this, &MainWindow::on_resized);

	document = new DialogDocument;
	create_components();
	populate_menubar();
	populate_main_toolbar();
	populate_mode_toolbar();
	populate_tools_toolbar();
	update_child_positions();

	grid_component->set_focus();
}

MainWindow::~MainWindow()
{
	// We do not have to free the GUI components here because the CL_GUIComponent
	// destructor deletes all child components.

	delete document;
}

CL_Rect MainWindow::get_startup_geometry()
{
	return CL_Rect(200, 200, 1024, 768);
}

CL_GUITopLevelDescription MainWindow::get_startup_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_title(cl_text("ClanLib GUI Editor"));
	desc.set_allow_resize(true);
	return desc;
}

void MainWindow::create_components()
{
	menubar = new CL_MenuBar(this);
	toolbar_main = new CL_ToolBar(this);
	toolbar_tools = new CL_ToolBar(this);
	toolbar_mode = new CL_ToolBar(this);
	statusbar = new CL_StatusBar(this);
	statusbar->set_status_text(cl_text("Ready"));
	view_border = new ViewBorder(this);
	grid_component = new GridComponent(view_border, this);
	grid_component->set_tab_order_controller(true);
	property_component = new PropertyComponent(this);
	property_component_tab_order = new PropertyComponentTabOrder(this);
	property_component_tab_order->set_visible(false);

	toolbar_tools->set_id_name(cl_text("tools"));
	toolbar_tools->set_single_selection(true);
}

void MainWindow::populate_menubar()
{
	CL_PopupMenu menu_file;
	menu_file.insert_item(cl_text("New"));
	menu_file.insert_item(cl_text("Open"));
	menu_file.insert_item(cl_text("Save"));
	menu_file.insert_item(cl_text("Exit"));
	menubar->add_menu(cl_text("File"), menu_file);

	CL_PopupMenu menu_edit;
	menu_edit.insert_item(cl_text("Undo"));
	menu_edit.insert_item(cl_text("Redo"));
	menu_edit.insert_item(cl_text("Cut"));
	menu_edit.insert_item(cl_text("Copy"));
	menu_edit.insert_item(cl_text("Paste"));
	menu_edit.insert_item(cl_text("Delete"));
	menu_edit.insert_item(cl_text("Select All"));
	menubar->add_menu(cl_text("Edit"), menu_edit);
/*
	CL_PopupMenu menu_view;
	menubar->add_menu(cl_text("View"), menu_view);
*/
	CL_PopupMenu menu_tools;
	menu_tools.insert_item(cl_text("Options"));
	CL_PopupMenuItem pmi_source_generator = menu_tools.insert_item("Source Generator...");
	pmi_source_generator.func_clicked().set(this, &MainWindow::on_menu_source_generator);
	menubar->add_menu(cl_text("Tools"), menu_tools);

	CL_PopupMenu menu_help;
	menu_help.insert_item(cl_text("About GUI Editor"));
	menubar->add_menu(cl_text("Help"), menu_help);
}

void MainWindow::populate_tools_toolbar()
{
	toolbar_tools->func_item_selected().set(this, &MainWindow::on_tool_selected);

	CL_Sprite sprite(cl_text("gfx/pointer.png"), get_gc());
	CL_ToolBarItem tbi = toolbar_tools->insert_item(sprite, 0, cl_text("Select"), 1337);
	tbi.set_toggling(true);

	const std::vector<ComponentType *> &types = ComponentTypes::get_types();
	for (std::vector<ComponentType *>::size_type index = 0; index < types.size(); index++)
	{
		CL_Sprite sprite(types[index]->icon, get_gc());
		tbi = toolbar_tools->insert_item(sprite, 0, types[index]->name, types[index]->id);
		tbi.set_toggling(true);
	}
}

void MainWindow::populate_main_toolbar()
{
	toolbar_main->func_item_clicked().set(this, &MainWindow::on_main_toolbar_clicked);
	toolbar_main->insert_item(CL_Sprite(cl_text("new_16x16.png"), get_gc()), 0, cl_text("New"), main_toolbar_new);
	toolbar_main->insert_item(CL_Sprite(cl_text("open_16x16.png"), get_gc()), 0, cl_text("Open"), main_toolbar_open);
	toolbar_main->insert_item(CL_Sprite(cl_text("save_16x16.png"), get_gc()), 0, cl_text("Save As"), main_toolbar_save);
}

void MainWindow::populate_mode_toolbar()
{
	toolbar_mode->func_item_clicked().set(this, &MainWindow::on_mode_toolbar_clicked);
	toolbar_mode->insert_item(CL_Sprite(cl_text("gfx/editor_mode_position.png"), get_gc()), 0, cl_text("Pos"), edit_mode_position);
	toolbar_mode->insert_item(CL_Sprite(cl_text("gfx/editor_mode_taborder.png"), get_gc()), 0, cl_text("Tab"), edit_mode_taborder);

	toolbar_mode->set_single_selection(true);

	int count = toolbar_mode->get_item_count();
	for (int i=0; i<count; ++i)
	{
		toolbar_mode->get_item(i).set_toggling(true);
	}

	toolbar_mode->get_item_by_id(edit_mode_position).set_pressed(true);
}

void MainWindow::on_close()
{
	exit_with_code(0);
}

void MainWindow::on_resized()
{
	update_child_positions();
}

void MainWindow::on_tool_selected(CL_ToolBarItem item)
{
	grid_component->on_add_component(item.get_id());
}

void MainWindow::update_child_positions()
{
	CL_Rect client_area = get_client_area();
	CL_Size size = client_area.get_size();

	menubar->set_geometry(CL_Rect(client_area.left, client_area.top, client_area.right, client_area.top + 22));
	toolbar_main->set_geometry(CL_Rect(client_area.left, client_area.top + 22, client_area.left+int(size.width*0.5), client_area.top+22+26));
	toolbar_mode->set_geometry(CL_Rect(client_area.left+int(size.width*0.5), client_area.top + 22, client_area.right, client_area.top+22+26));
	toolbar_tools->set_geometry(CL_Rect(client_area.left, client_area.top+22+26, client_area.left+24*5, client_area.bottom-24));
	statusbar->set_geometry(CL_Rect(client_area.left, client_area.bottom-24, client_area.right, client_area.bottom));
	property_component->set_geometry(CL_Rect(client_area.right-200, client_area.top+22+26, client_area.right, client_area.bottom-24));
	property_component_tab_order->set_geometry(property_component->get_geometry());
	view_border->set_geometry(CL_Rect(client_area.left+24*5, client_area.top+22+26, client_area.right-200, client_area.bottom-24));
}

void MainWindow::on_main_toolbar_clicked(CL_ToolBarItem item)
{
	CL_String filename;

	switch(item.get_id())
	{
	case main_toolbar_new:
		selection.clear();
		property_component->clear();
		delete document;
		document = new DialogDocument;
		delete grid_component;
		grid_component = new GridComponent(view_border, this);
		update_child_positions();
		break;

	case main_toolbar_open:
		filename = SystemDialog::show_open_file_dialog();
		if (filename.empty())
			return;
//		document->load(filename);
		grid_component->load(filename);
		break;

	case main_toolbar_save:
		filename = SystemDialog::show_save_file_dialog();
		if (filename.empty())
			return;
//		document->save(filename);
		grid_component->save(filename);
	}
}

void MainWindow::on_menu_source_generator()
{
	CL_GUIManager mgr = get_gui_manager();

	SourceGeneratorDialog srcgen(&mgr);

	int retval = srcgen.exec();
}

void MainWindow::on_mode_toolbar_clicked( CL_ToolBarItem item )
{
	if (editor_mode == item.get_id())
		return;

	editor_mode = EditorMode(item.get_id());

	switch (editor_mode)
	{
		case edit_mode_position:
			property_component_tab_order->set_visible(false);
			property_component->set_visible(true);
			break;
		case edit_mode_taborder:
			property_component_tab_order->set_visible(true);
			property_component->set_visible(false);
			break;
	}
}








