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
#include "pixel_command_sprite.h"
#include "API/SWRender/blit_argb8_sse.h"
#include "API/SWRender/pixel_thread_context.h"
#include "../Renderers/pixel_fill_renderer.h"
#include "../Renderers/pixel_triangle_renderer.h"
#include "API/Display/2D/color.h"

namespace clan
{

PixelCommandSprite::PixelCommandSprite(const Vec2f init_points[3], const Vec4f init_primcolor, const Vec2f init_texcoords[3], int init_sampler)
{
	for (int i = 0; i < 3; i++)
	{
		points[i] = init_points[i];
		texcoords[i] = init_texcoords[i];
	}
	primcolor = init_primcolor;
	sampler = init_sampler;
}

void PixelCommandSprite::run(PixelThreadContext *context)
{
	if (sampler != 4)
	{
		// Check for non-rotated sprite
		if (   ( (int) points[0].y == (int) (points[1].y) )
			&& ( (int) points[0].x == (int) (points[2].x) ) )
		{
			render_sprite(context);
		}
		else
		{
			render_sprite_rotated(context);
		}
	}
	else
	{
		Rect dest = get_dest_rect(context);
		Colorf color(primcolor.r, primcolor.g, primcolor.b, primcolor.a);

		if (dest.left < dest.right && dest.top < dest.bottom)
		{
			PixelFillRenderer fill_renderer;
			fill_renderer.set_dest(context->colorbuffer0.data, context->colorbuffer0.size.width, context->colorbuffer0.size.height);
			fill_renderer.set_clip_rect(context->clip_rect);
			fill_renderer.set_core(context->core, context->num_cores);
			fill_renderer.fill_rect(dest, color);
		}
	}
}
void PixelCommandSprite::render_sprite_rotated(PixelThreadContext *context)
{
	float x[3] = { points[0].x, points[1].x, points[2].x };
	float y[3] = { points[0].y, points[1].y, points[2].y };
	float tx[3] = { texcoords[0].s, texcoords[1].s, texcoords[2].s };
	float ty[3] = { texcoords[0].t, texcoords[1].t, texcoords[2].t };
	float red[3] = { primcolor.r, primcolor.r, primcolor.r };
	float green[3] = { primcolor.g, primcolor.g, primcolor.g };
	float blue[3] = { primcolor.b, primcolor.b, primcolor.b };
	float alpha[3] = { primcolor.a, primcolor.a, primcolor.a };
	
	PixelTriangleRenderer triangle_renderer;
	triangle_renderer.set_clip_rect(context->clip_rect);
	triangle_renderer.set_vertex_arrays(x,y,tx,ty,red,green,blue,alpha);
	triangle_renderer.set_dest(context->colorbuffer0.data, context->colorbuffer0.size.width, context->colorbuffer0.size.height);
	triangle_renderer.set_src(context->samplers[sampler].data, context->samplers[sampler].size.width, context->samplers[sampler].size.height);
	triangle_renderer.set_core(context->core, context->num_cores);
	triangle_renderer.set_blend_function(context->cur_blend_src, context->cur_blend_dest, context->cur_blend_src_alpha, context->cur_blend_dest_alpha);
	triangle_renderer.render_nearest(0, 1, 2);

	x[0] = points[1].x;
	y[0] = points[1].y;
	x[1] = points[1].x + (points[2].x - points[0].x);
	y[1] = points[1].y + (points[2].y - points[0].y);
	x[2] = points[2].x;
	y[2] = points[2].y;

	tx[0] = texcoords[1].x;
	ty[0] = texcoords[1].y;
	tx[1] = texcoords[1].x + (texcoords[2].x - texcoords[0].x);
	ty[1] = texcoords[1].y + (texcoords[2].y - texcoords[0].y);
	tx[2] = texcoords[2].x;
	ty[2] = texcoords[2].y;
	triangle_renderer.render_nearest(0, 1, 2);
}

void PixelCommandSprite::render_sprite(PixelThreadContext *context)
{
	Rect box = get_dest_rect(context);
	if (box.left < box.right && box.top < box.bottom)
	{
		float dx = (texcoords[1].x-texcoords[0].x)/(points[1].x-points[0].x);
		bool scale = (dx < 0.999f || dx > 1.001f);
		bool white = (primcolor == Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
		if (context->cur_blend_src == blend_constant_color)
		{
			if (scale)
				render_glyph_scale(context, box);
			else
				render_glyph_noscale(context, box);
		}
		else
		{
			if (scale)
				render_sprite_scale(context, box);
			else if (white)
				render_sprite_noscale_white(context, box);
			else
				render_sprite_noscale(context, box);

		}
	}
}

void PixelCommandSprite::render_sprite_scale_linear(PixelThreadContext *context, const Rect &box)
{
	Scanline scanline;
	scanline.src = context->samplers[sampler].data;
	scanline.src_width = context->samplers[sampler].size.width;
	scanline.src_height = context->samplers[sampler].size.height;
	scanline.startx = box.left;
	scanline.endx = box.right;
	scanline.x1 = points[0].x;
	scanline.x2 = points[1].x;
	scanline.tx1 = texcoords[0].x;
	scanline.tx2 = texcoords[1].x;

	int skip_lines = find_first_line_for_core(box.top, context->core, context->num_cores)-box.top;
	for (int y = box.top + skip_lines; y < box.bottom; y+=context->num_cores)
	{
		float t = y+0.5f-points[0].y;
		float ty = texcoords[0].y+(texcoords[2].y-texcoords[0].y)/(points[2].y-points[0].y)*t;
		scanline.ty1 = ty;
		scanline.ty2 = ty;

		scanline.dest = context->colorbuffer0.data + context->colorbuffer0.size.width * y;
		render_linear_scanline(&scanline);
	}
}

void PixelCommandSprite::render_sprite_scale(PixelThreadContext *context, const Rect &box)
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
	int sse_width = width / 2 * 2;

	__m128i one, half, color;
	BlitARGB8SSE::set_one(one);
	BlitARGB8SSE::set_half(half);
	BlitARGB8SSE::set_color(
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
			BlitARGB8SSE::load_pixels(spixel, src_line[tx0>>15], src_line[tx1>>15]);
			BlitARGB8SSE::load_pixels(dpixel, dest+i);
			BlitARGB8SSE::multiply_color(spixel, color);
			BlitARGB8SSE::blend_normal(dpixel, spixel, one, half);
			BlitARGB8SSE::store_pixels(dest+i, dpixel);
		}

		if (i != width)
		{
			__m128i spixel, dpixel;
			BlitARGB8SSE::load_pixel(spixel, src_line[tx>>15]);
			BlitARGB8SSE::load_pixel(dpixel, dest[i]);
			BlitARGB8SSE::multiply_color(spixel, color);
			BlitARGB8SSE::blend_normal(dpixel, spixel, one, half);
			BlitARGB8SSE::store_pixel(dest[i], dpixel);
		}

		ty += dty;
	}
}

