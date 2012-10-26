/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "API/Core/System/keep_alive.h"
#include "API/Core/System/system.h"
#include "API/Core/System/event.h"
#include <algorithm>

namespace clan
{

class KeepAliveObject_Impl
{
public:
    void *thread_id;
    Event wakeup_event;
};

void cl_alloc_tls_keep_alive_slot();
void cl_set_keep_alive_vector(std::vector<KeepAliveObject *> *v);
std::vector<KeepAliveObject *> *cl_get_keep_alive_vector();
Callback_2<int /*retval*/, const std::vector<Event> &/*events*/, int /*timeout */ > cl_keepalive_func_event_wait;
Callback_0<void *> cl_keepalive_func_thread_id;
Callback_v1<void *> cl_keepalive_func_awake_thread;

void KeepAlive::process(int timeout)
{
	// Get the objects to wait for
	std::vector<KeepAliveObject *> objects = get_objects();
	std::vector<Event> events;
	for (std::vector<KeepAliveObject *>::size_type i = 0; i < objects.size(); i++)
	{
		events.push_back(objects[i]->impl->wakeup_event);
	}
		
	int time_start = System::get_time();
	while (true)
	{
		int time_elapsed = System::get_time() - time_start;
		int time_to_wait = timeout - time_elapsed;
		if (time_to_wait < 0)
			time_to_wait = 0;

		if (timeout < 0)	// Wait forever option
		{
			time_to_wait = -1;
		}

		// Wait for the events
		int wakeup_reason;
		if (!cl_keepalive_func_event_wait.is_null())
		{
			wakeup_reason = cl_keepalive_func_event_wait.invoke(events, time_to_wait);
		}
		else
		{
			wakeup_reason = Event::wait(events, time_to_wait);
		}

		// Check for Timeout
		if (wakeup_reason < 0)
		{
			break;
		}

		timeout = 0;	// Event found, reset the timeout

		// Process the event
		if ( ((unsigned int) wakeup_reason) < events.size())	// (Note, wakeup_reason is >=0)
		{
            objects[wakeup_reason]->impl->wakeup_event.reset();
			objects[wakeup_reason]->process();
		}
	}
}

Callback_2<int /*retval*/, const std::vector<Event> &/*events*/, int /*timeout */ > &KeepAlive::func_event_wait()
{
	return cl_keepalive_func_event_wait;
}

Callback_0<void *> &KeepAlive::func_thread_id()
{
    return cl_keepalive_func_thread_id;
}

Callback_v1<void *> &KeepAlive::func_awake_thread()
{
    return cl_keepalive_func_awake_thread;
}

std::vector<KeepAliveObject *> KeepAlive::get_objects()
{
	std::vector<KeepAliveObject*> *tls_objects = cl_get_keep_alive_vector();
	if (tls_objects)
		return *tls_objects;
	else
		return std::vector<KeepAliveObject *>();
}

KeepAliveObject::KeepAliveObject()
: impl(new KeepAliveObject_Impl())
{
    if (!KeepAlive::func_thread_id().is_null())
        impl->thread_id = KeepAlive::func_thread_id().invoke();
    
	std::vector<KeepAliveObject*> *tls_objects = cl_get_keep_alive_vector();
	if (!tls_objects)
	{
		tls_objects = new std::vector<KeepAliveObject*>();
		cl_set_keep_alive_vector(tls_objects);
	}
	tls_objects->push_back(this);
}

KeepAliveObject::~KeepAliveObject()
{
	std::vector<KeepAliveObject*> *tls_objects = cl_get_keep_alive_vector();
	tls_objects->erase(std::find(tls_objects->begin(), tls_objects->end(), this));
	if (tls_objects->empty())
	{
		delete tls_objects;
		cl_set_keep_alive_vector(0);
	}
}

void KeepAliveObject::set_wakeup_event()
{
    impl->wakeup_event.set();
    if (!KeepAlive::func_awake_thread().is_null())
        KeepAlive::func_awake_thread().invoke(impl->thread_id);
}

#ifdef WIN32

static DWORD cl_tls_keep_alive_index = TLS_OUT_OF_INDEXES;
static Mutex cl_tls_keep_alive_mutex;

void cl_alloc_tls_keep_alive_slot()
{
	if (cl_tls_keep_alive_index == TLS_OUT_OF_INDEXES)
	{
		MutexSection mutex_lock(&cl_tls_keep_alive_mutex);
		cl_tls_keep_alive_index = TlsAlloc();
		if (cl_tls_keep_alive_index == TLS_OUT_OF_INDEXES)
			throw Exception("No TLS slots available!");
		TlsSetValue(cl_tls_keep_alive_index, 0);
	}
}

void cl_set_keep_alive_vector(std::vector<KeepAliveObject *> *v)
{
	cl_alloc_tls_keep_alive_slot();
	TlsSetValue(cl_tls_keep_alive_index, v);
}

std::vector<KeepAliveObject *> *cl_get_keep_alive_vector()
{
	cl_alloc_tls_keep_alive_slot();
	return reinterpret_cast<std::vector<KeepAliveObject*> *>(TlsGetValue(cl_tls_keep_alive_index));
}

#elif defined(__APPLE__)

static bool cl_tls_keep_alive_index_created = false;
static pthread_key_t cl_tls_keep_alive_index;
static Mutex cl_tls_keep_alive_mutex;

void cl_alloc_tls_keep_alive_slot()
{
	if (!cl_tls_keep_alive_index_created)
	{
		MutexSection mutex_lock(&cl_tls_keep_alive_mutex);
		pthread_key_create(&cl_tls_keep_alive_index, 0);
		cl_tls_keep_alive_index_created = true;
	}
}

void cl_set_keep_alive_vector(std::vector<KeepAliveObject *> *v)
{
	cl_alloc_tls_keep_alive_slot();
	pthread_setspecific(cl_tls_keep_alive_index, v);
}

std::vector<KeepAliveObject *> *cl_get_keep_alive_vector()
{
	cl_alloc_tls_keep_alive_slot();
	return reinterpret_cast<std::vector<KeepAliveObject*> *>(pthread_getspecific(cl_tls_keep_alive_index));
}

#else

__thread std::vector<KeepAliveObject*> *cl_tls_keep_alive = 0;

void cl_alloc_tls_keep_alive_slot()
{
}

void cl_set_keep_alive_vector(std::vector<KeepAliveObject *> *v)
{
	cl_tls_keep_alive = v;
}

std::vector<KeepAliveObject *> *cl_get_keep_alive_vector()
{
	return cl_tls_keep_alive;
}

#endif

}
