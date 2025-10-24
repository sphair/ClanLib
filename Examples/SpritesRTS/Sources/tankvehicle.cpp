#include <ClanLib/display.h>
#include <ClanLib/sound.h>

#include "tankvehicle.h"
#include "world.h"
#include "missile.h"

TankVehicle::TankVehicle(TankType type, World *world)
: GameObject(world)
{
	switch(type)
	{
	case SPACE_SHOOT:
		spriteBodyStill = new CL_Sprite("SpaceShootBodyStill", world->resources);
		spriteBodyMoving = new CL_Sprite("SpaceShootBodyMoving", world->resources);
		spriteTurretStill = new CL_Sprite("SpaceShootTurretStill", world->resources);
		spriteTurretShooting = new CL_Sprite("SpaceShootTurretShooting", world->resources);
		spriteTurretReloading = new CL_Sprite("SpaceShootTurretReloading", world->resources);
		spriteTurretGunFlash = new CL_Sprite("SpaceShootTurretGunFlash", world->resources);
		spriteSelected = new CL_Sprite("SpaceShootSelected", world->resources);
		spriteRedGlow = new CL_Sprite("RedGlow", world->resources);

		collisionBody = new CL_CollisionOutline("Gfx/spaceshoot_body_still1.png");
		collisionBody->set_alignment(origin_center);

		soundTurret = new CL_SoundBuffer("SpaceShootTurret", world->resources);
		soundTurret->set_volume(0.3f);
		soundTurret->prepare();
		
		bodyTurnSpeed = 80.0f;
		turretTurnSpeed = 150.0f;
		moveSpeed = 100.0f;
		
		break;
	}
	
	bodyAngle = turretAngle = destBodyAngle = deltaBodyAngle = destTurretAngle = deltaTurretAngle = 0.0f;
	
	selected = false;
	isFiring = false;
	reverse = false;
	
	spriteBody = spriteBodyStill;
	spriteTurret = spriteTurretStill;
}

bool TankVehicle::isSelected() const
{
	return selected;
}

void TankVehicle::select()
{
	selected = true;
}

void TankVehicle::deselect()
{
	selected = false;
}

void TankVehicle::setPos(int x, int y)
{
	posX = destPosX = (float)x;
	posY = destPosY = (float)y;

	collisionBody->set_translation(posX, posY);
}

void TankVehicle::setTurretTargetPos(int x, int y)
{
	CL_Vector vector = CL_Vector(posX - x, posY - y);
	CL_Vector up = CL_Vector(0, 1);

	// Calculate angle from current sprite position to mouse position
	float angle = float(up.angle(vector) * 180.0f / PI);
	if(x < posX)
		angle = 360.0f - angle;

	setDestTurretAngle(angle);
}

void TankVehicle::setTargetPos(int x, int y)
{
	destPosX = (float)x;
	destPosY = (float)y;

	CL_Vector vector = CL_Vector(posX - destPosX, posY - destPosY);
	CL_Vector up = CL_Vector(0, 1);

	// Calculate angle from current sprite position to mouse position
	float angle = float(up.angle(vector) * 180.0f / PI);
	if(x < posX)
		angle = 360.0f - angle;

	setDestAngle(angle);
	
	// Calculate delta position (to be used per frame-update)
	deltaPosX = -(float)vector[0] / (float)(vector.norm());
	deltaPosY = -(float)vector[1] / (float)(vector.norm());

	spriteBody = spriteBodyMoving;
}

void TankVehicle::setAngle(float angle)
{
	bodyAngle = angle;
	collisionBody->set_angle(angle);
}

void TankVehicle::setTurretAngle(float angle)
{
	turretAngle = angle;
}

void TankVehicle::setDestAngle(float destAngle)
{
	destBodyAngle = destAngle;
	deltaBodyAngle = destBodyAngle - bodyAngle;
	
	if(deltaBodyAngle > 180.0f)
	{
		deltaBodyAngle -= 360.0f;
		bodyAngle += 360.0f;
	}
	if(deltaBodyAngle < -180.0f)
	{
		deltaBodyAngle += 360.0f;
		bodyAngle -= 360.0f;
	}
}

void TankVehicle::setDestTurretAngle(float destAngle)
{
	destTurretAngle = destAngle;
	deltaTurretAngle = destTurretAngle - turretAngle;
	
	if(deltaTurretAngle > 180.0f)
	{
		deltaTurretAngle -= 360.0f;
		turretAngle += 360.0f;
	}
	if(deltaTurretAngle < -180.0f)
	{
		deltaTurretAngle += 360.0f;
		turretAngle -= 360.0f;
	}
}

bool TankVehicle::hitCheck(CL_CollisionOutline *outline, GameObject *other)
{
	return collisionBody->collide(*outline);
}

bool TankVehicle::hitCheck(const CL_Rect &rect)
{
	int width = spriteBody->get_width();
	int height = spriteBody->get_height();
	
	return (rect.left <= posX + width / 2 && rect.top <= posY + height / 2 && rect.right >= posX - width / 2 && rect.bottom >= posY - height / 2);
}

