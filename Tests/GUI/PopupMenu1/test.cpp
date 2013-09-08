#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;

class App
{
public:
	int main(const std::vector<std::string> &args)
	{
		GUIManager gui;

		GUIWindowManagerSystem wm;
		gui.set_window_manager(wm); 
		gui.add_theme("../../../Resources/GUIThemeAero/theme.css");
		gui.add_resources(clan::XMLResourceDocument("../../../Resources/GUIThemeAero/resources.xml"));
 
		DisplayWindowDescription win_desc;
		win_desc.set_allow_resize(true);
		win_desc.set_title("Popup Menu test app.");
		win_desc.set_drop_shadow(false);
		win_desc.set_size(Size(600,600), false);
		Window root(&gui, win_desc);			
		root.func_close().set(this, &App::on_close, &root); 

		MenuBar menubar(&root);
		menubar.set_geometry(Rect(0,0,400,24));

		PopupMenu menu_file;
		menu_file.insert_item("New");
		menu_file.insert_item("Open");
		menu_file.insert_item("Save");
		menu_file.insert_item("Exit");
		menubar.add_menu("File", menu_file);

		PopupMenu menu_edit;
		menu_edit.insert_item("Undo");
		menu_edit.insert_item("Redo");
		menu_edit.insert_separator();
		menu_edit.insert_item("Cut");
		menu_edit.insert_item("Copy");
		menu_edit.insert_separator();
		PopupMenuItem item_submenu = menu_edit.insert_item("Submenu");
		menu_edit.insert_separator();
		menu_edit.insert_item("Paste");
		menu_edit.insert_item("Delete");
		menu_edit.insert_separator();
		menu_edit.insert_item("Select All");

		PopupMenu menu_submenu;
		menu_submenu.insert_item("foo");
		menu_submenu.insert_item("bar");
		menu_submenu.insert_item("foobar");
		item_submenu.set_submenu(menu_submenu);

 		menubar.add_menu("Edit", menu_edit);

		PushButton button1(&root);
		button1.set_geometry(Rect(20, 100, 200, 125));
		button1.set_text("Click for pop-up menu");

		button1.func_clicked().set(this, &App::on_button1_clicked, &button1);

		ComboBox combobox1(&root);
		combobox1.set_geometry(Rect(20, 140, 200, 162));
		combobox1.set_popup_menu(menu_edit);
		combobox1.set_editable(false);

		gui.exec();

		return 0;
	}

	bool on_close(Window *win)
	{
		//cl_log_event("TestApp", "Shutdown");
		win->exit_with_code(0);
		return true;
	}

	void on_button1_clicked(PushButton *button1)
	{
		enum MenuIds
		{
			id_item1,
			id_item2,
			id_item3
		};

		PopupMenu menu;
		menu.insert_item("Item 1", id_item1);
		menu.insert_item("Item 2", id_item2);
		menu.insert_item("Item 3", id_item3);

		int x = button1->get_geometry().get_width()/2;
		int y = button1->get_geometry().get_height()/2;
		menu.start(button1, button1->component_to_screen_coords(Point(x, y)));
	}
};

// This is the Program class that is called by Application
class Program
{
public:
	static int main(const std::vector<std::string> &args)
	{
		SetupCore setup_core;
		SetupDisplay setup_display;
		SetupGL setup_gl;

		//FileLogger logger("log.txt");

		// Start the Application
		App app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate Application, informing it where the Program is located
Application app(&Program::main);

