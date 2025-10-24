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

#ifndef header_gameobject_ghost
#define header_gameobject_ghost

#if _MSC_VER > 1000
#pragma once
#endif

#include "gameobject_moving.h"

class GameObject_Ghost : public GameObject_Moving
{
//!Construction:
public:
	//: Construct ghost at (x,y).
	GameObject_Ghost(int x, int y, World *world);

//!Operations:
public:
	//: Draw ghost on graphic context.
	virtual void show(int view_x, int view_y, CL_GraphicContext *gc = 0);

	//: Move ghost.
	virtual bool turn(float time_elapsed);
	
//!Implementation:
private:
	//: Called when ghost reaches its destination.
	virtual bool event_reached_dest();
	
	int anim_pos, anim_dir;

	int move_dir;
	
	bool red_ghost;

	CL_Sprite spr_ghost;

	CL_SoundBuffer sfx_ghost;

	CL_SoundBuffer sfx_ghost_dead;
};

#endif
