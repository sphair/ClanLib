/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "Display/precomp.h"
#include "API/Display/System/timer.h"
#include "API/Core/System/system.h"
#include "API/Display/System/run_loop.h"
#include <map>
#include <thread>
#include <algorithm>

namespace clan
{
	class ActiveTimer
	{
	public:
		ActiveTimer(std::weak_ptr<TimerImpl> impl) : timer_impl(std::move(impl)) { }

		std::weak_ptr<TimerImpl> timer_impl;
		bool is_repeating = false;
		int timeout = 0;
		std::chrono::steady_clock::time_point next_awake_time;
		std::function<void()> func_expired;
	};

	class TimerImpl
	{
	public:
		~TimerImpl();

		bool is_repeating = false;
		int timeout = 0;
		std::shared_ptr<ActiveTimer> active;
		std::function<void()> func_expired;
	};

	class TimerThread
	{
	public:
		void start(std::shared_ptr<TimerImpl> timer)
		{
			std::unique_lock<std::mutex> lock(mutex);

			if (!timer->active)
			{
				auto active = std::make_shared<ActiveTimer>(timer);
				timers.push_back(active);
				timer->active = active;
			}

			// Copy timer fields to keep TimerImpl fields updateable outside the mutex lock
			timer->active->timeout = timer->timeout;
			timer->active->is_repeating = timer->is_repeating;
			timer->active->func_expired = timer->func_expired;
			timer->active->next_awake_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(timer->timeout);
			stop_flag = false;

			lock.unlock();
			timers_changed_event.notify_one();

			if (!thread_created)
			{
				thread_created = true;
				thread = std::thread([=]() { worker_main(); });
			}
		}

		void stop(TimerImpl *timer)
		{
			std::unique_lock<std::mutex> lock(mutex);

			if (timer->active)
			{
				auto it = std::find(timers.begin(), timers.end(), timer->active);
				if (it != timers.end())
					timers.erase(it);
				timer->active.reset();
			}

			bool no_timers = timers.empty();
			if (no_timers)
				stop_flag = true;

			lock.unlock();
			timers_changed_event.notify_one();

			if (no_timers && thread_created)
			{
				thread.join();
				thread_created = false;
			}
		}

	private:
		void worker_main()
		{
			std::unique_lock<std::mutex> lock(mutex);
			while (!stop_flag)
			{
				fire_timers();

				if (timers.empty())
					timers_changed_event.wait(lock);
				else
					timers_changed_event.wait_until(lock, next_awake_time());
			}
		}

		void fire_timers()
		{
			auto cur_time = std::chrono::steady_clock::now();
			for (auto it = timers.begin(); it != timers.end();)
			{
				auto &timer = *it;
				bool remove = false;

				if (timer->next_awake_time < cur_time)
				{
					if (timer->func_expired)
					{
						// Copy timer fields to detach them from the mutex lock
						auto timer_impl = timer->timer_impl;
						auto func_expired = timer->func_expired;

						RunLoop::main_thread_async([=]()
						{
							// Only fire the timer if it is still valid when we reached the main thread
							if (timer_impl.lock())
								func_expired();
						});
					}

					if (timer->is_repeating)
					{
						while (timer->next_awake_time < cur_time)
							timer->next_awake_time += std::chrono::milliseconds(timer->timeout);
					}
					else
					{
						remove = true;
					}
				}

				if (remove)
				{
					// Since the timer is now stopping, we must notify the implementation that the timer is no longer active, else we will not be able to restart it
					auto timer_impl = timer->timer_impl.lock();
					if (timer_impl)
						timer_impl->active.reset();
					it = timers.erase(it);
				}
				else
					++it;
			}
		}

		std::chrono::steady_clock::time_point next_awake_time()
		{
			std::chrono::steady_clock::time_point t;
			bool first = true;
			for (auto &timer : timers)
			{
				if (first)
				{
					t = timer->next_awake_time;
					first = false;
				}
				else
				{
					if (t > timer->next_awake_time)
						t = timer->next_awake_time;
				}
			}
			return t;
		}

		bool thread_created = false;
		std::thread thread;
		std::mutex mutex;
		std::condition_variable timers_changed_event;
		bool stop_flag = false;
		std::vector<std::shared_ptr<ActiveTimer>> timers;
	};

	TimerThread timer_thread;

	TimerImpl::~TimerImpl()
	{
		timer_thread.stop(this);
	}

	Timer::Timer() : impl(std::make_shared<TimerImpl>())
	{
	}

	bool Timer::is_repeating() const
	{
		return impl->is_repeating;
	}

	unsigned int Timer::get_timeout() const
	{
		return impl->timeout;
	}

	std::function<void()> &Timer::func_expired()
	{
		return impl->func_expired;
	}

	void Timer::start(unsigned int timeout, bool repeat)
	{
		impl->timeout = timeout;
		impl->is_repeating = repeat;
		timer_thread.start(impl);
	}

	void Timer::stop()
	{
		timer_thread.stop(impl.get());
	}
}
