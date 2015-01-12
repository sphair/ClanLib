/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "fontblowup.h"
#include "program.h"

/////////////////////////////////////////////////////////////////////////////
// World construction:

World::World(ResourceManager &resources, DisplayWindow &window) :
	resources(resources), 
	map(0), 
	player(0), 
	score(0)
{
	canvas = Canvas(window);

	// Load all resources in the game section now. This isn't a
	// requirement, but prevents game from loading them when object is
	// first time created.

	cc.connect(window.get_ic().get_keyboard().sig_key_down(), this, &World::on_key_down);
	fnt_clansoft = clan::Font::resource(canvas, "Game/fnt_clansoft", FontDescription(), resources);

	map = new Map(resources, canvas);
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
}

/////////////////////////////////////////////////////////////////////////////
// World attributes:

/////////////////////////////////////////////////////////////////////////////
// World operations:

void World::run(DisplayWindow &window)
{
	game_display_window = window;

	quit = false;
	
	// Create map:
	map->generate_level(51, 51, 20);

	// Populate map with pacman:
	player = new GameObject_Pacman(map->get_width()/2, map->get_height()/2, this);
	player->AttachKeyboard(game_display_window);
	objects.push_back(player);

	// Create four ghosts in each corner of map:
	objects.push_back(new GameObject_Ghost(1, 1, this));
	objects.push_back(new GameObject_Ghost(map->get_width()-2, 1, this));
	objects.push_back(new GameObject_Ghost(1, map->get_height()-2, this));
	objects.push_back(new GameObject_Ghost(map->get_width()-2, map->get_height()-2, this));


	// Start the game simulation:

	
	int center_x = 0;
	int center_y = 0;

	bool welcome_shown = false;

	GameTime game_time;
	ubyte64 score_time = System::get_time();
	while (!quit)
	{
		game_time.update();
		float time_elapsed = game_time.get_time_elapsed();

		if (System::get_time()-score_time > 1000 && player != NULL)
		{
			score -= 50;
			if (score < 0) score = 0;
			score_time = System::get_time(); 
		}

		int width = map->get_tile_width()-6;
		int height = map->get_tile_height()-6;

		if (player != NULL)
		{
			if (canvas.get_width() > map->get_width()*width)
			{
				center_x = -(canvas.get_width()/2-map->get_width()*width/2);
			}
			else
			{
				center_x = int((player->get_x()+0.5)*width-canvas.get_width()/2);
				if (center_x < 0) center_x = 0;
				if (center_x > map->get_width()*width-canvas.get_width()) center_x = map->get_width()*width-canvas.get_width();
			}
			if (canvas.get_height() > map->get_height()*height)
			{
				center_y = -(canvas.get_height()/2-map->get_height()*height/2);
			}
			else
			{
				center_y = int((player->get_y()+0.5)*height-canvas.get_height()/2);
				if (center_y < 0) center_y = 0;
				if (center_y > map->get_height()*height-canvas.get_height()) center_y = map->get_height()*height-canvas.get_height();
			}
		}
		
		canvas.clear();
		map->draw(center_x, center_y, canvas);

		std::list<GameObject*>::iterator it;
		for (
			it = objects.begin();
			it != objects.end();
			it++)
		{
			GameObject *cur = *it;
			cur->show(center_x, center_y, canvas);
		}

			canvas.flush();

		float time_elapsed2 = time_elapsed;
		while (time_elapsed2 > 0)
		{
			float turn_time = (time_elapsed2 > 0.05f) ? 0.05f : time_elapsed2;
			it = objects.begin();
			while (it != objects.end())
			{
				if ((*it)->turn(turn_time) == false)
				{
					if ((*it) == player) player = NULL;
					delete (*it);
					it = objects.erase(it);
				}
				else
				{
					it++;
				}
			}
			
			time_elapsed2 -= 0.05f;
		}

		if (player == NULL)
		{
			std::string text1 = "YOU ARE DEAD";
			std::string text2 = "Press space to restart";
			Sizef size1 = fnt_clansoft.measure_text(canvas, text1).advance;
			Sizef size2 = fnt_clansoft.measure_text(canvas, text2).advance;
			fnt_clansoft.draw_text(canvas, canvas.get_width()/2 - size1.width/2, canvas.get_width()/2 - size1.height, text1, Colorf::firebrick);
			fnt_clansoft.draw_text(canvas, canvas.get_width()/2 - size2.width/2, canvas.get_height()*3/4 - size2.height, text2, Colorf::lightgrey);
		}
		
		if (game_time.get_current_time_ms() <= 3000) // 3 sec
		{
			std::string text1 = "Welcome to the Pacman Game";
			Sizef size1 = fnt_clansoft.measure_text(canvas, text1).advance;
			fnt_clansoft.draw_text(canvas, canvas.get_width()/2 - size1.width/2, canvas.get_height() - 32 - size1.height, text1, Colorf::lightgoldenrodyellow);
		}
		else if (welcome_shown == false && blowups.size() < 10)
		{
			const char *text1 = "Welcome to the Pacman Game";
			Sizef size1 = fnt_clansoft.measure_text(canvas, text1).advance;
			blowups.push_back(new FontBlowUp(canvas, text1, canvas.get_width()/2, canvas.get_height() - 32 - size1.height, fnt_clansoft, Colorf::lightgoldenrodyellow));
			welcome_shown = true;
		}

		std::string fps = string_format("%1 fps", (int) (game_time.get_updates_per_second() + 0.5f));
		fnt_clansoft.draw_text(canvas, 20, 52, fps);

		std::string text2 = string_format("%1 bonus bananas", score);
		Sizef size2 = fnt_clansoft.measure_text(canvas, text2).advance;
		fnt_clansoft.draw_text(canvas, canvas.get_width() - 20 - size2.width, 52, text2);

		{ // vc++ needs this scope to ensure for() is scoped...
			for (
				std::list<FontBlowUp*>::iterator it = blowups.begin();
				it != blowups.end();
				)
			{
				if ((*it)->show(canvas, time_elapsed) == false)
				{
					delete *it;
					it = blowups.erase(it);
				}
				else
				{
					it++;
				}
			}
		}

		window.flip(1);

		if (map->get_eggs_left() == 0) break; // level completed

		KeepAlive::process();
	}

}

/////////////////////////////////////////////////////////////////////////////
// World implementation:

void World::on_key_down(const InputEvent &key)
{
	if (key.id == keycode_escape) quit = true;

	if (key.id == keycode_space && player == NULL)
	{
		player = new GameObject_Pacman(map->get_width()/2, map->get_height()/2, this);
		player->AttachKeyboard(game_display_window);
		objects.push_back(player);

		if (blowups.size() < 10)
		{
			blowups.push_back(new FontBlowUp(canvas, "YOU ARE DEAD", 320, 320, fnt_clansoft, Colorf::firebrick));
			blowups.push_back(new FontBlowUp(canvas, "Press space to restart", 320, 360, fnt_clansoft, Colorf::lightgrey));
		}
	}
}
