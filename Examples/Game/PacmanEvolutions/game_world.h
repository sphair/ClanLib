
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

	std::shared_ptr<GameObject_Pacman> player;
	std::vector<std::shared_ptr<GameObject_Ghost> > ghosts;
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
	std::string text_fps;
	std::string text_score;
	std::string text_message_bottom;
	std::string text_message_center0;
	std::string text_message_center1;
	bool blow_up_text;
};
