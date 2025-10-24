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
#include "world.h"
#include "gameobject_ghost.h"
#include "gameobject_pacman.h"

/////////////////////////////////////////////////////////////////////////////
// World construction:

World::World(CL_ResourceManager *resources) :
	resources(resources), map(0), player(0), score(0)
{
	// Load all resources in the game section now. This isn't a
	// requirement, but prevents game from loading them when object is
	// first time created.
	resources->load_section("Game");

	fnt_clansoft = CL_Font("Game/fnt_clansoft", resources);
	fnt_clansoft.set_alpha(0.5f);

	sample = CL_SoundBuffer("resources/ancient.mod");

	map = new Map(resources);
}

World::~World()
{
	for (
		std::list<GameObject*>::iterator it = objects.begin();
		it != objects.end();
		it++)
	{
		delete *it;
	}
	objects.clear();
	
	resources->unload_section("Game");
}

/////////////////////////////////////////////////////////////////////////////
// World attributes:

/////////////////////////////////////////////////////////////////////////////
// World operations:

void World::run(CL_DisplayWindow &window)
{
	quit = false;
	CL_Slot slot_key_down = window.get_ic()->get_keyboard().sig_key_down().connect(this, &World::on_key_down);
	
	// Create map:
	map->generate_level(51, 51, 20);

	// Populate map with pacman:
	player = new GameObject_Pacman(map->get_width()/2, map->get_height()/2, this);
	objects.push_back(player);

	// Create four ghosts in each corner of map:
	for (int i=0; i<4; i++)
	{
		objects.push_back(new GameObject_Ghost(1, 1, this));
		objects.push_back(new GameObject_Ghost(map->get_width()-2, 1, this));
		objects.push_back(new GameObject_Ghost(1, map->get_height()-2, this));
		objects.push_back(new GameObject_Ghost(map->get_width()-2, map->get_height()-2, this));
	}

	// Prepare the music
	CL_SoundBuffer_Session playback(sample.prepare());
	playback.set_looping(true);
	playback.play();

	// Start the game simulation:
	int start_time = CL_System::get_time();
	int cur_frame = 0;

	int begin_time = CL_System::get_time();
	int score_time = CL_System::get_time();
	
	int center_x = 0;
	int center_y = 0;

	CL_FramerateCounter frameratecounter;

	while (!quit)
	{
		float time_elapsed = (CL_System::get_time() - begin_time)/(float) 1000;
		begin_time = CL_System::get_time();
		cur_frame++;

		if (CL_System::get_time()-score_time > 1000 && player != NULL)
		{
			score -= 50;
			if (score < 0) score = 0;
			score_time = CL_System::get_time(); 
		}

		int width = map->get_tile_width()-6;
		int height = map->get_tile_height()-6;

		if (player != NULL)
		{
			center_x = int((player->get_x()+0.5)*width-window.get_width()/2);
			center_y = int((player->get_y()+0.5)*height-window.get_height()/2);
			if (center_x < 0) center_x = 0;
			if (center_y < 0) center_y = 0;
			if (center_x > map->get_width()*width-window.get_width()) center_x = map->get_width()*width-window.get_width();
			if (center_y > map->get_height()*height-window.get_height()) center_y = map->get_height()*height-window.get_height();
		}
		
		map->draw(center_x, center_y, window.get_gc());

		std::list<GameObject*>::iterator it;
		for (
			it = objects.begin();
			it != objects.end();
			it++)
		{
			GameObject *cur = *it;
			cur->show(center_x, center_y, window.get_gc());
		}

		float time_elapsed2 = time_elapsed;
		while (time_elapsed2 > 0)
		{
			float turn_time = (time_elapsed2 > 0.05f) ? 0.05f : time_elapsed2;
			for (
				it = objects.begin();
				it != objects.end();
				)
			{
				if ((*it)->turn(turn_time) == false)
				{
					if ((*it) == player) player = NULL;
					delete (*it);
					it = objects.erase(it);
				}
				else
					++it;
			}
			
			time_elapsed2 -= 0.05f;
		}

		if (player == NULL)
		{
			fnt_clansoft.set_alignment(origin_bottom_center);
			fnt_clansoft.draw(window.get_width()/2, 320, "YOU ARE DEAD", window.get_gc());
			fnt_clansoft.draw(window.get_width()/2, 360, "Press space to restart", window.get_gc());
		}
		
		if ((CL_System::get_time()-start_time) <= 3000) // 5 sec
		{
			fnt_clansoft.set_alignment(origin_bottom_center);
			fnt_clansoft.draw(window.get_width()/2, window.get_height() - 20, "Welcome to the ClanSoft Pacman Game", window.get_gc());
		}
/*		else if (welcome_shown == false && blowups.size() < 10)
		{
			blowups.push_back(new FontBlowUp("Welcome to the ClanSoft Pacman Game", 320, 20, fnt_clansoft));
			welcome_shown = true;
		}*/
		
		int fps = frameratecounter.get_fps();
		fnt_clansoft.set_alignment(origin_top_left);
		fnt_clansoft.draw(20, 20, CL_String::from_int(fps) + " fps");

		fnt_clansoft.set_alignment(origin_top_right);
		fnt_clansoft.draw(window.get_width()-20, 20, CL_String::format("%1 bonus bananas", score), window.get_gc());
/*
		{ // vc++ needs this scope to ensure for() is scoped...
			for (
				std::list<FontBlowUp*>::iterator it = blowups.begin();
				it != blowups.end();
				)
			{
				if ((*it)->show(time_elapsed) == false)
				{
					delete *it;
					it = blowups.erase(it);
				}
				else
					++it;
			}
		}
*/
		window.flip();

		if (map->get_eggs_left() == 0) break; // level completed

		CL_System::keep_alive();
	}

	float delta_time = (CL_System::get_time()-start_time)/(float) 1000;
	std::cout << "Frames per second: " << cur_frame / delta_time << std::endl;
}

/////////////////////////////////////////////////////////////////////////////
// World implementation:

void World::on_key_down(const CL_InputEvent &key)
{
	if (key.id == CL_KEY_ESCAPE) quit = true;

	if (key.id == CL_KEY_SPACE && player == NULL)
	{
		player = new GameObject_Pacman(map->get_width()/2, map->get_height()/2, this);
		objects.push_back(player);
/*
		if (blowups.size() < 10)
		{
			blowups.push_back(new FontBlowUp("YOU ARE DEAD", 320, 320, fnt_clansoft));
			blowups.push_back(new FontBlowUp("Press space to restart", 320, 360, fnt_clansoft));
		}
*/		
	}
}
