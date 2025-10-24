
#pragma once

#include "crash_reporter.h"

class DetectHang : CL_KeepAliveObject
{
public:
	DetectHang()
	{
		thread.start(this, &DetectHang::worker_main);
	}

	~DetectHang()
	{
		stop.set();
		thread.join();
	}

private:
	CL_Event get_wakeup_event() { return wakeup; }

	void process()
	{
		CL_MutexSection mutex_lock(&mutex);
		wakeup.reset();
		awoken.set();
	}

	void worker_main()
	{
		while (true)
		{
			CL_MutexSection mutex_lock(&mutex);
			awoken.reset();
			wakeup.set();
			mutex_lock.unlock();
			int reason = CL_Event::wait(awoken, stop, 30000);
			if (reason == -1)
				CrashReporter::invoke();
			else if (reason == 1)
				break;

			reason = CL_Event::wait(stop, 30000);
			if (reason == 0)
				break;
		}
	}

	CL_Mutex mutex;
	CL_Event wakeup, awoken, stop;
	CL_Thread thread;
};
