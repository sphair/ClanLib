
#pragma once

#include "game_object.h"

class GameObject_Moving : public GameObject
{
public:
	GameObject_Moving(int x, int y, GameWorld *world);
	virtual float get_x() { return x; }
	virtual float get_y() { return y; }

protected:
	void set_speed(float new_speed) { speed = new_speed; }
	void set_destination(int x, int y) { dest_x = x; dest_y = y; }
	bool move(float time_elapsed);
	virtual bool event_reached_dest() = 0;

	bool verify_move(int dx, int dy);
	bool do_move(int dx, int dy);

	float x, y;
	int dest_x, dest_y;
	float speed;
};
