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

#include "Missile.h"
#include "Enemy.h"
#include "Player.h"

MissileDesc::MissileDesc()
{
	speed_ = 500.0f;
	pos_.x = 0.0f;
	pos_.y = 0.0f;
	mType_ = T_BULLET;
	does_hurt_player_ = false;
	does_hurt_enemy_ = true;

	angle_ = Angle(0,angle_degrees);
}

MissileDesc::~MissileDesc()
{

}
void MissileDesc::should_hurt_player(bool value)
{
	does_hurt_player_ = value;
}
void MissileDesc::should_hurt_enemy(bool value)
{
	does_hurt_enemy_ = value;
}
void MissileDesc::set_game(Game &game_)
{
	game = &game_;
}
void MissileDesc::set_pos(Vec2f pos)
{
	pos_ = pos;
}

void MissileDesc::set_speed(float speed)
{
	speed_ = speed;
}

void MissileDesc::set_angle(Angle angle)
{
	angle_ = angle;
}

void MissileDesc::set_type(MISSILE_TYPE mType)
{
	mType_ = mType;
}
void MissileDesc::fire()
{
	new Missile(*this);
}

//________________________________________________________________________
//________________________________________________________________________
Missile::Missile(MissileDesc &desc)
{
	game	= desc.game;
	speed	= desc.speed_;
	angle	= desc.angle_;
	pos		= desc.pos_;
	mType   = desc.mType_;
	does_hurt_player = desc.does_hurt_player_;
	does_hurt_enemy = desc.does_hurt_enemy_;

	GraphicContext &gc = game->get_gc();
	ResourceManager &resources = game->get_resources();

	//___________________________________________________________________
	//																G F X

	switch(mType)
	{
	case MissileDesc::T_BULLET:
		bullet = new Sprite(gc,"Bullet1",&resources);
		col_out =  CollisionOutline("Gfx/bullet1.png");
		damage = 20;
		break;
	case MissileDesc::T_ENERGY:
		bullet = new Sprite(gc,"Bullet2",&resources);
		col_out =  CollisionOutline("Gfx/bullet2.png");
		damage = 10;
		break;
	case MissileDesc::T_ROCKET:
		bullet = new Sprite(gc,"Rocket",&resources);
		col_out =  CollisionOutline("Gfx/rocket.png");
		damage = 50;
		break;
	}

	int x,y;
	Origin origin;
	bullet->get_alignment(origin,x,y);
	col_out.set_alignment(origin,x,y);

	bullet->set_linear_filter(false);
	bullet->set_angle(angle);

	//___________________________________________________________________
	//													    P H Y S I C S

	x_speed = speed*cos(angle.to_radians());
	y_speed = speed*sin(angle.to_radians());

	lifeTime= 2000.0f;
	currentLifeTime=0.0f;
	col_out.set_translation(pos.x,pos.y);
	col_out.set_angle(angle);

	//___________________________________________________________________
	//													          C O R E

	update_slot	= game->get_update_sig().connect(this,&Missile::update); 
	draw_slot	= game->get_draw_sig().connect(this,&Missile::draw); 

}

Missile::~Missile()
{
	if(bullet!=NULL) delete bullet;
}
void Missile::update(int time_elapsed_ms)
{
	currentLifeTime += time_elapsed_ms;
	float time_elapsed = time_elapsed_ms*0.001f;
	pos.x+=x_speed*time_elapsed;
	pos.y+=y_speed*time_elapsed;

	col_out.set_translation(pos.x,pos.y);

	if(currentLifeTime>lifeTime)
	{
		game->add_for_deletion(this);
	}


	if(does_hurt_enemy)
	{
		std::list<Enemy *>::iterator it;
		for(it=Enemy::enemy_list.begin(); it!= Enemy::enemy_list.end(); ++it)
		{
			Enemy *enemy = (*it);
			if(enemy->get_collision_outline().collide(col_out))
			{
				enemy->hurt(damage);
				game->add_for_deletion(this);
				break;
			}
		}
	}

	if(does_hurt_player)
	{
		Player *player= Player::getPlayer1();
		if(player->get_collision_outline().collide(col_out))
		{
			player->hurt(damage);
			game->add_for_deletion(this);
		}
	}
}
void Missile::draw(Canvas &canvas)
{
	bullet->draw(canvas,pos.x,pos.y);
}