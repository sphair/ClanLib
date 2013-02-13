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

#include "Scene3D/precomp.h"
#include "work_queue.h"

namespace clan
{


WorkQueue::WorkQueue(GraphicContext gc)
: gc(gc)
{
/*	DisplayWindowDescription desc;
	desc.set_size(Size(320,200), false);
	desc.set_title("OpenGL Worker Window");
	desc.set_visible(false);
	window_single = DisplayWindow(desc);*/

	int num_cores = max(System::get_num_cores() - 1, 1);
	for (int i = 0; i < num_cores; i++)
	{
		Thread thread;
		thread.start(this, &WorkQueue::worker_main);
		threads.push_back(thread);
	}
}

WorkQueue::~WorkQueue()
{
	stop_event.set();
	for (size_t i = 0; i < threads.size(); i++)
		threads[i].join();
	for (size_t i = 0; i < queued_items.size(); i++)
		delete queued_items[i];
	for (size_t i = 0; i < finished_items.size(); i++)
		delete finished_items[i];
}

void WorkQueue::queue(WorkItem *item) // transfers ownership
{
	MutexSection mutex_lock(&mutex);
	queued_items.push_back(item);
	mutex_lock.unlock();
	work_available_event.set();
}

void WorkQueue::process()
{
//	GraphicContext gc = window_single.get_gc();
	MutexSection mutex_lock(&mutex);
	std::vector<WorkItem *> items;
	items.swap(finished_items);
	mutex_lock.unlock();
	for (size_t i = 0; i < items.size(); i++)
	{
		try
		{
			items[i]->work_completed(gc);
		}
		catch (...)
		{
			mutex_lock.lock();
			finished_items.insert(finished_items.begin(), items.begin() + i, items.end());
			throw;
		}
		delete items[i];
	}

/*	if (!items.empty())
	{
		OpenGL::set_active(gc);
		glFinish(); // To do: check if we can get away with just using glFlush
	}*/
}

void WorkQueue::worker_main()
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

