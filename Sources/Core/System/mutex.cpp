/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
*/

#include "Core/precomp.h"
#include "API/Core/System/mutex.h"
#include "API/Core/System/exception.h"

namespace clan
{

#ifdef _MSC_VER
	#include <intrin.h>
	#define cl_compiler_barrier() _ReadWriteBarrier()
#else
	#define cl_compiler_barrier()  __asm__ __volatile__("" : : : "memory")
#endif

#if !defined(WIN32) && !defined(__APPLE__)
// We need to do this because the posix threads library under linux obviously
// suck:
extern "C"
{
#if defined (__FreeBSD__) || defined(__OpenBSD__)
	int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int kind);
#else
	int pthread_mutexattr_setkind_np(pthread_mutexattr_t *attr, int kind);
#endif
}
#endif

/////////////////////////////////////////////////////////////////////////////
// Mutex Construction:

Mutex::Mutex()
{
#ifdef WIN32
	InitializeCriticalSection(&critical_section);
#else
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	#if defined(__FreeBSD__) || defined(__APPLE__)
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	#else
	#if PTHREAD_MUTEX_RECURSIVE_NP
	pthread_mutexattr_setkind_np(&attr, PTHREAD_MUTEX_RECURSIVE);
	#else
	pthread_mutexattr_setkind_np(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
	#endif
	#endif
	pthread_mutex_init(&handle, &attr);
	pthread_mutexattr_destroy(&attr);
#endif
}

Mutex::~Mutex()
{
#ifdef WIN32
	DeleteCriticalSection(&critical_section);
#else
	pthread_mutex_destroy(&handle);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Mutex Attributes:


/////////////////////////////////////////////////////////////////////////////
// Mutex Operations:

void Mutex::lock()
{
#ifdef WIN32
	EnterCriticalSection(&critical_section);
#else
	pthread_mutex_lock(&handle);
#endif
}

bool Mutex::try_lock()
{
#ifdef WIN32
	BOOL result = TryEnterCriticalSection(&critical_section);
	return (result != FALSE);
#else
	int result = pthread_mutex_trylock(&handle);
	return (result == 0);
#endif
}

void Mutex::unlock()
{
	cl_compiler_barrier();
#ifdef WIN32
	LeaveCriticalSection(&critical_section);
#else
	pthread_mutex_unlock(&handle);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Mutex Implementation:

}
