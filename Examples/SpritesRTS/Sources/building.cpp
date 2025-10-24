#include <ClanLib/display.h>
#include <ClanLib/sound.h>

#include "building.h"
#include "world.h"

Building::Building(BuildingType buiding_type, World *world)
: GameObject(world)
{
	switch(buiding_type)
	{
	case HELI_PAD:
		sprite = new CL_Sprite("helipad", world->resources);
		collisionBuilding = new CL_CollisionOutline("Gfx/helipad.png");
		collisionBuilding->set_alignment(origin_center);
		break;
	}
	
	exploding = false;
}

Building::~Building()
{
	delete sprite;
	delete collisionBuilding;
}

void Building::setPos(int x, int y)
{
	posX = (float)x;
	posY = (float)y;
	collisionBuilding->set_translation(posX, posY);
}

void Building::setAngle(float newAngle)
{
	angle = newAngle;
	sprite->set_angle(angle);
	collisionBuilding->set_angle(angle);
}

void Building::draw()
{
	sprite->draw((int)posX, (int)posY);
}

bool Building::update(float timeElapsed)
{
	sprite->update(timeElapsed);
	return true;
}

bool Building::hitCheck(CL_CollisionOutline *outline, GameObject *other)
{
	return collisionBuilding->collide(*outline);
}