void PixelCommandSprite::render_sprite_noscale_white(PixelThreadContext *context, const Rect &box)
{
	float dx = (texcoords[1].x-texcoords[0].x)/(points[1].x-points[0].x);
	float dy = (texcoords[2].y-texcoords[0].y)/(points[2].y-points[0].y);
	float tx_left = texcoords[0].x + dx*(box.left+0.5f-points[0].x);
	float ty_top = texcoords[0].y + dy*(box.top+0.5f-points[0].y);
	int dty = (int)(dy*context->samplers[sampler].size.height * 32768);

	int start_tx = (int)(tx_left*context->samplers[sampler].size.width * 32768);
	int ty = (int)(ty_top*context->samplers[sampler].size.height * 32768);
	int skip_lines = find_first_line_for_core(box.top, context->core, context->num_cores)-box.top;
	ty += dty * skip_lines;
	dty *= context->num_cores;

	int width = box.get_width();
	int sse_width = width / 2 * 2;

	__m128i one, half;
	BlitARGB8SSE::set_one(one);
	BlitARGB8SSE::set_half(half);

	for (int y = box.top + skip_lines; y < box.bottom; y+=context->num_cores)
	{
		int tx = start_tx >> 15;

		unsigned int *src_line = context->samplers[sampler].data + (ty>>15) * context->samplers[sampler].size.width + tx;
		unsigned int *dest = context->colorbuffer0.data + y * context->colorbuffer0.size.width + box.left;
		int i;
		for (i = 0; i < sse_width; i+=2)
		{
			__m128i spixel, dpixel;
			BlitARGB8SSE::load_pixels(spixel, src_line+i);
			BlitARGB8SSE::load_pixels(dpixel, dest+i);
			BlitARGB8SSE::blend_normal(dpixel, spixel, one, half);
			BlitARGB8SSE::store_pixels(dest+i, dpixel);
		}

		if (i != width)
		{
			__m128i spixel, dpixel;
			BlitARGB8SSE::load_pixel(spixel, src_line[i]);
			BlitARGB8SSE::load_pixel(dpixel, dest[i]);
			BlitARGB8SSE::blend_normal(dpixel, spixel, one, half);
			BlitARGB8SSE::store_pixel(dest[i], dpixel);
		}

		ty += dty;
	}
}

