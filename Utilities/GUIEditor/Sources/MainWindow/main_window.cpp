/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Harry Storbacka
*/

#include "precomp.h"
#include "main_window.h"
#include "application.h"
#include "GridComponent/grid_component.h"
#include "PropertyComponent/property_component.h"
#include "view_border.h"
#include "ComponentTypes/component_types.h"
#include "ComponentTypes/component_type.h"
#include "GridComponent/grid_object.h"

enum MainToolbarID { main_toolbar_new, main_toolbar_open, main_toolbar_saveas, main_toolbar_save };

MainWindow::MainWindow(Application *application)
: CL_Window(application->get_gui(), get_startup_description()),
  application(application), grid_component(0), property_component(0), selected_tool(1337)
{
	set_id_name("editor");
	func_close().set(this, &MainWindow::on_close);
	func_resized().set(this, &MainWindow::on_resized);
	func_process_message().set(this, &MainWindow::on_process_messages);

	create_components();
//	populate_menubar();
	populate_main_toolbar();
	populate_tools_toolbar();

	create_new_document();
	update_child_positions();
}

MainWindow::~MainWindow()
{
	// We do not have to free the GUI components here because the CL_GUIComponent
	// destructor deletes all child components.
}

CL_GUITopLevelDescription MainWindow::get_startup_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_title("ClanLib GUI Editor");
	desc.set_allow_resize(true);
	desc.set_size(CL_Size(1100, 768), false);
	return desc;
}

void MainWindow::create_components()
{
//	menubar = new CL_MenuBar(this);
	toolbar_main = new CL_ToolBar(this);
	toolbar_tools = new CL_ToolBar(this);
	statusbar = new CL_StatusBar(this);
	statusbar->set_status_text("Ready");
	view_border = new ViewBorder(this);
	property_component = new PropertyComponent(this);

	toolbar_tools->set_id_name("tools");
	toolbar_tools->set_single_selection(true);
}

void MainWindow::populate_menubar()
{
	CL_PopupMenu menu_file;
	menu_file.insert_item("New").set_enabled(false);
	menu_file.insert_item("Open").set_enabled(false);
	menu_file.insert_item("Save").set_enabled(false);
	menu_file.insert_item("Exit").set_enabled(false);
	menubar->add_menu("File", menu_file);

	CL_PopupMenu menu_edit;
	menu_edit.insert_item("Undo").set_enabled(false);
	menu_edit.insert_item("Redo").set_enabled(false);
	menu_edit.insert_item("Cut").set_enabled(false);
	menu_edit.insert_item("Copy").set_enabled(false);
	menu_edit.insert_item("Paste").set_enabled(false);
	menu_edit.insert_item("Delete").set_enabled(false);
	menu_edit.insert_item("Select All").set_enabled(false);
	menubar->add_menu("Edit", menu_edit);
/*
	CL_PopupMenu menu_view;
	menubar->add_menu("View", menu_view);
*/
/*	CL_PopupMenu menu_tools;
	menu_tools.insert_item("Options");
	CL_PopupMenuItem pmi_source_generator = menu_tools.insert_item("Source Generator...");
	pmi_source_generator.func_clicked().set(this, &MainWindow::on_menu_source_generator);
	menubar->add_menu("Tools", menu_tools);*/

	CL_PopupMenu menu_help;
	menu_help.insert_item("About GUI Editor").set_enabled(false);
	menubar->add_menu("Help", menu_help);
}

void MainWindow::populate_tools_toolbar()
{
	toolbar_tools->func_item_selected().set(this, &MainWindow::on_tool_selected);

	CL_Sprite sprite(get_gc(), "gfx/pointer.png");
	CL_ToolBarItem tbi = toolbar_tools->insert_item(sprite, 0, "Select", 1337);
	tbi.set_toggling(true);

	const std::vector<ComponentType *> &types = ComponentTypes::get_types();
	for (std::vector<ComponentType *>::size_type index = 0; index < types.size(); index++)
	{
		CL_Sprite sprite(get_gc(), types[index]->icon);
		CL_ToolBarItem tbi = toolbar_tools->insert_item(sprite, 0, types[index]->name, types[index]->id);
		tbi.set_toggling(true);
	}
}

void MainWindow::populate_main_toolbar()
{
	toolbar_main->func_item_clicked().set(this, &MainWindow::on_main_toolbar_clicked);
	toolbar_main->insert_item(CL_Sprite(get_gc(), "new_16x16.png"), 0, "New", main_toolbar_new);
	toolbar_main->insert_item(CL_Sprite(get_gc(), "open_16x16.png"), 0, "Open", main_toolbar_open);
	toolbar_main->insert_item(CL_Sprite(get_gc(), "save_16x16.png"), 0, "Save As", main_toolbar_saveas);
	toolbar_main->insert_item(CL_Sprite(get_gc(), "save_16x16.png"), 0, "Save", main_toolbar_save);
}

bool MainWindow::on_close()
{
	exit_with_code(0);
	return true;
}

void MainWindow::on_resized()
{
	update_child_positions();
}

void MainWindow::on_tool_selected(CL_ToolBarItem item)
{
	selected_tool = item.get_id();
}

