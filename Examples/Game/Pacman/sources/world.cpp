/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "framerate_counter.h"
#include "program.h"

/////////////////////////////////////////////////////////////////////////////
// World construction:

World::World(ResourceManager *resources, DisplayWindow &window) :
	resources(resources), 
	map(0), 
	player(0), 
	score(0)
{
	canvas = Canvas(window);
	gc = canvas.get_gc();

	// Load all resources in the game section now. This isn't a
	// requirement, but prevents game from loading them when object is
	// first time created.
//FIXME:	resources->load_section("Game");

	fnt_clansoft = Font_Sprite(canvas, "Game/fnt_clansoft", resources);
	//fnt_clansoft = Font(gc, L"Tahoma", 32);

//	sample = SoundBuffer("resources/ancient.mod");

	map = new Map(resources, gc);
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
	
//FIXME:	resources->unload_section("Game");
}

/////////////////////////////////////////////////////////////////////////////
// World attributes:

/////////////////////////////////////////////////////////////////////////////
// World operations:

void World::run(DisplayWindow &window)
{
	game_display_window = window;

	quit = false;
	Slot slot_key_down = window.get_ic().get_keyboard().sig_key_down().connect(this, &World::on_key_down);

	// Create map:
	map->generate_level(51, 51, 20);

	// Populate map with pacman:
	player = new GameObject_Pacman(map->get_width()/2, map->get_height()/2, this);
	player->AttachKeyboard(game_display_window);
	objects.push_back(player);

	// Create four ghosts in each corner of map:
//FIXME: (why was this loop in place?)	for (int i=0; i<4; i++)
	{
		objects.push_back(new GameObject_Ghost(1, 1, this));
		objects.push_back(new GameObject_Ghost(map->get_width()-2, 1, this));
		objects.push_back(new GameObject_Ghost(1, map->get_height()-2, this));
		objects.push_back(new GameObject_Ghost(map->get_width()-2, map->get_height()-2, this));
	}


	// Prepare the music
//	SoundBuffer_Session playback(sample.prepare());
//	playback.set_looping(true);
//	playback.play();

	// Start the game simulation:
	ubyte64 start_time = System::get_time();

	ubyte64 begin_time = System::get_time();
	ubyte64 score_time = System::get_time();
	
	int center_x = 0;
	int center_y = 0;

	bool welcome_shown = false;

	FramerateCounter frameratecounter;

	while (!quit)
	{
		float time_elapsed = (System::get_time() - begin_time)/(float) 1000;
		begin_time = System::get_time();

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
			if (gc.get_width() > map->get_width()*width)
			{
				center_x = -(gc.get_width()/2-map->get_width()*width/2);
			}
			else
			{
				center_x = int((player->get_x()+0.5)*width-gc.get_width()/2);
				if (center_x < 0) center_x = 0;
				if (center_x > map->get_width()*width-gc.get_width()) center_x = map->get_width()*width-gc.get_width();
			}
			if (gc.get_height() > map->get_height()*height)
			{
				center_y = -(gc.get_height()/2-map->get_height()*height/2);
			}
			else
			{
				center_y = int((player->get_y()+0.5)*height-gc.get_height()/2);
				if (center_y < 0) center_y = 0;
				if (center_y > map->get_height()*height-gc.get_height()) center_y = map->get_height()*height-gc.get_height();
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
			Size size1 = fnt_clansoft.get_text_size(gc, text1);
			Size size2 = fnt_clansoft.get_text_size(gc, text2);
			fnt_clansoft.draw_text(canvas, gc.get_width()/2 - size1.width/2, gc.get_width()/2 - size1.height, text1, Colorf::firebrick);
			fnt_clansoft.draw_text(canvas, gc.get_width()/2 - size2.width/2, gc.get_height()*3/4 - size2.height, text2, Colorf::lightgrey);
		}
		
		if ((System::get_time()-start_time) <= 3000) // 3 sec
		{
			std::string text1 = "Welcome to the Pacman Game";
			Size size1 = fnt_clansoft.get_text_size(gc, text1);
			fnt_clansoft.draw_text(canvas, gc.get_width()/2 - size1.width/2, gc.get_height() - 20 - size1.height, text1, Colorf::lightgoldenrodyellow);
		}
		else if (welcome_shown == false && blowups.size() < 10)
		{
			char *text1 = "Welcome to the Pacman Game";
			Size size1 = fnt_clansoft.get_text_size(gc, text1);
			blowups.push_back(new FontBlowUp(canvas, text1, gc.get_width()/2, gc.get_height() - 20 - size1.height, fnt_clansoft, Colorf::lightgoldenrodyellow));
			welcome_shown = true;
		}

		std::string fps = string_format("%1 fps", frameratecounter.get_framerate());
		fnt_clansoft.draw_text(canvas, 20, 52, fps);

		std::string text2 = string_format("%1 bonus bananas", score);
		Size size2 = fnt_clansoft.get_text_size(gc, text2);
		fnt_clansoft.draw_text(canvas, gc.get_width() - 20 - size2.width, 52, text2);

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

		canvas.flush();
		window.flip(0);
		frameratecounter.frame_shown();

		if (map->get_eggs_left() == 0) break; // level completed

		KeepAlive::process();
	}

	Console::write_line("Frames per second: %1", frameratecounter.get_framerate());
	// MessageBox(0, string_format("Frames per second: %1", frameratecounter.get_framerate()).c_str(), TEXT("FPS"), MB_OK);
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
