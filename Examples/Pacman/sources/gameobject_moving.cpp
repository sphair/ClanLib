/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include "precomp.h"
#include "gameobject_moving.h"
#include "world.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Moving construction:

GameObject_Moving::GameObject_Moving(int x, int y, World *world)
: world(world), x(float(x)), y(float(y)), dest_x(x), dest_y(y), speed(1.0f)
{
}

GameObject_Moving::~GameObject_Moving()
{
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Moving attributes:

	
/////////////////////////////////////////////////////////////////////////////
// GameObject_Moving operations:

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

/////////////////////////////////////////////////////////////////////////////
// GameObject_Moving implementation:

bool GameObject_Moving::verify_move(int dx, int dy)
{
	if (world->map->get_tile_type(dest_x + dx, dest_y + dy) != Map::tile_wall) return true;
	return false;
}

bool GameObject_Moving::do_move(int dx, int dy)
{
	if (verify_move(dx,dy) == false) return false;

	dest_x += dx;
	dest_y += dy;

	return true;
}
