
#pragma once

#include "game_object_moving.h"

class GameObject_Ghost : public GameObject_Moving
{
public:
	GameObject_Ghost(int x, int y, GameWorld *world);
	virtual bool turn(float time_elapsed);
	bool is_red_ghost() const { return red_ghost; }

private:
	virtual bool event_reached_dest();

	int move_dir;
	bool red_ghost;
};
