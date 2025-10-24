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

#include "SWRender/precomp.h"
#include "pixel_command_sprite.h"
#include "API/SWRender/blit_argb8_sse.h"
#include "API/SWRender/pixel_thread_context.h"
#include "../Renderers/pixel_fill_renderer.h"
#include "API/Display/2D/color.h"

CL_PixelCommandSprite::CL_PixelCommandSprite(const CL_Vec2f init_points[3], const CL_Vec4f init_primcolor, const CL_Vec2f init_texcoords[3], int init_sampler)
{
	for (int i = 0; i < 3; i++)
	{
		points[i] = init_points[i];
		texcoords[i] = init_texcoords[i];
	}
	primcolor = init_primcolor;
	sampler = init_sampler;
}

void CL_PixelCommandSprite::run(CL_PixelThreadContext *context)
{
	if (sampler != 4)
	{
		render_sprite(context);
	}
	else
	{
		CL_Rect dest = get_dest_rect(context);
		CL_Colorf color(primcolor.r, primcolor.g, primcolor.b, primcolor.a);

		if (dest.left < dest.right && dest.top < dest.bottom)
		{
			CL_PixelFillRenderer fill_renderer;
			fill_renderer.set_dest(context->colorbuffer0.data, context->colorbuffer0.size.width, context->colorbuffer0.size.height);
			fill_renderer.set_clip_rect(context->clip_rect);
			fill_renderer.set_core(context->core, context->num_cores);
			fill_renderer.fill_rect(dest, color);
		}
	}
}

void CL_PixelCommandSprite::render_sprite(CL_PixelThreadContext *context)
{
	CL_Rect box = get_dest_rect(context);
	if (box.left < box.right && box.top < box.bottom)
	{
		float dx = (texcoords[1].x-texcoords[0].x)/(points[1].x-points[0].x);
		float dy = (texcoords[2].y-texcoords[0].y)/(points[2].y-points[0].y);
		if (dx < 0.999f || dx > 1.001f)
			render_sprite_scale(context, box);
		else if (primcolor == CL_Vec4f(1.0f, 1.0f, 1.0f, 1.0f))
			render_sprite_noscale_white(context, box);
		else
			render_sprite_noscale(context, box);
	}
}

void CL_PixelCommandSprite::render_sprite_scale(CL_PixelThreadContext *context, const CL_Rect &box)
{
	float dx = (texcoords[1].x-texcoords[0].x)/(points[1].x-points[0].x);
	float dy = (texcoords[2].y-texcoords[0].y)/(points[2].y-points[0].y);
	float tx_left = texcoords[0].x + dx*(box.left+0.5f-points[0].x);
	float ty_top = texcoords[0].y + dy*(box.top+0.5f-points[0].y);
	int dtx = (int)(dx*context->samplers[sampler].size.width * 32768);
	int dty = (int)(dy*context->samplers[sampler].size.height * 32768);

	int start_tx = (int)(tx_left*context->samplers[sampler].size.width * 32768);
	int ty = (int)(ty_top*context->samplers[sampler].size.height * 32768);
	int skip_lines = find_first_line_for_core(box.top, context->core, context->num_cores)-box.top;
	ty += dty * skip_lines;
	dty *= context->num_cores;

	int width = box.get_width();
	int height = box.get_height();
	int sse_width = width / 2 * 2;

	__m128i one, half, color;
	CL_BlitARGB8SSE::set_one(one);
	CL_BlitARGB8SSE::set_half(half);
	CL_BlitARGB8SSE::set_color(
		color,
		(int)(primcolor.r * 256.0f + 0.5f),
		(int)(primcolor.g * 256.0f + 0.5f),
		(int)(primcolor.b * 256.0f + 0.5f),
		(int)(primcolor.a * 256.0f + 0.5f));

	for (int y = box.top + skip_lines; y < box.bottom; y+=context->num_cores)
	{
		int tx = start_tx;

		unsigned int *src_line = context->samplers[sampler].data + (ty>>15) * context->samplers[sampler].size.width;
		unsigned int *dest = context->colorbuffer0.data + y * context->colorbuffer0.size.width + box.left;
		int i;
		for (i = 0; i < sse_width; i+=2)
		{
			int tx0, tx1;
			tx0 = tx;
			tx += dtx;
			tx1 = tx;
			tx += dtx;

			__m128i spixel, dpixel;
			CL_BlitARGB8SSE::load_pixels(spixel, src_line[tx0>>15], src_line[tx1>>15]);
			CL_BlitARGB8SSE::load_pixels(dpixel, dest+i);
			CL_BlitARGB8SSE::multiply_color(spixel, color);
			CL_BlitARGB8SSE::blend_normal(dpixel, spixel, one, half);
			CL_BlitARGB8SSE::store_pixels(dest+i, dpixel);
		}

		if (i != width)
		{
			__m128i spixel, dpixel;
			CL_BlitARGB8SSE::load_pixel(spixel, src_line[tx>>15]);
			CL_BlitARGB8SSE::load_pixel(dpixel, dest[i]);
			CL_BlitARGB8SSE::multiply_color(spixel, color);
			CL_BlitARGB8SSE::blend_normal(dpixel, spixel, one, half);
			CL_BlitARGB8SSE::store_pixel(dest[i], dpixel);
		}

		ty += dty;
	}
}

