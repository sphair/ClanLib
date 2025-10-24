#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/gl.h>
#include <ClanLib/guistylesilver.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

class GUICredit
{
public:
	GUICredit(CL_ResourceManager *resources, CL_Component *parent, CL_Deck *deck)
	: component_manager("credit", resources, parent), deck(deck)
	{
		slot_menu = ((CL_Button *) component_manager.get_component("cmd_menu"))->sig_clicked().connect(
			this, &GUICredit::on_menu);
	}

public:
	void on_menu()
	{
		deck->swap("menu");
	}

	CL_Component *get_component()
	{
		return component_manager.get_component("credit");
	}

	CL_ComponentManager component_manager;
	CL_Deck *deck;
	CL_Slot slot_menu;
};

class GUIOption
{
public:
	GUIOption(CL_ResourceManager *resources, CL_Component *parent, CL_Deck *deck)
	: component_manager("option", resources, parent), deck(deck)
	{
		slot_menu = ((CL_Button *) component_manager.get_component("cmd_menu"))->sig_clicked().connect(
			this, &GUIOption::on_menu);
	}

public:
	void on_menu()
	{
		deck->swap("menu");
	}

	CL_Component *get_component()
	{
		return component_manager.get_component("option");
	}

	CL_ComponentManager component_manager;
	CL_Deck *deck;
	CL_Slot slot_menu;
};

class GUIMainMenu
{
public:
	GUIMainMenu(CL_ResourceManager *resources, CL_Component *parent, CL_Deck *deck)
	: component_manager("menu", resources, parent), deck(deck)
	{
		slot_option = ((CL_Button *) component_manager.get_component("cmd_option"))->sig_clicked().connect(
			this, &GUIMainMenu::on_option);
		slot_credit = ((CL_Button *) component_manager.get_component("cmd_credit"))->sig_clicked().connect(
			this, &GUIMainMenu::on_credit);
	}

public:
	void on_option()
	{
		deck->swap("option");
	}

	void on_credit()
	{
		deck->swap("credit");
	}

	CL_Component *get_component()
	{
		return component_manager.get_component("menu");
	}

	CL_ComponentManager component_manager;
	CL_Deck *deck;
	CL_Slot slot_option;
	CL_Slot slot_credit;
};

class GUIDeck : public CL_ClanApplication
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
			CL_SetupGUI setup_gui;
			CL_SetupGL setup_gl;
			CL_SetupDisplay setup_display;

			CL_DisplayWindow window("GUIScript", 640, 480);

			CL_ResourceManager resources("../../Resources/GUIStyleSilver/gui.xml");
			CL_ResourceManager extra_resources("guistuff.xml");
			resources.add_resources(extra_resources);

			CL_StyleManager_Silver style(&resources);
			CL_GUIManager gui(&style);
			gui_manager = &gui;

			// instantiate a deck object for the menu.
			deck = new CL_Deck();

			menu = new GUIMainMenu(&resources, gui_manager, deck);
			option = new GUIOption(&resources, gui_manager, deck);
			credit = new GUICredit(&resources, gui_manager, deck);

			deck->add("menu", menu->get_component());
			deck->add("option", option->get_component());
			deck->add("credit", credit->get_component());

			CL_Slot slot_paint = gui.sig_paint().connect(this, &GUIDeck::on_gui_paint);

			// Connect the Window close event
			CL_Slot slot_quit = window.sig_window_close().connect(this, &GUIDeck::on_window_close);
			quit = false;

			while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE) && !quit)
			{
				gui.show();

				CL_Display::flip();
				CL_System::keep_alive(10);
			}

			// delete menu deck object.
			
			delete menu;
			delete option;
			delete credit;
			delete deck;
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
		quit = true;
	}

	void on_gui_paint()
	{
		CL_Display::clear(CL_Color(150, 150, 150));
	}

	CL_GUIManager *gui_manager;
	CL_Deck *deck; // the menu deck to manage the menu pages.

	GUIMainMenu *menu;
	GUIOption *option;
	GUICredit *credit;

	bool quit;
} app;
