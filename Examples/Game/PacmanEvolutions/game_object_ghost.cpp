
#include "precomp.h"
#include "game_object_ghost.h"
#include "game_world.h"
#include "map.h"
#include <stdlib.h>

GameObject_Ghost::GameObject_Ghost(int x, int y, GameWorld *world)
: GameObject_Moving(x, y, world)
{
	//sfx_ghost = CL_SoundBuffer("Game/sfx_ghost", world->resources);
	//sfx_ghost_dead = CL_SoundBuffer("Game/sfx_ghost_dead", world->resources);

	move_dir = rand()%4;
	red_ghost = false;
	set_speed(3.0);
}

bool GameObject_Ghost::turn(float time_elapsed)
{
	// pacman hitchecking:
	if (world->player && world->player->hit_check(x, y)) // HA! Got player... nam nam!
	{
		if (world->player->get_got_powerup()) // ARRRG! He's got a powerup!
		{
//			sfx_ghost_dead.play();
			world->score += 1000;
			return false;
		}
		else
			world->player->you_are_dead();
	}

	return move(time_elapsed);
}

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
	if (world->map.has_trail(dest_x, dest_y) && world->player)
	{
		float dx = fabs(dest_x - world->player->get_x());
		float dy = fabs(dest_y - world->player->get_y());
		float div = (dx+dy);
		if (div < 10 && div > 0.1)
		{
			float vol = 1/div;
			//CL_SoundBuffer_Session ses = sfx_ghost.prepare();
			//ses.set_volume(vol);
			//ses.play();
		}

		// follow trail if any:
		for (int i=0; i<3; i++)
			if (world->map.has_trail(dest_x+try_dirs_x[i], dest_y+try_dirs_y[i]))
				follow_trail = !world->player->get_got_powerup();
		// don't follow trail if player got power up or is dead
	}
	
//	if (follow_trail) CL_Console::write_line("ghost in follow trail mode");

	int dirs_avail = 0;
	for (int i=0; i<3; i++)
	{
		if (follow_trail)
		{
			if (world->map.has_trail(dest_x+try_dirs_x[i], dest_y+try_dirs_y[i])) dirs_avail++;
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
			if ((follow_trail && world->map.has_trail(dest_x+try_dirs_x[i], dest_y+try_dirs_y[i])) ||
				(!follow_trail && verify_move(try_dirs_x[i], try_dirs_y[i])))
			{
				if (new_dir == 0)
				{
					if (world->map.has_trail(dest_x, dest_y))
					{
						world->map.eat_trail(dest_x, dest_y);
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
