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

/// \addtogroup clanCore_System clanCore System
/// \{


#pragma once


#include "../api_core.h"

#ifdef WIN32
#include <windows.h>
#else
#ifndef __USE_UNIX98
#define __USE_UNIX98
#endif
#include <pthread.h>
#endif

/// \brief Mutex class.
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_API_CORE CL_Mutex
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a mutex object.
	CL_Mutex();

	~CL_Mutex();


/// \}
/// \name Attributes
/// \{

public:


/// \}
/// \name Operations
/// \{

public:
	/// \brief Lock mutex.
	void lock();

	/// \brief Attempt to lock mutex.
	bool try_lock();

	/// \brief Unlock mutex.
	void unlock();


/// \}
/// \name Implementation
/// \{

private:
#ifdef WIN32
	CRITICAL_SECTION critical_section;
#else
	pthread_mutex_t handle;
#endif
/// \}
};

/// \brief Mutex locking helper.
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_API_CORE CL_MutexSection
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a mutex section.
	CL_MutexSection(CL_Mutex *mutex, bool lock_mutex = true)
	: mutex(mutex), lock_count(0)
	{
		if (lock_mutex)
			lock();
	}

	~CL_MutexSection()
	{
		if (lock_count > 0 && mutex)
			mutex->unlock();
		lock_count = 0;
	}


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the amounts of recursive mutex locks performed by this section.
	int get_lock_count() const
	{
		return lock_count;
	}


/// \}
/// \name Operations
/// \{

public:
	/// \brief Lock the mutex.
	void lock()
	{
		if (mutex)
			mutex->lock();
		lock_count++;
	}

	/// \brief Attempt to lock mutex.
	bool try_lock()
	{
		if (mutex == 0 || mutex->try_lock())
		{
			lock_count++;
			return true;
		}
		return false;
	}

	/// \brief Unlock mutex.
	void unlock()
	{
		if (lock_count <= 0)
			return;

		if (mutex)
			mutex->unlock();
		lock_count--;
	}


/// \}
/// \name Implementation
/// \{

private:
	CL_Mutex *mutex;

	int lock_count;
/// \}
};


/// \}
