#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/guistylesilver.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

// The main application
class App : public CL_ClanApplication
{
public:
	int main(int argc, char** argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore setup_core;
			CL_SetupDisplay setup_display;
			CL_SetupGL setup_gl;
			CL_SetupGUI setup_gui;
 
			// Create our displaywindow
			CL_DisplayWindow display("GUI Simple example", 640, 480);

			// Connect the Window close event
			CL_Slot slot_quit = display.sig_window_close().connect(this, &App::on_quit);
			CL_SlotContainer slots; //for the lazy, this is a good way of dealing with a lot of slots

			// Create the GUI using the default style
			CL_ResourceManager gui_resources("../../Resources/GUIStyleSilver/gui.xml");
			CL_StyleManager_Silver style(&gui_resources);
			CL_GUIManager gui(&style);
			guimanager = &gui;

			// Make sure our background is drawn under the GUI
			CL_Slot slot_draw = gui.sig_paint().connect(this, &App::on_draw);

			// Create some GUI components
			CL_Window window_small(
				CL_Rect(310, 10, 620, 80), 
				"Small window", 
				CL_Window::close_button, 
				&gui);

			CL_Label label1(CL_Point(10,10), "This example will close if you close both windows,\nselect File->Quit, or press Escape.", window_small.get_client_area());
			
			CL_Window window(
				CL_Rect(10, 10, 300, 300), 
				"Window with menu", 
				CL_Window::close_button | CL_Window::maximize_button | CL_Window::minimize_button | CL_Window::help_button, 
				&gui);

			CL_Label label2(CL_Point(10, 40), "If you try to close this window, you will get a\nmessage box asking if you are sure.\n\nTry also the File->Open menu.", window.get_client_area());

			// Override default close handler with our own.
			// Its important to notice we use the connect_virtual() function, and
			// not the normal connect() function. This makes sure we are the only
			// one who will get called when sig_close() is activated. Had we used
			// connect() both ours and the default close handler would be called.
			CL_Slot slot_close_1 = window.sig_close_button_clicked().connect_virtual(this, &App::on_close_1);
			CL_Slot slot_close_2 = window_small.sig_close_button_clicked().connect_virtual(this, &App::on_close_2);

			// Create a menu bar in one of the GUI windows
			CL_Menu menu(window.get_client_area());

			// Add standard stuff
			menu.create_item("File/New");
			menu.create_item("File/Open");
			menu.create_item("File/Save");
			menu.create_item("File/Save As...");
			menu.create_item("File/Quit");

			// Add a icon to save menuitem
			CL_MenuItem_Silver *item_save_style = (CL_MenuItem_Silver *)(menu.get_item("File/Save")->get_style());
			item_save_style->set_icon(new CL_Surface("save.tga"), true);

			// Create some localized menuitems
			menu.create_item("Localized/Expression/imbored", "Localized/ClanSpeak/dumdidumdum");
			menu.create_item("Localized/Expression/reconnect", "Localized/ClanSpeak/DOOM DE DOOM");
			menu.create_item("Localized/Expression/yousuck", "Localized/ClanSpeak/Ermn");

			// Create button which won't close menu
			CL_MenuNode *button_node = menu.create_node("Components/Button/node_1");
			button_node->set_close_on_click(false);
			CL_Button *menu_button = new CL_Button( "Don't close", button_node );

			// Create another button which will close menu on click
			CL_Button *menu_button2 = new CL_Button( "Close on click", menu.create_node("Components/Button/node_2"));

			// Create a listbox within menu
			CL_MenuNode *list_node = menu.create_node("Components/Listbox/node_1");
			list_node->set_close_on_click(false);
			menu_list = new CL_ListBox(CL_Rect(17,0,140,50), list_node );
			menu_list->insert_item(std::string("Double click me"));
			menu_list->insert_item(std::string("or me"));
			slots.connect( menu_list->sig_mouse_dblclk(), this, &App::OnListDoubleClick);

			//note, if we want to catch a normal click and not a double click, we could do it like
			//this:
			//slots.connect( menu_list->sig_selection_changed(), this, &App::OnListSelectionChanged);
			//and in the callback would get menu_list->get_current_item() for the index.
 
			// Create a toggle and a disabled item
			menu.create_toggle_item("Tests/toggling item");
			menu.create_item("Tests/disabled")->enable(false);

			// Hook into some menus
			CL_Slot slot_menu_load = menu.get_node("File/Open")->sig_clicked().connect(this, &App::on_load);
			CL_Slot slot_menu_quit = menu.get_node("File/Quit")->sig_clicked().connect(this, &App::on_quit);

			// Disable save as
			menu.get_node("File/Save As...")->enable(false);

			quit1 = false;
			quit2 = false;

			// Main loop
			while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE) && !(quit1 && quit2))
			{
				gui.show();

				CL_System::keep_alive();
				CL_Display::flip();
			}

		}
		catch (CL_Error e)
		{
 			std::cout << e.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}
	
	// When we use connect_virtual, we have a new parameter - the slot parent.
	void on_close_1(CL_SlotParent_v0 &parent_handler)
	{
		if(CL_MessageBox::info(
			"Really quit?",
			"You have expressed a wish to close the window. Are you really sure ?",
			"Yes", "I don't know", "No",
			guimanager) == 0)
		{
			// If the user answers Yes in the MessageBox, we call the default
			// clanGUI handler for close(), which will close the window.
			parent_handler();
			quit1 = true;
		}
	}

	void OnListDoubleClick(const CL_InputEvent &input)
	{
		int index = menu_list->get_item(input.mouse_pos);
		if (index == -1) return;
		
		CL_MessageBox::info("Hey!", "You clicked " + menu_list->get_text(index)+" (index "+ CL_String::from_int(index)+")", "Ok", "", "", guimanager);
	}

	void on_close_2(CL_SlotParent_v0 &parent_handler)
	{
		parent_handler();	// Close window
		quit2 = true;
	}

	void on_quit()
	{
		quit1 = quit2 = true;
	}

	void on_load()
	{
		std::string filename = CL_FileDialog::open("", "*.cpp", guimanager);
		if(filename != "")
			CL_MessageBox::info("Filename", filename, guimanager);
	}

	void on_draw()
	{
		CL_Display::clear();
	}

private:
	CL_Component *guimanager;
	bool quit1, quit2;
	CL_ListBox *menu_list;
} app;
