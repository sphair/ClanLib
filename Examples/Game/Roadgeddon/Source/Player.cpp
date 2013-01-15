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
	ResourceManager &resources = game_.get_resources();

	vehicle		= new Sprite(gc,"Car1",&resources);
	col_out = CollisionOutline("Gfx/car1.png");

	int x,y;
	Origin origin;
	vehicle->get_alignment(origin,x,y);
	col_out.set_alignment(origin,x,y);

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
	missile.set_type(MissileDesc::T_BULLET);
	
	//________________________________________________________________________
	//															   R E N D E R
	
	draw_slot = game_.get_draw_sig().connect(this,&Player::draw);
	update_slot = game_.get_update_sig().connect(this,&Player::update);
	
	//________________________________________________________________________
	//															 P H Y S I C S
	
	
	pos_x=0;
	pos_y=game->get_height()-40;
	xAcc = 1600.0f;
	yAcc = 800.0f;

	x_max_speed=300.0f;
	y_max_speed=60.0f;

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
		if(go_up)	y_speed+=yAcc* time_elapsed;
		if(go_down)	y_speed-=yAcc* time_elapsed;

		if(x_speed>x_max_speed)	x_speed = x_max_speed;
		if(x_speed<-x_max_speed) x_speed = -x_max_speed;

		if(y_speed>y_max_speed) y_speed = y_max_speed;
		if(y_speed<-y_max_speed) y_speed = -y_max_speed;

		pos_x+=x_speed*time_elapsed;
		pos_y-=y_speed*time_elapsed;

		col_out.set_translation(pos_x,pos_y);
		turret->set_angle(turret_angle);

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

		if(pos_x<0)
		{
			pos_x=0.0f;
			x_speed=0.0f;
		}
		if(pos_x>width)
		{
			pos_x=(float)width;
			x_speed=0.0f;
		}

		if(pos_y>height-30)
		{
			pos_y=(float)(height-30);
			y_speed=0.0f;
		}
		if(pos_y<height-80)
		{
			pos_y=(float)(height-80);
			y_speed=0.0f;
		}

		time_since_last_shoot+=time_elapsed_ms;
	}
}
void Player::draw(Canvas &canvas)
{
	turretPos.x = pos_x-4;
	turretPos.y = pos_y-20;

	vehicle->draw(canvas,pos_x,pos_y);
	turret->draw(canvas,(int)turretPos.x,(int)turretPos.y);
	turretBase->draw(canvas,pos_x-18,pos_y-4);
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
	return Vec2f(pos_x,pos_y);
}

void Player::hurt(float damage)
{
	life-=damage;
	
	if(life<0)
	{
		game->play_sample(1);
		is_dead= true;
		col_out.set_translation(-999,-999);
		draw_slot.disable();
	}
	else
	{
		game->play_sample(3);
	}
}