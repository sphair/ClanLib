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
#include "pixel_command_pixels.h"
#include "API/SWRender/blit_argb8_sse.h"
#include "API/SWRender/pixel_thread_context.h"

namespace clan
{

CL_PixelCommandPixels::CL_PixelCommandPixels(const CL_Rect &dest_rect, const CL_PixelBuffer &image, const CL_Rect &src_rect, const CL_Colorf &primary_color)
: dest_rect(dest_rect), image(image), src_rect(src_rect), primary_color(primary_color)
{
}

void CL_PixelCommandPixels::run(CL_PixelThreadContext *context)
{
	CL_Rect box = get_clipped_dest_rect(context);
	if (box.get_width() > 0 && box.get_height() > 0)
	{
		if (dest_rect.get_width() != src_rect.get_width())
			render_pixels_scale(context, box);
		else if (primary_color == CL_Colorf::white)
			render_pixels_noscale_white(context, box);
		else
			render_pixels_noscale(context, box);
	}
}

void CL_PixelCommandPixels::render_pixels_scale(CL_PixelThreadContext *context, const CL_Rect &box)
{
	int dtx = src_rect.get_width() * 32768 / dest_rect.get_width();
	int dty = src_rect.get_height() * 32768 / dest_rect.get_height();

	int start_tx = src_rect.left * 32768;
	int ty = src_rect.top * 32768;
	int skip_lines = find_first_line_for_core(box.top, context->core, context->num_cores)-dest_rect.top;
	ty += dty * skip_lines;
	dty *= context->num_cores;

	start_tx += (box.left - dest_rect.left) * dtx;

	int width = box.get_width();
	int sse_width = width / 2 * 2;

	__m128i one, half, color;
	CL_BlitARGB8SSE::set_one(one);
	CL_BlitARGB8SSE::set_half(half);
	CL_BlitARGB8SSE::set_color(
		color,
		(int)(primary_color.r * 256.0f + 0.5f),
		(int)(primary_color.g * 256.0f + 0.5f),
		(int)(primary_color.b * 256.0f + 0.5f),
		(int)(primary_color.a * 256.0f + 0.5f));

	unsigned int *src = (unsigned int *) image.get_data();
	int src_width = image.get_width();

	for (int y = dest_rect.top + skip_lines; y < box.bottom; y+=context->num_cores)
	{
		int tx = start_tx;

		unsigned int *src_line = src + (ty>>15) * src_width;
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

void CL_PixelCommandPixels::render_pixels_noscale(CL_PixelThreadContext *context, const CL_Rect &box)
{
	int dty = src_rect.get_height() * 32768 / dest_rect.get_height();

	int start_tx = src_rect.left;
	int ty = src_rect.top * 32768;
	int skip_lines = find_first_line_for_core(box.top, context->core, context->num_cores)-dest_rect.top;
	ty += dty * skip_lines;
	dty *= context->num_cores;

	start_tx += box.left - dest_rect.left;

	int width = box.get_width();
	int sse_width = width / 2 * 2;

	__m128i one, half, color;
	CL_BlitARGB8SSE::set_one(one);
	CL_BlitARGB8SSE::set_half(half);
	CL_BlitARGB8SSE::set_color(
		color,
		(int)(primary_color.r * 256.0f + 0.5f),
		(int)(primary_color.g * 256.0f + 0.5f),
		(int)(primary_color.b * 256.0f + 0.5f),
		(int)(primary_color.a * 256.0f + 0.5f));

	unsigned int *src = (unsigned int *) image.get_data();
	int src_width = image.get_width();

	for (int y = dest_rect.top + skip_lines; y < box.bottom; y+=context->num_cores)
	{
		unsigned int *src_line = src + (ty>>15) * src_width + start_tx;
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

void CL_PixelCommandPixels::render_pixels_noscale_white(CL_PixelThreadContext *context, const CL_Rect &box)
{
	int dty = src_rect.get_height() * 32768 / dest_rect.get_height();

	int start_tx = src_rect.left;
	int ty = src_rect.top * 32768;
	int skip_lines = find_first_line_for_core(box.top, context->core, context->num_cores)-dest_rect.top;
	ty += dty * skip_lines;
	dty *= context->num_cores;

	start_tx += box.left - dest_rect.left;

	int width = box.get_width();
	int sse_width = width / 2 * 2;

	__m128i one, half;
	CL_BlitARGB8SSE::set_one(one);
	CL_BlitARGB8SSE::set_half(half);

	unsigned int *src = (unsigned int *) image.get_data();
	int src_width = image.get_width();

	for (int y = dest_rect.top + skip_lines; y < box.bottom; y+=context->num_cores)
	{
		unsigned int *src_line = src + (ty>>15) * src_width + start_tx;
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

CL_Rect CL_PixelCommandPixels::get_clipped_dest_rect(CL_PixelThreadContext *context) const
{
	CL_Rect dest = dest_rect;
	dest.left = cl_max(cl_min(dest.left, context->clip_rect.right), context->clip_rect.left);
	dest.right = cl_max(cl_min(dest.right, context->clip_rect.right), context->clip_rect.left);
	dest.top = cl_max(cl_min(dest.top, context->clip_rect.bottom), context->clip_rect.top);
	dest.bottom = cl_max(cl_min(dest.bottom, context->clip_rect.bottom), context->clip_rect.top);
	return dest;
}

}