void PixelCommandSprite::render_sprite_noscale(PixelThreadContext *context, const Rect &box)
{
	float dx = (texcoords[1].x-texcoords[0].x)/(points[1].x-points[0].x);
	float dy = (texcoords[2].y-texcoords[0].y)/(points[2].y-points[0].y);
	float tx_left = texcoords[0].x + dx*(box.left+0.5f-points[0].x);
	float ty_top = texcoords[0].y + dy*(box.top+0.5f-points[0].y);
	int dty = (int)(dy*context->samplers[sampler].size.height * 32768);

	int start_tx = (int)(tx_left*context->samplers[sampler].size.width * 32768);
	int ty = (int)(ty_top*context->samplers[sampler].size.height * 32768);
	int skip_lines = find_first_line_for_core(box.top, context->core, context->num_cores)-box.top;
	ty += dty * skip_lines;
	dty *= context->num_cores;

	int width = box.get_width();
	int sse_width = width / 2 * 2;

	__m128i one, half, color;
	BlitARGB8SSE::set_one(one);
	BlitARGB8SSE::set_half(half);
	BlitARGB8SSE::set_color(
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
			BlitARGB8SSE::load_pixels(spixel, src_line+i);
			BlitARGB8SSE::load_pixels(dpixel, dest+i);
			BlitARGB8SSE::multiply_color(spixel, color);
			BlitARGB8SSE::blend_normal(dpixel, spixel, one, half);
			BlitARGB8SSE::store_pixels(dest+i, dpixel);
		}

		if (i != width)
		{
			__m128i spixel, dpixel;
			BlitARGB8SSE::load_pixel(spixel, src_line[i]);
			BlitARGB8SSE::load_pixel(dpixel, dest[i]);
			BlitARGB8SSE::multiply_color(spixel, color);
			BlitARGB8SSE::blend_normal(dpixel, spixel, one, half);
			BlitARGB8SSE::store_pixel(dest[i], dpixel);
		}

		ty += dty;
	}
}

void PixelCommandSprite::render_glyph_scale(PixelThreadContext *context, const Rect &box)
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
	int sse_width = width / 2 * 2;

	__m128i one, half, color;
	BlitARGB8SSE::set_one(one);
	BlitARGB8SSE::set_half(half);
	BlitARGB8SSE::set_color(
		color,
		(int)(context->cur_blend_color.r * 256.0f + 0.5f),
		(int)(context->cur_blend_color.g * 256.0f + 0.5f),
		(int)(context->cur_blend_color.b * 256.0f + 0.5f),
		(int)(context->cur_blend_color.a * 256.0f + 0.5f));

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
			BlitARGB8SSE::load_pixels(spixel, src_line[tx0>>15], src_line[tx1>>15]);
			BlitARGB8SSE::load_pixels(dpixel, dest+i);
			BlitARGB8SSE::blend_lcd(dpixel, spixel, one, half, color);
			BlitARGB8SSE::store_pixels(dest+i, dpixel);
		}

		if (i != width)
		{
			__m128i spixel, dpixel;
			BlitARGB8SSE::load_pixel(spixel, src_line[tx>>15]);
			BlitARGB8SSE::load_pixel(dpixel, dest[i]);
			BlitARGB8SSE::blend_lcd(dpixel, spixel, one, half, color);
			BlitARGB8SSE::store_pixel(dest[i], dpixel);
		}

		ty += dty;
	}
}

