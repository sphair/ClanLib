
#include "precomp.h"
#include "game_object_moving.h"
#include "game_world.h"
#include "map.h"

GameObject_Moving::GameObject_Moving(int x, int y, GameWorld *world)
: GameObject(world), x(float(x)), y(float(y)), dest_x(x), dest_y(y), speed(1.0f)
{
}

bool GameObject_Moving::move(float time_elapsed)
{
	bool big_move = true;

	int bug = 0;

	do
	{
		bug++;
		if (bug > 10) break; // something is broken!

		big_move = true;
		float delta_x = dest_x-x;

		if (fabs(delta_x) < time_elapsed*speed)
		{
			x = (float)dest_x;
		}
		else if (delta_x>0)
		{
			x += time_elapsed*speed;
		}
		else
		{
			x -= time_elapsed*speed;
		}

		float delta_y = dest_y-y;
		if (fabs(delta_y) < time_elapsed*speed)
		{
			y = (float)dest_y;
		}
		else if (delta_y>0)
		{
			y += time_elapsed*speed;
		}
		else
		{
			y -= time_elapsed*speed;
		}

		if (
			fabs(delta_x) < time_elapsed*speed &&
			fabs(delta_y) < time_elapsed*speed)
		{
			if (delta_x < delta_y) time_elapsed -= fabs(delta_y)/speed;
			else time_elapsed -= fabs(delta_x)/speed;
			big_move = !event_reached_dest();
		}
	} while (!big_move);

	return true;
}

bool GameObject_Moving::verify_move(int dx, int dy)
{
	return !world->map.has_wall(dest_x + dx, dest_y + dy);
}

bool GameObject_Moving::do_move(int dx, int dy)
{
	if (verify_move(dx,dy) == false) return false;

	dest_x += dx;
	dest_y += dy;

	return true;
}
