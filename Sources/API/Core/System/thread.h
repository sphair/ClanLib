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

#ifndef header_thread
#define header_thread

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

class CL_Thread_Generic;
class CL_ThreadId_Generic;

//: Thread callback interface.
//- !group=Core/System!
//- !header=core.h!
//- <p>When a thread is created, it will call run() in its attached CL_Runnable interface.</p>
class CL_Runnable
{
//! Construction:
public:
	virtual ~CL_Runnable() { return; }

public:
//! Overrideables:
	//: Called when a thread is run.
	virtual void run()=0;
};

//: Thread Priority Enum
//- !group=Core/System!
//- !header=core.h!
enum EThreadPriority
{
	cl_priority_above_normal,
	cl_priority_below_normal,
	cl_priority_highest,
	cl_priority_idle,
	cl_priority_lowest,
	cl_priority_normal,
	cl_priority_time_critical
};

//: ThreadId Class
//- !group=Core/System!
//- !header=core.h!
class CL_API_CORE CL_ThreadId
{
//! Construction:
public:
	//: Create a ThreadId.
	CL_ThreadId();

	CL_ThreadId(const CL_ThreadId &copy);

	//: Destructor.
	~CL_ThreadId();

//! Operations:
public:
	CL_ThreadId &operator =(const CL_ThreadId &copy);

	bool operator ==(const CL_ThreadId &cmp) const;

	bool operator !=(const CL_ThreadId &cmp) const;

	bool operator <(const CL_ThreadId &cmp) const;
	
//! Implementation:
private:
	CL_ThreadId_Generic *impl;
};

//: Thread Class
//- !group=Core/System!
//- !header=core.h!
class CL_API_CORE CL_Thread
{
//! Construction:
public:
	//: Create a thread.
	//param runnable: CL_Runnable object to be used as the thread run function.
	//param delete_runnable: If true, deletes the CL_Runnable object, when CL_Thread is destroyed.
	//param func: Callback function used as the thread run function. Example: int my_callback(void *value).
	//param value: Value parameter passed to callback function.
	CL_Thread(CL_Runnable *runnable, bool delete_runnable = false);

	CL_Thread(int (*func)(void*), void *value);

	CL_Thread(const CL_Thread &copy);

	CL_Thread();

	inline bool is_initialized() {
		return impl != 0;
	}
	
	//: Destructor.
	~CL_Thread();
	
//! Attributes:
public:
	//: Returns the thread ID of the calling thread.
	static CL_ThreadId get_current_id();
	
	//: Returns the thread ID of this thread.
	unsigned int get_thread_id() const;

//! Operations:
public:
	//: Copy assignement operator.
	CL_Thread &operator =(const CL_Thread &copy);

	//: Starts the thread.
	void start();

	//: Terminate the thread. (use with caution under win98)
	void terminate();

	//: Wait until the thread finishes its execution.
	void wait();

	//: Set the thread priority.
	void set_priority(EThreadPriority priority);

//! Implementation:
private:
	CL_Thread_Generic *impl;
};

// Add support for doing cross platform Thread Local Storage variables:
#ifdef MSVC
	#define CL_TLS __declspec(thread)
#else
	#define CL_TLS __thread
#endif

#endif
