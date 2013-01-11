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


#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H


#include "L_DroppingEffect.h"
#include "L_ExplosionEffect.h"
#include "L_ShootingEffect.h"


//! Effect manager is used for easier controlling or managing multiple effects at the same time.
class L_EffectManager
{

protected:
	int effect_num;


public:
	std::list<L_ParticleEffect*> effect_list;

	L_EffectManager();

	~L_EffectManager();

	void add(L_ParticleEffect* effect_t);

	//get number of effect
	int get_effect_num(void);

	//get number of particle in this effect manager
	int get_particle_num(void);

	void draw(clan::Canvas &canvas, int x_offset=0, int y_offset=0);

	void run(int time_elapesed_t, bool trigger_all=true);

};

#endif
