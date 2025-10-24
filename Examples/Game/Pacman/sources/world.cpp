/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#ifdef USE_SOFTWARE_RENDERER
#include <ClanLib/swrender.h>
#include "post_effect_command.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// World construction:

World::World(CL_ResourceManager *resources, CL_DisplayWindow &window) :
	resources(resources), map(0), player(0), score(0)
{
	gc = window.get_gc();

	// Load all resources in the game section now. This isn't a
	// requirement, but prevents game from loading them when object is
	// first time created.
//FIXME:	resources->load_section("Game");

//FIXME:	fnt_clansoft = CL_Font("Game/fnt_clansoft", resources);
//FIXME:	fnt_clansoft.set_alpha(0.5f);
	fnt_clansoft = CL_Font(gc, L"Tahoma", 32);

//	sample = CL_SoundBuffer("resources/ancient.mod");

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

void World::run(CL_DisplayWindow &window)
{
	game_display_window = window;

	gc = window.get_gc();

	quit = false;
	CL_Slot slot_key_down = window.get_ic().get_keyboard().sig_key_down().connect(this, &World::on_key_down);

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
//	CL_SoundBuffer_Session playback(sample.prepare());
//	playback.set_looping(true);
//	playback.play();

	// Start the game simulation:
	int start_time = CL_System::get_time();

	int begin_time = CL_System::get_time();
	int score_time = CL_System::get_time();
	
	int center_x = 0;
	int center_y = 0;

	bool welcome_shown = false;

	FramerateCounter frameratecounter;

	while (!quit)
	{
		float time_elapsed = (CL_System::get_time() - begin_time)/(float) 1000;
		begin_time = CL_System::get_time();

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
		
		gc.clear();
		map->draw(center_x, center_y, gc);

		std::list<GameObject*>::iterator it;
		for (
			it = objects.begin();
			it != objects.end();
			it++)
		{
			GameObject *cur = *it;
			cur->show(center_x, center_y, gc);
		}

		gc.flush_batcher();

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
			CL_String text1 = "YOU ARE DEAD";
			CL_String text2 = "Press space to restart";
			CL_Size size1 = fnt_clansoft.get_text_size(gc, text1);
			CL_Size size2 = fnt_clansoft.get_text_size(gc, text2);
			fnt_clansoft.draw_text(gc, gc.get_width()/2 - size1.width/2, gc.get_width()/2 - size1.height, text1, CL_Colorf::firebrick);
			fnt_clansoft.draw_text(gc, gc.get_width()/2 - size2.width/2, gc.get_height()*3/4 - size2.height, text2, CL_Colorf::lightgrey);
		}
		
		if ((CL_System::get_time()-start_time) <= 3000) // 3 sec
		{
			CL_String text1 = "Welcome to the Pacman Game";
			CL_Size size1 = fnt_clansoft.get_text_size(gc, text1);
			fnt_clansoft.draw_text(gc, gc.get_width()/2 - size1.width/2, gc.get_height() - 20 - size1.height, text1, CL_Colorf::lightgoldenrodyellow);
		}
		else if (welcome_shown == false && blowups.size() < 10)
		{
			blowups.push_back(new FontBlowUp(gc, "Welcome to the Pacman Game", gc.get_width()/2, gc.get_height() - 20, fnt_clansoft, CL_Colorf::lightgoldenrodyellow));
			welcome_shown = true;
		}

		CL_String fps = cl_format("%1 fps", frameratecounter.get_framerate());
		fnt_clansoft.draw_text(gc, 20, 30, fps);

		CL_String text2 = cl_format("%1 bonus bananas", score);
		CL_Size size2 = fnt_clansoft.get_text_size(gc, text2);
		fnt_clansoft.draw_text(gc, gc.get_width() - 20 - size2.width, 30, text2);

		{ // vc++ needs this scope to ensure for() is scoped...
			for (
				std::list<FontBlowUp*>::iterator it = blowups.begin();
				it != blowups.end();
				)
			{
				if ((*it)->show(gc, time_elapsed) == false)
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
/*
#ifdef USE_SOFTWARE_RENDERER
		static int bw_transparency = 0;
		static int bw_dir = 1;
		CL_GraphicContext_SWRender gc_swr = gc;
		gc_swr.queue_command(new(gc_swr.get_pipeline()) PostEffectCommand(bw_transparency));
		bw_transparency += bw_dir;
		if (bw_transparency == 0)
			bw_dir = 1;
		else if (bw_transparency == 255)
			bw_dir = -1;
#endif
*/
		window.flip(0);
		frameratecounter.frame_shown();

		if (map->get_eggs_left() == 0) break; // level completed

		CL_KeepAlive::process();
	}

	CL_Console::write_line("Frames per second: %1", frameratecounter.get_framerate());
	// MessageBox(0, cl_format("Frames per second: %1", frameratecounter.get_framerate()).c_str(), TEXT("FPS"), MB_OK);
}

/////////////////////////////////////////////////////////////////////////////
// World implementation:

void World::on_key_down(const CL_InputEvent &key, const CL_InputState &state)
{
	if (key.id == CL_KEY_ESCAPE) quit = true;

	if (key.id == CL_KEY_SPACE && player == NULL)
	{
		player = new GameObject_Pacman(map->get_width()/2, map->get_height()/2, this);
		player->AttachKeyboard(game_display_window);
		objects.push_back(player);

		if (blowups.size() < 10)
		{
			blowups.push_back(new FontBlowUp(gc, "YOU ARE DEAD", 320, 320, fnt_clansoft, CL_Colorf::firebrick));
			blowups.push_back(new FontBlowUp(gc, "Press space to restart", 320, 360, fnt_clansoft, CL_Colorf::lightgrey));
		}
	}
}
