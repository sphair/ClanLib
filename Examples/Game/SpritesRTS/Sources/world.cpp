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
**    
*/
#include "precomp.h"

#include "world.h"
#include "tankvehicle.h"
#include "building.h"

World::World(clan::DisplayWindow &display_window) : window(display_window), quit(false)
{
	cc.connect(window.sig_window_close(), clan::bind_member(this, &World::on_window_close));

	canvas = clan::Canvas(window);

	clan::BlendStateDescription blend_desc;
	blendstate_default = clan::BlendState(canvas, blend_desc);
	blend_desc.set_blend_function(clan::blend_zero, clan::blend_one_minus_src_alpha, clan::blend_zero, clan::blend_one_minus_src_alpha);
	blendstate_cl_blend_zero_cl_blend_one_minus_src_alpha = clan::BlendState(canvas, blend_desc);

	blend_desc.set_blend_function(clan::blend_src_alpha, clan::blend_one_minus_src_alpha, clan::blend_src_alpha, clan::blend_one_minus_src_alpha);
	blendstate_cl_blend_src_alpha_cl_blend_one_minus_src_alpha = clan::BlendState(canvas, blend_desc);

	// Setup resources
	resources =   clan::XMLResourceManager::create(clan::XMLResourceDocument("resources.xml"));

	background = clan::Image::resource(canvas, "background", resources);
	
	// Receive mouse clicks
	cc.connect(window.get_ic().get_keyboard().sig_key_down(), clan::bind_member(this, &World::onKeyDown));
	cc.connect(window.get_ic().get_mouse().sig_key_down(), clan::bind_member(this, &World::onMouseDown));
	cc.connect(window.get_ic().get_mouse().sig_key_dblclk(), clan::bind_member(this, &World::onMouseDown));

	cc.connect(window.get_ic().get_mouse().sig_key_up(), clan::bind_member(this, &World::onMouseUp));
	cc.connect(window.get_ic().get_mouse().sig_pointer_move(), clan::bind_member(this, &World::onMouseMove));

	dragging = mouseDown = false;
	
	// Add some tanks
	initLevel();

	// Run the main loop
	run();
}

World::~World()
{
	// Delete all gameobjects
	std::list<GameObject *>::iterator it;
	for(it = objects.begin(); it != objects.end(); ++it)
		delete (*it);

}

void World::initLevel()
{
	TankVehicle *tank1 = new TankVehicle(TankVehicle::SPACE_SHOOT, this);
	tank1->setPos(667, 207);
	
	TankVehicle *tank2 = new TankVehicle(TankVehicle::SPACE_SHOOT, this);
	tank2->setPos(854, 422);

	Building *helipad = new Building(Building::HELI_PAD, this);
	helipad->setPos(254, 222);

	addObject(helipad);
	addTank(tank1);
	addTank(tank2);
}

void World::addObject(GameObject *object)
{
	objects.push_back(object);
}

void World::addTank(TankVehicle *tank)
{
	objects.push_back(tank);
	tanks.push_back(tank);
}

bool World::hitCheck(clan::CollisionOutline &outline, GameObject *other)
{
	std::list<GameObject *>::iterator it;
	for(it = objects.begin(); it != objects.end(); ++it)
	{
		if( (*it) != other )
		{
			if( (*it)->hitCheck(outline, other) )
				return true;
		}
	}

	return false;
}

void World::onKeyDown(const clan::InputEvent &key)
{
	// Fire missile on space
	if(key.id == clan::keycode_space)
	{
		std::list<TankVehicle *>::iterator it;
		for(it = tanks.begin(); it != tanks.end(); ++it)
			(*it)->fire(false);
	}
}

