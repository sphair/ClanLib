//===============================================================================
//
// LinearParticle Copyright (c) 2006 Wong Chin Foo
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in a
// product, an acknowledgment in the product documentation would be
// appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//===============================================================================


#include "precomp.h"
#include "L_Particle.h"


L_Particle::L_Particle()
{}


L_Particle::L_Particle(const L_Particle& par)
{
	surface = par.surface;
	vec.set(par.vec);

	life = par.life;
	remaining_life = par.remaining_life;
	motion_controller = par.motion_controller;
	ref_size = par.ref_size;
	current_size = par.current_size;
	current_radian = par.current_radian;
	set_color(par.current_color);

	if( par.coloring_type > 0 )
	{
		color1.a = par.color1.a;
		color1.r = par.color1.r;
		color1.g = par.color1.g;
		color1.b = par.color1.b;

		color2.a = par.color2.a;
		color2.r = par.color2.r;
		color2.g = par.color2.g;
		color2.b = par.color2.b;
	}

	switch_time = par.switch_time;
	coloring_type = par.coloring_type;
	last_time_e = par.last_time_e;
	sizing_type = par.sizing_type;
	size_ratio = par.size_ratio;
	size_ratio2 = par.size_ratio2;
	rotating_type = par.rotating_type;
	radian = par.radian;
	radian_rm = par.radian_rm;
	coloring_switch = par.coloring_switch;

	coloring_starting_time = par.coloring_starting_time;
	sizing_starting_time = par.sizing_starting_time;
	rotating_starting_time = par.rotating_starting_time;

	coloring_starting_f = par.coloring_starting_f;
	sizing_starting_f = par.sizing_starting_f;
	rotating_starting_f = par.rotating_starting_f;

	prerun_user_func = par.prerun_user_func;
	prerun_user_data = par.prerun_user_data;
}


L_Particle::L_Particle(clan::Sprite* surf, int life_t)
{
   surf->set_linear_filter(true);
	surface = surf;
	L_SET_ADDITIVE_BLENDING(*surface);

	life = life_t;
	remaining_life = life_t;

	//====default values
	x_pos = 200;
	y_pos = 200;
	motion_controller = NULL;
	ref_size = 1;
	current_size = 1;
	current_radian = 0.0f;
	current_color.r = 255;
	current_color.g = 255;
	current_color.b = 255;
	current_color.a = 255;
	switch_time = 0;
	coloring_type = 0;
	last_time_e = 0;
	sizing_type = 0;
	rotating_type = 0;
	coloring_switch = 0;

	coloring_starting_time = INT_MIN;
	sizing_starting_time = INT_MIN;
	rotating_starting_time = INT_MIN;

	coloring_starting_f = 0;
	sizing_starting_f = 0;
	rotating_starting_f = 0;

	prerun_user_func = NULL;
	prerun_user_data = NULL;
}


void L_Particle::copy_from(const L_Particle& par)
{
	surface = par.surface;
	vec.set(par.vec);

	life = par.life;
	remaining_life = par.remaining_life;
	motion_controller = par.motion_controller;
	ref_size = par.ref_size;
	current_size = par.current_size;
	current_radian = par.current_radian;
	set_color(par.current_color);

	if( par.coloring_type > 0 )
	{
		color1.a = par.color1.a;
		color1.r = par.color1.r;
		color1.g = par.color1.g;
		color1.b = par.color1.b;

		color2.a = par.color2.a;
		color2.r = par.color2.r;
		color2.g = par.color2.g;
		color2.b = par.color2.b;
	}

	switch_time = par.switch_time;
	coloring_type = par.coloring_type;
	last_time_e = par.last_time_e;
	sizing_type = par.sizing_type;
	size_ratio = par.size_ratio;
	size_ratio2 = par.size_ratio2;
	rotating_type = par.rotating_type;
	radian = par.radian;
	radian_rm = par.radian_rm;
	coloring_switch = par.coloring_switch;

	coloring_starting_time = par.coloring_starting_time;
	sizing_starting_time = par.sizing_starting_time;
	rotating_starting_time = par.rotating_starting_time;

	coloring_starting_f = par.coloring_starting_f;
	sizing_starting_f = par.sizing_starting_f;
	rotating_starting_f = par.rotating_starting_f;

	prerun_user_func = par.prerun_user_func;
	prerun_user_data = par.prerun_user_data;
}


