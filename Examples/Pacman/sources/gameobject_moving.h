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

#ifndef header_gameobject_moving
#define header_gameobject_moving

#if _MSC_VER > 1000
#pragma once
#endif

#include "gameobject.h"

class World;

class GameObject_Moving : public GameObject
{
//!Construction:
public:
	//: Construct object at position (x, y).
	GameObject_Moving(int x, int y, World *world);

	//: Destructor.
	virtual ~GameObject_Moving();

//!Attributes:
public:
	//: Returns object x position.
	virtual float get_x() { return x; }
	
	//: Returns object y position.
	virtual float get_y() { return y; }
	
protected:
	//: World object.
	World *world;

//!Operations:
protected:
	//: Set movement speed of object.
	void set_speed(float new_speed) { speed = new_speed; }

	//: Set object to move to this destination.
	void set_destination(int x, int y) { dest_x = x; dest_y = y; }

	//: Moves the object.
	bool move(float time_elapsed);
	
	//: Called when object reaches destination.
	virtual bool event_reached_dest()=0;

//!Implementation:
protected:
	bool verify_move(int dx, int dy);

	bool do_move(int dx, int dy);
	
	float x, y;

	int dest_x, dest_y;

	float speed;
};

#endif
