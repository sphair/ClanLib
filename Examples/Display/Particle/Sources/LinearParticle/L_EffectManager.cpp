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
#include "L_EffectManager.h"


L_EffectManager::L_EffectManager()
{
	effect_num = 0;
}


L_EffectManager::~L_EffectManager()
{
	std::list<L_ParticleEffect*>::iterator iter = effect_list.begin();

	while( iter != effect_list.end() )
	{
		delete *iter;
		iter = effect_list.erase(iter);
	}
}


void L_EffectManager::add(L_ParticleEffect* effect_t)
{
	effect_list.push_back(effect_t);
}


void L_EffectManager::draw(clan::Canvas &canvas, int x_offset, int y_offset)
{
	std::list<L_ParticleEffect*>::iterator iter = effect_list.begin();

	while( iter != effect_list.end() )
	{
		(*iter)->draw(canvas, x_offset,y_offset);
		iter++;
	}
}


void L_EffectManager::run(int time_elapesed_t, bool trigger_all)
{
	std::list<L_ParticleEffect*>::iterator iter = effect_list.begin();

	while( iter != effect_list.end() )
	{
	    // delete dead effect which does not contain particle
		if( (*iter)->get_life() <= 0  && (*iter)->get_particle_num() <= 0 )
		{
		   	delete *iter;
			iter = effect_list.erase(iter);
		}

		else
		{
			if(trigger_all)
				(*iter)->trigger();

			(*iter)->run(time_elapesed_t);

			iter++;
		}
	}
}


int L_EffectManager::get_particle_num(void)
{
	std::list<L_ParticleEffect*>::iterator iter = effect_list.begin();

	int num = 0;
	while(iter != effect_list.end())
	{
		num += (*iter)->get_particle_num();
		iter++;
	}

	return num;
}


int L_EffectManager::get_effect_num(void)
{
	return effect_list.size();
}
