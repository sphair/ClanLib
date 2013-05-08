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
#include "API/Compute/compute_kernel.h"
#include "API/Compute/compute_buffer.h"
#include "API/Compute/compute_sampler.h"
#include "compute_kernel_impl.h"
#include "compute_buffer_impl.h"
#include "compute_sampler_impl.h"

namespace clan
{

ComputeKernel::ComputeKernel()
{
}

ComputeKernel::ComputeKernel(ComputeProgram &program, const std::string &kernel_name)
: impl(new ComputeKernel_Impl(program, kernel_name))
{
}

ComputeKernel::~ComputeKernel()
{
}

void ComputeKernel::throw_if_null() const
{
	if (is_null())
		throw Exception("CL_ComputeKernel is null!");
}

void ComputeKernel::set_arg(int index, const void *data, int size)
{
	cl_int result = impl->bindings->SetKernelArg(impl->handle, index, size, data);
	if (result != CL_SUCCESS)
		throw Exception("clSetKernelArg failed");
}

void ComputeKernel::set_arg_int(int index, int value)
{
	set_arg(index, &value, sizeof(int));
}

void ComputeKernel::set_arg_float(int index, float value)
{
	set_arg(index, &value, sizeof(float));
}

void ComputeKernel::set_arg_vec2f(int index, const Vec2f &value)
{
	set_arg(index, &value, sizeof(Vec2f));
}

void ComputeKernel::set_arg_vec3f(int index, const Vec3f &value)
{
	set_arg(index, &value, sizeof(Vec3f));
}

void ComputeKernel::set_arg_vec4f(int index, const Vec4f &value)
{
	set_arg(index, &value, sizeof(Vec4f));
}

void ComputeKernel::set_arg_buffer(int index, ComputeBuffer &buffer)
{
	cl_mem buffer_handle = buffer.impl->handle;
	set_arg(index, &buffer_handle, sizeof(cl_mem));
}

void ComputeKernel::set_arg_sampler(int index, ComputeSampler &sampler)
{
	cl_sampler sampler_handle = sampler.impl->handle;
	set_arg(index, &sampler_handle, sizeof(cl_sampler));
}

void ComputeKernel::set_arg_null(int index)
{
	set_arg(index, 0, 0);
}

}
