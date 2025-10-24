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

class CL_PixelTriangleRenderer
{
public:
	CL_PixelTriangleRenderer();

	void set_vertex_arrays(float *x, float *y, float *tx, float *ty, float *red, float *green, float *blue, float *alpha);
	void set_clip_rect(const CL_Rect &clip_rect);
	void set_dest(unsigned int *data, int width, int height);
	void set_src(unsigned int *data, int width, int height);
	void set_core(int core, int num_cores);
	void set_blend_function(CL_BlendFunc src, CL_BlendFunc dest, CL_BlendFunc src_alpha, CL_BlendFunc dest_alpha);

	void render_nearest(unsigned int v1, unsigned int v2, unsigned int v3);
	void render_linear(unsigned int v1, unsigned int v2, unsigned int v3);

private:
	struct LinePoint
	{
		float x;
		float tx;
		float ty;
		float r,g,b,a;
	};

	struct ScanLine
	{
		float slope_tx;
		float slope_ty;
		float slope_r;
		float slope_g;
		float slope_b;
		float slope_a;
		int start_x, end_x;
		float cur_tx;
		float cur_ty;
		float cur_r;
		float cur_g;
		float cur_b;
		float cur_a;
	};

	void sort_triangle_vertices(unsigned int &v1, unsigned int &v2, unsigned int &v3);
	void get_left_line_x(unsigned int v1, unsigned int v2, unsigned int y, LinePoint &out_point);
	void get_right_line_x(unsigned int v1, unsigned int v2, unsigned int y, LinePoint &out_point);
	void get_line_x(unsigned int v1, unsigned int v2, unsigned int horz_v, unsigned int y, LinePoint &out_point);
	void render_scanline_nearest(int y, const LinePoint &p0, const LinePoint &p1);
	void render_scanline_linear(int y, const LinePoint &p0, const LinePoint &p1);
	bool prepare_scanline(int y, const LinePoint &p0, const LinePoint &p1, ScanLine &out_scanline);
	void prepare_scanline2(int y, const LinePoint &p0, const LinePoint &p1, ScanLine &out_scanline);

	unsigned int *dest;
	int dest_width;
	int dest_height;

	unsigned int *src;
	int src_width;
	int src_height;

	float *x;
	float *y;
	float *tx;
	float *ty;
	float *red;
	float *green;
	float *blue;
	float *alpha;
	CL_Rect clip_rect;
	int core;
	int num_cores;
};
