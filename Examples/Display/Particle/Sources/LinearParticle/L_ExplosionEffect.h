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


#ifndef L_EXPLOSIONEFFECT
#define L_EXPLOSIONEFFECT


#include "L_ParticleEffect.h"


//! Explosion effect, suitable to be used for explosion, smoking effects.
class L_ExplosionEffect : public L_ParticleEffect
{

protected:
	int min_particles; //minimun number of particles each explosion
	int max_particles; //maximun number of particles each explosion
	L_REAL explosion_level; //the higher, the faster the particle's initial speed
	L_REAL speed_dis;

	virtual void howto_emit_particle(void);


public:
	/** period_t : time(milisec) interval between two emissions. \n
	min_particles_t, max_particles : minimun and maximun number of particles to be emitted in a period. \n
	explosion_level_t : initial speed for particles. */
	L_ExplosionEffect(int x, int y, int period_t, int min_particles_t, int max_particles_t, L_REAL explosion_level_t);

	/** Copy contructor */
	L_ExplosionEffect(const L_ExplosionEffect& cpy);

	/** Set minimun and maximun number of particles to be emitted in a period. */
	void set_min_max_particle(int min_par, int max_par);

	/** Set speed distortion to particles. The larger "speed_dis_t", the greater speed difference
	to be applied for each particle. */
	void set_speed_distortion(L_REAL speed_dis_t);

	void set_explosion_level(L_REAL level);

	virtual L_ParticleEffect* new_clone(void);

};


#endif
