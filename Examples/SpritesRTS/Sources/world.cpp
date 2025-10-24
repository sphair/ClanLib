#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "world.h"
#include "tankvehicle.h"
#include "building.h"

World::World()
{
	// Setup resources
	resources = new CL_ResourceManager("resources.xml");

	background = new CL_Surface("background", resources);
	
	// Receive mouse clicks
	slotKeyDown = CL_Keyboard::sig_key_down().connect(this, &World::onKeyDown);
	slotMouseDown = CL_Mouse::sig_key_down().connect(this, &World::onMouseDown);
	slotMouseUp = CL_Mouse::sig_key_up().connect(this, &World::onMouseUp);
	slotMouseMove = CL_Mouse::sig_move().connect(this, &World::onMouseMove);

	dragging = mouseDown = false;
	
	quit = false;

	// Add some tanks
	initLevel();
}

World::~World()
{
	// Delete all gameobjects
	std::list<GameObject *>::iterator it;
	for(it = objects.begin(); it != objects.end(); ++it)
		delete (*it);

	delete background;
	delete resources;
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

void World::onKeyDown(const CL_InputEvent &key)
{
	// Fire missile on space
	if(key.id == CL_KEY_SPACE)
	{
		std::list<TankVehicle *>::iterator it;
		for(it = tanks.begin(); it != tanks.end(); ++it)
			(*it)->fire(false);
	}
}

void World::onMouseDown(const CL_InputEvent &key)
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
				if(CL_Keyboard::get_keycode(CL_KEY_SHIFT))
				{
					if(CL_Keyboard::get_keycode(CL_KEY_CONTROL))	// Nuke when ctrl and shift is pressed
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

void World::onMouseUp(const CL_InputEvent &key)
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
				if(!CL_Keyboard::get_keycode(CL_KEY_SHIFT))
					tank->deselect();
		}
		
		dragging = false;
	}
		
	mouseDown = false;
}

void World::onMouseMove(const CL_InputEvent &key)
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
	while(!CL_Keyboard::get_keycode(CL_KEY_ESCAPE) && quit == false)
	{
		update();
		draw();

		CL_Display::flip(1);
		CL_System::keep_alive(1);
	};
}

void World::update()
{
	float timeElapsed = calcTimeElapsed();
	
	// Update all gameobjects
	std::list<GameObject *>::iterator it;
	for(it = objects.begin(); it != objects.end(); )
	{
		// If update returns false, object should be deleted
		if((*it)->update(timeElapsed) == false)
		{
			delete (*it);
			it = objects.erase(it);
		}
		else
			++it;
	}
}

// Calculate amount of time since last frame
float World::calcTimeElapsed()
{
	static float lastTime = 0;

	float newTime = (float)CL_System::get_time();
	if(lastTime == 0)
		lastTime = newTime;

	float deltaTime = (newTime - lastTime) / 1000.0f;
	lastTime = newTime;

	return deltaTime;
}

void World::draw()
{
	// Draw background 
	background->draw();
	
	// Draw all gameobjects
	std::list<GameObject *>::iterator it;
	for(it = objects.begin(); it != objects.end(); ++it)
		(*it)->draw();

	// Draw drag area
	if(dragging)
	{
		float s = (sinf(CL_System::get_time() / 200.0f) + 3.0f) / 4.0f;

		CL_Display::draw_rect(
			CL_Rect(dragArea.left - 1, dragArea.top - 1, dragArea.right + 1, dragArea.bottom + 1), 
			CL_Color(0, 255, 0, int(100 * s)));
		CL_Display::draw_rect(
			dragArea,
			CL_Color(0, 255, 0, int(200 * s)));
		CL_Display::draw_rect(
			CL_Rect(dragArea.left + 1, dragArea.top + 1, dragArea.right - 1, dragArea.bottom - 1), 
			CL_Color(0, 255, 0, int(100 * s)));
	}
}
