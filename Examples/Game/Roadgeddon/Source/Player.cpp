<<<<<<< .mine
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

#include "Player.h"
#include "Game.h"
#include "Missile.h"

//Init static vars
Player *Player::player1 = NULL;
bool Player::isPlayer1Playing = false;
//

Player* Player::getPlayer1()
{
	if(isPlayer1Playing) return Player::player1;
	else return NULL;
}

Player::Player(Game &game_)
{
	//Init static vars

	Player::player1 = this;
	Player::isPlayer1Playing = true;

	//
	game = &game_;
	GraphicContext gc = game_.get_gc();
	PhysicsContext pc = game_.get_pc();
	ResourceManager &resources = game_.get_resources();

	vehicle		= new Sprite(gc,"Car1",&resources);

	vehicle->set_linear_filter(false);
	
	//________________________________________________________________________
	//															   T U R R E T	
	turret		= new Sprite(gc,"Turret1",&resources);
	turretBase	= new Sprite(gc,"Turret1Base",&resources);
	
	turret->set_linear_filter(false);
	turretBase->set_linear_filter(false);
	
	turret_angle = Angle(0,angle_degrees);
	time_since_last_shoot = 0.0f;

	missile.set_game(*game);
	missile.set_speed(500.0f);
	missile.set_type(MissileDesc::mt_bullet);
	
	//________________________________________________________________________
	//															   R E N D E R
	
	draw_slot = game_.get_draw_sig().connect(this,&Player::draw);
	update_slot = game_.get_update_sig().connect(this,&Player::update);
	
	//________________________________________________________________________
	//															 P H Y S I C S
	
	BodyDescription body_desc(pc);
	body_desc.set_position(0, game->get_height()-40);
	body_desc.set_type(body_dynamic);
	body_desc.set_angular_damping(100.0f);

	PolygonShape shape(pc);
	shape.set_as_box(vehicle->get_width()/2.5f, vehicle->get_height()/4.0f, Vec2f(0.0f, 5.0f), Angle(0, angle_degrees));

	FixtureDescription fixture_desc(pc);
	fixture_desc.set_density(1000.0f);
	fixture_desc.set_shape(shape);
	
	body = Body(pc, body_desc);
	body.set_data(this);
	Fixture(pc, body, fixture_desc);

	/*
	pos_x=0;
	pos_y=game->get_height()-40;
	*/
	xAcc = 1000.0f;
	yAcc = 600.0f;

	x_max_speed=80.0f;
	y_max_speed=40.0f;

	x_speed=0.0f;
	y_speed=0.0f;

	go_right = false;
	go_left = false;
	go_up = false;
	go_down = false;
	doShoot1 = false;
	doShoot2 = false;

	wobble_timer = 0;

	turretBasePos.x = 0.0f;
	turretBasePos.y = 0.0f;

	turretPos.x = 0.0f;
	turretPos.y = 0.0f;
	//__________________________________________________________________________
	//															 G A M E P L A Y
	type = go_player;

	life = max_life = 100.0f;
	is_dead= false;
	target_x_pos = 0.0f;
	target_y_pos = 0.0f;

}

Player::~Player()
{
	if(vehicle!=NULL) delete vehicle;
	if(turret!=NULL) delete turret;
	if(turretBase!=NULL) delete turretBase;
}

void Player::update(int time_elapsed_ms)
{
	if(!is_dead)
	{
		aim_turret();

		float time_elapsed = time_elapsed_ms*0.001f;
		if(go_right) x_speed+=xAcc* time_elapsed;
		if(go_left)	x_speed-=xAcc* time_elapsed;
		if(go_up)	y_speed-=yAcc* time_elapsed;
		if(go_down)	y_speed+=yAcc* time_elapsed;

		if(x_speed>x_max_speed)	x_speed = x_max_speed;
		if(x_speed<-x_max_speed) x_speed = -x_max_speed;

		if(y_speed>y_max_speed) y_speed = y_max_speed;
		if(y_speed<-y_max_speed) y_speed = -y_max_speed;

		body.set_linear_velocity(Vec2f(x_speed,y_speed));

		turret->set_angle(turret_angle);
		vehicle->set_angle(body.get_angle());

		if(doShoot1)
		{
			if(time_since_last_shoot>100)
			{
				game->play_sample(5);
				missile.set_angle(turret_angle);
				missile.set_pos(turretPos);
				missile.fire();
				time_since_last_shoot=0;
			}
		}

		int width = game->get_width();
		int height = game->get_height();
		Vec2f pos =  body.get_position();

		if(pos.x<0)
		{
			
			body.set_position(Vec2f(0.0f, pos.y));
			x_speed=0.0f;
		}
		if(pos.x>width)
		{
			body.set_position(Vec2f(width, pos.y));
			x_speed=0.0f;
		}

		if(pos.y>height-25)
		{
			body.set_position(Vec2f(pos.x, height-25));
			y_speed=0.0f;
		}
		if(pos.y<height-70)
		{
			body.set_position(Vec2f(pos.x, height-70));
			y_speed=0.0f;
		}

		time_since_last_shoot+=time_elapsed_ms;
		wobble_timer+=time_elapsed_ms;

		turretBasePos.x = pos.x-20;
		turretBasePos.y = pos.y-9;

		turretPos.x = turretBasePos.x+14;
		turretPos.y = turretBasePos.y-18;

		if(wobble_timer>250)
		{
			wobble_timer = 0;
		}
	}
}
void Player::draw(Canvas &canvas)
{
	Vec2f pos = body.get_position();
	float wobble = wobble_timer>125 ? 1 : 0;

	vehicle->draw(canvas,pos.x,pos.y+wobble);
	turretBase->draw(canvas,turretBasePos.x+4,turretBasePos.y-4+wobble);
	turret->draw(canvas,turretPos.x,turretPos.y+wobble);
	turretBase->draw(canvas,turretBasePos.x,turretBasePos.y+wobble);
}

