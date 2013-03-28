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

#include "Compute/precomp.h"
#include "API/Compute/compute_command_queue.h"
#include "API/Compute/compute_kernel.h"
#include "API/Compute/compute_event.h"
#include "API/Compute/compute_buffer.h"
#include "API/Compute/compute_memory_map.h"
#include "API/Compute/compute_context.h"
#include "compute_command_queue_impl.h"
#include "compute_kernel_impl.h"
#include "compute_event_impl.h"
#include "compute_buffer_impl.h"
#include "compute_memory_map_impl.h"
#include "compute_context_impl.h"
#include "GL/GL3/gl3_graphic_context_provider.h"
#ifdef WIN32
#include "D3D/d3d_graphic_context_provider.h"
#endif

namespace clan
{

ComputeCommandQueue::ComputeCommandQueue()
{
}

ComputeCommandQueue::ComputeCommandQueue(ComputeContext &context, bool enable_out_of_order_execute, bool enable_profiling)
: impl(new ComputeCommandQueue_Impl(context, enable_out_of_order_execute, enable_profiling))
{
}

ComputeCommandQueue::~ComputeCommandQueue()
{
}

void ComputeCommandQueue::throw_if_null() const
{
	if (is_null())
		throw Exception("CL_ComputeCommandQueue is null!");
}

void ComputeCommandQueue::flush()
{
	cl_int result = impl->bindings->Flush(impl->handle);
	if (result != CL_SUCCESS)
		throw Exception("clFlush failed");
}

void ComputeCommandQueue::finish()
{
	cl_int result = impl->bindings->Finish(impl->handle);
	if (result != CL_SUCCESS)
		throw Exception("clFinish failed");
}

ComputeEvent ComputeCommandQueue::run_task(const ComputeKernel &kernel, const ComputeWaitList &wait_list)
{
	std::vector<cl_event> wait_list_ids = impl->get_event_ids(wait_list);
	cl_event event = 0;
	cl_int result = impl->bindings->EnqueueTask(impl->handle, kernel.impl->handle, wait_list.size(), wait_list_ids.empty() ? 0 : &wait_list_ids[0], &event);
	if (result != CL_SUCCESS)
		throw Exception("clEnqueueBarrierWithWaitList failed");
	return ComputeEvent(new ComputeEvent_Impl(impl->bindings, event));
}

ComputeEvent ComputeCommandQueue::run_1d(
	const ComputeKernel &kernel,
	size_t global_work_size,
	size_t local_work_size,
	int offset,
	const ComputeWaitList &wait_list)
{
	size_t global_offset[1] = { offset };
	size_t global_size[1] = { global_work_size };
	size_t local_size[1] = { local_work_size };

	std::vector<cl_event> wait_list_ids = impl->get_event_ids(wait_list);
	cl_event event = 0;
	cl_int result = impl->bindings->EnqueueNDRangeKernel(
		impl->handle,
		kernel.impl->handle,
		1,
		global_offset,
		global_work_size != 0 ? global_size : 0,
		local_work_size != 0 ? local_size : 0,
		wait_list.size(),
		wait_list_ids.empty() ? 0 : &wait_list_ids[0],
		&event);
	if (result != CL_SUCCESS)
		throw Exception("clEnqueueNDRangeKernel failed");
	return ComputeEvent(new ComputeEvent_Impl(impl->bindings, event));
}

ComputeEvent ComputeCommandQueue::run_2d(
	const ComputeKernel &kernel,
	size_t global_work_size_x,
	size_t global_work_size_y,
	size_t local_work_size_x,
	size_t local_work_size_y,
	int offset_x,
	int offset_y,
	const ComputeWaitList &wait_list)
{
	size_t global_offset[2] = { offset_x, offset_y };
	size_t global_size[2] = { global_work_size_x, global_work_size_y };
	size_t local_size[2] = { local_work_size_x, local_work_size_y };

	std::vector<cl_event> wait_list_ids = impl->get_event_ids(wait_list);
	cl_event event = 0;
	cl_int result = impl->bindings->EnqueueNDRangeKernel(
		impl->handle,
		kernel.impl->handle,
		2,
		global_offset,
		global_work_size_x != 0 ? global_size : 0,
		local_work_size_x != 0 ? local_size : 0,
		wait_list.size(),
		wait_list_ids.empty() ? 0 : &wait_list_ids[0],
		&event);
	if (result != CL_SUCCESS)
		throw Exception("clEnqueueNDRangeKernel failed");
	return ComputeEvent(new ComputeEvent_Impl(impl->bindings, event));
}

ComputeEvent ComputeCommandQueue::run_3d(
	const ComputeKernel &kernel,
	size_t global_work_size_x,
	size_t global_work_size_y,
	size_t global_work_size_z,
	size_t local_work_size_x,
	size_t local_work_size_y,
	size_t local_work_size_z,
	int offset_x,
	int offset_y,
	int offset_z,
	const ComputeWaitList &wait_list)
{
	size_t global_offset[3] = { offset_x, offset_y, offset_z };
	size_t global_size[3] = { global_work_size_x, global_work_size_y, global_work_size_z };
	size_t local_size[3] = { local_work_size_x, local_work_size_y, local_work_size_z };

	std::vector<cl_event> wait_list_ids = impl->get_event_ids(wait_list);
	cl_event event = 0;
	cl_int result = impl->bindings->EnqueueNDRangeKernel(
		impl->handle,
		kernel.impl->handle,
		3,
		global_offset,
		global_work_size_x != 0 ? global_size : 0,
		local_work_size_x != 0 ? local_size : 0,
		wait_list.size(),
		wait_list_ids.empty() ? 0 : &wait_list_ids[0],
		&event);
	if (result != CL_SUCCESS)
		throw Exception("clEnqueueNDRangeKernel failed");
	return ComputeEvent(new ComputeEvent_Impl(impl->bindings, event));
}

ComputeEvent ComputeCommandQueue::add_barrier(const ComputeWaitList &wait_list)
{
	std::vector<cl_event> wait_list_ids = impl->get_event_ids(wait_list);
	cl_event event = 0;
	cl_int result = impl->bindings->EnqueueBarrierWithWaitList(impl->handle, wait_list.size(), wait_list_ids.empty() ? 0 : &wait_list_ids[0], &event);
	if (result != CL_SUCCESS)
		throw Exception("clEnqueueBarrierWithWaitList failed");
	return ComputeEvent(new ComputeEvent_Impl(impl->bindings, event));
}

ComputeEvent ComputeCommandQueue::add_marker(const ComputeWaitList &wait_list)
{
	std::vector<cl_event> wait_list_ids = impl->get_event_ids(wait_list);
	cl_event event = 0;
	cl_int result = impl->bindings->EnqueueMarkerWithWaitList(impl->handle, wait_list.size(), wait_list_ids.empty() ? 0 : &wait_list_ids[0], &event);
	if (result != CL_SUCCESS)
		throw Exception("clEnqueueMarkerWithWaitList failed");
	return ComputeEvent(new ComputeEvent_Impl(impl->bindings, event));
}

ComputeEvent ComputeCommandQueue::read_buffer(ComputeBuffer &buffer, bool blocking_read, size_t offset, size_t size, void *ptr, const ComputeWaitList &wait_list)
{
	std::vector<cl_event> wait_list_ids = impl->get_event_ids(wait_list);
	cl_event event = 0;
	cl_int result = impl->bindings->EnqueueReadBuffer(impl->handle, buffer.impl->handle, blocking_read ? CL_TRUE : CL_FALSE, offset, size, ptr, wait_list.size(), wait_list_ids.empty() ? 0 : &wait_list_ids[0], &event);
	if (result != CL_SUCCESS)
		throw Exception("clEnqueueReadBuffer failed");
	return ComputeEvent(new ComputeEvent_Impl(impl->bindings, event));
}

ComputeEvent ComputeCommandQueue::write_buffer(ComputeBuffer &buffer, bool blocking_write, size_t offset, size_t size, void *ptr, const ComputeWaitList &wait_list)
{
	std::vector<cl_event> wait_list_ids = impl->get_event_ids(wait_list);
	cl_event event = 0;
	cl_int result = impl->bindings->EnqueueWriteBuffer(impl->handle, buffer.impl->handle, blocking_write ? CL_TRUE : CL_FALSE, offset, size, ptr, wait_list.size(), wait_list_ids.empty() ? 0 : &wait_list_ids[0], &event);
	if (result != CL_SUCCESS)
		throw Exception("clEnqueueWriteBuffer failed");
	return ComputeEvent(new ComputeEvent_Impl(impl->bindings, event));
}

ComputeEvent ComputeCommandQueue::fill_buffer(ComputeBuffer &buffer, const void *pattern, size_t pattern_size, size_t offset, size_t size, const ComputeWaitList &wait_list)
{
	std::vector<cl_event> wait_list_ids = impl->get_event_ids(wait_list);
	cl_event event = 0;
	cl_int result = impl->bindings->EnqueueFillBuffer(impl->handle, buffer.impl->handle, pattern, pattern_size, offset, size, wait_list.size(), wait_list_ids.empty() ? 0 : &wait_list_ids[0], &event);
	if (result != CL_SUCCESS)
		throw Exception("clEnqueueFillBuffer failed");
	return ComputeEvent(new ComputeEvent_Impl(impl->bindings, event));
}

ComputeEvent ComputeCommandQueue::copy_buffer(ComputeBuffer &src, ComputeBuffer &dest, size_t src_offset, size_t dest_offset, size_t size, const ComputeWaitList &wait_list)
{
	std::vector<cl_event> wait_list_ids = impl->get_event_ids(wait_list);
	cl_event event = 0;
	cl_int result = impl->bindings->EnqueueCopyBuffer(impl->handle, src.impl->handle, dest.impl->handle, src_offset, dest_offset, size, wait_list.size(), wait_list_ids.empty() ? 0 : &wait_list_ids[0], &event);
	if (result != CL_SUCCESS)
		throw Exception("clEnqueueCopyBuffer failed");
	return ComputeEvent(new ComputeEvent_Impl(impl->bindings, event));
}

ComputeMemoryMap ComputeCommandQueue::map_buffer(ComputeBuffer &buffer, BufferAccess access, size_t offset, size_t size, const ComputeWaitList &wait_list)
{
	cl_map_flags flags = 0;
	switch (access)
	{
	case access_read_only: flags = CL_MEM_READ_ONLY; break;
	case access_write_only: flags = CL_MEM_WRITE_ONLY; break;
	case access_write_discard: flags = CL_MEM_WRITE_ONLY; break;
	case access_read_write: flags = CL_MEM_READ_WRITE; break;
	}

	std::vector<cl_event> wait_list_ids = impl->get_event_ids(wait_list);
	cl_int result = 0;
	cl_event event = 0;
	void *ptr = impl->bindings->EnqueueMapBuffer(impl->handle, buffer.impl->handle, CL_FALSE, flags, offset, size, wait_list.size(), wait_list_ids.empty() ? 0 : &wait_list_ids[0], &event, &result);
	if (ptr == 0)
		throw Exception("clEnqueueMapBuffer failed");
	ComputeEvent mapped_event(new ComputeEvent_Impl(impl->bindings, event));
	return ComputeMemoryMap(new ComputeMemoryMap_Impl(ptr, size, *this, buffer, mapped_event));
}

ComputeEvent ComputeCommandQueue::unmap(ComputeBuffer &buffer, void *mapped_ptr, const ComputeWaitList &wait_list)
{
	std::vector<cl_event> wait_list_ids = impl->get_event_ids(wait_list);
	cl_event event = 0;
	cl_int result = impl->bindings->EnqueueUnmapMemObject(impl->handle, buffer.impl->handle, mapped_ptr, wait_list.size(), wait_list_ids.empty() ? 0 : &wait_list_ids[0], &event);
	if (result != CL_SUCCESS)
		throw Exception("clEnqueueUnmapMemObject failed");
	return ComputeEvent(new ComputeEvent_Impl(impl->bindings, event));
}

ComputeEvent ComputeCommandQueue::migrate_buffers(const std::vector<ComputeBuffer> &buffers, bool migrate_to_host, bool content_undefined, const ComputeWaitList &wait_list)
{
	std::vector<cl_event> wait_list_ids = impl->get_event_ids(wait_list);
	std::vector<cl_mem> buffer_ids = impl->get_buffer_ids(buffers);
	cl_mem_migration_flags flags = 0;
	if (migrate_to_host)
		flags |= CL_MIGRATE_MEM_OBJECT_HOST;
	if (content_undefined)
		flags |= CL_MIGRATE_MEM_OBJECT_CONTENT_UNDEFINED;
	cl_event event = 0;
	cl_int result = impl->bindings->EnqueueMigrateMemObjects(impl->handle, buffer_ids.size(), buffers.empty() ? 0 : &buffer_ids[0], flags, wait_list.size(), wait_list_ids.empty() ? 0 : &wait_list_ids[0], &event);
	if (result != CL_SUCCESS)
		throw Exception("clEnqueueMigrateMemObjects failed");
	return ComputeEvent(new ComputeEvent_Impl(impl->bindings, event));
}

ComputeEvent ComputeCommandQueue::acquire_display_objects(const std::vector<ComputeBuffer> &buffers, const ComputeWaitList &wait_list)
{
	GL3GraphicContextProvider *gl_provider = dynamic_cast<GL3GraphicContextProvider *>(impl->cc.impl->gc.get_provider());
#ifdef WIN32
	D3DGraphicContextProvider *d3d_provider = dynamic_cast<D3DGraphicContextProvider *>(impl->cc.impl->gc.get_provider());
#endif
	if (gl_provider)
	{
		OpenGL::set_active(impl->cc.impl->gc);
		glFlush();

		std::vector<cl_event> wait_list_ids = impl->get_event_ids(wait_list);
		std::vector<cl_mem> buffer_ids = impl->get_buffer_ids(buffers);
		cl_event event = 0;
		cl_int result = impl->bindings->EnqueueAcquireGLObjects(impl->handle, buffer_ids.size(), buffers.empty() ? 0 : &buffer_ids[0], wait_list.size(), wait_list_ids.empty() ? 0 : &wait_list_ids[0], &event);
		if (result != CL_SUCCESS)
			throw Exception("clEnqueueAcquireGLObjects failed");
		return ComputeEvent(new ComputeEvent_Impl(impl->bindings, event));
	}
#ifdef WIN32
	else if (d3d_provider)
	{
		std::vector<cl_event> wait_list_ids = impl->get_event_ids(wait_list);
		std::vector<cl_mem> buffer_ids = impl->get_buffer_ids(buffers);
		cl_event event = 0;
		cl_int result = impl->bindings->EnqueueAcquireD3D11ObjectsKHR(impl->handle, buffer_ids.size(), buffers.empty() ? 0 : &buffer_ids[0], wait_list.size(), wait_list_ids.empty() ? 0 : &wait_list_ids[0], &event);
		if (result != CL_SUCCESS)
			throw Exception("clEnqueueAcquireD3D11ObjectsKHR failed");
		return ComputeEvent(new ComputeEvent_Impl(impl->bindings, event));
	}
#endif
	else
	{
		throw Exception("Unsupported display target");
	}
}

ComputeEvent ComputeCommandQueue::release_display_objects(const std::vector<ComputeBuffer> &buffers, const ComputeWaitList &wait_list)
{
	GL3GraphicContextProvider *gl_provider = dynamic_cast<GL3GraphicContextProvider *>(impl->cc.impl->gc.get_provider());
#ifdef WIN32
	D3DGraphicContextProvider *d3d_provider = dynamic_cast<D3DGraphicContextProvider *>(impl->cc.impl->gc.get_provider());
#endif
	if (gl_provider)
	{
		std::vector<cl_event> wait_list_ids = impl->get_event_ids(wait_list);
		std::vector<cl_mem> buffer_ids = impl->get_buffer_ids(buffers);
		cl_event event = 0;
		cl_int result = impl->bindings->EnqueueReleaseGLObjects(impl->handle, buffer_ids.size(), buffers.empty() ? 0 : &buffer_ids[0], wait_list.size(), wait_list_ids.empty() ? 0 : &wait_list_ids[0], &event);
		if (result != CL_SUCCESS)
			throw Exception("clEnqueueReleaseGLObjects failed");
		return ComputeEvent(new ComputeEvent_Impl(impl->bindings, event));
	}
#ifdef WIN32
	else if (d3d_provider)
	{
		std::vector<cl_event> wait_list_ids = impl->get_event_ids(wait_list);
		std::vector<cl_mem> buffer_ids = impl->get_buffer_ids(buffers);
		cl_event event = 0;
		cl_int result = impl->bindings->EnqueueReleaseD3D11ObjectsKHR(impl->handle, buffer_ids.size(), buffers.empty() ? 0 : &buffer_ids[0], wait_list.size(), wait_list_ids.empty() ? 0 : &wait_list_ids[0], &event);
		if (result != CL_SUCCESS)
			throw Exception("clEnqueueReleaseD3D11ObjectsKHR failed");
		return ComputeEvent(new ComputeEvent_Impl(impl->bindings, event));
	}
#endif
	else
	{
		throw Exception("Unsupported display target");
	}
}

}