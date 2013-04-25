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

#pragma once

#include "API/Core/ErrorReporting/crash_reporter.h"
#include "API/Core/System/keep_alive.h"
#include "API/Core/System/mutex.h"
#include "API/Core/System/event.h"
#include "API/Core/System/thread.h"

namespace clan
{

class DetectHang_Impl : KeepAliveObject
{
public:
	DetectHang_Impl()
	{
		thread.start(this, &DetectHang_Impl::worker_main);
	}

	~DetectHang_Impl()
	{
		stop.set();
		thread.join();
	}

private:
	void process()
	{
		MutexSection mutex_lock(&mutex);
		awoken.set();
	}

	void worker_main()
	{
		while (true)
		{
			MutexSection mutex_lock(&mutex);
			awoken.reset();
			set_wakeup_event();
			mutex_lock.unlock();
			int reason = Event::wait(awoken, stop, 30000);
			if (reason == -1)
				CrashReporter::invoke();
			else if (reason == 1)
				break;

			reason = Event::wait(stop, 30000);
			if (reason == 0)
				break;
		}
	}

	Mutex mutex;
	Event awoken, stop;
	Thread thread;
};

}
