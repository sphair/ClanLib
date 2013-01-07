
#include "precomp.h"
#include "game_object_pacman.h"
#include "game_world.h"
#include "map.h"

GameObject_Pacman::GameObject_Pacman(int x, int y, GameWorld *world)
: GameObject_Moving(x, y, world)
{
	move_dir = 1;
	wanted_dir = -1;

	i_am_dead = false;
	got_powerup = false;

	set_speed(4.0);
}

bool GameObject_Pacman::turn(float time_elapsed)
{
	check_input();

	if (got_powerup && clan::System::get_time() - powerup_starttime>20000)
	{
		got_powerup = false;
	}

	if (i_am_dead) 
	{
//		sfx_dead.play();

		world->score -= 5000;
		if (world->score < 0) world->score = 0;
		return false;
	}	

	return move(time_elapsed);
}

bool GameObject_Pacman::hit_check(float hit_x, float hit_y)
{
	return (fabs(x-hit_x) < 0.25 && fabs(y-hit_y) < 0.25);
}

bool GameObject_Pacman::event_reached_dest()
{
	if (world->map.get_tile(dest_x, dest_y) == Map::flag_egg) // std
	{
		world->score += 125;
		world->map.eat_egg(dest_x, dest_y);
//		sfx_namnam.play();
	}
	if (world->map.get_tile(dest_x, dest_y) == Map::flag_powerup) // powerup
	{
		world->score += 500;
		world->map.eat_egg(dest_x, dest_y);
		got_powerup = true;
		powerup_starttime = clan::System::get_time();
//		sfx_powerup.play();

		world->next_age();
	}
	world->map.leave_trail(dest_x, dest_y);
	
	switch (wanted_dir)
	{
	case -1:
		break;

	case 0:
		if (verify_move(-1, 0)) 
		{
			move_dir = 0;
			wanted_dir = -1;
		}
		break;

	case 1:
		if (verify_move(1, 0)) 
		{
			move_dir = 1;
			wanted_dir = -1;
		}
		break;

	case 2:
		if (verify_move(0, -1)) 
		{
			move_dir = 2;
			wanted_dir = -1;
		}
		break;

	case 3:
		if (verify_move(0, 1)) 
		{
			move_dir = 3;
			wanted_dir = -1;
		}
		break;
	}


	switch (move_dir)
	{
	case 0:
		do_move(-1, 0);
		break;

	case 1:
		do_move(1, 0);
		break;

	case 2:
		do_move(0, -1);
		break;

	case 3:
		do_move(0, 1);
		break;
	}

	if (x == dest_x && y == dest_y) return false; // Damn - we're facing a wall!

	return true;
}

void GameObject_Pacman::check_input()
{
	switch (move_dir)
	{
	case 0:
		if (world->button_right)
		{
			move_dir = 1;
			wanted_dir = -1;
			do_move(1,0);
		}
		break;

	case 1:
		if (world->button_left)
		{
			move_dir = 0;
			wanted_dir = -1;
			do_move(-1,0);
		}
		break;

	case 2:
		if (world->button_down)
		{
			move_dir = 3;
			wanted_dir = -1;
			do_move(0,1);
		}
		break;

	case 3:
		if (world->button_up)
		{
			move_dir = 2;
			wanted_dir = -1;
			do_move(0,-1);
		}
		break;
	}

	if (move_dir < 2)
	{
		if (world->button_up)
			wanted_dir = 2;

		if (world->button_down)
			wanted_dir = 3;
	}
	else
	{
		if (world->button_left)
			wanted_dir = 0;

		if (world->button_right)
			wanted_dir = 1;
	}
}