void MainWindow::update_child_positions()
{
	CL_Rect client_area = get_client_area();
	CL_Size size = client_area.get_size();

//	menubar->set_geometry(CL_Rect(client_area.left, client_area.top, client_area.right, client_area.top + 22));
	toolbar_main->set_geometry(CL_Rect(client_area.left, client_area.top /*+ 22*/, client_area.right, client_area.top/*+22*/+31));
	toolbar_tools->set_geometry(CL_Rect(client_area.left, client_area.top/*+22*/+31, client_area.left+24*5, client_area.bottom-24));
	statusbar->set_geometry(CL_Rect(client_area.left, client_area.bottom-24, client_area.right, client_area.bottom));
	property_component->set_geometry(CL_Rect(client_area.right-200, client_area.top/*+22*/+31, client_area.right, client_area.bottom-24));
	view_border->set_geometry(CL_Rect(client_area.left+24*5, client_area.top/*+22*/+31, client_area.right-200, client_area.bottom-24));
	view_border->on_resized();
}

void MainWindow::create_new_document()
{
	selection.clear();
	property_component->clear();

	delete grid_component;
	grid_component = new GridComponent(view_border, this);

	grid_component->func_boundary_resized.set(this, &MainWindow::on_grid_resized);
	grid_component->set_boundary_size(CL_Size(320,200));
	property_component->set_dialog_size(CL_Size(320,200));
}

void MainWindow::on_main_toolbar_clicked(CL_ToolBarItem item)
{
	switch(item.get_id())
	{
		case main_toolbar_new:
		{
			create_new_document();
			update_child_positions();
			break;
		}
		case main_toolbar_open:
		{
			CL_String filename = show_open_file_dialog();
			if (!filename.empty())
			{
				load(filename);
			}
			break;
		}
		case main_toolbar_saveas:
		{
			CL_String filename = show_save_file_dialog();

			if (!filename.empty())
			{
				grid_component->save(filename);
			}
			break;
		}
		case main_toolbar_save:
		{
			CL_String filename = loaded_filename;

			if(filename.length() == 0)
				filename = show_save_file_dialog();

			if (!filename.empty())
			{
				grid_component->save(filename);
				loaded_filename = filename;
			}
			break;
		}
	}
}

CL_String MainWindow::show_open_file_dialog()
{
	CL_OpenFileDialog dlg(this);
	dlg.add_filter("XML and GUI files", "*.xml;*.gui", true);
	dlg.add_filter("All files", "*.*");
	dlg.set_initial_directory(CL_System::get_exe_path());
	if(dlg.show())
		return dlg.get_filename();
	else
		return CL_String();
}

CL_String MainWindow::show_save_file_dialog()
{
	CL_SaveFileDialog dlg(this);
	dlg.add_filter("XML and GUI files", "*.xml;*.gui", true);
	dlg.add_filter("All files", "*.*");
	dlg.set_initial_directory(CL_System::get_exe_path());
	if(dlg.show())
		return dlg.get_filename();
	else
		return CL_String();
}

void MainWindow::on_menu_source_generator()
{
}

void MainWindow::load(const CL_StringRef &filename)
{
	create_new_document();
	grid_component->load(filename);
	loaded_filename = filename;
	property_component->set_dialog_size(grid_component->get_dialog_size());
	update_child_positions();
}

void MainWindow::on_grid_resized()
{
	property_component->set_dialog_size(grid_component->get_dialog_size());
}

void MainWindow::on_process_messages(CL_GUIMessage &msg)
{
	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		if (e.type == CL_InputEvent::pressed)
		{
			if (e.id == CL_KEY_PRIOR)
			{
				if (selection.empty() && grid_component->get_first_child())
				{
					GridObject *object = dynamic_cast<GridObject*>(grid_component->get_first_child());
					if (object)
						selection.add_object(object);
				}
				else
				{
					GridObject *object = selection.get_selection().front();
					if (object->get_previous_sibling() || grid_component->get_last_child())
					{
						selection.clear();
						GridObject *sibling = dynamic_cast<GridObject*>(object->get_previous_sibling());
						if (sibling == 0)
							sibling = dynamic_cast<GridObject*>(grid_component->get_last_child());
						if (sibling)
						selection.add_object(sibling);
					}
				}
				grid_component->request_repaint();
			}
			else if (e.id == CL_KEY_NEXT)
			{
				if (selection.empty() && grid_component->get_first_child())
				{
					GridObject *object = dynamic_cast<GridObject*>(grid_component->get_first_child());
					if (object)
						selection.add_object(object);
				}
				else
				{
					GridObject *object = selection.get_selection().front();
					if (object->get_next_sibling() || grid_component->get_first_child())
					{
						selection.clear();
						GridObject *sibling = dynamic_cast<GridObject*>(object->get_next_sibling());
						if (sibling == 0)
							sibling = dynamic_cast<GridObject*>(grid_component->get_first_child());
						if (sibling)
							selection.add_object(sibling);
					}
				}
				grid_component->request_repaint();
			}
			else if (e.id == CL_KEY_F1)
			{
				toolbar_tools->get_item_by_id(1337).set_pressed(true);
			}
		}
	}
	if (msg.is_type(CL_GUIMessage_Close::get_type_name()))
	{
		exit_with_code(0);
	}
}

void MainWindow::use_select_tool()
{
	toolbar_tools->get_item_by_id(selected_tool).set_pressed(false);
	selected_tool = 1337;
	toolbar_tools->get_item_by_id(1337).set_pressed(true);
	toolbar_tools->request_repaint();
}
