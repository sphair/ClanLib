#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/guistylesilver.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

// An about dialog
class AboutDialog : public CL_Window
{
public:
	AboutDialog(CL_Component *parent) :
		CL_Window(
			CL_Rect(300, 200, 425, 300),
			"About",
			CL_Window::close_button | CL_Window::maximize_button | CL_Window::minimize_button | CL_Window::help_button,
			parent),
		label(
			CL_Point(20, 10),
			"GUI Example 1.0",
			get_client_area()),
		button(
			CL_Rect(20, 40, 100, 60),
			"Okay",
			get_client_area())
	{
		slot_quit = button.sig_clicked().connect((CL_Component *) this, &CL_Component::quit);
	}

private: 
	CL_Label label;
	CL_Button button;
	CL_Slot slot_quit;
};

// The main application
class App : public CL_ClanApplication
{
public:
	bool quit; 

	CL_GUIManager *gui;
	CL_StyleManager *style;
		
	CL_ListBox *listbox;
	CL_Label *label_scroll1;
	CL_Label *label_scroll2;
	CL_Label *label_radio;
	CL_Label *label_fps;
	CL_ProgressBar *progress_x;
	CL_ProgressBar *progress_y;
	CL_Frame *frame1;
	CL_Button *button_quit;
	CL_Button *button_disabled;
	CL_Button *button_about;
	CL_Button *button_message1;
	CL_Button *button_file;
	CL_InputBox *input1;
	CL_InputBox *input2;
	CL_CheckBox *checkbox;
	CL_CheckBox *checkbox_disabled1;
	CL_CheckBox *checkbox_disabled2;
	CL_Button *button_bitmap;
	CL_Button *button_bitmap_disabled;
	CL_Frame *frame2;
	CL_Button *button_add;
	CL_Button *button_remove;
	CL_Frame *frame3;
	CL_Frame *frame4;
	CL_ScrollBar *scrollbar1;
	CL_ScrollBar *scrollbar2;
	CL_ScrollBar *scrollbar3;
	CL_ScrollBar *scrollbar4;
	CL_ScrollBar *scrollbar5;
	CL_ScrollBar *scrollbar6;
	CL_ScrollBar *scrollbar7;
	CL_Button *button_bitmap2;
	CL_Label *label_rightclick;
	CL_RadioButton *r1;
	CL_RadioButton *r2;
	CL_RadioButton *r3;
	CL_RadioButton *r4;
	CL_Menu *menu;
	CL_Menu *popup_menu;
	CL_TreeView *treeview;
	CL_FramerateCounter *fps;
	
