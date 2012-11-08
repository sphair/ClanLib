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
#include "API/Compute/compute_program.h"
#include "compute_program_impl.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Math/cl_math.h"

namespace clan
{

ComputeProgram::ComputeProgram()
{
}

ComputeProgram::ComputeProgram(ComputeContext &cc, const std::string &source_code)
: impl(new ComputeProgram_Impl(cc, source_code))
{
}

ComputeProgram::~ComputeProgram()
{
}

void ComputeProgram::build(const std::string &build_options)
{
	cl_int result = impl->bindings->BuildProgram(impl->handle, 0, 0, build_options.c_str(), 0, 0);
	if (result != CL_SUCCESS)
	{
		cl_uint num_devices = 0;
		result = impl->bindings->GetProgramInfo(impl->handle, CL_PROGRAM_NUM_DEVICES, sizeof(cl_uint), &num_devices, 0);
		if (result != CL_SUCCESS)
			throw Exception("clBuildProgram and clGetProgramInfo failed");
		if (num_devices == 0)
			throw Exception("clBuildProgram failed and program has no devices");

		std::vector<cl_device_id> devices;
		devices.resize(num_devices);
		result = impl->bindings->GetProgramInfo(impl->handle, CL_PROGRAM_DEVICES, sizeof(cl_device_id) * num_devices, &devices[0], 0);
		if (result != CL_SUCCESS)
			throw Exception("clBuildProgram and clGetProgramInfo failed");

		size_t size = 0;
		result = impl->bindings->GetProgramBuildInfo(impl->handle, devices.front(), CL_PROGRAM_BUILD_LOG, 0, 0, &size);
		if (result != CL_SUCCESS)
			throw Exception("clBuildProgram and clGetProgramBuildInfo failed");

		DataBuffer buffer(size);
		result = impl->bindings->GetProgramBuildInfo(impl->handle, devices.front(), CL_PROGRAM_BUILD_LOG, buffer.get_size(), buffer.get_data(), &size);
		if (result != CL_SUCCESS)
			throw Exception("clBuildProgram and clGetProgramBuildInfo failed");
		if (size != 0)
		{
			buffer.set_size(size);
			std::string log(buffer.get_data(), buffer.get_size()-1);
			throw Exception(string_format("clBuildProgram failed: %1", log));
		}
		else
		{
			throw Exception("clBuildProgram failed with empty build log!");
		}
	}
}

void ComputeProgram::throw_if_null() const
{
	if (is_null())
		throw Exception("CL_ComputeProgram is null!");
}

}
