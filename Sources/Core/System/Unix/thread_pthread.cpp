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

#include <cstring>
#include <pthread.h>
#include "API/Core/System/cl_assert.h"
#include "API/Core/System/log.h"
#include "thread_pthread.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Thread_Helper unix implementation:

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
// CL_Thread unix/pthread implementation:

CL_Thread::CL_Thread(int (*func)(void*), void *value)
: impl(new CL_Thread_Generic)
{
	impl->ref_count = 1;
	impl->runnable = new CL_Thread_Helper(func, value);
	impl->delete_runnable = true;
	impl->running = false;
}

CL_Thread::CL_Thread(CL_Runnable *runnable, bool delete_runnable)
: impl(new CL_Thread_Generic)
{
	impl->ref_count = 1;
	impl->runnable = runnable;
	impl->delete_runnable = delete_runnable;
	impl->running = false;
}

CL_Thread::CL_Thread(const CL_Thread &copy)
: impl(copy.impl)
{
	if (impl) impl->ref_count++;
}

CL_Thread::CL_Thread()
: impl(0)
{
}

CL_Thread::~CL_Thread()
{
	if (impl)
	{
		impl->ref_count--;
		if (impl->ref_count == 0)
		{
			terminate();
			if (impl->delete_runnable) delete impl->runnable;
			delete impl;
		}
	}
}

CL_Thread &CL_Thread::operator =(const CL_Thread &copy)
{
	if (impl)
	{
		impl->ref_count--;
		if (impl->ref_count == 0)
		{
			terminate();
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
	cl_assert((impl != NULL));

	if (impl->running) return;

	cl_assert(
		pthread_create(
			&impl->thread,
			NULL,
			impl->run_init,
			impl)==0);

	impl->running = true;
}

void *CL_Thread_Generic::run_init(void *_self)
{
	CL_Thread_Generic *self = (CL_Thread_Generic *) _self;

	// kill thread immidiately - no cancelation point...
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	self->runnable->run();
	return NULL;
}

void CL_Thread::terminate()
{
	cl_assert(impl != NULL);

	if (impl->running) pthread_cancel(impl->thread);
	impl->running = false;
}

void CL_Thread::wait()
{
	cl_assert(impl != NULL);

	if (impl->running) pthread_join(impl->thread, NULL);
	impl->running = false;
}

void CL_Thread::set_priority(EThreadPriority priority)
{
	cl_assert(impl != NULL);

#ifdef __APPLE__
	struct sched_param param;
	memset(&param, 0, sizeof(struct sched_param));

	// on macosx the valid values for thread priority are 0-127
	// may be different on other posix systems - unknown.
	switch (priority)
	{
	case cl_priority_above_normal:	param.sched_priority = 50;	break;
	case cl_priority_below_normal:	param.sched_priority = 15;	break;
	case cl_priority_highest:		param.sched_priority = 100;	break;
	case cl_priority_idle:			param.sched_priority = 5;	break;
	case cl_priority_lowest:		param.sched_priority = 0;	break;
	case cl_priority_time_critical:	param.sched_priority = 127;	break;
	case cl_priority_normal:
	default:						param.sched_priority = 31;	break;
	}
	
	int i = pthread_setschedparam(impl->thread, SCHED_RR, &param);
	if (i != 0)
		throw CL_Error(CL_String::format("Failed to set thread priority (error %1)",i));
#endif
	// Linux doesn't let your set proirities real time due to its renice model
}

CL_ThreadId CL_Thread::get_current_id()
{
	return CL_ThreadId();
}

CL_ThreadId::CL_ThreadId()
:impl(new CL_ThreadId_Generic)
{
	impl->thread_id = pthread_self();
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
	// I think this is safe to do, but I'm not certain at this point
	impl->thread_id = copy.impl->thread_id;
	return *this;
}

bool CL_ThreadId::operator ==(const CL_ThreadId &cmp) const
{
	return pthread_equal(impl->thread_id, cmp.impl->thread_id);
}

bool CL_ThreadId::operator !=(const CL_ThreadId &cmp) const
{
	return !operator ==(cmp);
}

bool CL_ThreadId::operator <(const CL_ThreadId &cmp) const
{
	// first test with operator== because different pthread_t values from the same
	// thread may not be binary equal to each other - this routine is probably not
	// the best but it should get the job done for now
	if (operator ==(cmp)) return false;
	return memcmp(&impl->thread_id, &cmp.impl->thread_id, sizeof(pthread_t)) < 0;
}
