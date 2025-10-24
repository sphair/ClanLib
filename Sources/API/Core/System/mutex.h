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

//! clanCore="System"
//! header=core.h

#ifndef header_mutex
#define header_mutex

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#if _MSC_VER > 1000
#pragma once
#endif

class CL_Mutex_Generic;

//: Mutex interface.
//- !group=Core/System!
//- !header=core.h!
//- <p>If you don't know what a mutex is, read a book. :-)</p>
class CL_API_CORE CL_Mutex
{
public:
//! Construction:
	//: Call this to create a mutex. (provided for 0.4 backward compatibility)
	static CL_Mutex *create();

	//: Construct a new mutex.
	CL_Mutex();

	//: Destroy mutex.
	~CL_Mutex();
	
//! Operations:
	//: Enter the critical section.
	void enter();

	//: Leave the critical section.
	void leave();

	//: Releases the mutex this thread has taken out and sends the
	//: thread to sleep.
	//- <p>Other threads can then acquire the mutex and
	//- modify any data. When another thread calls notify, the thread
	//- that called wait will wake up again and automatically reacquire
	//- the mutex.</p>
	//-
	//- <p>Warning: This function must not be called on an unlocked 
	//- mutex. You must call enter first.</p>
	//-
	//- <p>Warning: When wait returns (after another thread has called
	//- notify/notifyAll, the current thread will have a mutex lock. Be
	//- sure to call leave at some point.</p>
	//-
	//- <p>Warning: Not implemented under Win32. If you need it,
	//- please implement it in Sources/Core/System/Win32/mutex_win32.*,
	//- and send me the patch. :-)</p>
	void wait();

	//: Notify wakes up the first thread that has gone to sleep on this
	//: mutex in order to wait for a notification.
	//- <p>For safety reasons, you are advised to acquire the mutex
	//- using 'enter' before calling notify. (Of course you have to
	//- release the mutex using 'leave' after notifying, otherwise the
	//- waiting thread cannot reacquire it).</p>
	//-
	//- <p>Warning: Not implemented under Win32. If you need it,
	//- please implement it in Sources/Core/System/Win32/mutex_win32.*,
	//- and send me the patch. :-)</p>
	void notify();

	//: NotifyAll wakes up all threads waiting on the mutex.
	//- <p>Warning: Not implemented under Win32. If you need it,
	//- please implement it in Sources/Core/System/Win32/mutex_win32.*,
	//- and send me the patch. :-)</p>
	//also: CL_Mutex::notify
	void notify_all();

private:
	// Disallow copy construction.
	CL_Mutex(const CL_Mutex &copy);

	// Disallow mutex copy assignment.
	void operator =(const CL_Mutex &copy);

	CL_Mutex_Generic *impl;
};

//: Lock a mutex until the end of a scope.
//- <p>This class is a way to ensure a mutex will be released at the end of 
//- a scope. When an instance is constructed, it will lock the mutex, and
//- when the instance is destroyed (at the exit of its scope), it will
//- unlock the mutex.</p>
class CL_MutexSection
{
//! Construction:
public:
	//: Mutex Section constructor.
	CL_MutexSection(CL_Mutex *mutex, bool lock_at_construct = true)
	: mutex(mutex), lock_count(0)
	{
		if (lock_at_construct) enter();
	}

	//: Mutex Section destructor.
	virtual ~CL_MutexSection()
	{
		while (lock_count > 0) leave();
	}

//! Operations:
public:
	//: Lock mutex.
	void enter()
	{
		mutex->enter();
		lock_count++;
	}

	//: Unlock mutex.
	void leave()
	{
		if (lock_count == 0) return;
		lock_count--;
		mutex->leave();
	}

//! Implementation:
private:
	CL_Mutex *mutex;

	int lock_count;
};

#endif
