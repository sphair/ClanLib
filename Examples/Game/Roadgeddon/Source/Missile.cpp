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

#include "Precomp.h"
#include "Missile.h"
#include "Enemy.h"
#include "Player.h"

MissileDesc::MissileDesc()
{
	speed_ = 50.0f;
	pos_.x = 0.0f;
	pos_.y = 0.0f;
	mType_ = mt_bullet;
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

void MissileDesc::set_type(MissileType mType)
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
	type = go_missile;

	game	= desc.game;
	speed	= desc.speed_;
	angle	= desc.angle_;
	pos		= desc.pos_;
	mType   = desc.mType_;
	does_hurt_player = desc.does_hurt_player_;
	does_hurt_enemy = desc.does_hurt_enemy_;

	Canvas canvas = game->get_canvas();
	PhysicsContext pc = game->get_pc();
	ResourceManager &resources = game->get_resources();

	//___________________________________________________________________
	//																G F X

	switch(mType)
	{
	case MissileDesc::mt_bullet:
		bullet = Sprite::resource(canvas, "Bullet1", resources);
		damage = 20;
		break;
	case MissileDesc::mt_energy:
		bullet = Sprite::resource(canvas, "Bullet2", resources);
		damage = 10;
		break;
	case MissileDesc::mt_rocket:
		bullet = Sprite::resource(canvas, "Rocket", resources);
		damage = 50;
		break;
	}

	bullet.set_linear_filter(false);
	bullet.set_angle(angle);

	//___________________________________________________________________
	//													    P H Y S I C S

	BodyDescription body_desc(pc);
	body_desc.set_position(0, game->get_height()-40);
	body_desc.set_type(body_dynamic);

	PolygonShape shape(pc);
	shape.set_as_box(bullet.get_width()/4, bullet.get_height()/4);

	FixtureDescription fixture_desc(pc);
	fixture_desc.set_density(50.0f);
	fixture_desc.set_shape(shape);
	
	body = Body(pc, body_desc);
	body.set_data(this);
	Fixture(pc, body, fixture_desc);

	body.set_position(pos);
	body.set_angle(angle);

	x_speed = speed*cos(angle.to_radians());
	y_speed = speed*sin(angle.to_radians());

	body.set_linear_velocity(Vec2f(x_speed, y_speed));

	lifeTime= 3000.0f;
	currentLifeTime=0.0f;

	//___________________________________________________________________
	//													          C O R E

	cc.connect(game->get_update_sig(), clan::Callback<void(int)>(this,&Missile::update)); 
	cc.connect(game->get_draw_sig(), clan::Callback<void(Canvas&)>(this,&Missile::draw)); 
	should_die = false;
}

Missile::~Missile()
{

}

bool Missile::should_collide_with(Body &body)
{
	PhysicsObject *data = body.get_data();
	if(data!=nullptr)
	{
		Gameobject *object = static_cast<Gameobject *> (data);

		switch(object->get_type())
		{
		case go_player:
			if(does_hurt_player) return true;
			else return false;

		case go_enemy:
			if(does_hurt_enemy) return true;
			else return false;

		case go_missile:
			return true;

		default:
			return false;
		}
	}
	else
		return false;
}

void Missile::on_collision_begin(Body &body)
{
	PhysicsObject *data = body.get_data();

	if(data!=nullptr)
	{
		Gameobject *object = static_cast<Gameobject *> (data);
		object->hurt(damage);
		should_die = true;
	}
}

void Missile::on_collision_end(Body &body)
{

}

void Missile::update(int time_elapsed_ms)
{
	currentLifeTime += time_elapsed_ms;
	float time_elapsed = time_elapsed_ms*0.001f;
	
	pos.x+=x_speed*time_elapsed;
	pos.y+=y_speed*time_elapsed;

	if(currentLifeTime>lifeTime || should_die)
	{
		game->add_for_deletion(this);

		body.kill();
	}
}
void Missile::draw(Canvas &canvas)
{
	Vec2f pos = body.get_position();
	bullet.draw(canvas,pos.x,pos.y);
}
