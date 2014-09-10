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
*/

#include "Core/precomp.h"
#include "API/Core/System/work_queue.h"
#include "API/Core/System/keep_alive.h"
#include "API/Core/System/event.h"
#include "API/Core/System/mutex.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/system.h"
#include "API/Core/System/interlocked_variable.h"
#include <algorithm>
#include "API/Core/Math/cl_math.h"

namespace clan
{

class WorkItemProcess : public WorkItem
{
public:
	WorkItemProcess(const std::function<void()> &func) : func(func) { }

	void process_work() { func(); }

private:
	std::function<void()> func;
};

class WorkItemWorkCompleted : public WorkItem
{
public:
	WorkItemWorkCompleted(const std::function<void()> &func) : func(func) { }

	void process_work() { }
	void work_completed() { func(); }

private:
	std::function<void()> func;
};

class WorkQueue_Impl : public KeepAliveObject
{
public:
	WorkQueue_Impl(bool serial_queue);
	~WorkQueue_Impl();

	void queue(WorkItem *item); // transfers ownership
	void work_completed(WorkItem *item); // transfers ownership

	int get_items_queued() const { return items_queued.get(); }

private:
	void process();
	void worker_main();

	bool serial_queue;
	std::vector<Thread> threads;
	Mutex mutex;
	Event stop_event, work_available_event;
	std::vector<WorkItem *> queued_items;
	std::vector<WorkItem *> finished_items;
	InterlockedVariable items_queued;
};

WorkQueue::WorkQueue(bool serial_queue)
	: impl(std::make_shared<WorkQueue_Impl>(serial_queue))
{
}

WorkQueue::~WorkQueue()
{
}

void WorkQueue::queue(WorkItem *item) // transfers ownership
{
	impl->queue(item);
}

void WorkQueue::queue(const std::function<void()> &func)
{
	impl->queue(new WorkItemProcess(func));
}

void WorkQueue::work_completed(const std::function<void()> &func)
{
	impl->work_completed(new WorkItemWorkCompleted(func));
}

int WorkQueue::get_items_queued() const
{
	return impl->get_items_queued();
}

/////////////////////////////////////////////////////////////////////////////

WorkQueue_Impl::WorkQueue_Impl(bool serial_queue)
	: serial_queue(serial_queue)
{
}

WorkQueue_Impl::~WorkQueue_Impl()
{
	stop_event.set();
	for (size_t i = 0; i < threads.size(); i++)
		threads[i].join();
	for (size_t i = 0; i < queued_items.size(); i++)
		delete queued_items[i];
	for (size_t i = 0; i < finished_items.size(); i++)
		delete finished_items[i];
}

void WorkQueue_Impl::queue(WorkItem *item) // transfers ownership
{
	if (threads.empty())
	{
		int num_cores = serial_queue ? 1 : clan::max(System::get_num_cores() - 1, 1);
		for (int i = 0; i < num_cores; i++)
		{
			Thread thread;
			thread.start(this, &WorkQueue_Impl::worker_main);
			threads.push_back(thread);
		}
	}

	MutexSection mutex_lock(&mutex);
	queued_items.push_back(item);
	items_queued.increment();
	mutex_lock.unlock();
	work_available_event.set();
}

void WorkQueue_Impl::work_completed(WorkItem *item) // transfers ownership
{
	MutexSection mutex_lock(&mutex);
	finished_items.push_back(item);
	items_queued.increment();
	mutex_lock.unlock();
	set_wakeup_event();
}

void WorkQueue_Impl::process()
{
	MutexSection mutex_lock(&mutex);
	std::vector<WorkItem *> items;
	items.swap(finished_items);
	mutex_lock.unlock();
	for (size_t i = 0; i < items.size(); i++)
	{
		try
		{
			items[i]->work_completed();
		}
		catch (...)
		{
			mutex_lock.lock();
			finished_items.insert(finished_items.begin(), items.begin() + i, items.end());
			throw;
		}
		delete items[i];
		items_queued.decrement();
	}
}

void WorkQueue_Impl::worker_main()
{
	while (true)
	{
		int wakeup_reason = Event::wait(stop_event, work_available_event);
		if (wakeup_reason != 1)
			break;
		MutexSection mutex_lock(&mutex);
		if (!queued_items.empty())
		{
			WorkItem *item = queued_items.front();
			queued_items.erase(queued_items.begin());
			mutex_lock.unlock();
			item->process_work();
			mutex_lock.lock();
			finished_items.push_back(item);
			mutex_lock.unlock();
			set_wakeup_event();
		}
		else
		{
			work_available_event.reset();
		}
	}
}

}
