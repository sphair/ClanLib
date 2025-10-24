#ifndef _TANKVEHICLE_H_
#define _TANKVEHICLE_H_

#include "gameobject.h"

class CL_Sprite;
class CL_SoundBuffer;
class World;

class TankVehicle : public GameObject
{
// Enums
public:
	enum TankType
	{
		SPACE_SHOOT
	};
	
// Construction
public:
	TankVehicle(TankType type, World *world);
	virtual ~TankVehicle() {}

// Attributes:
public:
	bool isSelected() const;
	
// Operations:
public:
	void setAngle(float angle);
	void setDestAngle(float angle);

	void setTurretAngle(float angle);
	void setDestTurretAngle(float angle);
	void setTurretTargetPos(int x, int y);
	
	void getPos(int &x, int &y);
	void setPos(int x, int y);
	void setTargetPos(int x, int y);
	
	void select();
	void deselect();
	
	void fire(bool nuke);
	
	bool hitCheck(CL_CollisionOutline *outline, GameObject *other);
	bool hitCheck(const CL_Rect &rect);
	bool hitCheck(int x, int y);
	
	virtual void draw();
	virtual bool update(float timeElapsed);

// Implementation:
private:
	CL_Sprite *spriteBody;
	CL_Sprite *spriteTurret;
	
	CL_Sprite *spriteBodyStill;
	CL_Sprite *spriteBodyMoving;
	CL_Sprite *spriteTurretStill;
	CL_Sprite *spriteTurretShooting;
	CL_Sprite *spriteTurretReloading;
	CL_Sprite *spriteSelected;
	CL_Sprite *spriteTurretGunFlash;
	CL_Sprite *spriteRedGlow;
	
	CL_SoundBuffer *soundTurret;

	CL_CollisionOutline *collisionBody;
	
	float bodyAngle;
	float destBodyAngle, deltaBodyAngle;

	float turretAngle;
	float destTurretAngle, deltaTurretAngle;

	float posX, posY;
	float destPosX, destPosY;
	float deltaPosX, deltaPosY;
	
	float bodyTurnSpeed;
	float turretTurnSpeed;
	float moveSpeed;
	
	bool selected;
	
	bool isFiring;
	bool reverse;
};

#endif
