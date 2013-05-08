/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Mark Page
*/

#include "SWRender/precomp.h"
#include "pixel_line_renderer.h"
#include "API/Display/2D/color.h"
#include "API/Core/Math/cl_math.h"
#include "API/Core/Math/vec2.h"
#include "API/Core/Math/line_segment.h"

namespace clan
{

PixelLineRenderer::PixelLineRenderer()
: dest(0), dest_width(0), dest_height(0), core(0), num_cores(1)
{
}

void PixelLineRenderer::set_clip_rect(const Rect &new_clip_rect)
{
	clip_rect = new_clip_rect;
}

void PixelLineRenderer::set_dest(unsigned int *data, int width, int height)
{
	dest = data;
	dest_width = width;
	dest_height = height;
}

void PixelLineRenderer::set_core(int new_core, int new_num_cores)
{
	core = new_core;
	num_cores = new_num_cores;
}

void PixelLineRenderer::set_blend_function(BlendFunc src, BlendFunc dest, BlendFunc src_alpha, BlendFunc dest_alpha)
{
}

int PixelLineRenderer::find_first_line_for_core(int y_start, int core, int num_cores)
{
	int y = y_start / num_cores;
	y *= num_cores;
	y += core;
	if (y < y_start)
		y += num_cores;
	return y;
}

void PixelLineRenderer::draw_line(const LineSegment2 &line_dest, const Colorf &primary_color)
{
	// Clip the input line
	LineSegment2 line(line_dest);
	bool clipped;
	line.clip(clip_rect, clipped);
	if (!clipped)	// Off screen
		return;

	if (line.p.y > line.q.y)	// We draw top down
	{
		Vec2i t(line.p);
		line.p = line.q;
		line.q = t;
	}

	#define alpha_component(a) (((a)&0xff000000)>>24)
	#define red_component(a) (((a)&0x00ff0000)>>16)
	#define green_component(a) (((a)&0x0000ff00)>>8)
	#define blue_component(a) ((a)&0x000000ff)

	unsigned int sred = (unsigned int) (primary_color.r*255);
	unsigned int sgreen = (unsigned int) (primary_color.g*255);
	unsigned int sblue = (unsigned int) (primary_color.b*255);
	unsigned int salpha = (unsigned int) (primary_color.a*255);

	// Special case - horizontal line
	if (line.p.y == line.q.y)
	{
		int dest_y = find_first_line_for_core(line.p.y, core, num_cores);
		if (dest_y == line.p.y)	// Ensure correct line --- This might not be required
		{
			// Draw left to right
			if (line.p.x > line.q.x)
			{
				// Swap left and right
				int t = line.p.x;
				line.p.x = line.q.x;
				line.q.x = t;
			}

			if(dest_y>=dest_height)
			{
				//abort
				return;
			}

			unsigned int *dest_line = dest+dest_y*dest_width;
			if (salpha == 255)
			{
				unsigned int color = (salpha<<24) + (sred<<16) + (sgreen<<8) + sblue;
				for (int x = line.p.x; x < line.q.x; x++)
				{
					dest_line[x] = color;
				}
			}
			else
			{
				unsigned int pos_salpha = salpha*256/255;
				unsigned int neg_salpha = 256-salpha;

				for (int x = line.p.x; x < line.q.x; x++)
				{
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
			}
		}
	}
	// All other lines
	else
	{
		int dest_y = find_first_line_for_core(line.p.y, core, num_cores);
		unsigned int *dest_line = dest+dest_y*dest_width+line.p.x;

		float line_ratio = ( (float) (line.q.x - line.p.x))  / ( (float) (line.q.y - line.p.y) );

		int dest_line_incr = dest_width * num_cores;

		if (salpha == 255)
		{
			unsigned int color = (salpha<<24) + (sred<<16) + (sgreen<<8) + sblue;
			int length_y = line.q.y - line.p.y;
			for (int y=dest_y - line.p.y; y < length_y; y += num_cores)
			{
				int xstart = (int) (( ((float) y) * line_ratio));
				int xend = (int) (( ((float) (y+1)) * line_ratio));

				if (xstart == xend)	// Disallow gaps
					xend++;

				if (xstart > xend)	// Reverse draw
				{
					int t = xstart;
					xstart = xend;
					xend = t;
				}
				for (int x = xstart; x < xend; x++)
					dest_line[x] = color;

				dest_line += dest_line_incr;
			}
		}
		else
		{
			unsigned int pos_salpha = salpha*256/255;
			unsigned int neg_salpha = 256-salpha;

			int length_y = line.q.y - line.p.y;
			for (int y=dest_y - line.p.y; y < length_y; y += num_cores)
			{
				int xstart = (int) (( ((float) y) * line_ratio));
				int xend = (int) (( ((float) (y+1)) * line_ratio));

				if (xstart == xend)	// Disallow gaps
					xend++;

				if (xstart > xend)	// Reverse draw
				{
					int t = xstart;
					xstart = xend;
					xend = t;
				}

				for (int x = xstart; x < xend; x++)
				{

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

				dest_line += dest_line_incr;
			}

		}
	}
}

}
