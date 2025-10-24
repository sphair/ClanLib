/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * Program WebSite: http://methane.sourceforge.net/index.html              *
 *                                                                         *
 ***************************************************************************/

//------------------------------------------------------------------------------
// Methane brothers main source file
//------------------------------------------------------------------------------
#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

#ifdef USE_GDI
#include <ClanLib/gdi.h>
#else
#include <ClanLib/gl.h>
#endif

#ifdef ENABLE_SOUND
#include <ClanLib/sound.h>
#include <ClanLib/mikmod.h>
#endif // ENABLE_SOUND

#include "global.h"

#include "doc.h"

//------------------------------------------------------------------------------
// Keyboard stuff
//------------------------------------------------------------------------------
static int LastKey = 0;

//------------------------------------------------------------------------------
// The Program
//------------------------------------------------------------------------------
class SuperMethaneBrothers
{
public:
	void on_button_press(const CL_InputEvent &key, const CL_InputState &state)
	{
		LastKey = key.id;
	}
        void on_window_close()
        {
                LastKey = CL_KEY_ESCAPE;
        }

	virtual int main(const std::vector<CL_String> &args)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		try
		{
#ifdef ENABLE_SOUND
			CL_SoundOutput sound_output(44100);
#endif // ENABLE_SOUND

			// Set the video mode
			CL_DisplayWindowDescription desc;
			desc.set_title("Super Methane Brothers");
			desc.set_size(CL_Size(SCR_WIDTH*2,SCR_HEIGHT*2), true);
			desc.set_allow_resize(true);
			CL_DisplayWindow window(desc);

			CMethDoc Game(window);

			// Connect the Window close event
			CL_Slot slot_quit = window.sig_window_close().connect(this, &SuperMethaneBrothers::on_window_close);

                        // Connect a keyboard handler to on_key_down()
			CL_Slot slot_keyboard_down = (window.get_ic().get_keyboard()).sig_key_down().connect(this, &SuperMethaneBrothers::on_button_press);

			Game.InitSoundDriver();
			Game.InitGame();
			Game.LoadScores();
			Game.StartGame();

			CL_GraphicContext gc = window.get_gc();

			CL_Font font(gc, "tahoma", 32);

//------------------------------------------------------------------------------
// The main game
//------------------------------------------------------------------------------

			int last_time = CL_System::get_time();

			int quit_flag = 0;
			int disable_scale_flag = 0;
			int full_screen_flag = 0;
			int on_options_screen = 1;
			int option_page = 0;
			int game_speed = 60;

			LastKey = 0;
			// Run until someone presses escape
			while (!quit_flag)
			{

//------------------------------------------------------------------------------
// Joystick Emulation and Video mode Control
//------------------------------------------------------------------------------

				JOYSTICK *jptr1;
				JOYSTICK *jptr2;
				jptr1 = &Game.m_GameTarget.m_Joy1;
				jptr2 = &Game.m_GameTarget.m_Joy2;

				if (LastKey)
				{
					if (LastKey == CL_KEY_ESCAPE) quit_flag = 1;

					if (on_options_screen)
					{
						if (LastKey == CL_KEY_SPACE)
						{
							option_page++;
							if (option_page==3)
							{
								on_options_screen = 0;	// Start the game
							}
						}
					}
				
					if (LastKey == CL_KEY_F9)
					{
						game_speed -= 20;
						if (game_speed <0) game_speed = 0;
					}
					if (LastKey == CL_KEY_F10)
					{
						game_speed += 20;
						if (game_speed >200) game_speed = 200;
					}

					jptr1->key = jptr2->key = ':';	// Fake key press (required for high score table)
					if ((LastKey >= CL_KEY_A) && (LastKey <= CL_KEY_Z)) jptr1->key = jptr2->key = LastKey - CL_KEY_A + 'A';
					if ((LastKey >= CL_KEY_0) && (LastKey <= CL_KEY_9)) jptr1->key = jptr2->key = LastKey - CL_KEY_0 + '0';
					if (LastKey == CL_KEY_SPACE) jptr1->key = jptr2->key = ' ';
					if (LastKey == CL_KEY_ENTER) jptr1->key = jptr2->key = 10;
					if (LastKey == CL_KEY_TAB)
					{
						Game.m_GameTarget.m_Game.TogglePuffBlow();
					}
					LastKey = 0;
				}

				if ((window.get_ic().get_keyboard()).get_keycode(CL_KEY_ESCAPE)) quit_flag = 1;
				if ((window.get_ic().get_keyboard()).get_keycode(CL_KEY_LEFT)) jptr1->left = 1; else jptr1->left = 0;
				if ((window.get_ic().get_keyboard()).get_keycode(CL_KEY_RIGHT)) jptr1->right = 1; else jptr1->right = 0;
				if ((window.get_ic().get_keyboard()).get_keycode(CL_KEY_UP)) jptr1->up = 1; else jptr1->up = 0;
				if ((window.get_ic().get_keyboard()).get_keycode(CL_KEY_DOWN)) jptr1->down = 1; else jptr1->down = 0;

				if ( ((window.get_ic().get_keyboard()).get_keycode(CL_KEY_LCONTROL)) || ((window.get_ic().get_keyboard()).get_keycode(CL_KEY_RCONTROL)) )
				{
					jptr1->fire = 1;
				}else	jptr1->fire = 0;


                                if ((window.get_ic().get_keyboard()).get_keycode(CL_KEY_A)) jptr2->left = 1; else jptr2->left = 0;
                                if ((window.get_ic().get_keyboard()).get_keycode(CL_KEY_D)) jptr2->right = 1; else jptr2->right = 0;
                                if ((window.get_ic().get_keyboard()).get_keycode(CL_KEY_W)) jptr2->up = 1; else jptr2->up = 0;
                                if ((window.get_ic().get_keyboard()).get_keycode(CL_KEY_S)) jptr2->down = 1; else jptr2->down = 0;

                                if ( ((window.get_ic().get_keyboard()).get_keycode(CL_KEY_LSHIFT)) || ((window.get_ic().get_keyboard()).get_keycode(CL_KEY_RSHIFT)) )
                                {
                                        jptr2->fire = 1;
                                }else   jptr2->fire = 0;

				// (CHEAT MODE ENABLED)
				if ((window.get_ic().get_keyboard()).get_keycode(CL_KEY_F11)) jptr1->next_level = 1; else jptr1->next_level = 0;

//------------------------------------------------------------------------------
// Do game main loop
//------------------------------------------------------------------------------
				gc.clear(CL_Colorf(0.0f,0.0f,0.0f));
				if (on_options_screen)
				{
					Game.DisplayOptions( window, font, option_page);
				}else
				{
					Game.MainLoop();
				}
//------------------------------------------------------------------------------
// Output the graphics
//------------------------------------------------------------------------------

				CL_DisplayMessageQueue::process();
				window.flip(1);

//------------------------------------------------------------------------------
// Control game speed
//------------------------------------------------------------------------------

				int time_diff = 0;
				do
				{
					int time_now = CL_System::get_time();
					time_diff = time_now - last_time;
					time_diff = game_speed - time_diff;
					if (time_diff > 50) CL_System::sleep(20);
				}while(time_diff > 0);
				last_time = last_time + game_speed; 
			}
			Game.SaveScores();

			Game.RemoveSoundDriver();
		}
		catch(CL_Exception& exception)
		{
			CL_Console::write_line("Exception caught:");
			CL_Console::write_line(exception.message);

			// Display the stack trace (if available)
			std::vector<CL_String> stacktrace = exception.get_stack_trace();
			int size = stacktrace.size();
			if (size > 0)
			{
				CL_Console::write_line("Stack Trace:");
				for (int cnt=0; cnt < size; cnt++)
				{
					CL_Console::write_line(stacktrace[cnt]);
				}
			}

			console.display_close_message();

			return -1;
		}

		return 0;
	}
};

// This is the Program class that is called by CL_ClanApplication
class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		// Initialize ClanLib base components
		CL_SetupCore setup_core;

		// Initialize the ClanLib display component
		CL_SetupDisplay setup_display;

#ifdef USE_GDI
		// Initilize the GDI drivers
		CL_SetupGDI setup_gdi;
#else
		// Initilize the OpenGL drivers
		CL_SetupGL setup_gl;
#endif

#ifdef ENABLE_SOUND
		CL_SetupSound setup_sound;
		CL_SetupMikMod setup_mikmod;
#endif // ENABLE_SOUND

		// Start the Application
		SuperMethaneBrothers app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

