#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class App
{
public:
	int main(const std::vector<CL_String> &args)
	{
		try
		{
			CL_ResourceManager resources(cl_text("../../../Resources/GUIThemeLuna/resources.xml")); 

			CL_GUIManager gui;
 
			CL_GUIWindowManagerSystem wm;
			gui.set_window_manager(wm); 

			CL_GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme);
			gui.set_css_document(cl_text("../../../Resources/GUIThemeLuna/theme.css"));

			CL_DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title(cl_text("Popup Menu test app."));
			win_desc.set_drop_shadow(false);
			win_desc.set_position(CL_Rect(200,200,600,600), false);
			CL_Window root(&gui, win_desc);			
			root.func_close().set(this, &App::on_close, &root); 

			CL_MenuBar menubar(&root);
			menubar.set_geometry(CL_Rect(0,0,400,24));

			CL_PopupMenu menu_file;
			menu_file.insert_item(cl_text("New"));
			menu_file.insert_item(cl_text("Open"));
			menu_file.insert_item(cl_text("Save"));
			menu_file.insert_item(cl_text("Exit"));
			menubar.add_menu(cl_text("File"), menu_file);

			CL_PopupMenu menu_edit;
			menu_edit.insert_item(cl_text("Undo"));
			menu_edit.insert_item(cl_text("Redo"));
			menu_edit.insert_separator();
			menu_edit.insert_item(cl_text("Cut"));
			menu_edit.insert_item(cl_text("Copy"));
			menu_edit.insert_separator();
			CL_PopupMenuItem item_submenu = menu_edit.insert_item(cl_text("Submenu"));
			menu_edit.insert_separator();
			menu_edit.insert_item(cl_text("Paste"));
			menu_edit.insert_item(cl_text("Delete"));
			menu_edit.insert_separator();
			menu_edit.insert_item(cl_text("Select All"));

			CL_PopupMenu menu_submenu;
			menu_submenu.insert_item(cl_text("foo"));
			menu_submenu.insert_item(cl_text("bar"));
			menu_submenu.insert_item(cl_text("foobar"));
			item_submenu.set_submenu(menu_submenu);

 			menubar.add_menu(cl_text("Edit"), menu_edit);

			CL_PushButton button1(&root);
			button1.set_geometry(CL_Rect(20, 100, 200, 125));
			button1.set_text(cl_text("Click for pop-up menu"));

			button1.func_clicked().set(this, &App::on_button1_clicked, &button1);

			CL_ComboBox combobox1(&root);
			combobox1.set_geometry(CL_Rect(20, 140, 200, 162));
			combobox1.set_popup_menu(menu_edit);
			combobox1.set_editable(false);

			CL_AcceleratorTable accel_table;
			gui.exec(accel_table);
		}
		catch (CL_Exception e)
		{
			CL_ConsoleWindow console(cl_text("Console"));
 			CL_Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
	}

	void on_close(CL_Window *win)
	{
		cl_log_event(cl_text("TestApp"), cl_text("Shutdown"));
		win->exit_with_code(0);
	}

	void on_button1_clicked(CL_PushButton *button1)
	{
		enum MenuIds
		{
			id_item1,
			id_item2,
			id_item3
		};

		CL_PopupMenu menu;
		menu.insert_item(cl_text("Item 1"), id_item1);
		menu.insert_item(cl_text("Item 2"), id_item2);
		menu.insert_item(cl_text("Item 3"), id_item3);

		int x = button1->get_geometry().get_width()/2;
		int y = button1->get_geometry().get_height()/2;
		menu.exec(button1, CL_Point(x, y));
	}
};

// This is the Program class that is called by CL_ClanApplication
class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;

		CL_FileLogger logger("log.txt");

		// Start the Application
		App app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

