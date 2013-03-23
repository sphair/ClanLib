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
#include "pixel_triangle_renderer.h"
#include "API/SWRender/blit_argb8_sse.h"

namespace clan
{

PixelTriangleRenderer::PixelTriangleRenderer()
: dest(0), dest_width(0), dest_height(0), src(0), src_width(0), src_height(0), x(0), y(0), tx(0), ty(0), red(0), blue(0), green(0), alpha(0), core(0), num_cores(1)
{
}

void PixelTriangleRenderer::set_vertex_arrays(float *new_x, float *new_y, float *new_tx, float *new_ty, float *new_red, float *new_green, float *new_blue, float *new_alpha)
{
	x = new_x;
	y = new_y;
	tx = new_tx;
	ty = new_ty;
	red = new_red;
	green = new_green;
	blue = new_blue;
	alpha = new_alpha;
}

void PixelTriangleRenderer::set_clip_rect(const Rect &new_clip_rect)
{
	clip_rect = new_clip_rect;
}

void PixelTriangleRenderer::set_dest(unsigned int *data, int width, int height)
{
	dest = data;
	dest_width = width;
	dest_height = height;
}

void PixelTriangleRenderer::set_src(unsigned int *data, int width, int height)
{
	src = data;
	src_width = width;
	src_height = height;
}

void PixelTriangleRenderer::set_core(int new_core, int new_num_cores)
{
	core = new_core;
	num_cores = new_num_cores;
}

void PixelTriangleRenderer::set_blend_function(BlendFunc src, BlendFunc dest, BlendFunc src_alpha, BlendFunc dest_alpha)
{
}

void PixelTriangleRenderer::render_nearest(unsigned int v1, unsigned int v2, unsigned int v3)
{
	sort_triangle_vertices(v1, v2, v3);

	int start_y = (int)(floor(y[v1]+0.5f));
	int middle_y = ((int)(floor(y[v2]-0.5f)))+1;
	int end_y = ((int)(floor(y[v3]-0.5f)))+1;

	start_y = cl_max(cl_min(start_y, clip_rect.bottom), clip_rect.top);
	middle_y = cl_max(cl_min(middle_y, clip_rect.bottom), clip_rect.top);
	end_y = cl_max(cl_min(end_y, clip_rect.bottom), clip_rect.top);


	// Band for the area covered by v1 to v2
	for (int y = start_y; y < middle_y; y++)
	{
		if (y%num_cores == core)
		{
			LinePoint p0, p1;
			get_left_line_x(v1, v2, y, p0);
			get_right_line_x(v1, v3, y, p1);
			render_scanline_nearest(y, p0, p1);
		}
	}

	// Band for the area covered by v2 to v3
	for (int y = middle_y; y < end_y; y++)
	{
		if (y%num_cores == core)
		{
			LinePoint p0, p1;
			get_left_line_x(v2, v3, y, p0);
			get_right_line_x(v1, v3, y, p1);
			render_scanline_nearest(y, p0, p1);
		}
	}
}

void PixelTriangleRenderer::render_linear(unsigned int v1, unsigned int v2, unsigned int v3)
{
	sort_triangle_vertices(v1, v2, v3);

	int start_y = (int)(floor(y[v1]+0.5f));
	int middle_y = ((int)(floor(y[v2]-0.5f)))+1;
	int end_y = ((int)(floor(y[v3]-0.5f)))+1;

	start_y = cl_max(cl_min(start_y, clip_rect.bottom), clip_rect.top);
	middle_y = cl_max(cl_min(middle_y, clip_rect.bottom), clip_rect.top);
	end_y = cl_max(cl_min(end_y, clip_rect.bottom), clip_rect.top);

	// Band for the area covered by v1 to v2
	for (int y = start_y; y < middle_y; y++)
	{
		if (y%num_cores == core)
		{
			LinePoint p0, p1;
			get_left_line_x(v1, v2, y, p0);
			get_right_line_x(v1, v3, y, p1);
			render_scanline_linear(y, p0, p1);
		}
	}

	// Band for the area covered by v2 to v3
	for (int y = middle_y; y < end_y; y++)
	{
		if (y%num_cores == core)
		{
			LinePoint p0, p1;
			get_left_line_x(v2, v3, y, p0);
			get_right_line_x(v1, v3, y, p1);
			render_scanline_linear(y, p0, p1);
		}
	}
}

void PixelTriangleRenderer::get_left_line_x(unsigned int v1, unsigned int v2, unsigned int yposi, LinePoint &out_point)
{
	get_line_x(v1, v2, v1, yposi, out_point);
}

void PixelTriangleRenderer::get_right_line_x(unsigned int v1, unsigned int v2, unsigned int yposi, LinePoint &out_point)
{
	get_line_x(v1, v2, v2, yposi, out_point);
}

void PixelTriangleRenderer::get_line_x(unsigned int v1, unsigned int v2, unsigned int horz_v, unsigned int yposi, LinePoint &out_point)
{
	if (y[v1] == y[v2]) // Horizontal line
	{
		out_point.x = x[horz_v];
		out_point.tx = tx[horz_v];
		out_point.ty = ty[horz_v];
		out_point.r = red[horz_v];
		out_point.g = green[horz_v];
		out_point.b = blue[horz_v];
		out_point.a = alpha[horz_v];
	}
	else
	{
		float ypos = yposi+0.5f;
		float dy = y[v2]-y[v1];
		float slope_x = (x[v2]-x[v1])/dy;
		float slope_tx = (tx[v2]-tx[v1])/dy;
		float slope_ty = (ty[v2]-ty[v1])/dy;
		float slope_r = (red[v2]-red[v1])/dy;
		float slope_g = (green[v2]-green[v1])/dy;
		float slope_b = (blue[v2]-blue[v1])/dy;
		float slope_a = (alpha[v2]-alpha[v1])/dy;
		float t = ypos-y[v1];
		out_point.x = x[v1]+slope_x*t;
		out_point.tx = tx[v1]+slope_tx*t;
		out_point.ty = ty[v1]+slope_ty*t;
		out_point.r = red[v1]+slope_r*t;
		out_point.g = green[v1]+slope_g*t;
		out_point.b = blue[v1]+slope_b*t;
		out_point.a = alpha[v1]+slope_a*t;
	}
}

void PixelTriangleRenderer::render_scanline_nearest(int y, const LinePoint &p0, const LinePoint &p1)
{
	ScanLine scanline;
	if (prepare_scanline(y, p0, p1, scanline))
	{
		scanline.cur_tx *= src_width;
		scanline.cur_ty *= src_height;
		scanline.slope_tx *= src_width;
		scanline.slope_ty *= src_height;

		int icur_tx = (int)(scanline.cur_tx*65536);
		int icur_ty = (int)(scanline.cur_ty*65536);
		int icur_r = (int)(scanline.cur_r*65536);
		int icur_g = (int)(scanline.cur_g*65536);
		int icur_b = (int)(scanline.cur_b*65536);
		int icur_a = (int)(scanline.cur_a*65536);
		int islope_tx = (int)(scanline.slope_tx*65536);
		int islope_ty = (int)(scanline.slope_ty*65536);
		int islope_r = (int)(scanline.slope_r*65536);
		int islope_g = (int)(scanline.slope_g*65536);
		int islope_b = (int)(scanline.slope_b*65536);
		int islope_a = (int)(scanline.slope_a*65536);

		unsigned int *dest_line = dest+y*dest_width+scanline.start_x;
		int length = scanline.end_x-scanline.start_x;

		__m128i one, half;
		BlitARGB8SSE::set_one(one);
		BlitARGB8SSE::set_half(half);

		__m128i tx = _mm_set_epi32(icur_tx, icur_tx+islope_tx, icur_tx+islope_tx*2, icur_tx+islope_tx*3);
		__m128i ty = _mm_set_epi32(icur_ty, icur_ty+islope_ty, icur_ty+islope_ty*2, icur_ty+islope_ty*3);
		__m128i color = _mm_set_epi32(icur_a, icur_r, icur_g, icur_b);
		__m128i inc_tx = _mm_set1_epi32(islope_tx*4);
		__m128i inc_ty = _mm_set1_epi32(islope_ty*4);
		__m128i inc_color = _mm_set_epi32(islope_a, islope_r, islope_g, islope_b);
		__m128i src_width16 = _mm_set1_epi32(src_width<<16);
		__m128i src_height16 = _mm_set1_epi32(src_height<<16);

		int sse_length = length/4;
		sse_length *= 4;
		for (int x = 0; x <sse_length; x+=4)
		{
			cl_blitargb8sse_texture_repeat(tx, ty, src_width16, src_height16);

			__m128i p4src, p4dest;
			cl_blitargb8sse_sample_nearest(p4src, tx, ty, src, src_width);
			p4dest = _mm_loadu_si128((__m128i*)(dest_line+x));

			__m128i color0 = color;
			__m128i color1 = _mm_add_epi32(color0, inc_color);
			__m128i color2 = _mm_add_epi32(color1, inc_color);
			__m128i color3 = _mm_add_epi32(color2, inc_color);
			color = _mm_add_epi32(color3, inc_color);

			__m128i src0, dest0, tmp_color;
			src0 = _mm_unpacklo_epi8(p4src, _mm_setzero_si128());
			dest0 = _mm_unpacklo_epi8(p4dest, _mm_setzero_si128());
			tmp_color = _mm_packs_epi32(_mm_srai_epi32(color0, 8), _mm_srai_epi32(color1, 8));
			cl_blitargb8sse_multiply_color(src0, tmp_color);
			cl_blitargb8sse_blend_normal(dest0, src0, one, half);

			__m128i src1, dest1;
			src1 = _mm_unpackhi_epi8(p4src, _mm_setzero_si128());
			dest1 = _mm_unpackhi_epi8(p4dest, _mm_setzero_si128());
			tmp_color = _mm_packs_epi32(_mm_srai_epi32(color2, 8), _mm_srai_epi32(color3, 8));
			cl_blitargb8sse_multiply_color(src1, tmp_color);
			cl_blitargb8sse_blend_normal(dest1, src1, one, half);

			p4dest = _mm_packus_epi16(dest0, dest1);
			_mm_storeu_si128((__m128i*)(dest_line+x), p4dest);

			tx = _mm_add_epi32(tx, inc_tx);
			ty = _mm_add_epi32(ty, inc_ty);
		}

		if (sse_length != length)
		{
			unsigned int dest_last[4] = { 0,0,0,0 };
			for (int x = sse_length; x < length; x++)
				dest_last[x-sse_length] = dest_line[x];

			cl_blitargb8sse_texture_repeat(tx, ty, src_width16, src_height16);

			__m128i p4src, p4dest;
			cl_blitargb8sse_sample_nearest(p4src, tx, ty, src, src_width);
			p4dest = _mm_loadu_si128((__m128i*)dest_last);

			__m128i color0 = color;
			__m128i color1 = _mm_add_epi32(color0, inc_color);
			__m128i color2 = _mm_add_epi32(color1, inc_color);
			__m128i color3 = _mm_add_epi32(color2, inc_color);
			color = _mm_add_epi32(color3, inc_color);

			__m128i src0, dest0;
			src0 = _mm_unpacklo_epi8(p4src, _mm_setzero_si128());
			dest0 = _mm_unpacklo_epi8(p4dest, _mm_setzero_si128());
			BlitARGB8SSE::multiply_color(src0, _mm_packs_epi32(_mm_srai_epi32(color0, 8), _mm_srai_epi32(color1, 8)));
			BlitARGB8SSE::blend_normal(dest0, src0, one, half);

			__m128i src1, dest1;
			src1 = _mm_unpackhi_epi8(p4src, _mm_setzero_si128());
			dest1 = _mm_unpackhi_epi8(p4dest, _mm_setzero_si128());
			BlitARGB8SSE::multiply_color(src1, _mm_packs_epi32(_mm_srai_epi32(color2, 8), _mm_srai_epi32(color3, 8)));
			BlitARGB8SSE::blend_normal(dest1, src1, one, half);

			p4dest = _mm_packus_epi16(dest0, dest1);
			_mm_storeu_si128((__m128i*) dest_last, p4dest);

			tx = _mm_add_epi32(tx, inc_tx);
			ty = _mm_add_epi32(ty, inc_ty);

			for (int x = sse_length; x < length; x++)
				dest_line[x] = dest_last[x-sse_length];
		}
	}
}

void PixelTriangleRenderer::render_scanline_linear(int y, const LinePoint &p0, const LinePoint &p1)
{
	ScanLine scanline;
	if (prepare_scanline(y, p0, p1, scanline))
	{
		scanline.cur_tx *= src_width;
		scanline.cur_ty *= src_height;
		scanline.slope_tx *= src_width;
		scanline.slope_ty *= src_height;

		int icur_tx = (int)(scanline.cur_tx*65536);
		int icur_ty = (int)(scanline.cur_ty*65536);
		int icur_r = (int)(scanline.cur_r*65536);
		int icur_g = (int)(scanline.cur_g*65536);
		int icur_b = (int)(scanline.cur_b*65536);
		int icur_a = (int)(scanline.cur_a*65536);
		int islope_tx = (int)(scanline.slope_tx*65536);
		int islope_ty = (int)(scanline.slope_ty*65536);
		int islope_r = (int)(scanline.slope_r*65536);
		int islope_g = (int)(scanline.slope_g*65536);
		int islope_b = (int)(scanline.slope_b*65536);
		int islope_a = (int)(scanline.slope_a*65536);

		unsigned int *dest_line = dest+y*dest_width+scanline.start_x;
		int length = scanline.end_x-scanline.start_x;

		__m128i one, half;
		BlitARGB8SSE::set_one(one);
		BlitARGB8SSE::set_half(half);

		int src_width16 = src_width<<16;
		int src_height16 = src_height<<16;

		for (int x = 0; x <length; x++)
		{
			while (icur_tx < 0)
				icur_tx += src_width16;
			while (icur_tx >= src_width16)
				icur_tx -= src_width16;
			while (icur_ty < 0)
				icur_ty += src_height16;
			while (icur_ty >= src_height16)
				icur_ty -= src_height16;

			int sx0 = icur_tx>>16;
			int sy0 = icur_ty>>16;
			unsigned int ifracx = ((unsigned int)icur_tx)&0xffff;
			unsigned int ifracy = ((unsigned int)icur_ty)&0xffff;

			unsigned int r0, g0, b0, a0;
			r0 = icur_r>>8;
			g0 = icur_g>>8;
			b0 = icur_b>>8;
			a0 = icur_a>>8;
			icur_tx += islope_tx;
			icur_ty += islope_ty;
			icur_r += islope_r;
			icur_g += islope_g;
			icur_b += islope_b;
			icur_a += islope_a;

			__m128i src0, dest0, primcolor;
			int offset = sy0*src_width+sx0;
			BlitARGB8SSE::load_pixel_linear(src0, src[offset], src[offset+1], src[offset+src_width], src[offset+1+src_width], ifracx, ifracy);
			BlitARGB8SSE::load_pixel(dest0, dest_line[x]);
			BlitARGB8SSE::set_color(primcolor, r0, g0, b0, a0);
			BlitARGB8SSE::multiply_color(src0, primcolor);
			BlitARGB8SSE::blend_normal(dest0, src0, one, half);
			BlitARGB8SSE::store_pixel(dest_line[x], dest0);
		}
	}
}

bool PixelTriangleRenderer::prepare_scanline(int y, const LinePoint &p0, const LinePoint &p1, ScanLine &out_scanline)
{
	if (p0.x < p1.x)
	{
		prepare_scanline2(y, p0, p1, out_scanline);
		return true;
	}
	else if (p1.x < p0.x)
	{
		prepare_scanline2(y, p1, p0, out_scanline);
		return true;
	}
	else
	{
		return false;
	}
}

void PixelTriangleRenderer::prepare_scanline2(int y, const LinePoint &p0, const LinePoint &p1, ScanLine &out_scanline)
{
	float dx = p1.x-p0.x;
	out_scanline.slope_tx = (p1.tx-p0.tx)/dx;
	out_scanline.slope_ty = (p1.ty-p0.ty)/dx;
	out_scanline.slope_r = (p1.r-p0.r)/dx;
	out_scanline.slope_g = (p1.g-p0.g)/dx;
	out_scanline.slope_b = (p1.b-p0.b)/dx;
	out_scanline.slope_a = (p1.a-p0.a)/dx;
	out_scanline.start_x = (int)floor(p0.x+0.5f);
	out_scanline.end_x = ((int)floor(p1.x-0.5f))+1;
	out_scanline.start_x = cl_max(clip_rect.left, out_scanline.start_x);
	out_scanline.end_x = cl_min(clip_rect.right, out_scanline.end_x);
	float offset = out_scanline.start_x+0.5f-p0.x;
	out_scanline.cur_tx = p0.tx + offset*out_scanline.slope_tx;
	out_scanline.cur_ty = p0.ty + offset*out_scanline.slope_ty;
	out_scanline.cur_r = p0.r + offset*out_scanline.slope_r;
	out_scanline.cur_g = p0.g + offset*out_scanline.slope_g;
	out_scanline.cur_b = p0.b + offset*out_scanline.slope_b;
	out_scanline.cur_a = p0.a + offset*out_scanline.slope_a;
}

void PixelTriangleRenderer::sort_triangle_vertices(unsigned int &v1, unsigned int &v2, unsigned int &v3)
{
	if ((y[v1] <= y[v2]) && (y[v2] <= y[v3]))
	{
		// (v1 < v2 < v3)
	}
	else if (y[v1] <= y[v3] && (y[v3] <= y[v2]))
	{
		// (v1 < v3 < v2)
		unsigned int t = v3;
		v3 = v2;
		v2 = t;
	}
	else if ((y[v2] <= y[v3]) && (y[v3] <= y[v1]))
	{
		// (v2 < v3 < v1)
		unsigned int t = v1;
		v1 = v2;
		v2 = v3;
		v3 = t;
	}
	else if ((y[v2] <= y[v1]) && (y[v1] <= y[v3]))
	{
		// (v2 < v1 < v3)
		unsigned int t = v1;
		v1 = v2;
		v2 = t;
	}
	else if ((y[v3] <= y[v2]) && (y[v2] <= y[v1]))
	{
		// (v3 < v2 < v1)
		unsigned int t = v1;
		v1 = v3;
		v3 = t;
	}
	else
	{
		// (v3 < v1 < v2)
		unsigned int t = v1;
		v1 = v3;
		v3 = v2;
		v2 = t;
	}
}

}