void CL_PixelCommandSprite::render_sprite_noscale_white(CL_PixelThreadContext *context, const CL_Rect &box)
{
	float dx = (texcoords[1].x-texcoords[0].x)/(points[1].x-points[0].x);
	float dy = (texcoords[2].y-texcoords[0].y)/(points[2].y-points[0].y);
	float tx_left = texcoords[0].x + dx*(box.left+0.5f-points[0].x);
	float ty_top = texcoords[0].y + dy*(box.top+0.5f-points[0].y);
	int dtx = (int)(dx*context->samplers[sampler].size.width * 32768);
	int dty = (int)(dy*context->samplers[sampler].size.height * 32768);

	int start_tx = (int)(tx_left*context->samplers[sampler].size.width * 32768);
	int ty = (int)(ty_top*context->samplers[sampler].size.height * 32768);
	int skip_lines = find_first_line_for_core(box.top, context->core, context->num_cores)-box.top;
	ty += dty * skip_lines;
	dty *= context->num_cores;

	int width = box.get_width();
	int height = box.get_height();
	int sse_width = width / 2 * 2;

	__m128i one, half;
	CL_BlitARGB8SSE::set_one(one);
	CL_BlitARGB8SSE::set_half(half);

	for (int y = box.top + skip_lines; y < box.bottom; y+=context->num_cores)
	{
		int tx = start_tx >> 15;

		unsigned int *src_line = context->samplers[sampler].data + (ty>>15) * context->samplers[sampler].size.width + tx;
		unsigned int *dest = context->colorbuffer0.data + y * context->colorbuffer0.size.width + box.left;
		int i;
		for (i = 0; i < sse_width; i+=2)
		{
			__m128i spixel, dpixel;
			CL_BlitARGB8SSE::load_pixels(spixel, src_line+i);
			CL_BlitARGB8SSE::load_pixels(dpixel, dest+i);
			CL_BlitARGB8SSE::blend_normal(dpixel, spixel, one, half);
			CL_BlitARGB8SSE::store_pixels(dest+i, dpixel);
		}

		if (i != width)
		{
			__m128i spixel, dpixel;
			CL_BlitARGB8SSE::load_pixel(spixel, src_line[i]);
			CL_BlitARGB8SSE::load_pixel(dpixel, dest[i]);
			CL_BlitARGB8SSE::blend_normal(dpixel, spixel, one, half);
			CL_BlitARGB8SSE::store_pixel(dest[i], dpixel);
		}

		ty += dty;
	}
}

