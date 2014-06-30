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
	Canvas &canvas = game_.get_canvas();
	PhysicsContext pc = game_.get_pc();
	ResourceManager &resources = game_.get_resources();

//________________________________________________________________________
//														   G A M E P L A Y
	type = go_enemy;
	eType = T_HOVERBOT;
	
	missile.set_game(*game);
	missile.set_speed(90.0f);
	missile.set_type(MissileDesc::mt_energy);
	missile.should_hurt_player(true);
	missile.should_hurt_enemy(false);
	turret_angle = Angle(0,angle_degrees);
	time_since_last_shoot = 0.0f;

//________________________________________________________________________
//															   R E N D E R
	enemy = Sprite::resource(canvas,"Enemy1", resources);
	enemy.set_play_loop(true);
	
	//int x,y;
	//Origin origin;
	//enemy->get_alignment(origin,x,y);

	update_callback = std::function<void(int)>(std::bind(&Enemy::update, this, std::placeholders::_1));
	cc.connect(game_.get_draw_sig(), std::function<void(Canvas&)>(this, &Enemy::draw));
    cc.connect(game_.get_update_sig(), update_callback);

//__________________________________________________________________________
//															   P H Y S I C S

	BodyDescription body_desc(pc);
	body_desc.set_position(0, game->get_height()-40);
	body_desc.set_type(body_dynamic);
	body_desc.set_angular_damping(100.0f);

	PolygonShape shape(pc);
	shape.set_as_box(enemy.get_width()/2.5f, enemy.get_height()/2.5f);

	FixtureDescription fixture_desc(pc);
	fixture_desc.set_density(1000.0f);
	fixture_desc.set_shape(shape);
	
	body = Body(pc, body_desc);
	body.set_data(this);
	Fixture(pc, body, fixture_desc);

	pos.x = 50;
	pos.y = 50;
	life = 100;
	is_dead = false;

	body.set_position(pos);

	speed = -35;

	body.set_linear_velocity(Vec2f(speed,0));

	target = Player::getPlayer1();
}

Enemy::~Enemy()
{
}
void Enemy::set_pos(float x,float y)
{
	pos.x = x;
	pos.y = y;
	body.set_position(pos);
}
void Enemy::update(int time_elapsed_ms)
{
	enemy.update(time_elapsed_ms);
	float time_elapsed = time_elapsed_ms * 0.001f;
	time_since_last_shoot+=time_elapsed_ms;
	pos = body.get_position();

	if(time_since_last_shoot>2000)
	{
		shoot();
	}
	else
	{
		Vec2f tpos = target->get_pos();
		aimAt( tpos.x,tpos.y);
	}
	if(is_dead || pos.x<-30)
	{
		remove_enemy(this);
		game->add_for_deletion(this);
		update_callback.clear();
	}
}
void Enemy::draw(Canvas &canvas)
{
	Vec2f pos = body.get_position();
	enemy.draw(canvas,pos.x,pos.y);
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

	Vec2f pos = body.get_position();

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

	Vec2f pos = body.get_position();
	missile.set_angle(turret_angle);
	missile.set_pos(pos);
	missile.fire();
	time_since_last_shoot=0;
	
}

bool Enemy::should_collide_with(Body &body)
{
	PhysicsObject *data = body.get_data();
	if(data!=nullptr)
	{
		//Gameobject *object = static_cast<Gameobject *> (data);
		return true;
	}
	else
		return false;
}

void Enemy::on_collision_begin(Body &body)
{

}

void Enemy::on_collision_end(Body &body)
{

}
