/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "API/Core/System/system.h"
#include <algorithm>
#include "API/Core/Math/cl_math.h"
#include <atomic>
#include <thread>
#include <condition_variable>

namespace clan
{
	class WorkItemProcess : public WorkItem
	{
	public:
		WorkItemProcess(const std::function<void()> &func) : func(func) { }

		void process_work() override { func(); }

	private:
		std::function<void()> func;
	};

	class WorkItemWorkCompleted : public WorkItem
	{
	public:
		WorkItemWorkCompleted(const std::function<void()> &func) : func(func) { }

		void process_work() override { }
		void work_completed() override { func(); }

	private:
		std::function<void()> func;
	};

	class WorkQueue_Impl
	{
	public:
		WorkQueue_Impl(bool serial_queue);
		~WorkQueue_Impl();

		void queue(WorkItem *item); // transfers ownership
		void work_completed(WorkItem *item); // transfers ownership

		int get_items_queued() const { return items_queued; }

		void process_work_completed();

	private:
		void worker_main();

		bool serial_queue = false;
		std::vector<std::thread> threads;
		std::mutex mutex;
		std::condition_variable worker_event;
		bool stop_flag = false;
		std::vector<WorkItem *> queued_items;
		std::vector<WorkItem *> finished_items;
		std::atomic_int items_queued;
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

	void WorkQueue::process_work_completed()
	{
		impl->process_work_completed();
	}

	/////////////////////////////////////////////////////////////////////////////

	WorkQueue_Impl::WorkQueue_Impl(bool serial_queue)
		: serial_queue(serial_queue)
	{
	}

	WorkQueue_Impl::~WorkQueue_Impl()
	{
		std::unique_lock<std::mutex> mutex_lock(mutex);
		stop_flag = true;
		mutex_lock.unlock();
		worker_event.notify_all();

		for (auto & elem : threads)
			elem.join();
		for (auto & elem : queued_items)
			delete elem;
		for (auto & elem : finished_items)
			delete elem;
	}

	void WorkQueue_Impl::queue(WorkItem *item) // transfers ownership
	{
		if (threads.empty())
		{
			int num_cores = serial_queue ? 1 : clan::max(System::get_num_cores() - 1, 1);
			for (int i = 0; i < num_cores; i++)
			{
				threads.push_back(std::thread(&WorkQueue_Impl::worker_main, this));
			}
		}

		std::unique_lock<std::mutex> mutex_lock(mutex);
		queued_items.push_back(item);
		++items_queued;
		mutex_lock.unlock();
		worker_event.notify_one();
	}

	void WorkQueue_Impl::work_completed(WorkItem *item) // transfers ownership
	{
		std::unique_lock<std::mutex> mutex_lock(mutex);
		finished_items.push_back(item);
		++items_queued;
	}

	void WorkQueue_Impl::process_work_completed()
	{
		std::unique_lock<std::mutex> mutex_lock(mutex);
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
			--items_queued;
		}
	}

	void WorkQueue_Impl::worker_main()
	{
		while (true)
		{
			std::unique_lock<std::mutex> mutex_lock(mutex);
			worker_event.wait(mutex_lock, [&]() { return stop_flag || !queued_items.empty(); });

			if (stop_flag)
				break;

			WorkItem *item = queued_items.front();
			queued_items.erase(queued_items.begin());
			mutex_lock.unlock();

			item->process_work();

			mutex_lock.lock();
			finished_items.push_back(item);
			mutex_lock.unlock();
		}
	}
}
