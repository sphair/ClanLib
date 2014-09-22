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

#pragma once

#include <vector>
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/path.h"
#include "API/Display/2D/brush.h"
#include "API/Display/Render/blend_state_description.h"
#include "API/Display/Render/blend_state.h"
#include "API/Display/Render/shader_object.h"
#include "API/Display/Render/texture_2d.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Render/program_object.h"
#include "render_batch_buffer.h"

namespace clan
{
	enum class PathFillMode;
	class Brush;

	class PathScanlineEdge
	{
	public:
		PathScanlineEdge() { }
		PathScanlineEdge(float x, bool up_direction) : x(x), up_direction(up_direction) { }

		float x = 0.0f;
		bool up_direction = false;
	};

	class PathScanline
	{
	public:
		std::vector<PathScanlineEdge> edges;
		std::vector<unsigned char> pixels;
	};

	class PathRenderer
	{
	public:
		PathRenderer(clan::GraphicContext &gc);

		void set_size(int width, int height);

		void clear();
		void line(float x0, float y0, float x1, float y1);
		void quadratic_bezier(float cp0_x, float cp0_y, float cp1_x, float cp1_y, float cp2_x, float cp2_y);
		void cubic_bezier(float cp0_x, float cp0_y, float cp1_x, float cp1_y, float cp2_x, float cp2_y, float cp3_x, float cp3_y);

		void stroke(clan::Canvas &canvas);
		void fill(RenderBatchBuffer *batch_buffer, clan::Canvas &canvas, PathFillMode mode, const Brush &brush);

	private:
		void subdivide_bezier(int level, float cp0_x, float cp0_y, float cp1_x, float cp1_y, float cp2_x, float cp2_y, float cp3_x, float cp3_y, float t0, float t1);
		static clan::Pointf point_on_bezier(float cp0_x, float cp0_y, float cp1_x, float cp1_y, float cp2_x, float cp2_y, float cp3_x, float cp3_y, float t);

		struct Vertex
		{
			Vertex() { }
			Vertex(const clan::Vec4f &position, const clan::Vec4f &color, const clan::Vec2f &texcoord) : position(position), color(color), texcoord(texcoord) { }

			clan::Vec4f position;
			clan::Vec4f color;
			clan::Vec2f texcoord;
		};

		int width = 0;
		int height = 0;
		std::vector<PathScanline> scanlines;
		clan::PixelBuffer mask;
		PrimitivesArray prim_array[RenderBatchBuffer::num_vertex_buffers];
		clan::BlendState blend_state;
	};

	class PathRasterRange
	{
	public:
		PathRasterRange(const PathScanline &scanline, PathFillMode mode);

		void next();

		bool found = false;
		float x0 = 0.0f;
		float x1 = 0.0f;

	private:
		const PathScanline &scanline;
		PathFillMode mode;
		size_t i = 0;
	};
}