void CL_PixelCommandSprite::render_sprite_noscale(CL_PixelThreadContext *context, const CL_Rect &box)
{
	float dx = (texcoords[1].x-texcoords[0].x)/(points[1].x-points[0].x);
	float dy = (texcoords[2].y-texcoords[0].y)/(points[2].y-points[0].y);
	float tx_left = texcoords[0].x + dx*(box.left+0.5f-points[0].x);
	float ty_top = texcoords[0].y + dy*(box.top+0.5f-points[0].y);
	int dtx = (int)(dx*context->samplers[sampler].size.width * 32768);
	int dty = (int)(dy*context->samplers[sampler].size.height * 32768);

	int start_tx = (int)(tx_left*context->samplers[sampler].size.width * 32768);
	int ty = (int)(ty_top*context->samplers[sampler].size.height * 32768);
	int skip_lines = find_first_line_for_core(box.top, context->core, context->num_cores)-box.top;
	ty += dty * skip_lines;
	dty *= context->num_cores;

	int width = box.get_width();
	int height = box.get_height();
	int sse_width = width / 2 * 2;

	__m128i one, half, color;
	CL_BlitARGB8SSE::set_one(one);
	CL_BlitARGB8SSE::set_half(half);
	CL_BlitARGB8SSE::set_color(
		color,
		(int)(primcolor.r * 256.0f + 0.5f),
		(int)(primcolor.g * 256.0f + 0.5f),
		(int)(primcolor.b * 256.0f + 0.5f),
		(int)(primcolor.a * 256.0f + 0.5f));

	for (int y = box.top + skip_lines; y < box.bottom; y+=context->num_cores)
	{
		int tx = start_tx >> 15;

		unsigned int *src_line = context->samplers[sampler].data + (ty>>15) * context->samplers[sampler].size.width + tx;
		unsigned int *dest = context->colorbuffer0.data + y * context->colorbuffer0.size.width + box.left;
		int i;
		for (i = 0; i < sse_width; i+=2)
		{
			__m128i spixel, dpixel;
			CL_BlitARGB8SSE::load_pixels(spixel, src_line+i);
			CL_BlitARGB8SSE::load_pixels(dpixel, dest+i);
			CL_BlitARGB8SSE::multiply_color(spixel, color);
			CL_BlitARGB8SSE::blend_normal(dpixel, spixel, one, half);
			CL_BlitARGB8SSE::store_pixels(dest+i, dpixel);
		}

		if (i != width)
		{
			__m128i spixel, dpixel;
			CL_BlitARGB8SSE::load_pixel(spixel, src_line[i]);
			CL_BlitARGB8SSE::load_pixel(dpixel, dest[i]);
			CL_BlitARGB8SSE::multiply_color(spixel, color);
			CL_BlitARGB8SSE::blend_normal(dpixel, spixel, one, half);
			CL_BlitARGB8SSE::store_pixel(dest[i], dpixel);
		}

		ty += dty;
	}
}

CL_Rect CL_PixelCommandSprite::get_dest_rect(CL_PixelThreadContext *context) const
{
	float x0, x1, y0, y1;
	if (points[0].x <= points[1].x)
	{
		x0 = points[0].x;
		x1 = points[1].x;
	}
	else
	{
		x0 = points[1].x;
		x1 = points[0].x;
	}

	if (points[0].y <= points[2].y)
	{
		y0 = points[0].y;
		y1 = points[2].y;
	}
	else
	{
		y0 = points[2].y;
		y1 = points[0].y;
	}

	CL_Rect dest;
	dest.left = (int)(x0 + 0.5f);
	dest.right = (int)(x1 - 0.5f) + 1;
	dest.top = (int)(y0 + 0.5f);
	dest.bottom = (int)(y1 - 0.5f) + 1;

	dest.left = cl_max(cl_min(dest.left, context->clip_rect.right), context->clip_rect.left);
	dest.right = cl_max(cl_min(dest.right, context->clip_rect.right), context->clip_rect.left);
	dest.top = cl_max(cl_min(dest.top, context->clip_rect.bottom), context->clip_rect.top);
	dest.bottom = cl_max(cl_min(dest.bottom, context->clip_rect.bottom), context->clip_rect.top);

	return dest;
}