bool TankVehicle::hitCheck(int x, int y)
{
	int width = spriteBody->get_width();
	int height = spriteBody->get_height();
	
	return (x >= posX - width / 2 && x <= posX + width / 2 && y >= posY - height / 2 && y <= posY + height / 2);
}

void TankVehicle::fire(bool nuke)
{
	if(isFiring == false)
	{
 		spriteTurret = spriteTurretShooting;
		spriteTurretShooting->restart();
	
		soundTurret->play();
		
		isFiring = true;
		
		int delta;
		if(nuke)
			delta = 18;
		else
			delta = 360;
			
		for(int i=0; i<360; i += delta)
		{
			Missile *missile = new Missile(world, this);
			missile->setPos((int)posX, (int)posY);
			missile->setSpeed(1000.0f);
			missile->setAngle(turretAngle + i);
			missile->move(43);
			world->addObject(missile);
		}
	}
}

bool TankVehicle::update(float timeElapsed)
{
	spriteBody->update(timeElapsed);
	spriteTurret->update(timeElapsed);

	if(spriteTurretShooting->is_finished() && spriteTurret == spriteTurretShooting)
	{
		spriteTurret = spriteTurretReloading;
		spriteTurretReloading->restart();
	}
	else if(spriteTurretReloading->is_finished() && spriteTurret == spriteTurretReloading)
	{
		spriteTurret = spriteTurretStill;
		spriteTurretStill->restart();

		isFiring = false;
	}

	if(deltaBodyAngle)
	{
		if(deltaBodyAngle > 0.0f)
		{
			bodyAngle += bodyTurnSpeed * timeElapsed;
			if(bodyAngle > destBodyAngle)
			{
				bodyAngle = destBodyAngle;
				deltaBodyAngle = 0.0f;
			}
		}
		else
		{
			bodyAngle -= bodyTurnSpeed * timeElapsed;
			if(bodyAngle < destBodyAngle)
			{
				bodyAngle = destBodyAngle;
				deltaBodyAngle = 0.0f;
			}
		}
	}
	
	if(deltaTurretAngle)
	{
		if(deltaTurretAngle > 0.0f)
		{
			turretAngle += turretTurnSpeed * timeElapsed;
			if(turretAngle > destTurretAngle)
			{
				turretAngle = destTurretAngle;
				deltaTurretAngle = 0.0f;
			}
		}
		else
		{
			turretAngle -= turretTurnSpeed * timeElapsed;
			if(turretAngle < destTurretAngle)
			{
				turretAngle = destTurretAngle;
				deltaTurretAngle = 0.0f;
			}
		}
	}
	
	if(deltaBodyAngle == 0.0f)
	{
		if(destPosX != posX || destPosY != posY)
		{
			posX += deltaPosX * moveSpeed * timeElapsed;
			posY += deltaPosY * moveSpeed * timeElapsed;
			collisionBody->set_translation(posX, posY);
			
			if((deltaPosX > 0 && posX > destPosX) || (deltaPosX < 0 && posX < destPosX))
				posX = destPosX;
			if((deltaPosY > 0 && posY > destPosY) || (deltaPosY < 0 && posY < destPosY))
				posY = destPosY;

			if(posX == destPosX && posY == destPosY)
			{
				spriteBody = spriteBodyStill;
				reverse = false;
			}
		}
	}
	
	spriteBody->set_angle(bodyAngle);
	spriteSelected->set_angle(bodyAngle);
	spriteTurret->set_angle(turretAngle);
	spriteTurretGunFlash->set_angle(turretAngle);
	
	collisionBody->set_angle(bodyAngle);
	
	if( !reverse )
	{
		if( world->hitCheck(collisionBody, this) )
		{
			setTargetPos(
				int(posX+cos((M_PI/180.0f)*(bodyAngle+90+int(rand()%60)-30))*30),
				int(posY+sin((M_PI/180.0f)*(bodyAngle+90+int(rand()%60)-30))*30));
			
			reverse = true;
		}
	}
	
	return true;
}

void TankVehicle::draw()
{
	// Draw selection	
	if(selected)
		spriteSelected->draw((int)posX, (int)posY);

	// Draw tankbody shadow
	spriteBody->set_blend_func(blend_zero, blend_one_minus_src_alpha);
	spriteBody->set_alpha(0.5f);
	spriteBody->draw((int)posX + 5, (int)posY + 5);

	// Draw tankbody
	spriteBody->set_blend_func(blend_src_alpha, blend_one_minus_src_alpha);
	spriteBody->set_alpha(1.0f);
	spriteBody->draw((int)posX, (int)posY);
	
	// Draw tankturret shadow
	spriteTurret->set_blend_func(blend_zero, blend_one_minus_src_alpha);
	spriteTurret->set_alpha(0.5f);
	spriteTurret->draw((int)posX + 5, (int)posY + 5);

	// Draw tankturret
	spriteTurret->set_blend_func(blend_src_alpha, blend_one_minus_src_alpha);
	spriteTurret->set_alpha(1.0f);
	spriteTurret->draw((int)posX, (int)posY);

	// Draw gunflash
	if(spriteTurret == spriteTurretShooting)
		spriteTurretGunFlash->draw((int)posX, (int)posY);
	
	// Draw glow
	spriteRedGlow->draw((int)posX, (int)posY);
}