	int main(int argc, char** argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			std::cout << "This example shows how to create components dynamically" << std::endl;
			std::cout << "(without using .gui files)" << std::endl;
			std::cout << std::endl;

			CL_SetupCore setup_core; 
			CL_SetupDisplay setup_disp;
			CL_SetupGL setup_gl;
			CL_SetupGUI setup_gui;

			CL_DisplayWindow window("GUITest example", 640, 480);

			// Create the GUI using the default style
			CL_ResourceManager gui_resources("../../Resources/GUIStyleSilver/gui.xml", false);
			style = new CL_StyleManager_Silver(&gui_resources);
			gui = new CL_GUIManager(style);

			// Load some extra resources
			CL_ResourceManager resources("resources.xml", false);

			// Create the menus
			create_menus();
			popup_menu = 0;

			// set menu icon
			CL_MenuItem_Silver *item_save_style = static_cast<CL_MenuItem_Silver *>(menu->get_item("File/Save")->get_style());
			item_save_style->set_icon( new CL_Surface("icon_save", &resources), true );

			// Create some standard buttons (with tab-id)
			frame1 = new CL_Frame(CL_Rect(10, 30, 113, 193), gui);
			button_quit = new CL_Button(CL_Rect(10, 10, 93, 33), "Quit", frame1);
			button_disabled = new CL_Button(CL_Rect(10, 40, 93, 63), "Disabled", frame1);
			button_disabled->enable(false);
			button_about = new CL_Button(CL_Rect(10, 70, 93, 93), "About", frame1);
			button_message1 = new CL_Button(CL_Rect(10, 100, 93, 123), "Message 1", frame1);
			button_file = new CL_Button(CL_Rect(10, 130, 93, 153), "Open File", frame1);

			button_quit->set_tab_id(0);
			button_about->set_tab_id(1);
			button_message1->set_tab_id(2);
			button_file->set_tab_id(3);

			// Create inputboxes
			input1 = new CL_InputBox(CL_Rect(140, 30, 280, 0), "Input here", gui);
			input2 = new CL_InputBox(CL_Rect(140, 60, 280, 0), "Disabled input here", gui);
			input2->enable(false);

			// Create checkboxes
			checkbox = new CL_CheckBox(CL_Point(140, 90), "Check this out", gui);
			checkbox_disabled1 = new CL_CheckBox(CL_Point(140, 110), "Disabled check", gui);
			checkbox_disabled1->enable(false);
			checkbox_disabled2 = new CL_CheckBox(CL_Point(140, 130), "Disabled check", gui);
			checkbox_disabled2->set_checked();
			checkbox_disabled2->enable(false);
			
			// Create progressbars
			progress_x = new CL_ProgressBar(CL_Rect(300, 30, 440, 50), 640, gui);
			progress_y = new CL_ProgressBar(CL_Rect(300, 60, 440, 80), 480, gui);

			// Create some bitmapped buttons
			CL_Surface surface_up("button_surface1", &resources);
			CL_Surface surface_highlighted("button_surface2", &resources);
			CL_Surface surface_down("button_surface3", &resources);
			CL_Surface surface_disabled("button_surface4", &resources);

			button_bitmap = new CL_Button(CL_Point(140, 150), "", gui);
			CL_Button_Silver *button_bitmap_style = (CL_Button_Silver *)button_bitmap->get_style();
			button_bitmap_style->set_surface_up(&surface_up);
			button_bitmap_style->set_surface_down(&surface_down);
			button_bitmap_style->set_surface_highlighted(&surface_highlighted);
			button_bitmap->find_preferred_size();
			
			button_bitmap_disabled = new CL_Button(CL_Point(205, 150), "", gui);
			button_bitmap_disabled->enable(false);
			CL_Button_Silver *button_bitmap_disabled_style = (CL_Button_Silver *)button_bitmap_disabled->get_style();
			button_bitmap_disabled_style->set_surface_up(&surface_up);
			button_bitmap_disabled_style->set_surface_disabled(&surface_disabled);
			button_bitmap_disabled->find_preferred_size();

			// Create a listbox and some extras
			frame2 = new CL_Frame(CL_Rect(10, 200, 200, 468), gui);
			button_add = new CL_Button(CL_Rect(10, 10, 180, 29), "Add item", frame2);
			button_remove = new CL_Button(CL_Rect(10, 29, 180, 48), "Remove item", frame2);

			listbox = new CL_ListBox(CL_Rect(10, 48, 180, 256), frame2);
//			listbox->set_multi_selection();

			// Add some dummy data to the listbox
			for(int i=0; i<20; i++)
			{
				std::string str = CL_String::format("ListItem %1...", i + 1);
				listbox->insert_item(str);
			}

			// Create some scrollbars
			frame3 = new CL_Frame(CL_Rect(210, 200, 450, 468), gui);
			frame4 = new CL_Frame(CL_Rect(10, 10, 450-210-10, 468-180-30), frame3);
			scrollbar1 = new CL_ScrollBar(CL_Rect(10, 10, 28, 218), 1, 1, 0, frame4);
			scrollbar2 = new CL_ScrollBar(CL_Rect(40, 10, 58, 218), 1, 2, 0, frame4);
			scrollbar3 = new CL_ScrollBar(CL_Rect(70, 10, 88, 218), 1, 3, 0, frame4);
			scrollbar4 = new CL_ScrollBar(CL_Rect(100, 10, 118, 218), 1, 10, 0, frame4);
			scrollbar5 = new CL_ScrollBar(CL_Rect(130, 10, 148, 218), 1, 100, 0, frame4);
			scrollbar6 = new CL_ScrollBar(CL_Rect(160, 10, 178, 218), 100, 1000, 0, frame4);
			scrollbar7 = new CL_ScrollBar(CL_Rect(10, 220, 180, 238), 1, 10, 1, frame4);
			scrollbar6->set_tracking(false);	
			label_scroll1 = new CL_Label(CL_Point(190, 30), "100  ", frame4);
			label_scroll2 = new CL_Label(CL_Point(190, 50), "100  ", frame4);

			// Create a treeview
			treeview = new CL_TreeView(CL_Rect(500, 90, 630, 470), gui);
			CL_TreeItem *item1 = treeview->insert_item("Item 1");
			CL_TreeItem *item11 = item1->get_node()->insert_item("Item 1-1");
			CL_TreeItem *item2 = treeview->insert_item("Item 2");
			CL_TreeItem *item21 = item2->get_node()->insert_item("Item 2-1");
			CL_TreeItem *item211 = item21->get_node()->insert_item("Item 2-1-1");
			CL_TreeItem *item212 = item21->get_node()->insert_item("Item 2-1-2");

/*			CL_TreeView treeview(CL_Rect(500, 90, 630, 470), gui);
			CL_Button treebutton("Item 2", NULL, gui->get_style_manager());	// Create a button as a treeitem to show off :)
			CL_TreeNode *node1 =  treeview.insert_item("Item 1");
			node1->insert_item("Item 1-1");
			CL_TreeNode *node2 =  treeview.insert_item(&treebutton);
			CL_TreeNode *node21 = node2->insert_item("Item 2-1");
			node21->insert_item("Item 2-1-1");
			node21->insert_item("Item 2-1-2");
*/
			label_rightclick = new CL_Label(CL_Point(500, 40), "Right click for popup", gui);
			
			label_fps = new CL_Label(CL_Rect(500, 60, 700, 75), "", gui);
/*
			button_bitmap2 = new CL_Button("", &surface_up, &surface_down, &surface_disabled, &surface_highlighted, NULL, style);
			popup = new CL_PopupMenu(CL_Rect(500, 90, 600, 470), NULL, style);
			subpopup = new CL_PopupMenu(CL_Rect(500, 90, 600, 470), NULL, style);
			popup->insert_item("Open File...");
			popup->insert_item("Save File");
			popup->insert_item("About", subpopup);
			popup->insert_item(button_bitmap2, false);
			popup->insert_item("Quit");
			subpopup->insert_item("ClanLib");
			subpopup->insert_item("This GUI Example");
*/
			// Create some radiobuttons
			CL_RadioGroup radio;
			r1 = new CL_RadioButton(CL_Point(300, 90), "Radio 1", gui);
			r2 = new CL_RadioButton(CL_Point(300, 110), "Radio 2", gui);
			r3 = new CL_RadioButton(CL_Point(300, 130), "Radio 3", gui);
			radio.add(r1);
			radio.add(r2);
			radio.add(r3);
			label_radio = new CL_Label(CL_Point(350, 160), "No radiobutton selected", gui);

			// Background draw (needed for modal dialogs)
			CL_Slot slot_paint = gui->sig_paint().connect(this, &App::on_paint);

			// Mouse movement
			CL_Slot slot_mouse_move = CL_Mouse::sig_move().connect(this, &App::on_mouse_move);

			// Quit
			CL_Slot slot_quit = button_quit->sig_clicked().connect(this, &App::on_quit);
			CL_Slot slot_window_quit = window.sig_window_close().connect(this, &App::on_quit);
			CL_Slot slot_menu_quit = menu->get_node("File/Quit")->sig_clicked().connect(this, &App::on_quit);
			
			// Radio button selection changed
			CL_Slot slot_radio = radio.sig_selection_changed().connect(this, &App::on_radio);

			// Show some about information (in a custom dialog)
			CL_Slot slot_about = button_about->sig_clicked().connect(this, &App::on_about);

			// Show a messagebox
			CL_Slot slot_message1 = button_message1->sig_clicked().connect(this, &App::on_message);

			// Show a filedialog
			CL_Slot slot_file = button_file->sig_clicked().connect(this, &App::on_file);

			// Add an item to the listbox
			CL_Slot slot_add_item = button_add->sig_clicked().connect(this, &App::on_add_item);

			// Remove selected items from listbox
			CL_Slot slot_remove_item = button_remove->sig_clicked().connect(this, &App::on_remove_item);

			// Display scroll values
			CL_Slot slot_scroll_moved = scrollbar6->sig_slider_moved().connect(this, &App::on_scroll_moved);
			CL_Slot slot_scroll_changed = scrollbar6->sig_value_changed().connect(this, &App::on_scroll_changed);

			// Open a popupmenu when rightclicking label
			CL_Slot slot_rightclick = label_rightclick->sig_mouse_down().connect(this, &App::on_rightclick_label);

//			menu->raise(); opt_submenu->raise(); 
//			bg_submenu->raise(); selector_submenu->raise();

			fps = new CL_FramerateCounter;
			
			quit = false;

			// Why the heck do I need to draw the GUI once before things start appearing??
			// - mbn 2nd nov 2003
			gui->show();

			// Main loop
			while (!quit)
			{
				if (gui->get_update_trigger()->get_flag())
				{
					gui->get_update_trigger()->reset();
					gui->show();
					CL_Display::flip();
				}

				CL_System::keep_alive();
			}

			delete gui;
			delete style;
			delete fps;
/*			
			CL_SetupGL::deinit();
			CL_SetupGUI::deinit();
			CL_SetupDisplay::deinit();
			CL_SetupCore::deinit();
*/
		}
		catch (CL_Error e)
		{
 			std::cout << e.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}

