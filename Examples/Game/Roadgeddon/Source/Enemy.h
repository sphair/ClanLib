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
#include "Missile.h"
#include "Gameobject.h"

class Enemy : public Gameobject
{
//_________________________________________________________________________
//																S T A T I C
public:
	static std::list<Enemy *> enemy_list;
	
	static void add_enemy(Enemy *object);
	static void remove_enemy(Enemy *object);
	
	static int staticID;

//Enums
	enum ENEMY_TYPE
	{
		T_HOVERBOT,
		T_WALKER,
		T_CAR

	} eType;
//_____________________________________________________________________________________
//																C O N S T R U C T I O N

	Enemy(Game &game_);
	~Enemy();

//_________________________________________________________________________________
//																A T T R I B U T E S
	virtual bool should_collide_with(Body &body);
//_________________________________________________________________________________
//																O P E R A T I O N S
public:

	virtual void on_collision_begin(Body &body);
	virtual void on_collision_end(Body &body);

	void draw(Canvas &canvas);
	void update(int time_elapsed_ms);
	void set_pos(float x,float y);
	virtual void hurt(float damage);
	void aimAtPlayer();

private:
	void aimAt(int x, int y);
	void shoot();

//_________________________________________________________________________________________
//																I M P L E M E N T A T I O N
	Game *game;
	Player *target;

	MissileDesc missile;

	Sprite enemy;

    Callback<void(int)> update_callback;
    CallbackContainer cc;

	Vec2f pos;
	Angle turret_angle;
	
	bool is_dead;
	int ID;
	float speed;
	float life;
	float time_since_last_shoot;
};
