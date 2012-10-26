/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#pragma once

#include "gameobject.h"

class Sprite;
class SoundBuffer;

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
	~Building();
	
// Attributes
public:
	
// Operations
public:
	void setPos(int x, int y);
	void setAngle(float angle);
	
	virtual void draw();
	virtual bool update(int timeElapsed_ms);
	virtual bool hitCheck(CollisionOutline *outline, GameObject *other);

// Implementation:
private:
	Sprite *sprite;
	CollisionOutline *collisionBuilding;
	
	float angle;	
	float posX;
	float posY;
	
	bool exploding;
};

