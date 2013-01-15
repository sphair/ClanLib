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

#include "Enemy.h"
#include "Player.h"

//init static data
int Enemy::staticID = 0;
std::list<Enemy *> Enemy::enemy_list;

void Enemy::add_enemy(Enemy *object)
{
	Enemy::enemy_list.push_back(object);
}
void Enemy::remove_enemy(Enemy *object)
{
	int ID = object->ID;
	std::list<Enemy *>::iterator it;
	for(it=enemy_list.begin(); it!= enemy_list.end(); ++it)
	{
		Enemy *enemy = (*it);
		if(enemy->ID==ID)
		{
			enemy_list.erase(it);
			break;
		}
	}
}
//

Enemy::Enemy(Game &game_)
{
	

	ID = staticID;
	staticID++;
	add_enemy(this);

	game = &game_;
	GraphicContext &gc = game_.get_gc();
	ResourceManager &resources = game_.get_resources();

//________________________________________________________________________
//														   G A M E P L A Y
	eType = T_HOVERBOT;
	
	missile.set_game(*game);
	missile.set_speed(300.0f);
	missile.set_type(MissileDesc::T_ENERGY);
	missile.should_hurt_player(true);
	missile.should_hurt_enemy(false);
	turret_angle = Angle(0,angle_degrees);
	time_since_last_shoot = 0.0f;

//________________________________________________________________________
//															   R E N D E R
	enemy = new Sprite(gc,"Enemy1",&resources);
	enemy->set_play_loop(true);

	col_out =  CollisionOutline("Gfx/Enemy1_frame1.png");
	
	int x,y;
	Origin origin;
	enemy->get_alignment(origin,x,y);
	col_out.set_alignment(origin,x,y);

	draw_slot = game_.get_draw_sig().connect(this,&Enemy::draw);
	update_slot = game_.get_update_sig().connect(this,&Enemy::update);
	
	pos.x = 50;
	pos.y = 50;
	life = 100;
	is_dead = false;

	speed = 100;

	target = Player::getPlayer1();
}

Enemy::~Enemy()
{
	if(enemy!=NULL) delete enemy;
	
}
void Enemy::set_pos(float x,float y)
{
	pos.x = x;
	pos.y = y;
	col_out.set_translation(pos.x,pos.y);
}
void Enemy::update(int time_elapsed_ms)
{
	enemy->update(time_elapsed_ms);
	float time_elapsed = time_elapsed_ms * 0.001f;
	time_since_last_shoot+=time_elapsed_ms;
	pos.x -=speed*time_elapsed;
	col_out.set_translation(pos.x,pos.y);

	if(time_since_last_shoot>2000)
	{
		shoot();
	}
	else
	{
		Vec2f tpos = target->get_pos();
		aimAt( tpos.x,tpos.y);
	}
	if(is_dead || pos.x<0)
	{
		remove_enemy(this);
		game->add_for_deletion(this);
		update_slot.disable();
	}
}
void Enemy::draw(Canvas &canvas)
{
	enemy->draw(canvas,pos.x,pos.y);

}

void Enemy::hurt(float damage)
{
	life-=damage;
	game->play_sample(2);

	if(life<0)
	{
		game->play_sample(1);
		is_dead = true;
	}

}

void Enemy::aimAt(int x, int y)
{

	float dx= x-pos.x;
	float dy= y-pos.y;
	float result = -(atan2(dx,dy))*(180/PI);

	result = result + 90;
	if(result<0)
	{
		result = result +360;
	}

	turret_angle.set_degrees(result);
}

void Enemy::shoot()
{
	Vec2f targetPos = target->get_pos();
		
	aimAt(targetPos.x,targetPos.y);

	missile.set_angle(turret_angle);
	missile.set_pos(pos);
	missile.fire();
	time_since_last_shoot=0;
	
}