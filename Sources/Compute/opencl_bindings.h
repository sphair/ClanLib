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

#pragma once

#include "opencl_platform_bindings.h"
#include <map>

namespace clan
{

class OpenCLBindings
{
public:
	static void bind();

	typedef cl_int (CL_API_CALL FuncGetPlatformIDs)(
		cl_uint          /* num_entries */,
		cl_platform_id * /* platforms */,
		cl_uint *        /* num_platforms */);

	typedef cl_int (CL_API_CALL FuncGetPlatformInfo)(
		cl_platform_id   /* platform */, 
		cl_platform_info /* param_name */,
		size_t           /* param_value_size */, 
		void *           /* param_value */,
		size_t *         /* param_value_size_ret */);

	static FuncGetPlatformIDs *GetPlatformIDs;
	static FuncGetPlatformInfo *GetPlatformInfo;

	static HMODULE handle;
	static std::map<cl_platform_id, OpenCLPlatformBindings> platform_bindings;
};

}
