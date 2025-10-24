/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

#include "GDI/precomp.h"
#include "rasterizer.h"
#include "API/Core/IOData/datatypes.h"
#ifndef _MSC_VER
#include <stdlib.h>
#endif
#include <emmintrin.h>
//#include <tmmintrin.h>

CL_Rasterizer::CL_Rasterizer()
: sampler0_data(0), sampler0_width(0), sampler0_height(0), colorbuffer0_data(0),
  colorbuffer0_width(0), colorbuffer0_height(0), core(0), num_cores(1)
{
	for (size_t i = 0; i < max_varyings; i++)
	{
#ifdef _MSC_VER
		varyings[i] = reinterpret_cast<VertexVarying*>(_aligned_malloc(sizeof(VertexVarying)*pipeline_length, 16));
		line_varyings[i] = reinterpret_cast<VertexVarying*>(_aligned_malloc(sizeof(VertexVarying)*max_varying_lines*2, 16));
#else
		if (posix_memalign( (void **) &varyings[i], 16, sizeof(VertexVarying)*pipeline_length))
		{
			throw CL_Exception(cl_text("Panic! posix_memalign failed"));
		}
		if (posix_memalign( (void **) &line_varyings[i], 16, sizeof(VertexVarying)*max_varying_lines*2))
		{
			free(varyings[i]);
			throw CL_Exception(cl_text("Panic! posix_memalign failed"));
		}
#endif
	}
}

CL_Rasterizer::~CL_Rasterizer()
{
	for (size_t i = 0; i < max_varyings; i++)
	{
#ifdef _MSC_VER
		_aligned_free(varyings[i]);
		_aligned_free(line_varyings[i]);
#else
		free(varyings[i]);
		free(line_varyings[i]);
#endif
	}
}

void CL_Rasterizer::set_core(int new_core, int new_num_cores)
{
	core = new_core;
	num_cores = new_num_cores;
}

void CL_Rasterizer::set_colorbuffer0(unsigned int *data, int width, int height)
{
	colorbuffer0_data = data;
	colorbuffer0_width = width;
	colorbuffer0_height = height;
}

void CL_Rasterizer::set_sampler0(const unsigned int *data, int width, int height)
{
	sampler0_data = data;
	sampler0_width = width;
	sampler0_height = height;
}

void CL_Rasterizer::set_clip_rect(const CL_Rect &new_clip_rect)
{
	clip_rect = new_clip_rect;
}

void CL_Rasterizer::render_triangle()
{
	render_triangle(0, 1, 2);
}

/*
void CL_Rasterizer::shade_vertices(unsigned int start, unsigned int end)
{
	for (unsigned int index = start; index < end; index++)
	{
		transform(attributes[position_index]+index, varyings[position_index]+index);
	}

//	__m128 value256 = _mm_set1_ps(256.0f);

	for (unsigned int varying_index = 1; varying_index < used_varyings; varying_index++)
	{
		for (unsigned int index = start; index < end; index++)
		{
			varyings[varying_index][index] = attributes[varying_index][index];
		}
	}
}

inline void CL_Rasterizer::transform(const SIMDVector *v_in, SIMDVector *v_out) const
{
	// to do: multiply vector with the proj_modelview_matrix
	*v_out = *v_in;
}
*/

void CL_Rasterizer::render_triangle(unsigned int v1, unsigned int v2, unsigned int v3)
{
	sort_triangle_vertices(v1, v2, v3);

	const VertexVarying *p1 = varyings[position_index]+v1;
	const VertexVarying *p2 = varyings[position_index]+v2;
	const VertexVarying *p3 = varyings[position_index]+v3;

	int start_y = (int)(varyings[position_index][v1].v_float[1]+0.5f);
	int middle_y = (int)(varyings[position_index][v2].v_float[1]+0.5f);
	int end_y = ((int)(varyings[position_index][v3].v_float[1]-0.5f))+1;

	if (start_y < clip_rect.top)
		start_y = clip_rect.top;
	if (middle_y > clip_rect.bottom)
		middle_y = clip_rect.bottom;
	if (end_y > clip_rect.bottom)
		end_y = clip_rect.bottom;

	// Band for the area covered by v1 to v2
	for (int y = start_y; y < middle_y; y += max_varying_lines)
	{
		int y2 = y + max_varying_lines;
		if (y2 > middle_y)
			y2 = middle_y;
		int lines = y2-y;
		prepare_line(y, lines, v1, v3, 0);
		prepare_line(y, lines, v1, v2, max_varying_lines);
		render_polygon_band(lines);
	}

	// Band for the area covered by v2 to v3
	for (int y = middle_y; y < end_y; y += max_varying_lines)
	{
		int y2 = y + max_varying_lines;
		if (y2 > end_y)
			y2 = end_y;
		int lines = y2-y;
		prepare_line(y, lines, v1, v3, 0);
		prepare_line(y, lines, v2, v3, max_varying_lines);
		render_polygon_band(lines);
	}
}

