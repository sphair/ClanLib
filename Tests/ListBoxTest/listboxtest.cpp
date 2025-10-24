#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/guistylesilver.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class TestItem : public	CL_ListItem
{
private:
	std::string key;

public:
	TestItem(std::string k, std::string v)
	{
		key = k;
		str = v;
	}

	std::string getKey()
	{
		return key;
	}
};

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
			CL_SetupCore::init();
			CL_SetupDisplay::init();
			CL_SetupGL::init();
			CL_SetupGUI::init();

			CL_DisplayWindow window("GUITest example", 640, 480);

			// Create the GUI using the default style
			CL_ResourceManager gui_resources("../../Resources/GUIStyleSilver/gui.xml", false);
			CL_StyleManager_Silver style(&gui_resources);
			CL_GUIManager gui(&style);

			TestItem item1("key01", "value01");
			TestItem item2("key02", "value02");
			TestItem item3("key03", "value03");

			CL_ListBox list(&gui);
			list.insert_item(&item1);
			list.insert_item(&item2);
			list.insert_item(&item3);

			// this  one prints "item1k key01"
			std::cout << "item1k " << item1.getKey() << std::endl;
			// this one prints "item1v value01"
			std::cout << "item1v " << item1.str << std::endl;

			// then I get the first item
			TestItem *item = (TestItem *)list.get_item(0);

			// this one should print "itemk key01", but instead, it prints "itemk value02"
			std::cout << "itemk " << item->getKey() << std::endl;
			// however, the value is alright, this line prints "itemv value01"
			std::cout << "itemv " << item->str << std::endl;

			// Main loop
			while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
			{
				CL_Display::clear();
				gui.show();

				CL_System::keep_alive(10);
				CL_Display::flip();
			}

			CL_SetupGL::deinit();
			CL_SetupGUI::deinit();
			CL_SetupDisplay::deinit();
			CL_SetupCore::deinit();
		}
		catch (CL_Error e)
		{
 			std::cout << e.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}
} app;
