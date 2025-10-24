#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

#include "MenuSystem/NS_MenuManager.h"

#include "ZZ_Theme/stylemanager_zz.h"

class ZZ_ThemeExample : public CL_ClanApplication
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
            CL_SetupGL setup_gl;
            CL_SetupDisplay setup_display;
            CL_SetupGUI setup_gui;

            bool full = false;
            if (argc > 1)
            {
                if (std::string(argv[1]) == "-fullscreen")
                    full = true;
            }
			std::cout << ".creating main window" << std::endl;
            CL_DisplayWindow window("New Supaplex Theme example", 800, 600, full);

			std::cout << ".loading resources" << std::endl;
            CL_ResourceManager resources("resources.xml", false);
            CL_StyleManager_ZZ style(&resources);

            CL_Display::clear(CL_Color::black);
            {
                CL_Font fnt("Window/font", &resources);
                std::string str("Loading resources: please wait...");
                fnt.draw(CL_Display::get_width() / 2 - fnt.get_width(str)/2,
                    CL_Display::get_height() / 2, str);

                CL_Display::flip();
				std::cout << ".initializing\n";
				std::cout << ".load all\n";
                resources.load_all();
            }

            NS_MenuManager menu_manager(resources, style);
            menu_manager.run();
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