void PixelCommandSprite::render_glyph_noscale(PixelThreadContext *context, const Rect &box)
{
	float dx = (texcoords[1].x-texcoords[0].x)/(points[1].x-points[0].x);
	float dy = (texcoords[2].y-texcoords[0].y)/(points[2].y-points[0].y);
	float tx_left = texcoords[0].x + dx*(box.left+0.5f-points[0].x);
	float ty_top = texcoords[0].y + dy*(box.top+0.5f-points[0].y);
	int dty = (int)(dy*context->samplers[sampler].size.height * 32768);

	int start_tx = (int)(tx_left*context->samplers[sampler].size.width * 32768);
	int ty = (int)(ty_top*context->samplers[sampler].size.height * 32768);
	int skip_lines = find_first_line_for_core(box.top, context->core, context->num_cores)-box.top;
	ty += dty * skip_lines;
	dty *= context->num_cores;

	int width = box.get_width();
	int sse_width = width / 2 * 2;

	__m128i one, half, color;
	BlitARGB8SSE::set_one(one);
	BlitARGB8SSE::set_half(half);
	BlitARGB8SSE::set_color(
		color,
		(int)(context->cur_blend_color.r * 256.0f + 0.5f),
		(int)(context->cur_blend_color.g * 256.0f + 0.5f),
		(int)(context->cur_blend_color.b * 256.0f + 0.5f),
		(int)(context->cur_blend_color.a * 256.0f + 0.5f));

	for (int y = box.top + skip_lines; y < box.bottom; y+=context->num_cores)
	{
		int tx = start_tx >> 15;

		unsigned int *src_line = context->samplers[sampler].data + (ty>>15) * context->samplers[sampler].size.width + tx;
		unsigned int *dest = context->colorbuffer0.data + y * context->colorbuffer0.size.width + box.left;
		int i;
		for (i = 0; i < sse_width; i+=2)
		{
			__m128i spixel, dpixel;
			BlitARGB8SSE::load_pixels(spixel, src_line+i);
			BlitARGB8SSE::load_pixels(dpixel, dest+i);
			BlitARGB8SSE::blend_lcd(dpixel, spixel, one, half, color);
			BlitARGB8SSE::store_pixels(dest+i, dpixel);
		}

		if (i != width)
		{
			__m128i spixel, dpixel;
			BlitARGB8SSE::load_pixel(spixel, src_line[i]);
			BlitARGB8SSE::load_pixel(dpixel, dest[i]);
			BlitARGB8SSE::blend_lcd(dpixel, spixel, one, half, color);
			BlitARGB8SSE::store_pixel(dest[i], dpixel);
		}

		ty += dty;
	}
}


Rect PixelCommandSprite::get_dest_rect(PixelThreadContext *context) const
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

	Rect dest;
	dest.left = (int)(x0 + 0.5f);
	dest.right = (int)(x1 - 0.5f) + 1;
	dest.top = (int)(y0 + 0.5f);
	dest.bottom = (int)(y1 - 0.5f) + 1;

	dest.left = max(min(dest.left, context->clip_rect.right), context->clip_rect.left);
	dest.right = max(min(dest.right, context->clip_rect.right), context->clip_rect.left);
	dest.top = max(min(dest.top, context->clip_rect.bottom), context->clip_rect.top);
	dest.bottom = max(min(dest.bottom, context->clip_rect.bottom), context->clip_rect.top);

	return dest;
}