void L_Particle::set_size(L_REAL size)
{
	current_size = size;
	ref_size = size;

	size_ratio *= ref_size;
	size_ratio2 *= ref_size;
}


void L_Particle::set_life(int life_t)
{
	life = life_t;
	remaining_life = life_t;

	if( coloring_starting_time > INT_MIN )
		set_coloring_sp(coloring_starting_f);

	if( sizing_starting_time > INT_MIN )
		set_sizing_sp(sizing_starting_f);

	if( rotating_starting_time > INT_MIN )
		set_rotating_sp(rotating_starting_f);
}


void L_Particle::set_color(const L_Color& color)
{
	current_color.a = color.a;
	current_color.r = color.r;
	current_color.g = color.g;
	current_color.b = color.b;
}


void L_Particle::set_color(const char a, const char r, const char g, const char b)
{
	current_color.a = a;
	current_color.r = r;
	current_color.g = g;
	current_color.b = b;
}


void L_Particle::set_velocity(const L_Vector& v_t)
{
	vec.set(v_t);

	if( motion_controller != NULL &&
		motion_controller->motion_type == L_1D_ACCELERATION )
	{
		accel_vec_1d_to_2d.set(v_t);
		accel_vec_1d_to_2d.set_magnitude(motion_controller->acceleration_1d);
	}
}


void L_Particle::set_position(L_REAL x, L_REAL y)
{
	x_pos = x;
	y_pos = y;
}


void L_Particle::set_rotation(L_REAL radian)
{
	current_radian = radian;
}


void L_Particle::set_rotation2(L_REAL radian)
{
	current_radian = radian;
	radian_rm = radian;
}


void L_Particle::set_motion_controller(L_MotionController* motion_ctr)
{
	motion_controller = motion_ctr;
}


void L_Particle::set_blending_mode(int mode)
{
	if( mode == L_NORMAL_BLENDING )
		L_SET_NORMAL_BLENDING(*surface);
	else if( mode == L_ADDITIVE_BLENDING )
		L_SET_ADDITIVE_BLENDING(*surface);
}


void L_Particle::set_prerun_callback(L_PRERUN_USERFUNC* user_func, void* user_data)
{
	prerun_user_func = user_func;
	prerun_user_data = user_data;
}


void L_Particle::remove(void)
{
	remaining_life = 0;
}


void L_Particle::set_coloring_sp(L_REAL begin_at)
{
	coloring_starting_time = static_cast<int>( remaining_life * (1.0f - begin_at) );
}


void L_Particle::set_sizing_sp(L_REAL begin_at)
{
	sizing_starting_time = static_cast<int>( remaining_life * (1.0f - begin_at) );
}


void L_Particle::set_rotating_sp(L_REAL begin_at)
{
	rotating_starting_time = static_cast<int>( remaining_life * (1.0f - begin_at) );
}


void L_Particle::coloring1(const L_Color& color1_t, L_REAL begin_at)
{
	color1.a = current_color.a;
	color1.r = current_color.r;
	color1.g = current_color.g;
	color1.b = current_color.b;

	color2.a = color1_t.a;
	color2.r = color1_t.r;
	color2.g = color1_t.g;
	color2.b = color1_t.b;

	coloring_type = 1;

	set_coloring_sp(begin_at);
	coloring_starting_f = begin_at;
}


void L_Particle::coloring2(const L_Color& color1_t, const L_Color& color2_t, L_REAL begin_at)
{
	color1.a = color1_t.a;
	color1.r = color1_t.r;
	color1.g = color1_t.g;
	color1.b = color1_t.b;

	color2.a = color2_t.a;
	color2.r = color2_t.r;
	color2.g = color2_t.g;
	color2.b = color2_t.b;

	coloring_type = 1;

	set_coloring_sp(begin_at);
	coloring_starting_f = begin_at;
}


