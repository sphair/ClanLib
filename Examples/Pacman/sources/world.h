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

#ifndef header_world
#define header_world

#if _MSC_VER > 1000
#pragma once
#endif

#include "gameobject.h"
#include "gameobject_pacman.h"
#include "map.h"

class World
{
//!Construction:
public:
	World(CL_ResourceManager *resources);

	~World();

//!Attributes:
public:
	CL_ResourceManager *resources;
	
	Map *map;

	std::list<GameObject*> objects;

	GameObject_Pacman *player;

	int score;
	
	bool quit;

//!Operations:
public:
	void run(CL_DisplayWindow &window);

//!Slots:
public:
	//: Slot for shutting down
	void on_quit() { quit = true; }

//!Implementation:
private:
	void on_key_down(const CL_InputEvent &key);

	int view_x, view_y;

	CL_Font fnt_clansoft;
	
	CL_SoundBuffer sample;
};

#endif
