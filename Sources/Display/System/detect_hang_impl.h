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
*/

#pragma once

#include "API/Core/ErrorReporting/crash_reporter.h"
#include "API/Display/System/run_loop.h"
#include <mutex>
#include <thread>
#include <condition_variable>

namespace clan
{
	class DetectHang_Impl
	{
	public:
		DetectHang_Impl()
		{
			thread = std::thread(&DetectHang_Impl::worker_main, this);
		}

		~DetectHang_Impl()
		{
			{
				std::unique_lock<std::mutex> mutex_lock(mutex);
				stop_flag = true;
			}
			stop_condition.notify_all();
			thread.join();
		}

	private:
		void worker_main()
		{
			while (true)
			{
				std::unique_lock<std::mutex> mutex_lock(mutex);

				std::future<void> heartbeat = RunLoop::main_thread_task([](){});

				if (stop_condition.wait_for(mutex_lock, std::chrono::seconds(30), [&]() -> bool { return stop_flag; }))
					break;

				if (heartbeat.wait_for(std::chrono::seconds(1)) == std::future_status::timeout)
					CrashReporter::invoke();
			}
		}

		std::mutex mutex;
		std::condition_variable stop_condition;
		bool stop_flag = false;
		std::thread thread;
	};
}
