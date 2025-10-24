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
#include "pixel_command_triangle.h"
#include "API/SWRender/pixel_thread_context.h"
#include "../Renderers/pixel_triangle_renderer.h"

CL_PixelCommandTriangle::CL_PixelCommandTriangle(const CL_Vec2f init_points[3], const CL_Vec4f init_primcolor[3], const CL_Vec2f init_texcoords[3], int init_sampler)
{
	for (int i = 0; i < 3; i++)
	{
		points[i] = init_points[i];
		primcolor[i] = init_primcolor[i];
		texcoords[i] = init_texcoords[i];
	}
	sampler = init_sampler;
}

void CL_PixelCommandTriangle::run(CL_PixelThreadContext *context)
{
	float x[3] = { points[0].x, points[1].x, points[2].x };
	float y[3] = { points[0].y, points[1].y, points[2].y };
	float tx[3] = { texcoords[0].s, texcoords[1].s, texcoords[2].s };
	float ty[3] = { texcoords[0].t, texcoords[1].t, texcoords[2].t };
	float red[3] = { primcolor[0].r, primcolor[1].r, primcolor[2].r };
	float green[3] = { primcolor[0].g, primcolor[1].g, primcolor[2].g };
	float blue[3] = { primcolor[0].b, primcolor[1].b, primcolor[2].b };
	float alpha[3] = { primcolor[0].a, primcolor[1].a, primcolor[2].a };

	CL_PixelTriangleRenderer triangle_renderer;
	triangle_renderer.set_clip_rect(context->clip_rect);
	triangle_renderer.set_vertex_arrays(x,y,tx,ty,red,green,blue,alpha);
	triangle_renderer.set_dest(context->colorbuffer0.data, context->colorbuffer0.size.width, context->colorbuffer0.size.height);
	triangle_renderer.set_src(context->samplers[sampler].data, context->samplers[sampler].size.width, context->samplers[sampler].size.height);
	triangle_renderer.set_core(context->core, context->num_cores);
	triangle_renderer.set_blend_function(context->cur_blend_src, context->cur_blend_dest, context->cur_blend_src_alpha, context->cur_blend_dest_alpha);
	triangle_renderer.render_nearest(0, 1, 2);
}