void L_Particle::coloring3(const L_Color& color1_t, const L_Color& color2_t, unsigned int switch_time_t, L_REAL begin_at)
{
	color1.a = color1_t.a;
	color1.r = color1_t.r;
	color1.g = color1_t.g;
	color1.b = color1_t.b;

	color2.a = color2_t.a;
	color2.r = color2_t.r;
	color2.g = color2_t.g;
	color2.b = color2_t.b;

	switch_time = switch_time_t;

	coloring_type = 2;

	set_coloring_sp(begin_at);
	coloring_starting_f = begin_at;
}


void L_Particle::sizing1(L_REAL size_r, L_REAL begin_at)
{
	sizing_type = 1;
	size_ratio = current_size;
	size_ratio2 = size_r;

	set_sizing_sp(begin_at);
	sizing_starting_f = begin_at;
}


void L_Particle::sizing2(L_REAL size_r1, L_REAL size_r2, L_REAL begin_at)
{
	sizing_type = 1;
	size_ratio = size_r1;
	size_ratio2 = size_r2;

	set_sizing_sp(begin_at);
	sizing_starting_f = begin_at;
}


void L_Particle::sizing3(L_REAL size_r, L_REAL begin_at)
{
	sizing_type = 2;
	size_ratio = size_r;

	set_sizing_sp(begin_at);
	sizing_starting_f = begin_at;
}


void L_Particle::rotating1(L_REAL radian_t, L_REAL begin_at)
{
	rotating_type = 1;
	radian = radian_t;

	set_rotating_sp(begin_at);
	rotating_starting_f = begin_at;
}


void L_Particle::rotating2(L_REAL radian_t, L_REAL begin_at)
{
	rotating_type = 2;
	radian = radian_t;
	radian_rm = current_radian;

	set_rotating_sp(begin_at);
	rotating_starting_f = begin_at;
}


void L_Particle::rotating3(L_REAL begin_at)
{
	rotating_type = 3;

	set_rotating_sp(begin_at);
	rotating_starting_f = begin_at;
}


void L_Particle::rotating4(L_REAL begin_at)
{
	rotating_type = 4;

	set_rotating_sp(begin_at);
	rotating_starting_f = begin_at;
}


void L_Particle::disable_coloring(void)
{
	coloring_starting_time = INT_MIN;
}


void L_Particle::disable_sizing(void)
{
	sizing_starting_time = INT_MIN;
}


void L_Particle::disable_rotating(void)
{
	rotating_starting_time = INT_MIN;
}


L_REAL L_Particle::get_size(void)
{
	return current_size;
}


L_REAL L_Particle::get_ref_size(void)
{
	return ref_size;
}


L_REAL L_Particle::get_radian(void)
{
	return current_radian;
}


L_Vector L_Particle::get_velocity(void)
{
	return vec;
}


int L_Particle::get_remaininig_life(void)
{
	return remaining_life;
}


bool L_Particle::is_alive(void)
{
	if(remaining_life <= 0)
		return false;

	else
		return true;
}


void L_Particle::coloring_process(void)
{
	if(remaining_life <= coloring_starting_time)
	{
		//coloring_type = 1a 1b
		if(coloring_type == 1)
		{
			current_color.a = color1.a - (color1.a - color2.a) * (coloring_starting_time - remaining_life)/coloring_starting_time;
			current_color.r = color1.r - (color1.r - color2.r) * (coloring_starting_time - remaining_life)/coloring_starting_time;
			current_color.g = color1.g - (color1.g - color2.g) * (coloring_starting_time - remaining_life)/coloring_starting_time;
			current_color.b = color1.b - (color1.b - color2.b) * (coloring_starting_time - remaining_life)/coloring_starting_time;
		}

		//coloring_type = 2
		else
		{
			static int time_e;
			time_e = (coloring_starting_time - remaining_life) % switch_time;

			if(time_e < last_time_e)
				coloring_switch = (coloring_switch+1) % 2;

			if(coloring_switch == 0)
			{
				current_color.a = color1.a - (color1.a - color2.a) * time_e/switch_time;
				current_color.r = color1.r - (color1.r - color2.r) * time_e/switch_time;
				current_color.g = color1.g - (color1.g - color2.g) * time_e/switch_time;
				current_color.b = color1.b - (color1.b - color2.b) * time_e/switch_time;
			}

			else
			{
				current_color.a = color2.a - (color2.a - color1.a) * time_e/switch_time;
				current_color.r = color2.r - (color2.r - color1.r) * time_e/switch_time;
				current_color.g = color2.g - (color2.g - color1.g) * time_e/switch_time;
				current_color.b = color2.b - (color2.b - color1.b) * time_e/switch_time;
			}

			last_time_e = time_e;
		}
	}
}


