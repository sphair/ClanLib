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
#include "gameobject_ghost.h"
#include "world.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Ghost construction:

GameObject_Ghost::GameObject_Ghost(int x, int y, World *world) :
	GameObject_Moving(x, y, world)
{
	spr_ghost = CL_Sprite("Game/spr_ghost", world->resources);

	sfx_ghost = CL_SoundBuffer("Game/sfx_ghost", world->resources);
	sfx_ghost_dead = CL_SoundBuffer("Game/sfx_ghost_dead", world->resources);

	move_dir = rand()%4;
	
	red_ghost = false;

	int anim_length = spr_ghost.get_frame_count() / 2;

	anim_pos = rand()%anim_length;
	anim_dir = rand()%2;
	if (anim_dir == 0) anim_dir = -1;

	if (anim_pos == 0) anim_dir = 1;
	if (anim_pos == anim_length-1) anim_dir = -1;

	set_speed(3.0);
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Ghost operations:

void GameObject_Ghost::show(int center_x, int center_y, CL_GraphicContext *gc)
{
	int width = world->map->get_tile_width()-6;
	int height = world->map->get_tile_height()-6;
	
	if (world->player == NULL)
	{
		spr_ghost.set_frame(anim_pos);
		spr_ghost.set_alpha(0.5f);
		spr_ghost.draw(
			(int) (x*width)-center_x,
			(int) (y*height)-center_y,
			gc);

		return;
	}

	if ((world->player->get_got_powerup() && int(x+y)%2==0) ||
		(!world->player->get_got_powerup() && red_ghost))
	{
		spr_ghost.set_frame(anim_pos + 6);
		spr_ghost.set_alpha(0.5f);
		spr_ghost.draw(
			(int) (x*width)-center_x,
			(int) (y*height)-center_y,
			gc);
	}
	else
	{
		spr_ghost.set_frame(anim_pos);
		spr_ghost.set_alpha(0.5f);
		spr_ghost.draw(
			(int) (x*width)-center_x,
			(int) (y*height)-center_y,
			gc);
	}
}

bool GameObject_Ghost::turn(float time_elapsed)
{
	int anim_length = spr_ghost.get_frame_count() / 2;

	anim_pos += anim_dir;
	if (anim_pos == 0 || anim_pos == anim_length-1) anim_dir *= -1;

	// pacman hitchecking:
	if (world->player != NULL)
	{
		if (world->player->hit_check(x, y)) // HA! Got player... nam nam!
		{
			if (world->player->get_got_powerup()) // ARRRG! He's got a powerup!
			{
				sfx_ghost_dead.play();
				world->score += 1000;
				return false;
			}
			else
				world->player->you_are_dead();
		}
	}

	return move(time_elapsed);
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Ghost implementation:

bool GameObject_Ghost::event_reached_dest()
{
	int try_dirs_x[3];
	int try_dirs_y[3];
	int try_dirs_id[3];

	switch (move_dir)
	{
	case 0: // left
		try_dirs_x[0] = 0; try_dirs_y[0]=-1; try_dirs_id[0] = 2;
		try_dirs_x[1] =-1; try_dirs_y[1]= 0; try_dirs_id[1] = 0;
		try_dirs_x[2] = 0; try_dirs_y[2]= 1; try_dirs_id[2] = 3;
		break;

	case 1: // right
		try_dirs_x[0] = 0; try_dirs_y[0]=-1; try_dirs_id[0] = 2;
		try_dirs_x[1] = 1; try_dirs_y[1]= 0; try_dirs_id[1] = 1;
		try_dirs_x[2] = 0; try_dirs_y[2]= 1; try_dirs_id[2] = 3;
		break;

	case 2: // up
		try_dirs_x[0] =-1; try_dirs_y[0]= 0; try_dirs_id[0] = 0;
		try_dirs_x[1] = 0; try_dirs_y[1]=-1; try_dirs_id[1] = 2;
		try_dirs_x[2] = 1; try_dirs_y[2]= 0; try_dirs_id[2] = 1;
		break;

	case 3: // down
		try_dirs_x[0] =-1; try_dirs_y[0]= 0; try_dirs_id[0] = 0;
		try_dirs_x[1] = 0; try_dirs_y[1]= 1; try_dirs_id[1] = 3;
		try_dirs_x[2] = 1; try_dirs_y[2]= 0; try_dirs_id[2] = 1;
		break;
	}

	bool follow_trail = false;
	if (world->map->get_tile_type(dest_x, dest_y) == Map::tile_trail)
	{
		if (world->player != NULL)
		{
			float dx = fabs(dest_x - world->player->get_x());
			float dy = fabs(dest_y - world->player->get_y());
			float div = (dx+dy);
			if (div < 10 && div > 0.1)
			{
				float vol = 1/div;
				CL_SoundBuffer_Session ses = sfx_ghost.prepare();
				ses.set_volume(vol);
				ses.play();
			}
		}

		// follow trail if any:
		for (int i=0; i<3; i++)
			if (world->map->get_tile_type(dest_x+try_dirs_x[i], dest_y+try_dirs_y[i]) == Map::tile_trail)
				if (world->player != NULL)
					follow_trail = !world->player->get_got_powerup();
		// don't follow trail if player got power up or is dead
	}
	
//	if (follow_trail) cout << "ghost in follow trail mode" << endl;

	int dirs_avail = 0;
	for (int i=0; i<3; i++)
	{
		if (follow_trail)
		{
			if (world->map->get_tile_type(dest_x+try_dirs_x[i], dest_y+try_dirs_y[i]) == Map::tile_trail) dirs_avail++;
		}
		else if (verify_move(try_dirs_x[i], try_dirs_y[i])) dirs_avail++;
	}

	if (dirs_avail == 0) // Dead end - turn around
	{
		do_move(try_dirs_x[1]*(-1), try_dirs_y[1]*(-1));
		switch (move_dir)
		{
			case 0: move_dir=1; break;
			case 1: move_dir=0; break;
			case 2: move_dir=3; break;
			case 3: move_dir=2; break;
		}
	}
	else
	{
		int new_dir = rand()%dirs_avail;
		for (int i=0; i<3; i++)
		{
			if ((follow_trail && world->map->get_tile_type(dest_x+try_dirs_x[i], dest_y+try_dirs_y[i]) == Map::tile_trail) ||
				(!follow_trail && verify_move(try_dirs_x[i], try_dirs_y[i])))
			{
				if (new_dir == 0)
				{
					if (world->map->get_tile_type(dest_x, dest_y) == Map::tile_trail)
					{
						world->map->eat_trail(dest_x, dest_y);
						red_ghost = true;
					}
					else
					{
						red_ghost = false;
					}

					do_move(try_dirs_x[i], try_dirs_y[i]);
					move_dir = try_dirs_id[i];
				}
				new_dir--;
			}
		}
	}

	return true;
}
