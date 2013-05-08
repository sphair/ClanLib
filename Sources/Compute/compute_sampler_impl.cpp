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
#include "compute_sampler_impl.h"
#include "compute_context_impl.h"

namespace clan
{

ComputeSampler_Impl::ComputeSampler_Impl(ComputeContext &context, bool normalized_coords, SamplerAddressingMode addressing_mode, SamplerFilterMode filter_mode)
: bindings(context.impl->bindings), handle(0)
{
	cl_addressing_mode address = 0;
	cl_filter_mode filter = 0;
	switch (addressing_mode)
	{
	case sampler_address_none: address = CL_ADDRESS_NONE; break;
	case sampler_address_clamp_to_edge: address = CL_ADDRESS_CLAMP_TO_EDGE; break;
	case sampler_address_clamp: address = CL_ADDRESS_CLAMP; break;
	case sampler_address_repeat: address = CL_ADDRESS_REPEAT; break;
	case sampler_address_mirrored_repeat: address = CL_ADDRESS_MIRRORED_REPEAT; break;
	}
	switch (filter_mode)
	{
	case sampler_filter_nearest: filter = CL_FILTER_NEAREST; break;
	case sampler_filter_linear: filter = CL_FILTER_LINEAR; break;
	}

	handle = bindings->CreateSampler(context.impl->handle, normalized_coords ? CL_TRUE : CL_FALSE, address, filter, 0);
	if (handle == 0)
		throw Exception("clCreateSampler failed");
}

ComputeSampler_Impl::~ComputeSampler_Impl()
{
	if (bindings && handle)
		bindings->ReleaseSampler(handle);
}

}
