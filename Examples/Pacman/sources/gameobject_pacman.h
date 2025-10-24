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

#ifndef header_gameobject_pacman
#define header_gameobject_pacman

#if _MSC_VER > 1000
#pragma once
#endif

#include "gameobject_moving.h"

class GameObject_Pacman : public GameObject_Moving
{
//!Construction:
public:
	//: Construct a pacman at position (x,y).
	GameObject_Pacman(int x, int y, World *world);

//!Attributes:
public:
	//: Returns true if pacman got the powerup.
	bool get_got_powerup() { return got_powerup; }

//!Operations:
public:
	//: Draw pacman onto graphic context.
	virtual void show(int view_x, int view_y, CL_GraphicContext *gc = 0);

	//: Move pacman.
	virtual bool turn(float time_elapsed);

	//: Kill the pacman.
	void you_are_dead() { i_am_dead = true; }

	//: Check if pacman is at specified location.
	virtual bool hit_check(float x, float y);

//!Implementation:
private:
	//: Called when pacman reaches its destination.
	virtual bool event_reached_dest();

	void on_key_down(const CL_InputEvent &key);
	
	int anim_pos, anim_dir;

	int move_dir;

	int wanted_dir;

	bool i_am_dead;

	bool got_powerup;

	int powerup_starttime;

	//: Pacman images.
	CL_Sprite spr_pacman;

	//: Sound effect played when pacman is created.
	CL_SoundBuffer sfx_start;
	
	//: Sound effect of pacman eating an egg.
	CL_SoundBuffer sfx_namnam;

	//: Sound effect of pacman eating a powerup.
	CL_SoundBuffer sfx_powerup;
	
	//: Sound effect of pacman dieing.
	CL_SoundBuffer sfx_dead;

	CL_SlotContainer slots;
};

#endif
