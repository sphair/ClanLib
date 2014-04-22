/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Mark Page
*/

#include "Core/precomp.h"
#include "API/Core/System/timer.h"
#include "API/Core/System/keep_alive.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/mutex.h"
#include "API/Core/System/event.h"
#include "API/Core/System/system.h"
#include "API/Core/Signals/callback.h"
#include <map>

namespace clan
{

class Timer_Object
{
public:
	Timer_Object() : stopped(true) {}

	bool stopped;
	ubyte64 end_time;
	unsigned int timeout;
	bool repeating;
	Callback_v0 func_expired;
};

/////////////////////////////////////////////////////////////////////////////
// Timer_Thread Class:

class Timer_Thread : public KeepAliveObject
{
public:
	Timer_Thread() : timeout(-1), stop_thread(false)
	{
		thread.start(this, &Timer_Thread::timer_main);
	}

	~Timer_Thread()
	{
		MutexSection mutex_lock(&mutex);
		stop_thread = true;
		mutex_lock.unlock();
		update_event.set();
		thread.join();

		// Delete all timer objects in the map
		for (std::map<int, Timer_Object* >::iterator it = timer_objects.begin(); it != timer_objects.end(); ++it)
		{
			delete it->second;
		}
	}

	void start(int timer_id, unsigned int new_timeout, bool repeat)
	{
		MutexSection mutex_lock(&mutex);

		Timer_Object &object = get_timer_object(timer_id);
		object.stopped = false;
		object.end_time = System::get_time() + new_timeout;
		object.timeout = new_timeout;
		object.repeating = repeat;

		if ( (timeout == -1) || (new_timeout < ( (unsigned int) timeout) ) )
		{
			// Only break into the thread when a shorter timeout is required
			update_event.set();
		}
	}

	void stop(int timer_id)
	{
		MutexSection mutex_lock(&mutex);
		Timer_Object &object = get_timer_object(timer_id);
		object.stopped = true;
	}

	void remove_timer(int timer_id)
	{
		// Remove the unused timers, to prevent memory leaks
		MutexSection mutex_lock(&mutex);
		std::map<int, Timer_Object *>::iterator it = timer_objects.find(timer_id);
		if (it != timer_objects.end())
		{
			delete it->second;
			timer_objects.erase(it);
		}
	}

	void process()
	{
		MutexSection mutex_lock(&mutex);

		ubyte64 current_time = System::get_time();

		// Scan for events and trigger them
		for (std::map<int, Timer_Object *>::iterator it = timer_objects.begin(); it != timer_objects.end();)
		{
			Timer_Object &object = *(it->second);
			++it;	// We need to update the iterator here - Because func_expired may remove the timer object

			// Found a timer
			const int grace_period = 1;	// Allow 1ms grace
			if (object.end_time <= (current_time + grace_period))
			{
				if (!object.stopped)
				{
					if (object.repeating)
					{
						object.end_time += object.timeout;
						if (object.end_time <= current_time)
						{
							// An event has been missed, reset the timer
							object.end_time = current_time + object.timeout;
						}
					}
					else
					{
						object.stopped = true;
					}

					if (!object.func_expired.is_null())
						object.func_expired.invoke();
				}
			}
		}
	}

	Callback_v0 &get_func_expired(int timer_id)
	{
		MutexSection mutex_lock(&mutex);
		return get_timer_object(timer_id).func_expired;
	}

private:
	Timer_Object &get_timer_object(int timer_id)
	{
		// Find existing timer
		std::map<int, Timer_Object* >::iterator it = timer_objects.find(timer_id);
		if (it != timer_objects.end())
		{
			return *(it->second);
		}

		Timer_Object *object = new Timer_Object;
		timer_objects[timer_id] = object;
		return *object;
	}

	void timer_main()
	{
		while (true)
		{
			MutexSection mutex_lock(&mutex);
			update_event.reset();
			if (stop_thread)
				break;

			ubyte64 current_time = System::get_time();
			timeout = -1;
			bool found_timer = false;

			// Scan for timers to find the next one to call
			for (std::map<int, Timer_Object *>::iterator it = timer_objects.begin(); it != timer_objects.end(); ++it)
			{
				Timer_Object &object = *(it->second);
				if (!object.stopped)
				{
					if (!found_timer)
					{
						// First timer in the list
						timeout = object.end_time - current_time;
						found_timer = true;
					}
					else
					{
						// Get the smallest timeout
						int next_timeout = object.end_time - current_time;
						if (next_timeout < timeout)
						{
							timeout = next_timeout;
						}
					}
					// Force update, if a timer was missed
					if (timeout < 1)
						timeout = 1;
				}
			}

			mutex_lock.unlock();

			if (Event::wait(update_event, timeout) == -1)
				set_wakeup_event();
		}
	}

	Thread thread;
	Event update_event;
	Mutex mutex;
	int timeout;
	bool stop_thread;

	std::map<int, Timer_Object *> timer_objects;
};

/////////////////////////////////////////////////////////////////////////////
// Timer_Impl Class:

class Timer_Impl
{
public:
	Timer_Impl() : timeout(0), repeating(false), id(-1)
	{
		// Create a static timer thread if none exist
		MutexSection mutex_lock(&timer_thread_mutex);
		if (!timer_thread_instance_count)
		{
			timer_thread = new(Timer_Thread);
		}
		timer_thread_instance_count++;
		timer_thread_max_id++;
		id = timer_thread_max_id;
	}

	~Timer_Impl()
	{
		// Destroy the static timer thread if this is the last timer
		MutexSection mutex_lock(&timer_thread_mutex);
		timer_thread->remove_timer(id);
		timer_thread_instance_count--;
		if (!timer_thread_instance_count)
		{
			delete timer_thread;
			timer_thread = NULL;
		}
	}

	void start(unsigned int new_timeout, bool repeat)
	{
		MutexSection mutex_lock(&timer_thread_mutex);
		timer_thread->start(id, new_timeout, repeat);
	}

	void stop()
	{
		MutexSection mutex_lock(&timer_thread_mutex);
		timer_thread->stop(id);
	}

	bool is_repeating() const { return repeating; }
	unsigned int get_timeout() const { return timeout; }

	Callback_v0 &func_expired()
	{
		MutexSection mutex_lock(&timer_thread_mutex);
		return timer_thread->get_func_expired(id);
	}

private:
	static Timer_Thread *timer_thread;
	static int timer_thread_instance_count;
	static Mutex timer_thread_mutex;
	static int timer_thread_max_id;	// Unique timer id

	unsigned int timeout;
	bool repeating;
	int id;
};

Timer_Thread *Timer_Impl::timer_thread = NULL;
int Timer_Impl::timer_thread_instance_count = 0;
Mutex Timer_Impl::timer_thread_mutex;
int Timer_Impl::timer_thread_max_id = 0;

/////////////////////////////////////////////////////////////////////////////
// Timer Construction:

Timer::Timer()
: impl(new Timer_Impl)
{
}

Timer::~Timer()
{
}

bool Timer::is_repeating() const
{
	return impl->is_repeating();
}

unsigned int Timer::get_timeout() const
{
	return impl->get_timeout();
}

/////////////////////////////////////////////////////////////////////////////
// Timer Events:

Callback_v0 &Timer::func_expired()
{
	return impl->func_expired();
}

/////////////////////////////////////////////////////////////////////////////
// Timer Operations:

void Timer::start(unsigned int timeout, bool repeat)
{
	impl->start(timeout, repeat);
}

void Timer::stop()
{
	impl->stop();
}

/////////////////////////////////////////////////////////////////////////////
// Timer Implementation:

}
