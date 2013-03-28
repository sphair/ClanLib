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
#include "opencl_bindings.h"

namespace clan
{

void OpenCLBindings::bind()
{
#ifdef WIN32
	// To do: should we silently ignore errors here and just make zero platforms available?

	if (handle == 0)
		handle = LoadLibrary(L"OpenCL.dll");
	if (handle == 0)
		throw Exception("Unable to load OpenCL.dll");

	GetPlatformIDs = reinterpret_cast<FuncGetPlatformIDs*>(GetProcAddress(handle, "clGetPlatformIDs"));
	GetPlatformInfo = reinterpret_cast<FuncGetPlatformInfo*>(GetProcAddress(handle, "clGetPlatformInfo"));

	if (GetPlatformIDs && GetPlatformInfo)
	{
		cl_uint num_platforms = 0;
		cl_int result = GetPlatformIDs(0, 0, &num_platforms);
		if (result != CL_SUCCESS)
			throw Exception("clGetPlatformIDs failed");

		if (num_platforms == 0)
			return;

		std::vector<cl_platform_id> ids;
		ids.resize(num_platforms);
		result = GetPlatformIDs(num_platforms, &ids[0], &num_platforms);
		if (result != CL_SUCCESS)
			throw Exception("clGetPlatformIDs failed (2)");

		for (size_t i = 0; i < num_platforms; i++)
			platform_bindings[ids[i]].bind(ids[i]);
	}
#else
	throw Exception("OpenCLBindings::bind not implemented yet for this platform");
#endif
}

OpenCLBindings::FuncGetPlatformIDs *OpenCLBindings::GetPlatformIDs = 0;
OpenCLBindings::FuncGetPlatformInfo *OpenCLBindings::GetPlatformInfo = 0;

#ifdef WIN32
HMODULE OpenCLBindings::handle = 0;
#endif
std::map<cl_platform_id, OpenCLPlatformBindings> OpenCLBindings::platform_bindings;

}