	void on_paint()
	{
		label_fps->set_text(fps->get_fps());
				
		CL_Display::clear(CL_Color(240, 242, 244));
	}

	void on_mouse_move(const CL_InputEvent &key)
	{
		progress_x->set_progress(key.mouse_pos.x);
		progress_y->set_progress(key.mouse_pos.y);
	}

	void on_rightclick_label(const CL_InputEvent &key)
	{
		// Create a menu
		if( popup_menu == 0 )
		{
			popup_menu = new CL_Menu(CL_Point(CL_Mouse::get_x(), CL_Mouse::get_y()), gui);
			popup_menu->set_vertical(true);
			popup_menu->set_root_collapsing(true);
			popup_menu->create_item("New");
			popup_menu->create_item("Open");
			popup_menu->create_item("Save");
			popup_menu->create_item("Save As...");
			popup_menu->create_item("Sub");
			popup_menu->create_item("Sub/menu");
			popup_menu->create_item("Sub/items");
		}
		else
		{
			popup_menu->set_position(CL_Mouse::get_x(), CL_Mouse::get_y());
			popup_menu->show();
		}
	}

	void on_radio(CL_RadioButton *button)
	{
		label_radio->set_text(button->get_text());
	}

	void on_about()
	{
		AboutDialog about(gui);
		about.run();
	}
	
