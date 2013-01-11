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


#ifndef L_PARTICLESYSTEM_H
#define L_PARTICLESYSTEM_H

#include "L_EffectEmitter.h"


//! LinearParticle clan::System
class L_ParticleSystem
{

public:

	/** Initialize LinearParticle. Call this before using LinearParticle. \n
	particle_pool_size : maximum number of existing particles allowed (the default size
	is 5000), set to higher number to allow more particles to exist simultaneously. */
	static void init(int particle_pool_size=5000);

	/** Deinitialize LinearParticle. Call this when LinearParticle is no longer to be used. */
	static void deinit(void);

};


#endif
