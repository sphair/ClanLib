
#include "precomp.h"
#include "main.h"
#include "form_view.h"
#include "property_list.h"

/////////////////////////////////////////////////////////////////////////////
// GUIEditor startup:

GUIEditor app;

int GUIEditor::main(int argc, char **argv)
{
	CL_SetupCore::init();
	CL_SetupDisplay::init();
	CL_SetupGL::init();
	CL_SetupGUI::init();

	try
	{
		CL_DisplayWindow window("ClanLib GUI Editor", 1024, 768, false, true);

		CL_ResourceManager resources("../../Resources/GUIStyleSilver/gui.xml", false);
		CL_StyleManager_Silver style(&resources);
		gui = new CL_GUIManager(&style);

		menubar = new CL_Menu(CL_Point(0, 0), gui, &style);
		menunode_file = new CL_MenuNode(menubar);
		menuitem_file = new CL_MenuItem("File", menunode_file, false, false);
		menunode_edit = new CL_MenuNode(menubar);
		menuitem_edit = new CL_MenuItem("Edit", menunode_edit, false, false);
		menunode_view = new CL_MenuNode(menubar);
		menuitem_view = new CL_MenuItem("View", menunode_view, false, false);
		menunode_help = new CL_MenuNode(menubar);
		menuitem_help = new CL_MenuItem("Help", menunode_help, false, false);

		menu_file = new CL_Menu(menunode_file, gui);
		menunode_new = new CL_MenuNode(menu_file);
		menuitem_new = new CL_MenuItem("New", menunode_new);
		menunode_open = new CL_MenuNode(menu_file);
		menuitem_open = new CL_MenuItem("Open", menunode_open);
		menunode_save = new CL_MenuNode(menu_file);
		menuitem_save = new CL_MenuItem("Save", menunode_save);
		menunode_exit = new CL_MenuNode(menu_file);
		menuitem_exit = new CL_MenuItem("Exit", menunode_exit);

		menu_edit = new CL_Menu(menunode_edit, gui);
		menunode_undo = new CL_MenuNode(menu_edit);
		menuitem_undo = new CL_MenuItem("Undo", menunode_undo);
		menunode_redo = new CL_MenuNode(menu_edit);
		menuitem_redo = new CL_MenuItem("Redo", menunode_redo);
		menunode_cut = new CL_MenuNode(menu_edit);
		menuitem_cut = new CL_MenuItem("Cut", menunode_cut);
		menunode_copy = new CL_MenuNode(menu_edit);
		menuitem_copy = new CL_MenuItem("Copy", menunode_copy);
		menunode_paste = new CL_MenuNode(menu_edit);
		menuitem_paste = new CL_MenuItem("Paste", menunode_paste);
		menunode_delete = new CL_MenuNode(menu_edit);
		menuitem_delete = new CL_MenuItem("Delete", menunode_delete);

		menu_view = new CL_Menu(menunode_view, gui);
		menunode_components = new CL_MenuNode(menu_view);
		menuitem_components = new CL_MenuItem("Components", menunode_components);
		menunode_connections = new CL_MenuNode(menu_view);
		menuitem_connections = new CL_MenuItem("Connections", menunode_connections);
		menunode_properties = new CL_MenuNode(menu_view);
		menuitem_properties = new CL_MenuItem("Properties", menunode_properties);
		menunode_tools = new CL_MenuNode(menu_view);
		menuitem_tools = new CL_MenuItem("Tools", menunode_tools);

		menu_help = new CL_Menu(menunode_help, gui);
		menunode_about = new CL_MenuNode(menu_help);
		menuitem_about = new CL_MenuItem("About ClanLib GUI Editor", menunode_about);

		frame_toolbox = new CL_Frame(CL_Rect(8, 8+20, 200, CL_Display::get_height()-8), gui);
		label_toolbox = new CL_Label(CL_Point(12, 8), "Tools:", frame_toolbox);
		list_tools = new CL_ListBox(CL_Rect(8, 24, frame_toolbox->get_width()-8, frame_toolbox->get_height()-8), frame_toolbox);

		frame_form = new CL_Frame(CL_Rect(208, 8+20, CL_Display::get_width()-208, CL_Display::get_height()-8), gui);
		label_form = new CL_Label(CL_Point(12, 8), "Form Layout:", frame_form);
		form_view = new FormView(CL_Rect(8, 24, frame_form->get_width()-8, frame_form->get_height()-8), frame_form);

		frame_propertylist = new CL_Frame(CL_Rect(CL_Display::get_width()-200, 8+20, CL_Display::get_width()-8, CL_Display::get_height()-8), gui);
		label_properties = new CL_Label(CL_Point(12, 8), "Properties:", frame_propertylist);
		property_list = new PropertyList(CL_Rect(8, 24, frame_propertylist->get_width()-8, frame_propertylist->get_height()-8), frame_propertylist);

		list_tools->insert_item("Select");
		std::map<std::string, CL_ComponentType *>::iterator it;
		for (it = CL_ComponentType::component_types.begin(); it != CL_ComponentType::component_types.end(); ++it)
		{
			list_tools->insert_item(it->first);
		}

		CL_SlotContainer slots;
		slots.connect(window.sig_window_close(), this, &GUIEditor::on_window_close);
		slots.connect(gui->sig_paint(), this, &GUIEditor::on_paint);
		slots.connect(gui->sig_resize(), this, &GUIEditor::on_resize);

		gui->show();
		gui->run();

		delete gui;
	}
	catch (CL_Error err)
	{
#ifdef WIN32
		MessageBox(0, err.message.c_str(), "ClanLib GUI Editor Fatal Error", MB_OK);
#else
		std::cout << "ClanLib GUI Editor Fatal Error" << err.message.c_str() << std::endl;
#endif
	}

	CL_SetupGL::deinit();
	CL_SetupDisplay::deinit();
	CL_SetupCore::deinit();

	return 0;
}

void GUIEditor::on_window_close()
{
	gui->quit();
}

void GUIEditor::on_paint()
{
	CL_Display::clear(CL_Color(240, 242, 244));
}

void GUIEditor::on_resize(int old_width, int old_height)
{
	int width = gui->get_width();
	int height = gui->get_height();

	menubar->set_width(width);

	frame_toolbox->set_position(CL_Rect(8, 8+20, 200, CL_Display::get_height()-8));
	list_tools->set_position(CL_Rect(8, 24, frame_toolbox->get_width()-8, frame_toolbox->get_height()-8));

	frame_form->set_position(CL_Rect(208, 8+20, CL_Display::get_width()-208, CL_Display::get_height()-8));
	form_view->set_position(CL_Rect(8, 24, frame_form->get_width()-8, frame_form->get_height()-8));

	frame_propertylist->set_position(CL_Rect(CL_Display::get_width()-200, 8+20, CL_Display::get_width()-8, CL_Display::get_height()-8));
	property_list->set_position(CL_Rect(8, 24, frame_propertylist->get_width()-8, frame_propertylist->get_height()-8));
}