void L_Particle::sizing_process(void)
{
	if(remaining_life <= sizing_starting_time)
	{
		if(sizing_type == 1)
		{
			current_size = size_ratio - (size_ratio - size_ratio2) * (L_REAL)(sizing_starting_time - remaining_life)/(L_REAL)sizing_starting_time;
		}

		//sizing_type = 2
		else
		{
			current_size = current_size + size_ratio * time_elapesed;
		}
	}
}


void L_Particle::rotating_process(void)
{
	if(remaining_life <= rotating_starting_time)
	{
		//rotating_type = 1
		if(rotating_type == 1)
		{
			current_radian = current_radian + radian*time_elapesed;

			if(current_radian >= L_2PI)
				current_radian -= L_2PI;

			else if(current_radian <= -L_2PI)
				current_radian += L_2PI;
		}

		//rotating_type = 2
		else if(rotating_type == 2)
		{
			current_radian = radian_rm + (L_REAL)(rotating_starting_time - remaining_life)/(L_REAL)rotating_starting_time * radian;
		}

		//rotating_type = 3
		else if(rotating_type == 3)
		{
			current_radian = L_RAND_REAL_2() * L_2PI;
		}

		//rotating_type = 4
		else
		{
			L_REAL current_radian_t = vec.get_angle();

			if( current_radian_t != L_REAL_MIN )
				current_radian = radian_rm + current_radian_t;
		}
	}
}


void L_Particle::motion_process(void)
{
	if( motion_controller != NULL )
	{
		if( motion_controller->motion_type == L_1D_ACCELERATION )
			vec = vec + accel_vec_1d_to_2d.get_scaled_vector(time_elapesed);

		else if( motion_controller->motion_type == L_2D_ACCELERATION )
			vec = vec + motion_controller->acceleration_2d.get_scaled_vector(time_elapesed);

		else if( motion_controller->motion_type == L_POINT_ACCELERATION )
		{
			L_Vector acc = motion_controller->acceleration_point_pos - L_Vector(x_pos, y_pos);
			acc.set_magnitude(motion_controller->acceleration_point_mag);

			vec = vec + acc.get_scaled_vector(time_elapesed);
		}

		if( vec.get_sqr_magnitude() > motion_controller->speed_limit_sqr )
			vec.set_magnitude(motion_controller->speed_limit);
	}

	x_pos += vec.x*time_elapesed;
	y_pos += vec.y*time_elapesed;
}


void L_Particle::initialize(void)
{
	time_elapesed = 0;
	coloring_process();
	sizing_process();
	rotating_process();
}


void L_Particle::run(int time_elapesed_t)
{
	time_elapesed = time_elapesed_t;

	if( prerun_user_func )
		prerun_user_func(*this, prerun_user_data);

	coloring_process();
	sizing_process();
	rotating_process();
	motion_process();

	if(life < L_INFINITE_LIFE)
		remaining_life -= time_elapesed_t;
}


void L_Particle::draw(clan::Canvas &canvas, int x_shift, int y_shift)
{

	surface->set_scale(current_size,current_size);
	surface->set_angle(clan::Angle(current_radian, clan::angle_radians));
	surface->set_color( clan::Color(current_color.r,current_color.g,current_color.b,current_color.a) );
	surface->draw(canvas, x_pos+x_shift, y_pos+y_shift);
}
