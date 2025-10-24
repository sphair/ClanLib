#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/gui.h>
#include <ClanLib/guistylesilver.h>

#include "SpriteManager.h"
#include "Game.h"

#include <vector>

class PilotsPuzzle : public CL_ClanApplication
{
public:
	virtual int main(int argc, char **argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();
		
		quit = false;
		is_showing_hints = false;
		is_new_game = true;

		try
		{
			// Initialize ClanLib base components
			CL_SetupCore setup_core;

			// Initialize the ClanLib display component
			CL_SetupDisplay setup_display;

			// Initilize the OpenGL drivers
			CL_SetupGL setup_gl;

			// Set a videomode - 640x480
			CL_DisplayWindow window("PilotsPuzzle Example", 640, 480);

			// Connect the Window close event
			CL_Slot slot_quit = window.sig_window_close().connect(this, &PilotsPuzzle::OnWindowClose);

			// Connect a keyboard handler to on_key_up()
			CL_Slot slot_input_up = (window.get_ic()->get_keyboard()).sig_key_up().connect(this, &PilotsPuzzle::OnInputDown);

#ifdef __APPLE__
			CL_ResourceManager gui_resources("GUIStyleSilver/gui.xml");
#else
			CL_ResourceManager gui_resources("../../Resources/GUIStyleSilver/gui.xml");
#endif
			CL_StyleManager_Silver style(&gui_resources);
			CL_GUIManager gui(&style);
			
			CL_Button btn_new_game(CL_Point(10, 10), "New Game", &gui);
			CL_Button btn_show_hints(CL_Point(10, 40), "Show Hints On/Off", &gui);
			CL_Slot slot_btn_hint = btn_show_hints.sig_clicked().connect(this, &PilotsPuzzle::OnShowHints);
			CL_Slot slot_btn_new_game = btn_new_game.sig_clicked().connect(this, &PilotsPuzzle::OnNewGame);

            CL_ResourceManager resources("resources.xml", false);
			CL_Font font("font", &resources);
			SpriteManager sprite_manager(resources);

			Game game(5, 5, sprite_manager);
			// Run until someone presses escape
			while (!quit)
			{
				if (is_new_game)
				{
					is_new_game = false;
					game = Game(5, 5, sprite_manager);
				}
				CL_Display::clear();

				gui.show();
				game.Draw(is_showing_hints ? Game::SHOW_HINTS : Game::NO_HINTS);
				if (game.IsSolved())
					font.draw(10, 70, "Game status: solved");
				else
					font.draw(10, 70, "Game status: not solved");

				CL_Display::flip();
				CL_System::keep_alive();
			}
		}
		catch(CL_Error error)
		{
			std::cout << "Exception caught : " << error.message.c_str() << std::endl;			

			// Display console close message and wait for a key
			console.display_close_message();

			return -1;
		}

		return 0;
	}

private:
	bool quit;
	bool is_showing_hints;
	bool is_new_game;

	void OnInputDown(const CL_InputEvent &key)
	{
		if(key.id == CL_KEY_ESCAPE)
			quit = true;
	}

	void OnShowHints()
	{
		is_showing_hints = !is_showing_hints;
	}

	void OnNewGame()
	{
		is_new_game = true;
	}

	void OnWindowClose()
	{
		quit = true;
	}
} my_app;
