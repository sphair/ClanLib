
#include "precomp.h"
#include "program.h"
#include "game_world.h"
#include "age_ascii.h"
#include "age_2d.h"
#include "age_3d.h"
#include <ClanLib/application.h>

CL_ClanApplication clanapp(&Program::main);

int Program::main(const std::vector<CL_String> &args)
{
	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupGL setup_gl;
	CL_SetupSound setup_sound;
	CL_SetupVorbis setup_vorbis;

	CL_DisplayWindowDescription window_desc;
	window_desc.set_title("Pacman Evolutions!");
	window_desc.set_size(CL_Size(1024, 768), true);

	CL_DisplayWindow window(window_desc);
	CL_GraphicContext &gc = window.get_gc();
	CL_InputContext ic = window.get_ic();
	
	bool exit = false;
	CL_Slot slot = window.sig_window_close().connect_functor([&exit] { exit = true; });

	try
	{
		GameWorld game_world;
		AgeAscii age_ascii(gc, &game_world);
		Age2D age_2d(gc, &game_world);
		Age3D age_3d(gc, &game_world);

		while (!exit)
		{
			game_world.button_left = ic.get_keyboard().get_keycode(CL_KEY_LEFT);
			game_world.button_right = ic.get_keyboard().get_keycode(CL_KEY_RIGHT);
			game_world.button_up = ic.get_keyboard().get_keycode(CL_KEY_UP);
			game_world.button_down = ic.get_keyboard().get_keycode(CL_KEY_DOWN);
			game_world.button_respawn = ic.get_keyboard().get_keycode(CL_KEY_SPACE);
			game_world.update();
			switch (game_world.age)
			{
			case GameWorld::age_ascii:
				age_ascii.render(gc);
				break;
			case GameWorld::age_2d:
				age_2d.render(gc);
				break;
			case GameWorld::age_3d:
				age_3d.render(gc);
				break;
			}
			window.flip();
			CL_KeepAlive::process();
		}
	}
	catch (CL_Exception &e)
	{
		MessageBox(window.get_hwnd(), CL_StringHelp::utf8_to_ucs2(e.get_message_and_stack_trace()).c_str(), L"Unhandled Exception", MB_OK|MB_ICONERROR);
	}

	return 0;
}
