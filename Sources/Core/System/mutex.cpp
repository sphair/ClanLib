/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#ifndef WIN32
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
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_Mutex Construction:

CL_Mutex::CL_Mutex()
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

CL_Mutex::~CL_Mutex()
{
#ifdef WIN32
	DeleteCriticalSection(&critical_section);
#else
	pthread_mutex_destroy(&handle);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CL_Mutex Attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_Mutex Operations:

void CL_Mutex::lock()
{
#ifdef WIN32
	EnterCriticalSection(&critical_section);
#else
	pthread_mutex_lock(&handle);
#endif
}

bool CL_Mutex::try_lock()
{
#ifdef WIN32
	BOOL result = TryEnterCriticalSection(&critical_section);
	return (result != FALSE);
#else
	throw CL_Exception("Congratulations, you just got the task of implementing CL_Mutex::try_lock() for unix!");
	return false;
#endif
}

void CL_Mutex::unlock()
{
#ifdef WIN32
	LeaveCriticalSection(&critical_section);
#else
	pthread_mutex_unlock(&handle);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CL_Mutex Implementation:
