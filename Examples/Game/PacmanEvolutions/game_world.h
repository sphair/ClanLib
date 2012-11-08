
#pragma once

#include "elapsed_timer.h"
#include "framerate_counter.h"
#include "map.h"
#include "game_object_pacman.h"
#include "game_object_ghost.h"

class GameWorld
{
public:
	GameWorld();
	void update();
	void next_age();

	ElapsedTimer timer;
	FramerateCounter fps_counter;

	CL_SharedPtr<GameObject_Pacman> player;
	std::vector<CL_SharedPtr<GameObject_Ghost> > ghosts;
	Map map;
	int score;

	enum EvolutionAge
	{
		age_ascii,
		age_2d,
		age_3d
	};

	EvolutionAge age;

	bool button_left;
	bool button_right;
	bool button_up;
	bool button_down;
	bool button_respawn;

	float total_time_elapsed;
	CL_String text_fps;
	CL_String text_score;
	CL_String text_message_bottom;
	CL_String text_message_center0;
	CL_String text_message_center1;
	bool blow_up_text;
};
