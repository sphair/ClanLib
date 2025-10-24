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
#include "L_ShootingEffect.h"


L_ShootingEffect::L_ShootingEffect(int x, int y, const L_Vector& shooting_vector_t, int period_t, int particle_per_period_t) :
L_ParticleEffect(period_t,x,y)
{
	set_shooting_vector(shooting_vector_t);
	particle_per_period = particle_per_period_t;
	angle_interval = 0;
	width_interval = 0;
	speed_dis = 0;
}


L_ShootingEffect::L_ShootingEffect(const L_ShootingEffect& cpy) :
L_ParticleEffect(cpy)
{
	shooting_vector = cpy.shooting_vector;
	angle_interval = cpy.angle_interval;
	width_interval = cpy.width_interval;
	speed_dis = cpy.speed_dis;
	particle_per_period = cpy.particle_per_period;

	shooting_magnitude_cache = cpy.shooting_magnitude_cache;
	shooting_angle_cache = cpy.shooting_angle_cache;
}


void L_ShootingEffect::howto_emit_particle(void)
{
	static L_REAL radian_t;
	static L_REAL current_speed_dis;

	static L_REAL width_t;
	static L_REAL x_shift;
	static L_REAL y_shift;

	static int j;

	shooting_angle_cache = shooting_vector.get_angle();

	for(j=0;j<particle_per_period;j++)
	{
		// === shooting interval(angle) ===
		if(angle_interval == 0)
			radian_t =0;

		else
			radian_t = ( L_RAND_REAL_1() - 0.5f ) * angle_interval;
		// === end ===


		// === speed distortion ===
		if(speed_dis == 0)
			current_speed_dis = 0;

		else
			current_speed_dis = ( L_RAND_REAL_1()*2.0f - 1.0f ) * speed_dis;
		// === end ===


		// === shooting interval(width) ===
		if(width_interval == 0)
		{
			x_shift = 0;
			y_shift = 0;
		}

		else
		{
			width_t = ( L_RAND_REAL_1() - 0.5f ) * width_interval;

			x_shift = width_t * L_SIN(shooting_angle_cache);
			y_shift = width_t * L_COS(shooting_angle_cache);
		}
		// === end ===

		L_Vector v_t = shooting_vector;
		v_t.set2(shooting_magnitude_cache+current_speed_dis, shooting_angle_cache+radian_t);

		create_particle(x_pos+x_shift, y_pos+y_shift, &v_t);
	}
}


void L_ShootingEffect::set_shooting_vector(const L_Vector& vector)
{
	shooting_vector = vector;

	shooting_magnitude_cache = shooting_vector.get_magnitude();
	shooting_angle_cache = shooting_vector.get_angle();
}


void L_ShootingEffect::set_shooting_vector(L_REAL magnitude, L_REAL radian)
{
	shooting_magnitude_cache = magnitude;
	shooting_angle_cache = radian;

	shooting_vector.set2(shooting_magnitude_cache, shooting_angle_cache);
}


void L_ShootingEffect::set_particle_per_period(int num)
{
	particle_per_period = num;
}


void L_ShootingEffect::set_angle_interval(L_REAL radian)
{
	if(radian < 0)
		angle_interval = -radian;

	else if(radian > L_2PI)
		angle_interval = L_2PI;

	else
		angle_interval = radian;
}


void L_ShootingEffect::set_width_interval(int width)
{
	width_interval = width;
}


void L_ShootingEffect::set_speed_distortion(L_REAL speed_dis_t)
{
	if(speed_dis_t < 0)
		speed_dis = -speed_dis_t;

	else
		speed_dis = speed_dis_t;
}


L_Vector L_ShootingEffect::get_shooting_vector(void)
{
	return shooting_vector;
}


L_ParticleEffect* L_ShootingEffect::new_clone(void)
{
	return new L_ShootingEffect(*this);
}