void World::onMouseDown(const clan::InputEvent &key)
{
	// Start dragging on left click
	if(key.id == clan::mouse_left)
	{
		dragArea.left = key.mouse_pos.x;
		dragArea.top = key.mouse_pos.y;
	
		mouseDown = true;
	}

	// Right click = move or fire
	if(key.id == clan::mouse_right)
	{
		std::list<TankVehicle *>::iterator it;
		for(it = tanks.begin(); it != tanks.end(); ++it)
		{
			TankVehicle *tank1 = (*it);

			if(tank1->isSelected())
			{
				bool fire = false;

				// Force firing
				if(key.shift)
				{
					if(key.ctrl)	// Nuke when ctrl and shift is pressed
						tank1->fire(true);
					else
						tank1->fire(false);
						
					fire = true;
				}
				else
				{
					// Check if clicked on other tank
					std::list<TankVehicle *>::iterator it2;
					for(it2 = tanks.begin(); it2 != tanks.end(); ++it2)
					{
						TankVehicle *tank2 = (*it2);

						if(tank1 != tank2 && tank2->hitCheck(key.mouse_pos.x, key.mouse_pos.y))
						{
							tank1->fire(false);
							fire = true;
						}
					}
				}
				
				// No fire, move unit
				if(fire == false)
					tank1->setTargetPos(key.mouse_pos.x, key.mouse_pos.y);
			}
		}
	}
}

void World::onMouseUp(const clan::InputEvent &key)
{
	if(key.id == clan::mouse_left)
	{
		// Set end of drag area
		dragArea.right = key.mouse_pos.x;
		dragArea.bottom = key.mouse_pos.y;

		dragArea.normalize();
		
		// Select/deselect units
		std::list<TankVehicle *>::iterator it;
		for(it = tanks.begin(); it != tanks.end(); ++it)
		{
			TankVehicle *tank = (*it);

			if(tank->hitCheck(dragArea))
				tank->select();
			else
				if(!key.shift)
					tank->deselect();
		}
		
		dragging = false;
	}
		
	mouseDown = false;
}

void World::onMouseMove(const clan::InputEvent &key)
{
	// Expand drag area
	if(mouseDown)
	{
		dragging = true;
		
		dragArea.right = key.mouse_pos.x;
		dragArea.bottom = key.mouse_pos.y;
	}
	
	// Make turrets target mousepos
	std::list<TankVehicle *>::iterator it;
	for(it = tanks.begin(); it != tanks.end(); ++it)
	{
		TankVehicle *tank = (*it);

		if(tank->isSelected())
			tank->setTurretTargetPos(key.mouse_pos.x, key.mouse_pos.y);
	}
}

void World::run()
{
	clan::GameTime game_time;
	while(!window.get_ic().get_keyboard().get_keycode(clan::keycode_escape))
	{
		game_time.update();
		if (quit)
			break;

		update(game_time.get_time_elapsed_ms());
		draw();

		window.flip(1);
		clan::KeepAlive::process();
	};
}

void World::update(int timeElapsed_ms)
{
	// Update all gameobjects
	std::list<GameObject *>::iterator it;
	for(it = objects.begin(); it != objects.end(); )
	{
		// If update returns false, object should be deleted
		if((*it)->update(timeElapsed_ms) == false)
		{
			delete (*it);
			it = objects.erase(it);
		}
		else
			++it;
	}
}

void World::draw()
{
	// Draw background 
	clan::Rect window_rect = window.get_viewport();
	background.draw(canvas, window_rect);

	// Draw all gameobjects
	std::list<GameObject *>::iterator it;
	for(it = objects.begin(); it != objects.end(); ++it)
		(*it)->draw();

	// Draw drag area
	if(dragging)
	{
		float s = (sinf(clan::System::get_time() / 200.0f) + 3.0f) / 4.0f;

		canvas.draw_box(clan::Rectf(dragArea.left - 1, dragArea.top - 1, dragArea.right + 1, dragArea.bottom + 1), 
				   clan::Colorf(0.0f, 1.0f, 0.0f, (100.0f * s)/256.0f));
		canvas.draw_box(clan::Rectf(dragArea.left, dragArea.top, dragArea.right, dragArea.bottom), 
				   clan::Colorf(0.0f, 1.0f, 0.0f, (200.0f * s)/256.0f));
		canvas.draw_box(clan::Rectf(dragArea.left + 1, dragArea.top + 1, dragArea.right - 1, dragArea.bottom - 1), 
				   clan::Colorf(0.0f, 1.0f, 0.0f, (100.0f * s)/256.0f));

	}

	canvas.flush();
}

void World::on_window_close()
{
	quit = true;
}

