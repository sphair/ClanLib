/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "compute_command_queue_impl.h"
#include "API/Compute/compute_event.h"
#include "API/Compute/compute_buffer.h"
#include "API/Compute/compute_kernel.h"
#include "API/Compute/compute_context.h"
#include "compute_event_impl.h"
#include "compute_buffer_impl.h"
#include "compute_context_impl.h"

namespace clan
{

ComputeCommandQueue_Impl::ComputeCommandQueue_Impl(ComputeContext &context, bool enable_out_of_order_execute, bool enable_profiling)
: cc(context), bindings(context.impl->bindings), handle(0)
{
	size_t num_devices = 0;
	cl_int result = bindings->GetContextInfo(context.impl->handle, CL_CONTEXT_DEVICES, 0, 0, &num_devices);
	num_devices /= sizeof(cl_device_id);
	if (result != CL_SUCCESS)
		throw Exception("clGetContextInfo failed");

	if (num_devices == 0)
		throw Exception("ComputeContext has no devices!");

	std::vector<cl_device_id> ids;
	ids.resize(num_devices);
	result = bindings->GetContextInfo(context.impl->handle, CL_CONTEXT_DEVICES, sizeof(cl_device_id) * num_devices, &ids[0], 0);
	if (result != CL_SUCCESS)
		throw Exception("clGetContextInfo failed (2)");

	cl_command_queue_properties properties = 0;
	if (enable_out_of_order_execute)
		properties |= CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
	if (enable_profiling)
		properties |= CL_QUEUE_PROFILING_ENABLE;

	cl_int errcode = 0;
	handle = bindings->CreateCommandQueue(context.impl->handle, ids.front(), properties, &errcode);
	if (handle == 0)
		throw Exception("clCreateCommandQueue failed");
}

ComputeCommandQueue_Impl::~ComputeCommandQueue_Impl()
{
	if (bindings && handle)
		bindings->ReleaseCommandQueue(handle);
}

std::vector<cl_event> ComputeCommandQueue_Impl::get_event_ids(const std::vector<ComputeEvent> &wait_list)
{
	std::vector<cl_event> wait_list_ids;
	wait_list_ids.reserve(wait_list.size());
	for (size_t i = 0; i < wait_list.size(); i++)
		wait_list_ids.push_back(wait_list[i].impl->handle);
	return wait_list_ids;
}

std::vector<cl_mem> ComputeCommandQueue_Impl::get_buffer_ids(const std::vector<ComputeBuffer> &buffers)
{
	std::vector<cl_mem> buffer_ids;
	buffer_ids.reserve(buffers.size());
	for (size_t i = 0; i < buffers.size(); i++)
		buffer_ids.push_back(buffers[i].impl->handle);
	return buffer_ids;
}

}
