/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#ifndef __USE_UNIX98
	#define __USE_UNIX98
#endif  

#include "Core/precomp.h"
#include "API/Core/System/cl_assert.h"
#include <pthread.h>
#include "mutex_pthread.h"

// We need to do this because the posix threads library under linux obviously
// suck:
extern "C"
{
#if defined(__APPLE__) || defined (__FreeBSD__) || defined(__OpenBSD__)
	int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int kind);
#else
	int pthread_mutexattr_setkind_np(pthread_mutexattr_t *attr, int kind);
#endif
}

CL_Mutex *CL_Mutex::create()
{
	return new CL_Mutex;
}
////////////////////////////////////////////////////////////////////////////////
// CL_Mutex Posix implementation

CL_Mutex::CL_Mutex()
: impl(new CL_Mutex_Generic)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);

#if defined(__APPLE__) || defined (__FreeBSD__) || defined(__OpenBSD__)
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
#else
#if PTHREAD_MUTEX_RECURSIVE_NP
// cygwin
	pthread_mutexattr_setkind_np(&attr, PTHREAD_MUTEX_RECURSIVE);
#else
	pthread_mutexattr_setkind_np(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
#endif
#endif
	pthread_mutex_init(&impl->mutex, &attr);
	pthread_mutexattr_destroy(&attr);

	pthread_cond_init(&impl->cond,0);
}

CL_Mutex::CL_Mutex(const CL_Mutex &copy)
{
	cl_assert(false);
}

void CL_Mutex::operator=(const CL_Mutex &copy)
{
	cl_assert(false);
}

CL_Mutex::~CL_Mutex()
{
	pthread_mutex_destroy(&impl->mutex);
	pthread_cond_destroy(&impl->cond);
	delete impl;
}
	
void CL_Mutex::enter()
{
	pthread_mutex_lock(&impl->mutex);
}

void CL_Mutex::leave()
{
	pthread_mutex_unlock(&impl->mutex);
}


void CL_Mutex::wait()
{
	pthread_cond_wait(&impl->cond,&impl->mutex);
}


void CL_Mutex::notify()
{
	pthread_cond_signal(&impl->cond);
}

void CL_Mutex::notify_all()
{
	pthread_cond_broadcast(&impl->cond);
}
