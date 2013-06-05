
#pragma once

#include "game_object_moving.h"

class GameObject_Pacman : public GameObject_Moving
{
public:
	GameObject_Pacman(int x, int y, GameWorld *world);

	virtual bool turn(float time_elapsed);
	virtual bool hit_check(float x, float y);

	bool get_got_powerup() { return got_powerup; }
	void you_are_dead() { i_am_dead = true; }

	int get_move_dir() const { return move_dir; }

private:
	virtual bool event_reached_dest();
	void check_input();

	int move_dir;
	int wanted_dir;
	bool i_am_dead;
	bool got_powerup;
	clan::ubyte64 powerup_starttime;
};
