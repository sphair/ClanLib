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
#include "pixel_command_line.h"
#include "API/SWRender/pixel_thread_context.h"
#include "../Renderers/pixel_line_renderer.h"
#include "API/Core/Math/line_segment.h"
#include "API/Display/2D/color.h"

CL_PixelCommandLine::CL_PixelCommandLine(const CL_Vec2f init_points[2], const CL_Vec4f init_primcolor[2], const CL_Vec2f init_texcoords[2], int init_sampler)
{
	for (int i = 0; i < 2; i++)
	{
		points[i] = init_points[i];
		primcolor[i] = init_primcolor[i];
		texcoords[i] = init_texcoords[i];
	}
	sampler = init_sampler;
}

void CL_PixelCommandLine::run(CL_PixelThreadContext *context)
{
	CL_LineSegment2 line(
		CL_Vec2i(points[0].x, points[0].y),
		CL_Vec2i(points[1].x, points[1].y));

	CL_Colorf color(primcolor[0].r, primcolor[0].g, primcolor[0].b, primcolor[0].a);

	// Textured line is currently not supported - So ignore the samplers

	CL_PixelLineRenderer line_renderer;
	line_renderer.set_clip_rect(context->clip_rect);
	line_renderer.set_dest(context->colorbuffer0.data, context->colorbuffer0.size.width, context->colorbuffer0.size.height);
	line_renderer.set_core(context->core, context->num_cores);
	line_renderer.set_blend_function(context->cur_blend_src, context->cur_blend_dest, context->cur_blend_src_alpha, context->cur_blend_dest_alpha);
	line_renderer.draw_line(line, color);
}