	void on_scroll_moved(int value)
	{
		label_scroll1->set_text(value);
	}

	void on_scroll_changed(int value)
	{
		label_scroll2->set_text(value);
	}

	void on_message()
	{
		CL_MessageBox::info(
			"Message for the user",
			"This is a sample message...",
			"Yes", "I don't know", "No",
			gui);
	}

	void on_quit()
	{
		quit = true;
	}

	void on_remove_item()
	{
		int item = listbox->get_current_item();

		// If someone didn't select anything, just remove the first item from the list.
		if (item < 0)
			item = 0;

		listbox->remove_item(item);
	}

	void on_add_item()
	{
		std::string str = CL_String::format("New item %1", listbox->get_count() + 1);
		listbox->insert_item(str);
	}

	void on_file()
	{
		std::string filename = CL_FileDialog::open("", "*.cpp", gui);
		if(filename != "")
			CL_MessageBox::info("You selected a filename", filename, gui);
	}

	void create_menus()
	{
		// - - - - - File - - - - -
		menu = new CL_Menu( CL_Point(0,0), gui, style);
		menu->create_item("File/New");
		menu->create_item("File/Open");
		menu->create_item("File/Save");
		menu->create_item("File/Save As...");
		menu->create_item("File/Quit");

		// - - - - - Localized - - - - -
		menu->create_item("Localized/Expression/imbored", "Localized/ClanSpeak/dumdidumdum");
		menu->create_item("Localized/Expression/reconnect", "Localized/ClanSpeak/DOOM DE DOOM");
		menu->create_item("Localized/Expression/yousuck", "Localized/ClanSpeak/Ermn");

		// - - - - - Widgets - - - - -

		// buttons
		CL_MenuNode *button_node1 = menu->create_node("Widgets/Button/node_1");
		CL_MenuNode *button_node2 = menu->create_node("Widgets/Button/node_2");
		button_node1->set_close_on_click(false);
		button_node2->set_close_on_click(false);
		CL_Button *menu_button = new CL_Button( "Hello, World!", button_node1 );
		CL_Button *menu_button2 = new CL_Button( "dumdidum", button_node2);

		// Listbox
		CL_MenuNode *list_node = menu->create_node("Widgets/ListBox/node_1");
		list_node->set_close_on_click(false);
		CL_ListBox *menu_list = new CL_ListBox( CL_Rect(17,0,140,50), list_node );
		menu_list->insert_item(std::string("dumdidumdum"));
		menu_list->insert_item(std::string("doom de doom"));

		// Scrollbar
		CL_MenuNode *scroll_node = menu->create_node("Widgets/ScrollBar/node_1");
		CL_ScrollBar *scrollbar = new CL_ScrollBar(CL_Rect(30, 13, 200, 28), 1, 100, 1, scroll_node);
		scroll_node->set_close_on_click(false);

		// - - - - - Tests - - - - -

		menu->create_toggle_item("Tests/toggling item");
		menu->create_toggle_item("Tests/disabled toggle")->enable(false);
		menu->create_item("Tests/disabled")->enable(false);
		menu->create_item("Tests/no close")->set_close_on_click(false);

		menu->create_item("Tests/Disabled Menu/item 1");
		menu->create_item("Tests/Disabled Menu/item 2");
		menu->create_item("Tests/Disabled Menu/Submenu/item 1");
		menu->create_item("Tests/Disabled Menu/Submenu2/item 2");
		menu->get_node("Tests/Disabled Menu")->enable(false);
	}
} app;
