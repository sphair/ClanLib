#include <ClanLib/display.h>
#include <ClanLib/sound.h>

#include "missile.h"
#include "world.h"

Missile::Missile(World *world, GameObject *_owner)
: GameObject(world)
{
	CL_GraphicContext gc = world->get_gc();

	spriteMissile = new CL_Sprite(gc, "SpaceShootMissile", &world->resources);
	spriteExplosion = new CL_Sprite(gc, "Explosion", &world->resources);
	sound = new CL_SoundBuffer("MissileHit", &world->resources);

	collisionMissile = new CL_CollisionOutline("Gfx/spaceshoot_missile.png");
	collisionMissile->set_alignment(origin_center);
	
	sound->set_volume(1.0f);
	sound->prepare();
	
	sprite = spriteMissile;
	
	hidden = false;
	exploding = false;

	owner = _owner;
}

Missile::~Missile()
{
	delete spriteMissile;
	delete spriteExplosion;
	delete sound;
	delete collisionMissile;
}

void Missile::setPos(int x, int y)
{
	posX = (float)x;
	posY = (float)y;
	collisionMissile->set_translation(posX, posY);
}

void Missile::setAngle(float newAngle)
{
	angle = newAngle;
	sprite->set_angle(CL_Angle(angle, cl_degrees));
	collisionMissile->set_angle(CL_Angle(angle, cl_degrees));
}

void Missile::setSpeed(float newSpeed)
{
	speed = newSpeed;
}

void Missile::move(float length)
{
	posX += length * float(sin(angle * CL_PI / 180.0f));
	posY += length * float(-cos(angle * CL_PI / 180.0f));

	collisionMissile->set_translation(posX, posY);
}

void Missile::draw()
{
	if(!hidden)
	{
		CL_GraphicContext gc = world->get_gc();
		sprite->draw(gc, posX, posY);
	}
}

bool Missile::update(float timeElapsed)
{
	sprite->update(timeElapsed);

	if(exploding)
	{
		if(sprite->is_finished())
			return false;
	}
	else
	{
		move(speed * timeElapsed);

		if(world->hitCheck(collisionMissile, owner))
		{
			sound->play();

			sprite = spriteExplosion;
			sprite->set_angle(CL_Angle(0, cl_degrees));
			sprite->set_alpha(0.85f);

			exploding = true;
		}
	}

	CL_GraphicContext gc = world->get_gc();
	return !(posX < -100 || posY < -100 || posX > gc.get_width() + 100 || posY > gc.get_height() + 100);
}
