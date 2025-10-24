#ifndef _MISSILE_H_
#define _MISSILE_H_

#include "gameobject.h"

class CL_Sprite;
class CL_SoundBuffer;

class Missile : public GameObject
{
// Construction
public:
	Missile(World *world, GameObject *owner);
	virtual ~Missile();
	
// Attributes
public:
	
// Operations
public:
	void setPos(int x, int y);
	void setAngle(float angle);
	void setSpeed(float speed);
	void move(float length);
	
	virtual void draw();
	virtual bool update(float timeElapsed);

// Implementation:
private:
	CL_Sprite *spriteMissile;
	CL_Sprite *spriteExplosion;
	CL_SoundBuffer *sound;
	
	CL_Sprite *sprite;
	CL_CollisionOutline *collisionMissile;
	
	GameObject *owner;
	
	float angle;
	float speed;
	
	float posX;
	float posY;
	
	bool hidden;
	bool exploding;
};

#endif
