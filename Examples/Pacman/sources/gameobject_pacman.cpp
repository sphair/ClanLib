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
#include "gameobject_pacman.h"
#include "world.h"

/////////////////////////////////////////////////////////////////////////////
// GameObject_Pacman construction:

GameObject_Pacman::GameObject_Pacman(int x, int y, World *world) :
	GameObject_Moving(x, y, world)
{
	spr_pacman = CL_Sprite("Game/spr_pacman", world->resources);
	
	sfx_start = CL_SoundBuffer("Game/sfx_start", world->resources);
	sfx_namnam = CL_SoundBuffer("Game/sfx_namnam", world->resources);
	sfx_powerup = CL_SoundBuffer("Game/sfx_powerup", world->resources);
	sfx_dead = CL_SoundBuffer("Game/sfx_dead", world->resources);

	move_dir = 1;
	wanted_dir = -1;
	anim_pos = 0;
	anim_dir = 1;

	i_am_dead = false;
	got_powerup = false;

	set_speed(4.0);
	
	sfx_start.play();

	slots.connect(CL_Keyboard::sig_key_down(), this, &GameObject_Pacman::on_key_down);
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Pacman attributes:

/////////////////////////////////////////////////////////////////////////////
// GameObject_Pacman operations:

void GameObject_Pacman::show(int view_x, int view_y, CL_GraphicContext *gc)
{
	int width = world->map->get_tile_width() - 6;
	int height = world->map->get_tile_height() - 6;
	int anim_length = spr_pacman.get_frame_count() / 4;

	spr_pacman.set_frame(anim_pos+anim_length*move_dir);
	spr_pacman.draw(
		(int) (x*width)-view_x,
		(int) (y*height)-view_y,
		gc);
}

bool GameObject_Pacman::turn(float time_elapsed)
{
	if (got_powerup && CL_System::get_time() - powerup_starttime>20000)
	{
		got_powerup = false;
	}

	if (i_am_dead) 
	{
		sfx_dead.play();

		world->score -= 5000;
		if (world->score < 0) world->score = 0;
		return false;
	}	

	int anim_length = spr_pacman.get_frame_count() / 4;

	anim_pos += anim_dir;
	if (anim_pos >= anim_length-1 || anim_pos == 0) anim_dir*=-1;

	return move(time_elapsed);
}

bool GameObject_Pacman::hit_check(float hit_x, float hit_y)
{
	if (fabs(x-hit_x) < 0.25 && fabs(y-hit_y) < 0.25) return true;
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// GameObject_Pacman implementation:

bool GameObject_Pacman::event_reached_dest()
{
	if (world->map->get_tile_type(dest_x, dest_y) == Map::tile_egg) // std
	{
		world->score += 125;
		world->map->eat_egg(dest_x, dest_y);
		sfx_namnam.play();
	}
	if (world->map->get_tile_type(dest_x, dest_y) == Map::tile_powerup) // powerup
	{
		world->score += 500;
		world->map->eat_egg(dest_x, dest_y);
		got_powerup = true;
		powerup_starttime = CL_System::get_time();
		sfx_powerup.play();
	}
	world->map->leave_trail(dest_x, dest_y);
/*
	if (move_dir < 2)
	{
		if (CL_Keyboard::get_keycode(CL_KEY_UP) && verify_move(0, -1))
			move_dir = 2;

		if (CL_Keyboard::get_keycode(CL_KEY_DOWN) && verify_move(0, 1))
			move_dir = 3;
	}
	else
	{
		if (CL_Keyboard::get_keycode(CL_KEY_LEFT) && verify_move(-1, 0))
			move_dir = 0;

		if (CL_Keyboard::get_keycode(CL_KEY_RIGHT) && verify_move(1, 0))
			move_dir = 1;
	}
*/
	
	switch (wanted_dir)
	{
	case -1: break;

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

void GameObject_Pacman::on_key_down(const CL_InputEvent &key)
{
	switch (move_dir)
	{
	case 0:
		if (key.id == CL_KEY_RIGHT)
		{
			move_dir = 1;
			wanted_dir = -1;
			do_move(1,0);
		}
		break;

	case 1:
		if (key.id == CL_KEY_LEFT)
		{
			move_dir = 0;
			wanted_dir = -1;
			do_move(-1,0);
		}
		break;

	case 2:
		if (key.id == CL_KEY_DOWN)
		{
			move_dir = 3;
			wanted_dir = -1;
			do_move(0,1);
		}
		break;

	case 3:
		if (key.id == CL_KEY_UP)
		{
			move_dir = 2;
			wanted_dir = -1;
			do_move(0,-1);
		}
		break;
	}

	if (move_dir < 2)
	{
		if (key.id == CL_KEY_UP)
			wanted_dir = 2;

		if (key.id == CL_KEY_DOWN)
			wanted_dir = 3;
	}
	else
	{
		if (key.id == CL_KEY_LEFT)
			wanted_dir = 0;

		if (key.id == CL_KEY_RIGHT)
			wanted_dir = 1;
	}
}
