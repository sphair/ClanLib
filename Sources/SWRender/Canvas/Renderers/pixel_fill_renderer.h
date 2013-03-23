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

#pragma once

#include "API/Core/Math/rect.h"
#include "API/Display/Render/blend_mode.h"

namespace clan
{

class CL_PixelFillRenderer
{
public:
	CL_PixelFillRenderer();

	void set_clip_rect(const CL_Rect &clip_rect);
	void set_dest(unsigned int *data, int width, int height);
	void set_core(int core, int num_cores);
	void set_blend_function(CL_BlendFunc src, CL_BlendFunc dest, CL_BlendFunc src_alpha, CL_BlendFunc dest_alpha);

	void clear(const CL_Colorf &color);
	void fill_rect(const CL_Rect &dest, const CL_Colorf &primary_color);

private:
	static int find_first_line_for_core(int y_start, int core, int num_cores);

	struct PixelBufferData
	{
		PixelBufferData() : data(0) { }

		CL_Size size;
		unsigned int *data;
	};

	PixelBufferData colorbuffer0;

	CL_Rect clip_rect;
	int core;
	int num_cores;
};

}