void PixelCommandSprite::render_linear_scanline(Scanline *d)
{
	unsigned int *dest = d->dest;
	unsigned int *texdata = d->src;
	unsigned int texwidth = d->src_width;
	unsigned int texheight = d->src_height;

	float startoffset = d->startx+0.5f-d->x1;
	float deltax = d->x2-d->x1;
	float deltatx = d->tx2-d->tx1;
	float deltaty = d->ty2-d->ty1;

	int dxx = (int)(deltatx/deltax*32768);
	int dyy = (int)(deltaty/deltax*32768);
	int xx = (int)(d->tx1*32768+startoffset*dxx);
	int yy = (int)(d->ty1*32768+startoffset*dyy);

	xx *= texwidth;
	dxx *= texwidth;
	yy *= texheight;
	dyy *= texheight;

	xx -= 16300;
	yy -= 16300;

	__m128i zero = _mm_setzero_si128();
	__m128i half = _mm_set1_epi16(0x0040);
	__m128i texwidthmax = _mm_set1_epi32(texwidth-1);
	__m128i texheightmax = _mm_set1_epi32(texheight-1);

	__m128i one = _mm_set1_epi16(0x0100);
	__m128i half7f = _mm_set1_epi16(0x007f);

#ifdef WIN32
	__declspec(align(16)) int xx_input[4];
	__declspec(align(16)) int yy_input[4];
#else
	__attribute__ ((aligned (16))) int xx_input[4];
	__attribute__ ((aligned (16))) int yy_input[4];
#endif

	for (int i = 0; i < 4; i++)
	{
		xx_input[i] = xx;
		yy_input[i] = yy;
		xx += dxx;
		yy += dyy;
	}
	dxx *= 4;
	dyy *= 4;

	__m128i tx = _mm_load_si128((__m128i*)xx_input);
	__m128i ty = _mm_load_si128((__m128i*)yy_input);
	__m128i dtx = _mm_set1_epi32(dxx);
	__m128i dty = _mm_set1_epi32(dyy);
	__m128i xmmtexwidth = _mm_set1_epi32(texwidth);

	int length = d->endx - d->startx;
	int sselength = length/4;
	sselength *= 4;

	for (int c = 0; c < sselength; c+=4)
	{
		int x = d->startx + c;

		__m128i a = _mm_and_si128(tx, _mm_set1_epi32(0x7fff));
		__m128i b = _mm_and_si128(ty, _mm_set1_epi32(0x7fff));
		__m128i inv_a = _mm_sub_epi32(_mm_set1_epi32(0x8000), a);
		__m128i inv_b = _mm_sub_epi32(_mm_set1_epi32(0x8000), b);
		__m128i c1 = _mm_srli_epi32(_mm_mulhi_epi16(a, inv_b), 7);
		__m128i c2 = _mm_srli_epi32(_mm_mulhi_epi16(inv_a, b), 7);
		__m128i c3 = _mm_srli_epi32(_mm_mulhi_epi16(a, b), 7);
		__m128i c0 = _mm_sub_epi32(_mm_sub_epi32(_mm_sub_epi32(_mm_set1_epi32(128), c1), c2), c3);

		__m128i tx0 = _mm_srli_epi32(tx, 15);
		__m128i ty0 = _mm_srli_epi32(ty, 15);
		__m128i tx1 = _mm_add_epi32(tx0, _mm_set1_epi32(1));
		__m128i ty1 = _mm_add_epi32(ty0, _mm_set1_epi32(1));

		// wrapClampToEdge:
		tx0 = _mm_max_epi16(tx0, zero);
		ty0 = _mm_max_epi16(ty0, zero);
		tx1 = _mm_max_epi16(tx1, zero);
		ty1 = _mm_max_epi16(ty1, zero);
		tx0 = _mm_min_epi16(tx0, texwidthmax);
		ty0 = _mm_min_epi16(ty0, texheightmax);
		tx1 = _mm_min_epi16(tx1, texwidthmax);
		ty1 = _mm_min_epi16(ty1, texheightmax);

		ty0 = _mm_or_si128(_mm_slli_epi32(_mm_mulhi_epi16(ty0, xmmtexwidth), 16), _mm_mullo_epi16(ty0, xmmtexwidth));
		ty1 = _mm_or_si128(_mm_slli_epi32(_mm_mulhi_epi16(ty1, xmmtexwidth), 16), _mm_mullo_epi16(ty1, xmmtexwidth));



#ifdef WIN32
		__declspec(align(16)) int ioffsetp0[4], ioffsetp1[4], ioffsetp2[4], ioffsetp3[4];
#else
		__attribute__ ((aligned (16))) int ioffsetp0[4], ioffsetp1[4], ioffsetp2[4], ioffsetp3[4];
#endif


		_mm_store_si128((__m128i*)ioffsetp0, _mm_add_epi32(tx0, ty0));
		_mm_store_si128((__m128i*)ioffsetp1, _mm_add_epi32(tx1, ty0));
		_mm_store_si128((__m128i*)ioffsetp2, _mm_add_epi32(tx0, ty1));
		_mm_store_si128((__m128i*)ioffsetp3, _mm_add_epi32(tx1, ty1));

		__m128i p0 = _mm_set_epi32(texdata[ioffsetp0[3]], texdata[ioffsetp0[2]], texdata[ioffsetp0[1]], texdata[ioffsetp0[0]]);
		__m128i p1 = _mm_set_epi32(texdata[ioffsetp1[3]], texdata[ioffsetp1[2]], texdata[ioffsetp1[1]], texdata[ioffsetp1[0]]);
		__m128i p2 = _mm_set_epi32(texdata[ioffsetp2[3]], texdata[ioffsetp2[2]], texdata[ioffsetp2[1]], texdata[ioffsetp2[0]]);
		__m128i p3 = _mm_set_epi32(texdata[ioffsetp3[3]], texdata[ioffsetp3[2]], texdata[ioffsetp3[1]], texdata[ioffsetp3[0]]);

		c0 = _mm_packs_epi32(c0, c0);
		c1 = _mm_packs_epi32(c1, c1);
		c2 = _mm_packs_epi32(c2, c2);
		c3 = _mm_packs_epi32(c3, c3);

		__m128i xmmdest = _mm_loadu_si128((__m128i*)(dest+x));

		__m128i result, tmp0, tmp1;
		tmp0 = _mm_unpacklo_epi8(p0, zero);
		tmp1 = _mm_shufflelo_epi16(c0, _MM_SHUFFLE(0,0,0,0));
		tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(1,1,1,1));
		result = _mm_mullo_epi16(tmp0, tmp1);

		tmp0 = _mm_unpacklo_epi8(p1, zero);
		tmp1 = _mm_shufflelo_epi16(c1, _MM_SHUFFLE(0,0,0,0));
		tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(1,1,1,1));
		result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

		tmp0 = _mm_unpacklo_epi8(p2, zero);
		tmp1 = _mm_shufflelo_epi16(c2, _MM_SHUFFLE(0,0,0,0));
		tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(1,1,1,1));
		result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

		tmp0 = _mm_unpacklo_epi8(p3, zero);
		tmp1 = _mm_shufflelo_epi16(c3, _MM_SHUFFLE(0,0,0,0));
		tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(1,1,1,1));
		result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

		result = _mm_add_epi16(result, half);
		result = _mm_srli_epi16(result, 7);

		__m128i src0, dest0;
		__m128i src_alpha, invsrc_alpha;
		src0 = result;
		dest0 = _mm_unpacklo_epi8(xmmdest, zero);
		src_alpha = _mm_shufflelo_epi16(src0, _MM_SHUFFLE(3,3,3,3));
		src_alpha = _mm_shufflehi_epi16(src_alpha, _MM_SHUFFLE(3,3,3,3));
		invsrc_alpha = _mm_sub_epi16(one, src_alpha);

		src0 = _mm_mullo_epi16(src0, src_alpha);
		dest0 = _mm_mullo_epi16(dest0, invsrc_alpha);
		dest0 = _mm_add_epi16(dest0, src0);
		dest0 = _mm_add_epi16(dest0, half7f);
		dest0 = _mm_srli_epi16(dest0, 8);



		tmp0 = _mm_unpackhi_epi8(p0, zero);
		tmp1 = _mm_shufflelo_epi16(c0, _MM_SHUFFLE(2,2,2,2));
		tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(3,3,3,3));
		result = _mm_mullo_epi16(tmp0, tmp1);

		tmp0 = _mm_unpackhi_epi8(p1, zero);
		tmp1 = _mm_shufflelo_epi16(c1, _MM_SHUFFLE(2,2,2,2));
		tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(3,3,3,3));
		result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

		tmp0 = _mm_unpackhi_epi8(p2, zero);
		tmp1 = _mm_shufflelo_epi16(c2, _MM_SHUFFLE(2,2,2,2));
		tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(3,3,3,3));
		result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

		tmp0 = _mm_unpackhi_epi8(p3, zero);
		tmp1 = _mm_shufflelo_epi16(c3, _MM_SHUFFLE(2,2,2,2));
		tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(3,3,3,3));
		result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

		result = _mm_add_epi16(result, half);
		result = _mm_srli_epi16(result, 7);

		__m128i src1, dest1;
		src1 = result;
		dest1 = _mm_unpackhi_epi8(xmmdest, zero);
		src_alpha = _mm_shufflelo_epi16(src1, _MM_SHUFFLE(3,3,3,3));
		src_alpha = _mm_shufflehi_epi16(src_alpha, _MM_SHUFFLE(3,3,3,3));
		invsrc_alpha = _mm_sub_epi16(one, src_alpha);

		src1 = _mm_mullo_epi16(src1, src_alpha);
		dest1 = _mm_mullo_epi16(dest1, invsrc_alpha);
		dest1 = _mm_add_epi16(dest1, src1);
		dest1 = _mm_add_epi16(dest1, half7f);
		dest1 = _mm_srli_epi16(dest1, 8);

		xmmdest = _mm_packus_epi16(dest0, dest1);
		_mm_storeu_si128((__m128i*)(dest+x), xmmdest);


		tx = _mm_add_epi32(tx, dtx);
		ty = _mm_add_epi32(ty, dty);
	}


	if (sselength < length)
	{
		int x = d->startx + sselength;

		__m128i a = _mm_and_si128(tx, _mm_set1_epi32(0x7fff));
		__m128i b = _mm_and_si128(ty, _mm_set1_epi32(0x7fff));
		__m128i inv_a = _mm_sub_epi32(_mm_set1_epi32(0x8000), a);
		__m128i inv_b = _mm_sub_epi32(_mm_set1_epi32(0x8000), b);
		__m128i c1 = _mm_srli_epi32(_mm_mulhi_epi16(a, inv_b), 7);
		__m128i c2 = _mm_srli_epi32(_mm_mulhi_epi16(inv_a, b), 7);
		__m128i c3 = _mm_srli_epi32(_mm_mulhi_epi16(a, b), 7);
		__m128i c0 = _mm_sub_epi32(_mm_sub_epi32(_mm_sub_epi32(_mm_set1_epi32(128), c1), c2), c3);

		__m128i tx0 = _mm_srli_epi32(tx, 15);
		__m128i ty0 = _mm_srli_epi32(ty, 15);
		__m128i tx1 = _mm_add_epi32(tx0, _mm_set1_epi32(1));
		__m128i ty1 = _mm_add_epi32(ty0, _mm_set1_epi32(1));

		// wrapClampToEdge:
		tx0 = _mm_max_epi16(tx0, zero);
		ty0 = _mm_max_epi16(ty0, zero);
		tx1 = _mm_max_epi16(tx1, zero);
		ty1 = _mm_max_epi16(ty1, zero);
		tx0 = _mm_min_epi16(tx0, texwidthmax);
		ty0 = _mm_min_epi16(ty0, texheightmax);
		tx1 = _mm_min_epi16(tx1, texwidthmax);
		ty1 = _mm_min_epi16(ty1, texheightmax);

		ty0 = _mm_or_si128(_mm_slli_epi32(_mm_mulhi_epi16(ty0, xmmtexwidth), 16), _mm_mullo_epi16(ty0, xmmtexwidth));
		ty1 = _mm_or_si128(_mm_slli_epi32(_mm_mulhi_epi16(ty1, xmmtexwidth), 16), _mm_mullo_epi16(ty1, xmmtexwidth));

#ifdef WIN32
		__declspec(align(16)) int ioffsetp0[4], ioffsetp1[4], ioffsetp2[4], ioffsetp3[4];
#else
		__attribute__ ((aligned (16))) int ioffsetp0[4], ioffsetp1[4], ioffsetp2[4], ioffsetp3[4];
#endif

		_mm_store_si128((__m128i*)ioffsetp0, _mm_add_epi32(tx0, ty0));
		_mm_store_si128((__m128i*)ioffsetp1, _mm_add_epi32(tx1, ty0));
		_mm_store_si128((__m128i*)ioffsetp2, _mm_add_epi32(tx0, ty1));
		_mm_store_si128((__m128i*)ioffsetp3, _mm_add_epi32(tx1, ty1));

		__m128i p0 = _mm_set_epi32(texdata[ioffsetp0[3]], texdata[ioffsetp0[2]], texdata[ioffsetp0[1]], texdata[ioffsetp0[0]]);
		__m128i p1 = _mm_set_epi32(texdata[ioffsetp1[3]], texdata[ioffsetp1[2]], texdata[ioffsetp1[1]], texdata[ioffsetp1[0]]);
		__m128i p2 = _mm_set_epi32(texdata[ioffsetp2[3]], texdata[ioffsetp2[2]], texdata[ioffsetp2[1]], texdata[ioffsetp2[0]]);
		__m128i p3 = _mm_set_epi32(texdata[ioffsetp3[3]], texdata[ioffsetp3[2]], texdata[ioffsetp3[1]], texdata[ioffsetp3[0]]);

		c0 = _mm_packs_epi32(c0, c0);
		c1 = _mm_packs_epi32(c1, c1);
		c2 = _mm_packs_epi32(c2, c2);
		c3 = _mm_packs_epi32(c3, c3);

#ifdef WIN32
		__declspec(align(16)) unsigned int dpixels[4];
#else
		__attribute__ ((aligned (16))) unsigned int dpixels[4];
#endif

		for (int c = 0; c < length-sselength; c++)
			dpixels[c] = dest[x+c];

		__m128i xmmdest = _mm_load_si128((__m128i*)(dpixels));

		__m128i result, tmp0, tmp1;
		tmp0 = _mm_unpacklo_epi8(p0, zero);
		tmp1 = _mm_shufflelo_epi16(c0, _MM_SHUFFLE(0,0,0,0));
		tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(1,1,1,1));
		result = _mm_mullo_epi16(tmp0, tmp1);

		tmp0 = _mm_unpacklo_epi8(p1, zero);
		tmp1 = _mm_shufflelo_epi16(c1, _MM_SHUFFLE(0,0,0,0));
		tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(1,1,1,1));
		result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

		tmp0 = _mm_unpacklo_epi8(p2, zero);
		tmp1 = _mm_shufflelo_epi16(c2, _MM_SHUFFLE(0,0,0,0));
		tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(1,1,1,1));
		result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

		tmp0 = _mm_unpacklo_epi8(p3, zero);
		tmp1 = _mm_shufflelo_epi16(c3, _MM_SHUFFLE(0,0,0,0));
		tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(1,1,1,1));
		result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

		result = _mm_add_epi16(result, half);
		result = _mm_srli_epi16(result, 7);

		__m128i src0, dest0;
		__m128i src_alpha, invsrc_alpha;
		src0 = result;
		dest0 = _mm_unpacklo_epi8(xmmdest, zero);
		src_alpha = _mm_shufflelo_epi16(src0, _MM_SHUFFLE(3,3,3,3));
		src_alpha = _mm_shufflehi_epi16(src_alpha, _MM_SHUFFLE(3,3,3,3));
		invsrc_alpha = _mm_sub_epi16(one, src_alpha);

		src0 = _mm_mullo_epi16(src0, src_alpha);
		dest0 = _mm_mullo_epi16(dest0, invsrc_alpha);
		dest0 = _mm_add_epi16(dest0, src0);
		dest0 = _mm_add_epi16(dest0, half7f);
		dest0 = _mm_srli_epi16(dest0, 8);



		tmp0 = _mm_unpackhi_epi8(p0, zero);
		tmp1 = _mm_shufflelo_epi16(c0, _MM_SHUFFLE(2,2,2,2));
		tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(3,3,3,3));
		result = _mm_mullo_epi16(tmp0, tmp1);

		tmp0 = _mm_unpackhi_epi8(p1, zero);
		tmp1 = _mm_shufflelo_epi16(c1, _MM_SHUFFLE(2,2,2,2));
		tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(3,3,3,3));
		result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

		tmp0 = _mm_unpackhi_epi8(p2, zero);
		tmp1 = _mm_shufflelo_epi16(c2, _MM_SHUFFLE(2,2,2,2));
		tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(3,3,3,3));
		result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

		tmp0 = _mm_unpackhi_epi8(p3, zero);
		tmp1 = _mm_shufflelo_epi16(c3, _MM_SHUFFLE(2,2,2,2));
		tmp1 = _mm_shufflehi_epi16(tmp1, _MM_SHUFFLE(3,3,3,3));
		result = _mm_add_epi16(result, _mm_mullo_epi16(tmp0, tmp1));

		result = _mm_add_epi16(result, half);
		result = _mm_srli_epi16(result, 7);

		__m128i src1, dest1;
		src1 = result;
		dest1 = _mm_unpackhi_epi8(xmmdest, zero);
		src_alpha = _mm_shufflelo_epi16(src1, _MM_SHUFFLE(3,3,3,3));
		src_alpha = _mm_shufflehi_epi16(src_alpha, _MM_SHUFFLE(3,3,3,3));
		invsrc_alpha = _mm_sub_epi16(one, src_alpha);

		src1 = _mm_mullo_epi16(src1, src_alpha);
		dest1 = _mm_mullo_epi16(dest1, invsrc_alpha);
		dest1 = _mm_add_epi16(dest1, src1);
		dest1 = _mm_add_epi16(dest1, half7f);
		dest1 = _mm_srli_epi16(dest1, 8);

		xmmdest = _mm_packus_epi16(dest0, dest1);
		_mm_store_si128((__m128i*)(dpixels), xmmdest);

		for (int c = 0; c < length-sselength; c++)
			dest[x+c] = dpixels[c];
	}
}

}
