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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "test.h"

class InterlockCheck
{
public:
	InterlockCheck();
	~InterlockCheck();

	void start(int num_iterations);

	void wait_for_workers();
	bool is_still_active();
	void worker_main(int core);

	int active_cores;
	Event event_stop;
	std::vector<Thread> worker_threads;

	InterlockedVariable run_count;
	InterlockedVariable write_check;
	Event writer_event;
	std::vector<Event> reader_events;
	std::vector<InterlockedVariable> iteration_count;
};

void TestApp::test_interlock()
{
	Console::write_line(" Header: interlocked_variable.h");
	Console::write_line("  Class: InterlockedVariable");

	Console::write_line("   Function: 	InterlockedVariable()");
	{
		InterlockedVariable var;
		var.set(123);
		if (var.get() != 123)
			fail();
	}

	Console::write_line("   Function: 	InterlockedVariable(const InterlockedVariable &src)");
	{
		InterlockedVariable var;
		var.set(123);
		InterlockedVariable var2(var);
		var.set(333);
		if (var2.get() != 123)
			fail();
	}


	Console::write_line("   Function: 	InterlockedVariable &operator =(const InterlockedVariable &src)");
	{
		InterlockedVariable var;
		var.set(123);
		InterlockedVariable var2 = var;
		var.set(333);
		if (var2.get() != 123)
			fail();
	}

	Console::write_line("   Function: 	LONG get() const");
	{
		InterlockedVariable var;
		var.set(13);
		var.set(123);
		if (var.get() != 123)
			fail();
		if (var.get() != 123)
			fail();
		var.set(-555);
		if (var.get() != -555)
			fail();
		if (var.get() != -555)
			fail();
	}

	Console::write_line("   Function: 	void set(LONG new_value)");
	{
		InterlockedVariable var;
		var.set(13);
		if (var.get() != 13)
			fail();
		var.set(0);
		if (var.get() != 0)
			fail();
		var.set(-13);
		if (var.get() != -13)
			fail();
	}

	Console::write_line("   Function: 	LONG increment()");
	{
		InterlockedVariable var;
		var.set(123);
		var.increment();
		if (var.get() != 124)
			fail();
		var.set(-123);
		var.increment();
		if (var.get() != -122)
			fail();
	}

	Console::write_line("   Function: 	LONG decrement()");
	{
		InterlockedVariable var;
		var.set(123);
		var.decrement();
		if (var.get() != 122)
			fail();
		var.set(-123);
		var.decrement();
		if (var.get() != -124)
			fail();
	}

	Console::write_line("   Function: 	bool compare_and_swap(LONG expected_value, LONG new_value)");
	{
		InterlockedVariable var;
		var.set(123);
		if (!var.compare_and_swap(123, 456))
			fail();
		if (var.get() != 456)
			fail();
		if (var.compare_and_swap(123, 789))
			fail();
		if (var.get() != 456)
			fail();

	}

	Console::write_line("   Thread Checking");

	InterlockCheck check;
	for (int check_cnt = 0; check_cnt < 1024; check_cnt++)
	{
		check.start(20000);
		check.wait_for_workers();
		if (check.write_check.get() != 0)
			fail();
	}
	if (check.run_count.get() != (1024*2))
		fail();

}


InterlockCheck::InterlockCheck()
{
	active_cores = 2;	// Test is not designed for more than 2 active cores (for simplicity) System::get_num_cores();
	iteration_count.resize(active_cores);
	//reader_events.resize(active_cores);
	reader_events.push_back(Event());
	reader_events.push_back(Event());

	for (int core = 0; core < active_cores; core++)
	{
		Thread worker_thread;
		worker_thread.start(this, &InterlockCheck::worker_main, core);
		worker_threads.push_back(worker_thread);
	}
	run_count.set(0);

}

InterlockCheck::~InterlockCheck()
{
	event_stop.set();
	for (std::vector<Thread>::size_type i = 0; i < worker_threads.size(); i++)
		worker_threads[i].join();

}

void InterlockCheck::start(int num_iterations)
{
	iteration_count[0].set(num_iterations/2);
	iteration_count[1].set(num_iterations - iteration_count[0].get());
	write_check.set(num_iterations);
	for (int i = 0; i < 2; i++)
	{
		reader_events[i].set();
	}
}

void InterlockCheck::wait_for_workers()
{
	while (is_still_active())
	{
		writer_event.wait(100);
		writer_event.reset();
	}
}

bool InterlockCheck::is_still_active()
{
	for (int core = 0; core < active_cores; core++)
	{
		if (iteration_count[core].get() != 0)
			return true;
	}
	return false;
}

void InterlockCheck::worker_main(int core)
{
	while (true)
	{
		int wakeup_reason = Event::wait(reader_events[core], event_stop);

		if (wakeup_reason != 0)
			break;
		
		run_count.increment();

		reader_events[core].reset();

		while(true)
		{
			write_check.decrement();
			iteration_count[core].decrement();
			if (iteration_count[core].get() == 0)
				break;
		}
		writer_event.set();

	}
}

