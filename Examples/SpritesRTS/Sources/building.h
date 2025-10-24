#ifndef _BUILDING_H_
#define _BUILDING_H_

#include "gameobject.h"

class CL_Sprite;
class CL_SoundBuffer;

class Building : public GameObject
{
// Enums
public:
	enum BuildingType
	{
		HELI_PAD
	};

// Construction
public:
	Building(BuildingType building_type, World *world);
	virtual ~Building();
	
// Attributes
public:
	
// Operations
public:
	void setPos(int x, int y);
	void setAngle(float angle);
	
	virtual void draw();
	virtual bool update(float timeElapsed);
	virtual bool hitCheck(CL_CollisionOutline *outline, GameObject *other);

// Implementation:
private:
	CL_Sprite *sprite;
	CL_CollisionOutline *collisionBuilding;
	
	float angle;	
	float posX;
	float posY;
	
	bool exploding;
};

#endif
