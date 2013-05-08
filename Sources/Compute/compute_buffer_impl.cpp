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
#include "API/Compute/compute_context.h"
#include "compute_buffer_impl.h"
#include "compute_context_impl.h"

namespace clan
{

ComputeBuffer_Impl::ComputeBuffer_Impl(ComputeContext &context)
: bindings(context.impl->bindings), handle(0)
{
}

ComputeBuffer_Impl::ComputeBuffer_Impl(ComputeContext &context, int size, BufferAccess access, bool alloc_host_pointer)
: bindings(context.impl->bindings), handle(0)
{
	cl_mem_flags flags = to_mem_flags(access);
	if (alloc_host_pointer)
		flags |= CL_MEM_ALLOC_HOST_PTR;

	cl_int errcode = 0;
	handle = bindings->CreateBuffer(context.impl->handle, flags, size, 0, &errcode);
	if (handle == 0)
		throw Exception("clCreateBuffer failed");
}

ComputeBuffer_Impl::~ComputeBuffer_Impl()
{
	if (bindings && handle)
		bindings->ReleaseMemObject(handle);
}

cl_mem_flags ComputeBuffer_Impl::to_mem_flags(BufferAccess access)
{
	cl_mem_flags flags = 0;
	switch (access)
	{
	case access_read_only: flags = CL_MEM_READ_ONLY; break;
	case access_write_only: flags = CL_MEM_WRITE_ONLY; break;
	case access_write_discard: flags = CL_MEM_WRITE_ONLY; break;
	case access_read_write: flags = CL_MEM_READ_WRITE; break;
	}
	return flags;
}

}