bool Player::should_collide_with(Body &body)
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
void Player::on_collision_begin(Body &body)
{

}
void Player::on_collision_end(Body &body)
{

}
//_______________________________________________________________________
//													      C O N T R O L S

void Player::right(bool state)
{
	go_right = state;
}
void Player::left(bool state)
{
	go_left = state;
}
void Player::up(bool state)
{
	go_up = state;
}
void Player::down(bool state)
{
	go_down = state;
}
void Player::shoot1(bool state)
{
	doShoot1 = state;
}
void Player::shoot2(bool state)
{
	doShoot2 = state;
}

void Player::aim_at(int x, int y)
{
	target_x_pos = (float)x;
	target_y_pos = (float)y;
}

void Player::aim_turret()
{
	float dx= target_x_pos-turretPos.x;
	float dy= target_y_pos-turretPos.y;
	float result = -(atan2(dx,dy))*(180/PI);

	result = result + 90;
	if(result<0)
	{
		result = result +360;
	}

	turret_angle.set_degrees(result);
}

Vec2f Player::get_pos()
{
	return body.get_position();
}

void Player::hurt(float damage)
{
	life-=damage;
	
	if(life<0)
	{
		game->play_sample(1);
		is_dead= true;
		draw_slot.disable();

		body.kill();
	}
	else
	{
		game->play_sample(3);
	}
=======
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

#include "Player.h"
#include "Game.h"
#include "Missile.h"

//Init static vars
Player *Player::player1 = NULL;
bool Player::isPlayer1Playing = false;
//

Player* Player::getPlayer1()
{
	if(isPlayer1Playing) return Player::player1;
	else return NULL;
}

Player::Player(Game &game_)
{
	//Init static vars

	Player::player1 = this;
	Player::isPlayer1Playing = true;

	//
	game = &game_;
	GraphicContext gc = game_.get_gc();
	PhysicsContext pc = game_.get_pc();
	ResourceManager &resources = game_.get_resources();

	vehicle		= new Sprite(gc,"Car1",&resources);

	vehicle->set_linear_filter(false);
	
	//________________________________________________________________________
	//															   T U R R E T	
	turret		= new Sprite(gc,"Turret1",&resources);
	turretBase	= new Sprite(gc,"Turret1Base",&resources);
	
	turret->set_linear_filter(false);
	turretBase->set_linear_filter(false);
	
	turret_angle = Angle(0,angle_degrees);
	time_since_last_shoot = 0.0f;

	missile.set_game(*game);
	missile.set_speed(500.0f);
	missile.set_type(MissileDesc::mt_bullet);
	
	//________________________________________________________________________
	//															   R E N D E R
	
	draw_slot = game_.get_draw_sig().connect(this,&Player::draw);
	update_slot = game_.get_update_sig().connect(this,&Player::update);
	
	//________________________________________________________________________
	//															 P H Y S I C S
	
	BodyDescription body_desc(pc);
	body_desc.set_position(0, game->get_height()-40);
	body_desc.set_type(body_dynamic);
	body_desc.set_angular_damping(100.0f);

	PolygonShape shape(pc);
	shape.set_as_box(vehicle->get_width()/2.5f, vehicle->get_height()/4.0f, Vec2f(0.0f, 10.0f), Angle(0, angle_degrees));

	FixtureDescription fixture_desc(pc);
	fixture_desc.set_density(1000.0f);
	fixture_desc.set_shape(shape);
	
	body = Body(pc, body_desc);
	body.set_data(this);
	Fixture(pc, body, fixture_desc);

	/*
	pos_x=0;
	pos_y=game->get_height()-40;
	*/
	xAcc = 1000.0f;
	yAcc = 600.0f;

	x_max_speed=80.0f;
	y_max_speed=40.0f;

	x_speed=0.0f;
	y_speed=0.0f;
	

	go_right = false;
	go_left = false;
	go_up = false;
	go_down = false;
	doShoot1 = false;
	doShoot2 = false;

	//__________________________________________________________________________
	//															 G A M E P L A Y
	type = go_player;

	life = max_life = 100.0f;
	is_dead= false;
	target_x_pos = 0.0f;
	target_y_pos = 0.0f;

}

Player::~Player()
{
	if(vehicle!=NULL) delete vehicle;
	if(turret!=NULL) delete turret;
	if(turretBase!=NULL) delete turretBase;
}

void Player::update(int time_elapsed_ms)
{
	if(!is_dead)
	{
		aim_turret();

		float time_elapsed = time_elapsed_ms*0.001f;
		if(go_right) x_speed+=xAcc* time_elapsed;
		if(go_left)	x_speed-=xAcc* time_elapsed;
		if(go_up)	y_speed-=yAcc* time_elapsed;
		if(go_down)	y_speed+=yAcc* time_elapsed;

		if(x_speed>x_max_speed)	x_speed = x_max_speed;
		if(x_speed<-x_max_speed) x_speed = -x_max_speed;

		if(y_speed>y_max_speed) y_speed = y_max_speed;
		if(y_speed<-y_max_speed) y_speed = -y_max_speed;

		body.set_linear_velocity(Vec2f(x_speed,y_speed));

		turret->set_angle(turret_angle);
		vehicle->set_angle(body.get_angle());

		if(doShoot1)
		{
			if(time_since_last_shoot>100)
			{
				game->play_sample(5);
				missile.set_angle(turret_angle);
				missile.set_pos(turretPos);
				missile.fire();
				time_since_last_shoot=0;
			}
		}

		int width = game->get_width();
		int height = game->get_height();
		Vec2f pos =  body.get_position();

		if(pos.x<0)
		{
			
			body.set_position(Vec2f(0.0f, pos.y));
			x_speed=0.0f;
		}
		if(pos.x>width)
		{
			body.set_position(Vec2f(width, pos.y));
			x_speed=0.0f;
		}

		if(pos.y>height-30)
		{
			body.set_position(Vec2f(pos.x, height-30));
			y_speed=0.0f;
		}
		if(pos.y<height-80)
		{
			body.set_position(Vec2f(pos.x, height-80));
			y_speed=0.0f;
		}

		time_since_last_shoot+=time_elapsed_ms;
	}
}
void Player::draw(Canvas &canvas)
{
	Vec2f pos = body.get_position();
	turretPos.x = pos.x-4;
	turretPos.y = pos.y-20;

	vehicle->draw(canvas,pos.x,pos.y);
	turret->draw(canvas,(int)turretPos.x,(int)turretPos.y);
	turretBase->draw(canvas,pos.x-18,pos.y-4);
}

bool Player::should_collide_with(Body &body)
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
void Player::on_collision_begin(Body &body)
{

}
void Player::on_collision_end(Body &body)
{

}
//_______________________________________________________________________
//													      C O N T R O L S

void Player::right(bool state)
{
	go_right = state;
}
void Player::left(bool state)
{
	go_left = state;
}
void Player::up(bool state)
{
	go_up = state;
}
void Player::down(bool state)
{
	go_down = state;
}
void Player::shoot1(bool state)
{
	doShoot1 = state;
}
void Player::shoot2(bool state)
{
	doShoot2 = state;
}

void Player::aim_at(int x, int y)
{
	target_x_pos = (float)x;
	target_y_pos = (float)y;
}

void Player::aim_turret()
{
	float dx= target_x_pos-turretPos.x;
	float dy= target_y_pos-turretPos.y;
	float result = -(atan2(dx,dy))*(180/PI);

	result = result + 90;
	if(result<0)
	{
		result = result +360;
	}

	turret_angle.set_degrees(result);
}

Vec2f Player::get_pos()
{
	return body.get_position();
}

void Player::hurt(float damage)
{
	life-=damage;
	
	if(life<0)
	{
		game->play_sample(1);
		is_dead= true;
		draw_slot.disable();

		body.kill();
	}
	else
	{
		game->play_sample(3);
	}
>>>>>>> .r9615
}