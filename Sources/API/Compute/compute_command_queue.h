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

/// \addtogroup clanCompute_Compute clanCompute Compute
/// \{

#pragma once

#include "api_compute.h"
#include <memory>
#include "../Display/Image/buffer_usage.h"
#include "compute_event.h"
#include "compute_wait_list.h"

namespace clan
{

class ComputeContext;
class ComputeBuffer;
class ComputeKernel;
class ComputeMemoryMap;
class ComputeCommandQueue_Impl;

/// \brief Compute command queue
///
/// \xmlonly !group=Display/Display! !header=display.h! \endxmlonly
class CL_API_COMPUTE ComputeCommandQueue
{
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	ComputeCommandQueue();

	/// \brief Constructs a command queue.
	ComputeCommandQueue(ComputeContext &context, bool enable_out_of_order_execute = true, bool enable_profiling = false);

	~ComputeCommandQueue();
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;
/// \}

/// \name Operations
/// \{
public:
	ComputeEvent run_task(const ComputeKernel &kernel, const ComputeWaitList &wait_list = ComputeWaitList());

	ComputeEvent run_1d(
		const ComputeKernel &kernel,
		size_t global_work_size,
		const ComputeWaitList &wait_list = ComputeWaitList())
	{
		return run_1d(kernel, global_work_size, 0, 0, wait_list);
	}

	ComputeEvent run_1d(
		const ComputeKernel &kernel,
		size_t global_work_size,
		size_t local_work_size,
		const ComputeWaitList &wait_list = ComputeWaitList())
	{
		return run_1d(kernel, global_work_size, local_work_size, 0, wait_list);
	}

	ComputeEvent run_1d(
		const ComputeKernel &kernel,
		size_t global_work_size,
		size_t local_work_size,
		int offset,
		const ComputeWaitList &wait_list = ComputeWaitList());

	ComputeEvent run_2d(
		const ComputeKernel &kernel,
		size_t global_work_size_x,
		size_t global_work_size_y,
		const ComputeWaitList &wait_list = ComputeWaitList())
	{
		return run_2d(kernel, global_work_size_x, global_work_size_y, 0, 0, 0, 0, wait_list);
	}

	ComputeEvent run_2d(
		const ComputeKernel &kernel,
		size_t global_work_size_x,
		size_t global_work_size_y,
		size_t local_work_size_x,
		size_t local_work_size_y,
		const ComputeWaitList &wait_list = ComputeWaitList())
	{
		return run_2d(kernel, global_work_size_x, global_work_size_y, local_work_size_x, local_work_size_y, 0, 0, wait_list);
	}

	ComputeEvent run_2d(
		const ComputeKernel &kernel,
		size_t global_work_size_x,
		size_t global_work_size_y,
		size_t local_work_size_x,
		size_t local_work_size_y,
		int offset_x,
		int offset_y,
		const ComputeWaitList &wait_list = ComputeWaitList());

	ComputeEvent run_3d(
		const ComputeKernel &kernel,
		size_t global_work_size_x,
		size_t global_work_size_y,
		size_t global_work_size_z,
		const ComputeWaitList &wait_list = ComputeWaitList())
	{
		return run_3d(kernel, global_work_size_x, global_work_size_y, global_work_size_z, 0, 0, 0, 0, 0, 0, wait_list);
	}

	ComputeEvent run_3d(
		const ComputeKernel &kernel,
		size_t global_work_size_x,
		size_t global_work_size_y,
		size_t global_work_size_z,
		size_t local_work_size_x,
		size_t local_work_size_y,
		size_t local_work_size_z,
		const ComputeWaitList &wait_list = ComputeWaitList())
	{
		return run_3d(kernel, global_work_size_x, global_work_size_y, global_work_size_z, local_work_size_x, local_work_size_y, local_work_size_z, 0, 0, 0, wait_list);
	}

	ComputeEvent run_3d(
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
		const ComputeWaitList &wait_list = ComputeWaitList());

	ComputeEvent add_barrier(const ComputeWaitList &wait_list = ComputeWaitList());
	ComputeEvent add_marker(const ComputeWaitList &wait_list = ComputeWaitList());

	ComputeEvent read_buffer(ComputeBuffer &buffer, bool blocking_read, size_t offset, size_t size, void *ptr, const ComputeWaitList &wait_list = ComputeWaitList());
	ComputeEvent write_buffer(ComputeBuffer &buffer, bool blocking_write, size_t offset, size_t size, void *ptr, const ComputeWaitList &wait_list = ComputeWaitList());
	//CL_ComputeEvent read_buffer_rect(ComputeBuffer &buffer);
	//CL_ComputeEvent write_buffer_rect(ComputeBuffer &buffer);
	ComputeEvent fill_buffer(ComputeBuffer &buffer, const void *pattern, size_t pattern_size, size_t offset, size_t size, const ComputeWaitList &wait_list = ComputeWaitList());
	ComputeEvent copy_buffer(ComputeBuffer &src, ComputeBuffer &dest, size_t src_offset, size_t dest_offset, size_t size, const ComputeWaitList &wait_list = ComputeWaitList());
	//CL_ComputeEvent copy_buffer_rect(ComputeBuffer &src, ComputeBuffer &dest);

	//CL_ComputeEvent read_image(ComputeBuffer &buffer);
	//CL_ComputeEvent write_image(ComputeBuffer &buffer);
	//CL_ComputeEvent fill_image(ComputeBuffer &buffer);
	//CL_ComputeEvent copy_image(ComputeBuffer &buffer);
	//CL_ComputeEvent copy_image_to_buffer(ComputeBuffer &buffer);
	//CL_ComputeEvent copy_buffer_to_image(ComputeBuffer &buffer);

	ComputeMemoryMap map_buffer(ComputeBuffer &buffer, BufferAccess access, size_t offset, size_t size, const ComputeWaitList &wait_list = ComputeWaitList());
	//CL_ComputeMemoryMap map_image(ComputeBuffer &buffer);

	ComputeEvent migrate_buffers(const std::vector<ComputeBuffer> &buffers, bool migrate_to_host = false, bool content_undefined = false, const ComputeWaitList &wait_list = ComputeWaitList());

	ComputeEvent acquire_display_objects(const std::vector<ComputeBuffer> &buffers, const ComputeWaitList &wait_list = ComputeWaitList());
	ComputeEvent release_display_objects(const std::vector<ComputeBuffer> &buffers, const ComputeWaitList &wait_list = ComputeWaitList());

	void flush();
	void finish();
/// \}

/// \name Implementation
/// \{
private:
	ComputeEvent unmap(ComputeBuffer &buffer, void *mapped_ptr, const ComputeWaitList &wait_list = ComputeWaitList());

	std::shared_ptr<ComputeCommandQueue_Impl> impl;
	friend class ComputeMemoryMap_Impl;
/// \}
};

}

/// \}
