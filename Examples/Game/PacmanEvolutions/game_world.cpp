
#include "precomp.h"
#include "game_world.h"
#include "game_object_pacman.h"
#include "game_object_ghost.h"

GameWorld::GameWorld()
: score(0), age(age_ascii), button_left(false), button_right(false), button_up(false), button_down(false), button_respawn(false), total_time_elapsed(0.0f)
{
	player = CL_SharedPtr<GameObject_Pacman>(new GameObject_Pacman(map.get_width()/2, map.get_height()/2, this));
	ghosts.push_back(CL_SharedPtr<GameObject_Ghost>(new GameObject_Ghost(1, 1, this)));
	ghosts.push_back(CL_SharedPtr<GameObject_Ghost>(new GameObject_Ghost(map.get_width()-2, 1, this)));
	ghosts.push_back(CL_SharedPtr<GameObject_Ghost>(new GameObject_Ghost(1, map.get_height()-2, this)));
	ghosts.push_back(CL_SharedPtr<GameObject_Ghost>(new GameObject_Ghost(map.get_width()-2, map.get_height()-2, this)));
}

void GameWorld::update()
{
	float time_elapsed = timer.update();
	total_time_elapsed += time_elapsed;
	if (player)
	{
		bool destroy_object = !player->turn(time_elapsed);
		if (destroy_object)
			player.reset();
	}

	for (size_t i = 0; i < ghosts.size(); )
	{
		bool destroy_object = !ghosts[i]->turn(time_elapsed);
		if (destroy_object)
			ghosts.erase(ghosts.begin() + i);
		else
			i++;
	}

	fps_counter.frame_shown();

	text_score = cl_format("%1 bonus bananas", score);
	text_fps = cl_format("%1 fps", fps_counter.get_framerate());

	if (!player)
	{
		text_message_center0 = "YOU ARE DEAD";
		text_message_center1 = "Press space to restart";
	}

	if (total_time_elapsed < 3.0f)
	{
		text_message_bottom = "Welcome to Pacman Evolutions";
	}
	else
	{
		blow_up_text = true;
	}

	bool respawn = !player && button_respawn;
	if (respawn)
	{
		player = CL_SharedPtr<GameObject_Pacman>(new GameObject_Pacman(map.get_width()/2, map.get_height()/2, this));
		blow_up_text = true;
	}
}

void GameWorld::next_age()
{
	if (age == age_ascii)
		age = age_2d;
	else if (age == age_2d)
		age = age_3d;
}
