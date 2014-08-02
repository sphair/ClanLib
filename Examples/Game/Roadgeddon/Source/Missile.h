/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Arkadiusz Kalinowski
*/

#pragma once
#include "Precomp.h"
#include "Game.h"
#include "Gameobject.h"

class Missile;

class MissileDesc
{
public:

	enum MissileType
	{
		mt_bullet,
		mt_energy,
		mt_rocket

	} mType_;

	MissileDesc();
	~MissileDesc();
	
	void set_type(MissileType mType);
	void set_speed(float speed);
	void set_angle(Angle angle);
	void set_game(Game &game_);
	void should_hurt_player(bool value);
	void should_hurt_enemy(bool value);
	
	void fire();
	void set_pos(Vec2f pos);

private:
	
	Game *game;

	Vec2f pos_;
	Angle angle_;

	bool does_hurt_player_;
	bool does_hurt_enemy_;
	float speed_;

	friend class Missile;
};

//________________________________________________________________________
//________________________________________________________________________
class Missile : public Gameobject
{
//_____________________________________________________________________________________
//																C O N S T R U C T O R S
public:
	Missile(MissileDesc &desc);
	~Missile();

//_________________________________________________________________________________
//																A T T R I B U T E S
	virtual bool should_collide_with(Body &body);
//_________________________________________________________________________________
//																O P E R A T I O N S

	virtual void on_collision_begin(Body &body);
	virtual void on_collision_end(Body &body);

	void update(int time_elapsed_ms);
	void draw(Canvas &canvas);
//_________________________________________________________________________________________
//																I M P L E M E N T A T I O N
private:
	MissileDesc::MissileType mType;
	Game *game;

    SlotContainer cc;

	Sprite bullet;
	Vec2f pos;
	Angle angle;

	bool should_die;
	bool does_hurt_player;
	bool does_hurt_enemy;
	float lifeTime;
	float currentLifeTime;
	float speed;
	float x_speed,y_speed;
	float damage;



};

