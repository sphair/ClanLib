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


#ifndef L_PARTICLEMEM_H
#define L_PARTICLEMEM_H


#include "L_Particle.h"


#define L_NEW_PAR(NEW_PAR, SOURCE_PAR) \
	L_ParticleMem::head++; \
	\
	if( L_ParticleMem::head >= L_ParticleMem::mem_size ) \
		L_ParticleMem::head = 0; \
	\
    L_ParticleMem::mem[L_ParticleMem::head]->copy_from(SOURCE_PAR); \
	NEW_PAR = L_ParticleMem::mem[L_ParticleMem::head];


class L_ParticleMem
{

public:
	static L_Particle** mem;
	static int head;
	static int mem_size;

	static void init(int size);
	static void deinit(void);

};


#endif
