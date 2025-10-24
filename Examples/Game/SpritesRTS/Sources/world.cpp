/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

World::World(CL_DisplayWindow &display_window) : window(display_window), quit(false)
{
	CL_Slot slot_quit = window.sig_window_close().connect(this, &World::on_window_close);

	gc = window.get_gc();

	// Setup resources
	resources = CL_ResourceManager("resources.xml");

	background = CL_Texture("background", &resources, gc);
	
	// Receive mouse clicks
	slotKeyDown = window.get_ic().get_keyboard().sig_key_down().connect(this, &World::onKeyDown);
	slotMouseDown = window.get_ic().get_mouse().sig_key_down().connect(this, &World::onMouseDown);
	slotMouseUp = window.get_ic().get_mouse().sig_key_up().connect(this, &World::onMouseUp);
	slotMouseMove = window.get_ic().get_mouse().sig_pointer_move().connect(this, &World::onMouseMove);

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

bool World::hitCheck(CL_CollisionOutline *outline, GameObject *other)
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

void World::onKeyDown(const CL_InputEvent &key, const CL_InputState &state)
{
	// Fire missile on space
	if(key.id == CL_KEY_SPACE)
	{
		std::list<TankVehicle *>::iterator it;
		for(it = tanks.begin(); it != tanks.end(); ++it)
			(*it)->fire(false);
	}
}

void World::onMouseDown(const CL_InputEvent &key, const CL_InputState &state)
{
	// Start dragging on left click
	if(key.id == CL_MOUSE_LEFT)
	{
		dragArea.left = key.mouse_pos.x;
		dragArea.top = key.mouse_pos.y;
	
		mouseDown = true;
	}

	// Right click = move or fire
	if(key.id == CL_MOUSE_RIGHT)
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

void World::onMouseUp(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_MOUSE_LEFT)
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

void World::onMouseMove(const CL_InputEvent &key, const CL_InputState &state)
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
	while(!window.get_ic().get_keyboard().get_keycode(CL_KEY_ESCAPE))
	{
		if (quit)
			break;

		update();
		draw();

		window.flip(1);
		CL_KeepAlive::process();
	};
}

void World::update()
{
	int timeElapsed_ms = calcTimeElapsed();
	
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

// Calculate amount of time since last frame
int World::calcTimeElapsed()
{
	static unsigned int lastTime = 0;

	unsigned int newTime = CL_System::get_time();
	if(lastTime == 0)
		lastTime = newTime;

	int deltaTime = (newTime - lastTime);
	lastTime = newTime;

	return deltaTime;
}

void World::draw()
{
	// Draw background 
	CL_Rect window_rect = window.get_viewport();
	gc.set_texture(0, background);
	CL_Draw::texture(gc, window_rect);	
	gc.reset_texture(0);

	// Draw all gameobjects
	std::list<GameObject *>::iterator it;
	for(it = objects.begin(); it != objects.end(); ++it)
		(*it)->draw();

	// Draw drag area
	if(dragging)
	{
		float s = (sinf(CL_System::get_time() / 200.0f) + 3.0f) / 4.0f;

		
		CL_Draw::box(gc,
			CL_Rectf(dragArea.left - 1, dragArea.top - 1, dragArea.right + 1, dragArea.bottom + 1), 
			CL_Colorf(0.0f, 1.0f, 0.0f, (100.0f * s)/256.0f));
		CL_Draw::box(gc,
			CL_Rectf(dragArea.left, dragArea.top, dragArea.right, dragArea.bottom), 
			CL_Colorf(0.0f, 1.0f, 0.0f, (200.0f * s)/256.0f));
		CL_Draw::box(gc,
			CL_Rectf(dragArea.left + 1, dragArea.top + 1, dragArea.right - 1, dragArea.bottom - 1), 
			CL_Colorf(0.0f, 1.0f, 0.0f, (100.0f * s)/256.0f));
	}
}

void World::on_window_close()
{
	quit = true;
}

