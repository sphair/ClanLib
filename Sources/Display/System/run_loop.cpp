/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#include "Display/precomp.h"
#include "API/Display/System/run_loop.h"
#include "run_loop_impl.h"

namespace clan
{
	void RunLoop::run()
	{
		RunLoopImpl::get_instance()->run();
	}

	void RunLoop::exit()
	{
		RunLoopImpl::get_instance()->exit();
	}

	bool RunLoop::process(int timeout_ms)
	{
		return RunLoopImpl::get_instance()->process(timeout_ms);
	}

	void RunLoop::main_thread_async(std::function<void()> func)
	{
		RunLoopImpl *impl = RunLoopImpl::get_instance();
		
		std::unique_lock<std::mutex> lock(impl->mutex);
		bool needs_notify = impl->async_work.empty();
		impl->async_work.push_back(func);
		lock.unlock();

		if (needs_notify)
			impl->post_async_work_needed();
	}

	/////////////////////////////////////////////////////////////////////////

	RunLoopImpl *RunLoopImpl::get_instance()
	{
		if (!instance)
			throw Exception("No RunLoop handler linked");
		return instance;
	}

	RunLoopImpl::RunLoopImpl()
	{
		instance = this;
	}

	RunLoopImpl::~RunLoopImpl()
	{
		instance = 0;
	}

	void RunLoopImpl::process_async_work()
	{
		std::vector<std::function<void()>> current_async_work;

		std::unique_lock<std::mutex> lock(mutex);
		current_async_work.swap(async_work);
		lock.unlock();

		for (auto &work : current_async_work)
		{
			work();
		}
	}

	RunLoopImpl *RunLoopImpl::instance = 0;
}