void CL_Rasterizer::sort_triangle_vertices(unsigned int &v1, unsigned int &v2, unsigned int &v3)
{
	// Sort triangles by finding the longest line
	float len1 = length2(v1, v3);
	float len2 = length2(v1, v2);
	float len3 = length2(v2, v3);
	if (len2 >= len1 && len2 >= len3)
	{
		unsigned int t = v3;
		v3 = v2;
		v2 = t;
	}
	else if (len3 >= len1 && len3 >= len2)
	{
		unsigned int t = v1;
		v1 = v2;
		v2 = t;
	}

	// The v1 to v3 is now the longest line.

	// Now sort by height, so v1 is above v2 and v3:
	if (varyings[position_index][v1].v_float[1] > varyings[position_index][v3].v_float[1])
	{
		unsigned int t = v3;
		v3 = v1;
		v1 = t;
	}
}

float CL_Rasterizer::length2(unsigned int v1, unsigned int v2) const
{
	const VertexVarying *p1 = varyings[position_index]+v1;
	const VertexVarying *p2 = varyings[position_index]+v2;
	float dx = p2->v_float[0]-p1->v_float[0];
	float dy = p2->v_float[1]-p1->v_float[1];
	return dx*dx + dy*dy;
}

void CL_Rasterizer::prepare_line(int start_y, int length, unsigned int v1, unsigned int v2, unsigned int start_line_index)
{
	const VertexVarying *p1 = varyings[position_index]+v1;
	const VertexVarying *p2 = varyings[position_index]+v2;

	__m128 varying_slopes[max_varyings];
	__m128 dy = _mm_set1_ps(p2->v_float[1] - p1->v_float[1]);
	for (unsigned int i=0; i<used_varyings; i++)
	{
		__m128 varying1 = _mm_load_ps(reinterpret_cast<float*>(varyings[i]+v1));
		__m128 varying2 = _mm_load_ps(reinterpret_cast<float*>(varyings[i]+v2));
		__m128 dx1 = _mm_sub_ps(varying2, varying1);
		varying_slopes[i] = _mm_div_ps(dx1, dy);
	}

	unsigned int line_index = start_line_index;
	unsigned int end_line_index = start_line_index+length;
	for (float y = start_y+0.5f; line_index < end_line_index; y++, line_index++)
	{
		__m128 relative = _mm_set1_ps(y-p1->v_float[1]);
		for (unsigned int i=0; i<used_varyings; i++)
		{
			__m128 varying1 = _mm_load_ps(reinterpret_cast<float*>(varyings[i]+v1));
			__m128 xmm = _mm_mul_ps(relative, varying_slopes[i]);
			xmm = _mm_add_ps(xmm, varying1);
			_mm_store_ps(reinterpret_cast<float*>(line_varyings[i]+line_index), xmm);
		}
	}
}

void CL_Rasterizer::render_polygon_band(unsigned int num_lines)
{
	for (unsigned int line_index = 0; line_index < num_lines; line_index++)
	{
		const VertexVarying *p1 = line_varyings[position_index]+line_index;
		const VertexVarying *p2 = p1+max_varying_lines;

		if (p1->v_float[0] > p2->v_float[0])
		{
			p1 = p2;
			p2 = line_varyings[position_index]+line_index;
		}

		int line_start_x = (int)(p1->v_float[0]+0.5f);
		int line_end_x = (int)(p2->v_float[0]-0.5f);
		int y = (int)p1->v_float[1];

		if (line_start_x < clip_rect.left)
			line_start_x = clip_rect.left;
		if (line_end_x > clip_rect.right)
			line_end_x = clip_rect.right;

		if (line_start_x <= line_end_x)
		{
			__m128 varying_cur[max_varyings];
			__m128 varying_incr[max_varyings];
			__m128 dx = _mm_set1_ps(p2->v_float[0] - p1->v_float[0]);
			__m128 relative_start = _mm_set1_ps(p1->v_float[0]-line_start_x+0.5f);
			for (unsigned int i=0; i<used_varyings; i++)
			{
				__m128 varying1 = _mm_load_ps(reinterpret_cast<float*>(line_varyings[i]+line_index));
				__m128 varying2 = _mm_load_ps(reinterpret_cast<float*>(line_varyings[i]+line_index+num_lines));
				__m128 delta = _mm_sub_ps(varying2, varying1);
				varying_incr[i] = _mm_div_ps(delta, dx);
				varying_cur[i] = _mm_add_ps(varying1, _mm_mul_ps(relative_start, varying_incr[i]));
			}

			unsigned int *colorbuffer0_line = colorbuffer0_data + y*colorbuffer0_width;
			for (int x = line_start_x; x <= line_end_x; x++)
			{
/*				int texcoord[2];
				_mm_storel_epi64(reinterpret_cast<__m128i*>(texcoord), _mm_cvttps_epi32(varying_cur[texcoord_index]));
				texcoord[0] = texcoord[0] % sampler0_width;
				texcoord[1] = texcoord[1] % sampler0_height;
				if (texcoord[0] < 0)
					texcoord[0] += sampler0_width;
				if (texcoord[1] < 0)
					texcoord[1] += sampler0_height;
*/
				//const unsigned int *sampler0_pixel = sampler0_data;//+texcoord[0]+texcoord[1]*sampler0_width;
				//colorbuffer0_line[x] = *sampler0_pixel;

				__m128i primary_color = _mm_cvtps_epi32(varying_cur[primcolor_index]);
				cl_int64 result;
				_mm_storel_epi64((__m128i*) &result, primary_color);
				colorbuffer0_line[x] = result;


//				for (unsigned int i=0; i<used_varyings; i++)
//					varying_cur[i] = _mm_add_ps(varying_cur[i], varying_incr[i]);
			}
		}
	}
}
