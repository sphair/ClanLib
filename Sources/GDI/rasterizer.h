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

#pragma once

#include "API/Core/Math/rect.h"

class CL_Rasterizer
{
public:
	CL_Rasterizer();
	~CL_Rasterizer();

	void set_core(int core, int num_cores);
	void set_colorbuffer0(unsigned int *data, int width, int height);
	void set_sampler0(const unsigned int *data, int width, int height);
	void set_clip_rect(const CL_Rect &clip_rect);

	void render_triangle();

	enum
	{
		position_index = 0,
		texcoord_index = 1,
		primcolor_index = 2,

		pipeline_length = 3,
		max_attributes = 16,
		max_varyings = 2,//16,
		max_varying_lines = 64,
		used_varyings = 2
	};

	struct SIMDVector
	{
		union
		{
			float v_float[4];
			int v_int[4];
		};
	};

	typedef SIMDVector Uniform;
	typedef SIMDVector VertexAttribute;
	typedef SIMDVector VertexVarying;
	typedef SIMDVector LineVarying;

	VertexVarying *varyings[max_varyings]; // [pipeline_length];

private:
	LineVarying *line_varyings[max_varyings]; // [max_varying_lines*2];
	CL_Rect clip_rect;

	const unsigned int *sampler0_data;
	int sampler0_width;
	int sampler0_height;

	unsigned int *colorbuffer0_data;
	int colorbuffer0_width;
	int colorbuffer0_height;

	int core, num_cores;

	// VertexAttribute attributes[max_attributes][pipeline_length];
	// SIMDVector proj_modelview_matrix[4];
	// void shade_vertices(unsigned int start, unsigned int end)
	// inline void transform(const SIMDVector *v_in, SIMDVector *v_out) const;

	void render_triangle(unsigned int v1, unsigned int v2, unsigned int v3);
	void sort_triangle_vertices(unsigned int &v1, unsigned int &v2, unsigned int &v3);
	float length2(unsigned int v1, unsigned int v2) const;
	void prepare_line(int start_y, int length, unsigned int v1, unsigned int v2, unsigned int start_line_index);
	void render_polygon_band(unsigned int num_lines);
};
