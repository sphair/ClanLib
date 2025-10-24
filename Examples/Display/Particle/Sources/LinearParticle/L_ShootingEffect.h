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


#ifndef L_SHOOTINGEFFECT
#define L_SHOOTINGEFFECT


#include "L_ParticleEffect.h"


//! Particle effect with constrait degree of emission.
class L_ShootingEffect : public L_ParticleEffect
{

protected:
	L_REAL angle_interval;
	int width_interval;
	L_REAL speed_dis;
	int particle_per_period;

	L_Vector shooting_vector;
	L_REAL shooting_magnitude_cache;
	L_REAL shooting_angle_cache;

	virtual void howto_emit_particle(void);

public:

	/** shooting_vector_t : shooting direction and magnitude \n
	period_t : time(milisec) for next shooting \n
	particle_per_period_t : number of particles for a period */
	L_ShootingEffect(int x, int y, const L_Vector& shooting_vector_t, int period_t, int particle_per_period_t=1);

	/** Copy contructor */
	L_ShootingEffect(const L_ShootingEffect& cpy);

	/** Set shooting vector. */
	void set_shooting_vector(const L_Vector& vector);

	/** Set shooting vector by using magnitude and radian. */
	void set_shooting_vector(L_REAL magnitude, L_REAL radian);

	/** Set number of particles to be emitted for a period. */
	void set_particle_per_period(int num);

	/** Set field of shooting, set 0 value(default value) to have beam emission. */
	void set_angle_interval(L_REAL radian);

	/** Set width of the emission line, if width is 0(default value), particles emitted from a point. */
	void set_width_interval(int width);

	/** Set speed distortion to particles. The larger "speed_dis_t", the greater speed difference
	to be applied for each particle. */
	void set_speed_distortion(L_REAL speed_dis_t);

	/** Get current shooting vector. */
	L_Vector get_shooting_vector(void);

	virtual L_ParticleEffect* new_clone(void);

};

#endif
