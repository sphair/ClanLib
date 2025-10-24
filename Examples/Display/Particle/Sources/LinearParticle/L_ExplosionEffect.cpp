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
#include "L_ExplosionEffect.h"


L_ExplosionEffect::L_ExplosionEffect(int x, int y, int period_t, int min_particles_t, int max_particles_t, L_REAL explosion_level_t) :
L_ParticleEffect(period_t,x,y)
{
	min_particles = min_particles_t;
	max_particles = max_particles_t;

	explosion_level = explosion_level_t;
	speed_dis = 0;
}


L_ExplosionEffect::L_ExplosionEffect(const L_ExplosionEffect& cpy) :
L_ParticleEffect(cpy)
{
	min_particles = cpy.min_particles;
	max_particles = cpy.max_particles;
	explosion_level = cpy.explosion_level;
	speed_dis = cpy.speed_dis;
}


void L_ExplosionEffect::set_min_max_particle(int min_par, int max_par)
{
	min_particles = min_par;
	max_particles = max_par;
}


void L_ExplosionEffect::set_speed_distortion(L_REAL speed_dis_t)
{
	if(speed_dis_t < 0)
		speed_dis = -speed_dis;

	else
		speed_dis = speed_dis_t;
}


void L_ExplosionEffect::set_explosion_level(L_REAL level)
{
	explosion_level = level;
}


void L_ExplosionEffect::howto_emit_particle(void)
{
	static L_REAL radian;
	static int num_particles;
	static int j;

	num_particles = min_particles + rand() % (max_particles - min_particles + 1);

	for( j=0; j<num_particles; j++ )
	{
		radian = L_RAND_REAL_2()*L_2PI;

		static L_REAL current_speed_dis;

		if(speed_dis == 0)
			current_speed_dis = 0;

		else
			current_speed_dis = ( L_RAND_REAL_1()*2.0f - 1.0f ) * speed_dis;

		L_Vector v_t;
		v_t.set2( explosion_level+current_speed_dis, radian );

		create_particle(x_pos,y_pos,&v_t);
	}
}


L_ParticleEffect* L_ExplosionEffect::new_clone(void)
{
	return new L_ExplosionEffect(*this);
}
