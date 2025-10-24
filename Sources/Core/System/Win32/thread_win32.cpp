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

#include "Core/precomp.h"

#include "API/Core/System/thread.h"
#include "API/Core/System/error.h"
#include "API/Core/System/crash_reporter.h"
#include "thread_win32.h"
#include "API/Core/System/cl_assert.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Thread_Helper win32 implementation:

class CL_Thread_Helper : public CL_Runnable
{
public:
	CL_Thread_Helper(int (*func)(void*), void *value)
		: func(func), value(value)
	{
	}

	virtual void run()
	{
		func(value);
	}

private:
	int (*func)(void*);

	void *value;
};

/////////////////////////////////////////////////////////////////////////////
// CL_Thread win32 implementation:

CL_Thread::CL_Thread()
: impl(0)
{
}

CL_Thread::CL_Thread(int (*func)(void*), void *value)
: impl(new CL_Thread_Generic)
{
	impl->runnable = new CL_Thread_Helper(func, value);
	impl->delete_runnable = true;
	impl->thread_handle = NULL;
	impl->ref_count = 1;
}

CL_Thread::CL_Thread(CL_Runnable *runnable, bool delete_runnable)
: impl(new CL_Thread_Generic)
{
	impl->runnable = runnable;
	impl->delete_runnable = delete_runnable;
	impl->thread_handle = NULL;
	impl->ref_count = 1;
}

CL_Thread::CL_Thread(const CL_Thread &copy)
: impl(copy.impl)
{
	if (impl) impl->ref_count++;
}

CL_Thread::~CL_Thread()
{
	if (impl)
	{
		impl->ref_count--;
		if (impl->ref_count == 0)
		{
			if (impl->thread_handle != NULL) terminate();
			if (impl->delete_runnable) delete impl->runnable;
			delete impl;
		}
	}
}

unsigned long __stdcall CL_Thread_Generic::func_proxy(void *arg)
{
#ifndef _DEBUG
	// Create minidumps when not in debug mode.
	CL_CrashReporter crash_reporter;
#endif

	CL_Thread_Generic *self = (CL_Thread_Generic *) arg;
	self->runnable->run();

	return 0;
}

CL_Thread &CL_Thread::operator =(const CL_Thread &copy)
{
	if (impl)
	{
		impl->ref_count--;
		if (impl->ref_count == 0)
		{
			if (impl->thread_handle != NULL) terminate();
			if (impl->delete_runnable) delete impl->runnable;
			delete impl;
		}
	}

	impl = copy.impl;
	if (impl) impl->ref_count++;
	return *this;
}

void CL_Thread::start()
{
	cl_assert(impl != NULL);

	impl->thread_handle = CreateThread(
		NULL,
		0,
		impl->func_proxy,
		impl,
		0,
		&impl->thread_id);

	if (impl->thread_handle == NULL)
		throw CL_Error("Failed to create thread");
}

void CL_Thread::terminate()
{
	cl_assert(impl != NULL);

	TerminateThread(impl->thread_handle, 0);
	CloseHandle(impl->thread_handle);
	impl->thread_handle = NULL;
}

void CL_Thread::wait()
{
	cl_assert(impl != NULL);

	if (impl->thread_handle == NULL) return;

	WaitForSingleObject(impl->thread_handle, INFINITE);
	CloseHandle(impl->thread_handle);
	impl->thread_handle = NULL;
}

void CL_Thread::set_priority(EThreadPriority priority)
{
	cl_assert(impl != NULL);

	if (impl->thread_handle == NULL) return;

	int prio = THREAD_PRIORITY_NORMAL;
	switch (priority)
	{
	case cl_priority_above_normal:  prio = THREAD_PRIORITY_ABOVE_NORMAL;  break;
	case cl_priority_below_normal:  prio = THREAD_PRIORITY_BELOW_NORMAL;  break;
	case cl_priority_highest:       prio = THREAD_PRIORITY_HIGHEST;       break;
	case cl_priority_idle:          prio = THREAD_PRIORITY_IDLE;          break;
	case cl_priority_lowest:        prio = THREAD_PRIORITY_LOWEST;        break;
	case cl_priority_normal:        prio = THREAD_PRIORITY_NORMAL;        break;
	case cl_priority_time_critical: prio = THREAD_PRIORITY_TIME_CRITICAL; break;
	}

	BOOL result = SetThreadPriority(impl->thread_handle, prio);
	if (result == FALSE) throw CL_Error("Failed to set thread priority");
}

CL_ThreadId CL_Thread::get_current_id()
{
	return CL_ThreadId();
}

unsigned int CL_Thread::get_thread_id() const
{
	return impl->thread_id;
}


CL_ThreadId::CL_ThreadId()
:impl(new CL_ThreadId_Generic)
{
	impl->thread_id = GetCurrentThreadId();
}

CL_ThreadId::CL_ThreadId(const CL_ThreadId &copy)
:impl(new CL_ThreadId_Generic)
{
	*this = copy;
}

CL_ThreadId::~CL_ThreadId()
{
	delete impl;
}

CL_ThreadId &CL_ThreadId::operator=(const CL_ThreadId &copy)
{
	impl->thread_id = copy.impl->thread_id;
	return *this;
}

bool CL_ThreadId::operator ==(const CL_ThreadId &cmp) const
{
	return impl->thread_id == cmp.impl->thread_id;
}

bool CL_ThreadId::operator !=(const CL_ThreadId &cmp) const
{
	return !operator ==(cmp);
}

bool CL_ThreadId::operator <(const CL_ThreadId &cmp) const
{
	return impl->thread_id < cmp.impl->thread_id;
}
