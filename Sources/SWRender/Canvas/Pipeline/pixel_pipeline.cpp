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

#include "SWRender/precomp.h"
//#define PROFILE_PIPELINE
#include "pixel_pipeline.h"
#include "API/SWRender/pixel_thread_context.h"
#include "API/SWRender/pixel_command.h"

#ifdef _MSC_VER
	#include <intrin.h>
	#define cl_compiler_barrier() _ReadWriteBarrier()
#else
	#define cl_compiler_barrier()  __asm__ __volatile__("" : : : "memory")
#endif

#ifdef PROFILE_PIPELINE
#include "API/Core/Text/string_format.h"
#endif

namespace clan
{


CL_PixelPipeline::CL_PixelPipeline()
: active_cores(0), local_writer_index(0), local_reader_index(0), local_commands_written(0), cur_block(0)
{
#if defined(WIN32) && defined(PROFILE_PIPELINE)
	SetThreadIdealProcessor(GetCurrentThread(), 0);
	SetThreadAffinityMask(GetCurrentThread(), 1);
	profiler.start_time = __rdtsc();
#endif

	active_cores = CL_System::get_num_cores();
	for (size_t i = 0; i < queue_max; i++)
		command_queue[i] = 0;
	reader_indices.resize(active_cores);
	reader_active.resize(active_cores);

	// Do not change this code to event_more_commands.resize().
	// If you do this, the same CL_Event handle end up in every index due to resize(n) calling resize(n, CL_Event()).
	for (int core = 0; core < active_cores; core++)
		event_more_commands.push_back(CL_Event());

	for (int core = 0; core < active_cores; core++)
	{
		CL_Thread worker_thread;
		worker_thread.start(this, &CL_PixelPipeline::worker_main, core);
		worker_threads.push_back(worker_thread);
	}
}

CL_PixelPipeline::~CL_PixelPipeline()
{
	wait_for_workers();
#if defined(WIN32) && defined(PROFILE_PIPELINE)
	profiler.end_time = __rdtsc();
#endif
	event_stop.set();
	for (std::vector<CL_Thread>::size_type i = 0; i < worker_threads.size(); i++)
		worker_threads[i].join();

	for (size_t i = 0; i < queue_max; i++)
	{
		delete command_queue[i];
		command_queue[i] = 0;
	}

	if (cur_block && cur_block->refcount == 1)
		delete[] (char*) cur_block;

#if defined(WIN32) && defined(PROFILE_PIPELINE)
	MessageBoxA(0, cl_format("Queue = %1\r\nSetEvent = %2\r\nWaitForWorkers = %3\r\nWaitForSpace = %4\r\nAllocFree = %5",
		(int)(profiler.queue_time*100/(profiler.end_time-profiler.start_time)),
		(int)(profiler.set_event_time*100/(profiler.end_time-profiler.start_time)),
		(int)(profiler.wait_for_workers_time*100/(profiler.end_time-profiler.start_time)),
		(int)(profiler.wait_for_space_time*100/(profiler.end_time-profiler.start_time)),
		(int)(profiler.alloc_time*100/(profiler.end_time-profiler.start_time))).c_str(),
		"DEBUG", MB_OK);
#endif
}

void CL_PixelPipeline::queue(CL_UniquePtr<CL_PixelCommand> &command)
{
	wait_for_space();
	delete command_queue[local_writer_index];

#if defined(WIN32) && defined(PROFILE_PIPELINE)
	unsigned __int64 start_time = __rdtsc();
#endif

	command_queue[local_writer_index] = command.get();
	command.release();

	local_writer_index++;
	if (local_writer_index == queue_max)
		local_writer_index = 0;
	local_commands_written++;

	if (local_commands_written == fragment_size)
	{
		cl_compiler_barrier();
		writer_index.set(local_writer_index);
		local_commands_written = 0;

#if defined(WIN32) && defined(PROFILE_PIPELINE)
		unsigned __int64 start_event_time = __rdtsc();
#endif
		for (int i = 0; i < active_cores; i++)
		{
			if (reader_active[i].get() == 0)
				event_more_commands[i].set();
		}
#if defined(WIN32) && defined(PROFILE_PIPELINE)
		unsigned __int64 end_event_time = __rdtsc();
		profiler.set_event_time += end_event_time-start_event_time;
#endif
	}

#if defined(WIN32) && defined(PROFILE_PIPELINE)
	unsigned __int64 end_time = __rdtsc();
	profiler.queue_time += end_time-start_time;
#endif
}

void CL_PixelPipeline::wait_for_space()
{
#if defined(WIN32) && defined(PROFILE_PIPELINE)
	unsigned __int64 start_time = __rdtsc();
#endif

	int next_index = local_writer_index+1;
	if (next_index == queue_max)
		next_index = 0;
	if (next_index == local_reader_index)
	{
		update_local_reader_index();
		while (next_index == local_reader_index)
		{
			event_reader_done.wait();
			event_reader_done.reset();
			update_local_reader_index();
		}
	}

#if defined(WIN32) && defined(PROFILE_PIPELINE)
	unsigned __int64 end_time = __rdtsc();
	profiler.wait_for_space_time += end_time-start_time;
#endif
}

void CL_PixelPipeline::wait_for_workers()
{
#if defined(WIN32) && defined(PROFILE_PIPELINE)
	unsigned __int64 start_time = __rdtsc();
#endif

	if (local_commands_written > 0)
	{
		cl_compiler_barrier();
		writer_index.set(local_writer_index);
		local_commands_written = 0;

		for (int i = 0; i < active_cores; i++)
			event_more_commands[i].set();
	}

	if (local_reader_index != local_writer_index)
	{
		update_local_reader_index();
		while (local_reader_index != local_writer_index)
		{
			event_reader_done.wait();
			event_reader_done.reset();
			update_local_reader_index();
		}
	}

#if defined(WIN32) && defined(PROFILE_PIPELINE)
	unsigned __int64 end_time = __rdtsc();
	profiler.wait_for_workers_time += end_time-start_time;
#endif
}

void CL_PixelPipeline::update_local_reader_index()
{
	local_reader_index = local_writer_index;
	for (int i = 0; i < active_cores; i++)
	{
		int reader_index = reader_indices[i].get();
		if (reader_index > local_writer_index)
			reader_index -= queue_max;
		if (reader_index < local_reader_index)
			local_reader_index = reader_index;
	}
	if (local_reader_index < 0)
		local_reader_index += queue_max;
}

void CL_PixelPipeline::worker_main(int core)
{
#if defined(WIN32) && defined(PROFILE_PIPELINE)
	SetThreadIdealProcessor(GetCurrentThread(), core);
	SetThreadAffinityMask(GetCurrentThread(), 1 << core);
	unsigned __int64 ticks_waiting = 0;
	unsigned __int64 ticks_working = 0;
#endif
	CL_PixelThreadContext context(core, active_cores);
	while (true)
	{
#if defined(WIN32) && defined(PROFILE_PIPELINE)
		unsigned __int64 wait_start_time = __rdtsc();
#endif
		int wakeup_reason = CL_Event::wait(event_more_commands[core], event_stop);
		if (wakeup_reason != 0)
			break;
		event_more_commands[core].reset();
#if defined(WIN32) && defined(PROFILE_PIPELINE)
		unsigned __int64 wait_end_time = __rdtsc();
		ticks_waiting += wait_end_time-wait_start_time;
#endif
		process_commands(&context);
#if defined(WIN32) && defined(PROFILE_PIPELINE)
		unsigned __int64 commands_end_time = __rdtsc();
		ticks_working += commands_end_time-wait_end_time;
#endif
	}
#if defined(WIN32) && defined(PROFILE_PIPELINE)
	MessageBoxA(
		0,
		cl_format("Pipeline core %1 spent %2 percent of its time waiting for commands",
		core,
		(int)(ticks_waiting*100/(ticks_working+ticks_waiting))).c_str(),
		"DEBUG", MB_OK);
#endif
}

void CL_PixelPipeline::process_commands(CL_PixelThreadContext *context)
{
	while (true)
	{
		int worker_reader_index = reader_indices[context->core].get();
		int worker_writer_index = writer_index.get();
		int worker_commands_retired = 0;
		if (worker_reader_index == worker_writer_index)
			break;

		reader_active[context->core].set(1);
		while (worker_reader_index != worker_writer_index)
		{
			CL_PixelCommand *command = command_queue[worker_reader_index];
			command->run(context);

			worker_reader_index++;
			if (worker_reader_index == queue_max)
				worker_reader_index = 0;
			worker_commands_retired++;

			if (worker_commands_retired == fragment_size)
			{
				cl_compiler_barrier();
				reader_indices[context->core].set(worker_reader_index);
				worker_commands_retired = 0;
				event_reader_done.set();
			}
		}

		if (worker_commands_retired > 0)
		{
			cl_compiler_barrier();
			reader_indices[context->core].set(worker_reader_index);
			worker_commands_retired = 0;
			event_reader_done.set();
		}
		reader_active[context->core].set(0);
	}
}

void *CL_PixelPipeline::alloc_command(size_t s)
{
#if defined(WIN32) && defined(PROFILE_PIPELINE)
	unsigned __int64 start_time = __rdtsc();
#endif

	s += sizeof(unsigned int);
	s = ((s+63)/64)*64; // Place each command in its own cache line (is this really smart?)
	if (cur_block == 0 || cur_block->pos+s > cur_block->size)
	{
		if (cur_block)
		{
			cur_block->refcount--;
			if (cur_block->refcount == 0)
				delete[] (char*) cur_block;
		}
		cur_block = 0;

		size_t block_size = cl_max(s, (size_t)16*1024);
		char *data = new char[sizeof(AllocBlock) + block_size];
		cur_block = (AllocBlock *)data;
		cur_block->size = block_size;
		cur_block->data = data + sizeof(AllocBlock);
		cur_block->pos = 0;
		cur_block->refcount = 1;
	}

	char *d = cur_block->data + cur_block->pos;
	unsigned int *p = (unsigned int *) d;
	d += sizeof(unsigned int);
	*p = (unsigned int) cur_block->pos;
	cur_block->pos += s;
	cur_block->refcount++;

#if defined(WIN32) && defined(PROFILE_PIPELINE)
	unsigned __int64 end_time = __rdtsc();
	profiler.alloc_time += end_time-start_time;
#endif

	return d;
}

void CL_PixelPipeline::free_command(void *d)
{
#if defined(WIN32) && defined(PROFILE_PIPELINE)
	unsigned __int64 start_time = __rdtsc();
#endif

	char *data = (char *) d;
	data -= sizeof(unsigned int);
	unsigned int *p = (unsigned int *) data;
	AllocBlock *block = (AllocBlock *)(data - (*p) - sizeof(AllocBlock));
	block->refcount--;
	if (block->refcount == 0)
		delete[] (char*) block;

#if defined(WIN32) && defined(PROFILE_PIPELINE)
	unsigned __int64 end_time = __rdtsc();
	profiler.alloc_time += end_time-start_time;
#endif
}

}
