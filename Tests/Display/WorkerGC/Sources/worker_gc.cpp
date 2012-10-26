/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#include "precomp.h"
#include "worker_gc.h"

WorkerGC::WorkerGC() : worker_thread_started(false), worker_thread_processing(false), exception_set(false), exception("")
{
	worker_thread.start(this, &WorkerGC::worker_thread_loop);
	worker_thread_started = true;
}

WorkerGC::~WorkerGC()
{
	if (worker_thread_started)
	{
		event_stop.set();
		worker_thread.join();
	}
}

void WorkerGC::Start(const CL_StringRef &name)
{
	if (worker_thread_processing)
		throw CL_Exception("You forgot to get the result");
	worker_thread_processing = true;
	sprite_name = name;
	event_start.set();
}

bool WorkerGC::IsReady()
{
	return event_completed.wait(0);
}

CL_Sprite WorkerGC::Get()
{
	if (!worker_thread_processing)
		throw CL_Exception("You forgot to start!");
	worker_thread_processing = false;

	event_completed.wait();
	event_completed.reset();

	if (exception_set)
	{
		throw exception;
	}

	CL_Sprite sprite = retrieved_sprite;
	retrieved_sprite = CL_Sprite();
	return sprite;

}

void WorkerGC::worker_thread_loop()
{
	CL_DisplayWindowDescription desc;
	desc.set_title("ClanLib Worker Thread");
	desc.set_size(CL_Size(320, 320), true);
	//desc.set_visible(false);
	window = CL_DisplayWindow(desc);

	while (true)
	{
		int wakeup_reason = CL_Event::wait(event_start, event_stop);

		if (wakeup_reason != 0)
			break;

		event_start.reset();
	
		try
		{

			CL_GraphicContext gc = window.get_gc();

			// Test code 
			unsigned int time_ms = CL_System::get_time();
			while((CL_System::get_time() - time_ms) < 2000)
			{
				gc.clear(CL_Colorf(1.0f,0.0f,0.2f));
				CL_KeepAlive::process();
				window.flip(0);
			}
			window.set_visible(false, false);
			retrieved_sprite = CL_Sprite(gc, sprite_name);
			CL_OpenGL::set_active(gc);
			glFlush();
		}
		catch (CL_Exception &error)
		{
			exception_set = true;
			exception = error;
		}

		event_completed.set();
	}
	window = CL_DisplayWindow();
}

