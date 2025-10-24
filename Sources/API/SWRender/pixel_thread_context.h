/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

/// \addtogroup clanSWRender_Display clanSWRender Display
/// \{

#pragma once

#include "api_swrender.h"
#include "pixel_buffer_data.h"
#include "../Display/Render/blend_mode.h"
#include "api_swrender.h"

/// \brief Thread specific rendering data for pixel commands
///
/// \xmlonly !group=SWRender/Display! !header=swrender.h! \endxmlonly
class CL_API_SWRender CL_PixelThreadContext
{
//!Construction
public:
	CL_PixelThreadContext(int core, int num_cores);

//!Attributes
public:
	int core;
	int num_cores;

	CL_PixelBufferData colorbuffer0;
	CL_Rect clip_rect;

	enum { max_samplers = 6 };
	CL_PixelBufferData samplers[max_samplers];
	CL_PixelBuffer pixelbuffer_white;

	CL_BlendFunc cur_blend_src;
	CL_BlendFunc cur_blend_dest;
	CL_BlendFunc cur_blend_src_alpha;
	CL_BlendFunc cur_blend_dest_alpha;
};

/// \}
