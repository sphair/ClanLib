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
#include "API/SWRender/pixel_thread_context.h"

namespace clan
{

CL_PixelThreadContext::CL_PixelThreadContext(int core, int num_cores)
: core(core),
  num_cores(num_cores),
  cur_blend_src(cl_blend_src_alpha),
  cur_blend_dest(cl_blend_one_minus_src_alpha),
  cur_blend_src_alpha(cl_blend_one), 
  cur_blend_dest_alpha(cl_blend_one_minus_src_alpha)
{
	unsigned int white = 0xffffffff;
	pixelbuffer_white = CL_PixelBuffer(1, 1, cl_argb8, &white);
	for (int i=0; i<max_samplers; i++)
		samplers[i].set(pixelbuffer_white);
}

}
