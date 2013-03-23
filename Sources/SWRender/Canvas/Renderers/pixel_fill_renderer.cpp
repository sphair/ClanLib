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
#include "pixel_fill_renderer.h"
#include "API/Display/2D/color.h"
#include <emmintrin.h>

namespace clan
{


PixelFillRenderer::PixelFillRenderer()
: core(0), num_cores(1)
{
}

void PixelFillRenderer::set_clip_rect(const Rect &new_clip_rect)
{
	clip_rect = new_clip_rect;
}

void PixelFillRenderer::set_dest(unsigned int *data, int width, int height)
{
	colorbuffer0.data = data;
	colorbuffer0.size = Size(width, height);
}

void PixelFillRenderer::set_core(int new_core, int new_num_cores)
{
	core = new_core;
	num_cores = new_num_cores;
}

void PixelFillRenderer::set_blend_function(BlendFunc src, BlendFunc dest, BlendFunc src_alpha, BlendFunc dest_alpha)
{
}

void PixelFillRenderer::clear(const Colorf &color)
{
	int dest_buffer_width = colorbuffer0.size.width;
	unsigned char *dest_data = (unsigned char *) colorbuffer0.data;

	Color c = color;
	unsigned int color8888 = (c.get_alpha() << 24) + (c.get_red() << 16) + (c.get_green() << 8) + c.get_blue();
	unsigned char *ptr_color8888 = (unsigned char *) &color8888;

	for (int y = find_first_line_for_core(clip_rect.top, core, num_cores); y < clip_rect.bottom; y += num_cores)
	{
		unsigned char *line = dest_data + y * dest_buffer_width * 4 + clip_rect.left * 4;
		unsigned int line_align = ((line) - ((unsigned char *) 0)) & 0xf; // A gcc safe way of obtaining an address
		int pos = 0;
		int length = clip_rect.get_width()*4;

		// Write single bytes until we are byte aligned:
		if (line_align)
		{
			int prefix_length = min(length, (int) (16 - line_align));
			for (; pos < prefix_length; pos++)
				line[pos] = ptr_color8888[pos&0x3];
		}

		// Figure out how our 16 bytes should look like after we applied the alignment:
		unsigned int b0 = ptr_color8888[(pos+0)&0x3];
		unsigned int b1 = ptr_color8888[(pos+1)&0x3];
		unsigned int b2 = ptr_color8888[(pos+2)&0x3];
		unsigned int b3 = ptr_color8888[(pos+3)&0x3];
		__m128i c_sse = _mm_set1_epi32((b3<<24)+(b2<<16)+(b1<<8)+b0);

		// Fill 16 byte aligned:
		int align_length = length-pos-15;
		for (; pos < align_length; pos+=16)
			_mm_stream_si128((__m128i*)(line+pos), c_sse);

		// Fill remaining bytes:
		for (; pos < length; pos++)
			line[pos] = ptr_color8888[pos&0x3];
	}
}

void PixelFillRenderer::fill_rect(const Rect &dest, const Colorf &primary_color)
{
	int dest_buffer_width = colorbuffer0.size.width;
	unsigned int *dest_data = colorbuffer0.data;

	int start_x = max(dest.left, clip_rect.left);
	int end_x = min(dest.right, clip_rect.right);
	int start_y = max(dest.top, clip_rect.top);
	int end_y = min(dest.bottom, clip_rect.bottom);
	if (start_x < end_x && start_y < end_y)
	{
		int dest_y = find_first_line_for_core(start_y, core, num_cores);

		unsigned int *dest_line = dest_data+dest_y*dest_buffer_width+start_x;

		int line_length = end_x-start_x;
		int dest_line_incr = dest_buffer_width * num_cores;

		unsigned int sred = (unsigned int) (primary_color.r*255);
		unsigned int sgreen = (unsigned int) (primary_color.g*255);
		unsigned int sblue = (unsigned int) (primary_color.b*255);
		unsigned int salpha = (unsigned int) (primary_color.a*255);

		if (salpha == 255)
		{
			unsigned int color = (salpha<<24) + (sred<<16) + (sgreen<<8) + sblue;
			while (dest_y < end_y)
			{
				for (int x = 0; x < line_length; x++)
					dest_line[x] = color;

				dest_y += num_cores;
				dest_line += dest_line_incr;
			}
		}
		else
		{
			unsigned int pos_salpha = salpha*256/255;
			unsigned int neg_salpha = 256-salpha;
			while (dest_y < end_y)
			{
				for (int x = 0; x < line_length; x++)
				{
					#define alpha_component(a) (((a)&0xff000000)>>24)
					#define red_component(a) (((a)&0x00ff0000)>>16)
					#define green_component(a) (((a)&0x0000ff00)>>8)
					#define blue_component(a) ((a)&0x000000ff)

					unsigned int dest_color = dest_line[x];
					unsigned int dred = red_component(dest_color);
					unsigned int dgreen = green_component(dest_color);
					unsigned int dblue = blue_component(dest_color);
					unsigned int dalpha = alpha_component(dest_color);

					unsigned red = (dred * neg_salpha + sred * pos_salpha) >> 8;
					unsigned green = (dgreen * neg_salpha + sgreen * pos_salpha) >> 8;
					unsigned blue = (dblue * neg_salpha + sblue * pos_salpha) >> 8;
					unsigned alpha = (dalpha * neg_salpha + salpha * pos_salpha) >> 8;
					dest_line[x] = (alpha<<24) + (red<<16) + (green<<8) + blue;
				}

				dest_y += num_cores;
				dest_line += dest_line_incr;
			}
		}
	}
}

int PixelFillRenderer::find_first_line_for_core(int y_start, int core, int num_cores)
{
	int y = y_start / num_cores;
	y *= num_cores;
	y += core;
	if (y < y_start)
		y += num_cores;
	return y;
}

}
