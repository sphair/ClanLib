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


#ifndef L_DROPPINGEFFECT_H
#define L_DROPPINGEFFECT_H


#include "L_ParticleEffect.h"


//! The simplest among particle effects. It drops a particle on its position each period.
class L_DroppingEffect : public L_ParticleEffect
{

protected:
	virtual void howto_emit_particle(void);


public:
	/** period_t : time(milisec) for next dropping */
	L_DroppingEffect(int x, int y, int period_t);
	L_DroppingEffect(const L_DroppingEffect& cpy);

	virtual L_ParticleEffect* new_clone(void);
};


#endif
