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

			CL_DisplayWindow display("GUI TreeView example", 640, 580);

			// Create the GUI using the default style
			CL_ResourceManager gui_resources("../../Resources/GUIStyleSilver/gui.xml");
			CL_StyleManager_Silver style(&gui_resources);
			CL_GUIManager guimanager(&style);
			gui = &guimanager;

			// Create a deep treeview
			treeview1 = new CL_TreeView(CL_Rect(10, 280, 530, 410), gui);

			treeview1->add_column("Column 1", 200);
			treeview1->add_column("Column 2", 100);
			treeview1->add_column("Column 3", 150);
			treeview1->add_column("Visible", 100);

			CL_Surface icon("save.tga");
			CL_TreeItem *item1_1 = treeview1->insert_item("Item 1", "Item 1b", "Item 1c");
			CL_TreeItem *item1_11 = item1_1->get_node()->insert_item("Item 1-1");
			CL_TreeItem *item1_2 = treeview1->insert_item("Item 2");
			CL_TreeItem *item1_21 = item1_2->get_node()->insert_item("Item 2-1", "Item 2-1b");
			CL_TreeItem *item1_211 = item1_21->get_node()->insert_item("Item 2-1-1");
			CL_TreeItem *item1_212 = item1_21->get_node()->insert_item("Item 2-1-2", "Item 2-1-2b", "Item 2-1-2c");
			CL_TreeItem *item1_213 = item1_21->get_node()->insert_item("Item 2-1-3", "Item 2-1-3b", "Item 2-1-3c");
			CL_TreeItem *item1_214 = item1_21->get_node()->insert_item("Item 2-1-4", "Item 2-1-3b", "Item 2-1-3c");
			CL_TreeItem *item1_215 = item1_21->get_node()->insert_item("Item 2-1-5", "Item 2-1-3b", "Item 2-1-3c");
			CL_TreeItem *item1_216 = item1_21->get_node()->insert_item("Item 2-1-6", "Item 2-1-3b", "Item 2-1-3c");

			item1_1->set_checkbox(3);
			item1_11->set_checkbox(3);
			item1_2->set_checkbox(3);
			item1_21->set_checkbox(3);
			item1_211->set_checkbox(3);
			item1_212->set_checkbox(3);

			item1_1->set_icon(&icon);
			item1_11->set_icon(&icon);
			item1_2->set_icon(&icon);
			item1_21->set_icon(&icon);
			item1_211->set_icon(&icon);
			item1_212->set_text_margin(16);

			CL_Button *button_rootdecoration = new CL_Button(CL_Rect(540, 280, 630, 300), "Root decoration", gui);
			CL_Button *button_header = new CL_Button(CL_Rect(540, 305, 630, 325), "Header", gui);

			// Create a files treeview
			CL_TreeView *treeview2 = new CL_TreeView(CL_Rect(10, 10, 630, 270), gui);

			treeview2->add_column("Name", 220);
			treeview2->add_column("Type", 160);
			treeview2->add_column("Total size", 70);
			treeview2->add_column("Free space", 70);
			treeview2->add_column("Comment", 99);

			treeview2->show_root_decoration(false);
			treeview2->set_selectable(false);

			CL_TreeItem *item2_1 = treeview2->insert_item(" Files Stored on This Computer");
			CL_TreeItem *item2_11 = item2_1->get_node()->insert_item("Shared Documents", "File folder");
			CL_TreeItem *item2_12 = item2_1->get_node()->insert_item("Sphairs Documents", "File folder");
			CL_TreeItem *item2_13 = item2_1->get_node()->insert_item("Mbns Documents", "File folder");
			CL_TreeItem *item2_14 = item2_1->get_node()->insert_item("Kens Documents", "File folder");
			CL_TreeItem *item2_15 = item2_1->get_node()->insert_item("All Documents", "File folder");

			item2_1->set_custom_height(30);
			item2_11->set_icon(new CL_Surface("sharedfolder.png"), true);
			item2_12->set_icon(new CL_Surface("folder.png"), true);
			item2_13->set_icon(new CL_Surface("folder.png"), true);
			item2_14->set_icon(new CL_Surface("folder.png"), true);
			item2_15->set_icon(new CL_Surface("sharedfolder.png"), true);

			CL_TreeItem *item2_2 = treeview2->insert_item(" Hard Disk Drives");
			CL_TreeItem *item2_21 = item2_2->get_node()->insert_item("Local Disk (C:)", "Local disk", "104 GB", "63,2 GB");
			CL_TreeItem *item2_22 = item2_2->get_node()->insert_item("Code (D:)", "Local disk", "154 GB", "103,5 GB");
			CL_TreeItem *item2_23 = item2_2->get_node()->insert_item("Games (E:)", "Local disk", "66,7 GB", "2,17 GB");

			item2_2->set_custom_height(30);
			item2_21->set_icon(new CL_Surface("disk.png"), true);
			item2_22->set_icon(new CL_Surface("disk.png"), true);
			item2_23->set_icon(new CL_Surface("disk.png"), true);

			CL_TreeItem *item2_3 = treeview2->insert_item(" Devices with Removable Storage");
			CL_TreeItem *item2_31 = item2_3->get_node()->insert_item("3.5 Floppy (A:)", "3.5-Inch Floppy Disk", "", "", "Floppy rules!");
			CL_TreeItem *item2_32 = item2_3->get_node()->insert_item("DVD Drive (H:)", "CD Drive");

			item2_3->set_custom_height(30);
			item2_31->set_icon(new CL_Surface("floppy.png"), true);
			item2_32->set_icon(new CL_Surface("cd.png"), true);

			// Create a listbox using treeview
			listbox = new CL_TreeView(CL_Rect(10, 420, 530, 510), gui);
			listbox->show_header(false);
			listbox->show_root_decoration(false);
			for(int i = 1; i < 7; ++i)
				listbox->insert_item(CL_String::format("ListBox item %1", i));

			CL_Button *button_add = new CL_Button(CL_Rect(540, 420, 630, 440), "Add item", gui);
			CL_Button *button_remove = new CL_Button(CL_Rect(540, 445, 630, 465), "Remove item", gui);
			CL_Button *button_clear = new CL_Button(CL_Rect(540, 470, 630, 490), "Clear items", gui);

			label = new CL_Label(CL_Rect(10, 550, 530, 570), "Click the treeview", gui);

			CL_Button *button_quit = new CL_Button(CL_Rect(540, 550, 630, 570), "Quit", gui);

			CL_SlotContainer slots;

			slots.connect(guimanager.sig_paint(), this, &App::on_paint);
			slots.connect(display.sig_window_close(), this, &App::on_window_close);
			slots.connect(button_quit->sig_clicked(), this, &App::on_window_close);
			slots.connect(button_rootdecoration->sig_clicked(), this, &App::on_button_rootdecoration);
			slots.connect(button_header->sig_clicked(), this, &App::on_button_header);
			slots.connect(button_add->sig_clicked(), this, &App::on_button_add);
			slots.connect(button_remove->sig_clicked(), this, &App::on_button_remove);
			slots.connect(button_clear->sig_clicked(), this, &App::on_button_clear);
			slots.connect(treeview1->sig_item_clicked(), this, &App::on_treeview);
			slots.connect(treeview2->sig_item_clicked(), this, &App::on_treeview);
			slots.connect(listbox->sig_item_clicked(), this, &App::on_treeview);

			guimanager.run();
		}
		catch (CL_Error e)
		{
 			std::cout << e.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}
	
private:
	void on_window_close()
	{
		gui->quit();
	}
	
	void on_paint()
	{
		CL_Display::clear(CL_Color(240, 242, 244));
	}

	void on_button_rootdecoration()
	{
		treeview1->show_root_decoration(!treeview1->is_root_decoration_visible());
	}

	void on_button_header()
	{
		treeview1->show_header(!treeview1->is_header_visible());
	}

	void on_button_add()
	{
		listbox->insert_item("New item");
	}

	void on_button_remove()
	{
		CL_TreeNode *node = listbox->get_current_item();
		if(node == 0)
			CL_MessageBox::info("No item selected", gui);
		else
			listbox->remove_item(node);
	}

	void on_button_clear()
	{
		listbox->clear();
	}

	void on_treeview(const CL_TreeNode &node)
	{
		CL_TreeItem *treeitem = (CL_TreeItem *)node.get_component();
		label->set_text("You clicked: " + treeitem->get_text() + ".");
	}

	CL_TreeView *treeview1;
	CL_TreeView *listbox;
	CL_GUIManager *gui;
	CL_Label *label;
} app;
