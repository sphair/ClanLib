
#include "precomp.h"
#include "program.h"
#include "game_world.h"
#include "age_ascii.h"
#include "age_2d.h"
#include "age_3d.h"
#include <ClanLib/application.h>

clan::Application clanapp(&Program::main);

int Program::main(const std::vector<std::string> &args)
{
	clan::SetupCore setup_core;
	clan::SetupDisplay setup_display;
	clan::SetupGL setup_gl;
	clan::SetupSound setup_sound;

	clan::DisplayWindowDescription window_desc;
	window_desc.set_title("Pacman Evolutions!");
	window_desc.set_size(clan::Size(1024, 768), true);

	clan::DisplayWindow window(window_desc);
	clan::Canvas canvas(window);
	clan::InputContext ic = window.get_ic();
	
	bool exit = false;
	clan::SlotContainer sc;
	sc.connect(window.sig_window_close(), [&exit] { exit = true; });

	GameWorld game_world;
	AgeAscii age_ascii(canvas, &game_world);
	Age2D age_2d(canvas, &game_world);
	Age3D age_3d(canvas, &game_world);

	//game_world.next_age();
	//game_world.next_age();

	clan::GameTime game_time;

	while (!exit)
	{
		game_time.update();

		game_world.button_left = ic.get_keyboard().get_keycode(clan::keycode_left);
		game_world.button_right = ic.get_keyboard().get_keycode(clan::keycode_right);
		game_world.button_up = ic.get_keyboard().get_keycode(clan::keycode_up);
		game_world.button_down = ic.get_keyboard().get_keycode(clan::keycode_down);
		game_world.button_respawn = ic.get_keyboard().get_keycode(clan::keycode_space);
		game_world.update(game_time);
		switch (game_world.age)
		{
		case GameWorld::age_ascii:
			age_ascii.render(canvas);
			break;
		case GameWorld::age_2d:
			age_2d.render(canvas, game_time.get_time_elapsed_ms());
			break;
		case GameWorld::age_3d:
			age_3d.render(canvas, game_time.get_time_elapsed_ms());
			break;
		}
		window.flip();
		clan::KeepAlive::process();
	}

	return 0;
}
