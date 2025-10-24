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
*/

#pragma once

#include "API/SWRender/pixel_thread_context.h"
#include "API/Core/System/event.h"
#include "API/Core/System/thread.h"
#include "API/Core/System/interlocked_variable.h"
#include "API/Core/System/uniqueptr.h"

#include "API/SWRender/pixel_command.h"
#include <memory>

class CL_PixelCommand;

class CL_PixelPipeline
{
public:
	CL_PixelPipeline();
	~CL_PixelPipeline();

	void queue(CL_PixelCommand *command) { CL_UniquePtr<CL_PixelCommand> cmd(command); queue(cmd); } 
	void queue(CL_UniquePtr<CL_PixelCommand> &command);

	void wait_for_workers();

	void *alloc_command(size_t s);
	void free_command(void *d);

private:
	void worker_main(int core);
	void process_commands(CL_PixelThreadContext *context);
	void wait_for_space();
	void update_local_reader_index();

	int active_cores;
	CL_Event event_stop;
	std::vector<CL_Thread> worker_threads;

	enum { queue_max = 12*1024, fragment_size = 3*1024 };
	CL_PixelCommand *command_queue[queue_max];

	int local_writer_index;
	int local_reader_index;
	int local_commands_written;
	CL_InterlockedVariable writer_index;
	std::vector<CL_InterlockedVariable> reader_indices;
	std::vector<CL_Event> event_more_commands;
	CL_Event event_reader_done;

	std::vector<CL_InterlockedVariable> reader_active;

	struct AllocBlock
	{
		size_t size;
		char *data;
		size_t pos;
		size_t refcount;
	};
	AllocBlock *cur_block;

	#ifdef PROFILE_PIPELINE
	struct PerformanceCounters
	{
		PerformanceCounters() : start_time(0), end_time(0), queue_time(0), set_event_time(0), wait_for_workers_time(0), wait_for_space_time(0), alloc_time(0) { }
		unsigned __int64 start_time;
		unsigned __int64 end_time;
		unsigned __int64 queue_time;
		unsigned __int64 set_event_time;
		unsigned __int64 wait_for_workers_time;
		unsigned __int64 wait_for_space_time;
		unsigned __int64 alloc_time;
	} profiler;
	#endif
};
