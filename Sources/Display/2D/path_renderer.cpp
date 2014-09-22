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

#include "Display/precomp.h"
#include "path_renderer.h"
#include <algorithm>

namespace clan
{
	PathRenderer::PathRenderer(clan::GraphicContext &gc)
	{
		clan::BlendStateDescription blend_desc;
		blend_desc.set_blend_function(clan::blend_one, clan::blend_one_minus_src_alpha, clan::blend_one, clan::blend_one_minus_src_alpha);
		blend_state = clan::BlendState(gc, blend_desc);
	}

	void PathRenderer::set_size(int new_width, int new_height)
	{
		if (width != new_width || height != new_height)
		{
			width = new_width;
			height = new_height;
			scanlines.resize(height * 2);
			mask = clan::PixelBuffer(width, height, clan::tf_r8);
			memset(mask.get_data(), 0, mask.get_height() * mask.get_pitch());
		}
	}

	void PathRenderer::clear()
	{
		for (size_t y = 0; y < scanlines.size(); y++)
		{
			auto &scanline = scanlines[y];
			if (!scanline.edges.empty())
			{
				memset(mask.get_line(y / 2), 0, width);
				scanline.edges.clear();
			}
		}
	}

	void PathRenderer::line(float x0, float y0, float x1, float y1)
	{
		x0 *= 2.0f;
		x1 *= 2.0f;
		y0 *= 2.0f;
		y1 *= 2.0f;

		bool up_direction = y1 < y0;
		float dy = y1 - y0;

		const float epsilon = std::numeric_limits<float>::epsilon();
		if (dy < -epsilon || dy > epsilon)
		{
			int start_y = static_cast<int>(std::floor(clan::min(y0, y1) + 0.5f));
			int end_y = static_cast<int>(std::floor(clan::max(y0, y1) - 0.5f)) + 1;

			start_y = clan::max(start_y, 0);
			end_y = clan::min(end_y, height * 2);

			float rcp_dy = 1.0f / dy;

			for (int y = start_y; y < end_y; y++)
			{
				float ypos = y + 0.5f;
				float x = x0 + (x1 - x0) * (ypos - y0) * rcp_dy;
				scanlines[y].edges.push_back(PathScanlineEdge(x, up_direction));
			}
		}
	}

	void PathRenderer::stroke(clan::Canvas &canvas)
	{
		// https://www.mapbox.com/blog/drawing-antialiased-lines/
	}

	void PathRenderer::fill(RenderBatchBuffer *batch_buffer, clan::Canvas &canvas, PathFillMode mode, const Brush &brush)
	{
		if (scanlines.empty()) return;

		canvas.flush();

		for (size_t y = 0; y < scanlines.size(); y++)
		{
			auto &scanline = scanlines[y];
			std::sort(scanline.edges.begin(), scanline.edges.end(), [](PathScanlineEdge &a, PathScanlineEdge &b) { return a.x < b.x; });

			unsigned char *line = mask.get_line_uint8(y / 2);

			PathRasterRange range(scanline, mode);
			while (true)
			{
				range.next();
				if (!range.found) break;

				int x0 = static_cast<int>(range.x0 + 0.5f);
				int x1 = static_cast<int>(range.x1 - 0.5f) + 1;

				x0 = clan::max(x0, 0);
				x1 = clan::min(x1, width * 2);

				for (int x = x0; x < x1; x++)
				{
					int pixel = line[x / 2];
					pixel = clan::min(pixel + 64, 255);
					line[x / 2] = pixel;
				}
			}
		}

		clan::GraphicContext gc = canvas.get_gc();
		clan::Texture2D texture(gc, mask);
		texture.set_min_filter(clan::filter_nearest);
		texture.set_mag_filter(clan::filter_nearest);

		clan::Vec4f solid_color;

		if (brush.type == BrushType::solid)
		{
			solid_color = clan::Vec4f(brush.color.r, brush.color.g, brush.color.b, brush.color.a);
		}
		else if (brush.type == BrushType::linear)
		{
			solid_color = clan::Vec4f(brush.stops.front().color.r, brush.stops.front().color.g, brush.stops.front().color.b, brush.stops.front().color.a);
		}
		else if (brush.type == BrushType::radial)
		{
			solid_color = clan::Vec4f(1.0f, 1.0f, 0.0f, 1.0f);
		}
		else if (brush.type == BrushType::image)
		{
			solid_color = clan::Vec4f(1.0f, 0.0f, 0.0f, 1.0f);
		}

		std::vector<Vertex> vertices;
		vertices.push_back(Vertex(clan::Vec4f(-1.0f, -1.0f, 0.0f, 1.0f), solid_color, clan::Vec2f(0.0f, 1.0f)));
		vertices.push_back(Vertex(clan::Vec4f(1.0f, -1.0f, 0.0f, 1.0f), solid_color, clan::Vec2f(1.0f, 1.0f)));
		vertices.push_back(Vertex(clan::Vec4f(-1.0f, 1.0f, 0.0f, 1.0f), solid_color, clan::Vec2f(0.0f, 0.0f)));
		vertices.push_back(Vertex(clan::Vec4f(1.0f, -1.0f, 0.0f, 1.0f), solid_color, clan::Vec2f(1.0f, 1.0f)));
		vertices.push_back(Vertex(clan::Vec4f(1.0f, 1.0f, 0.0f, 1.0f), solid_color, clan::Vec2f(1.0f, 0.0f)));
		vertices.push_back(Vertex(clan::Vec4f(-1.0f, 1.0f, 0.0f, 1.0f), solid_color, clan::Vec2f(0.0f, 0.0f)));

		int gpu_index;
		VertexArrayVector<Vertex> gpu_vertices(batch_buffer->get_vertex_buffer(gc, gpu_index));

		if (prim_array[gpu_index].is_null())
		{
			prim_array[gpu_index] = PrimitivesArray(gc);
			prim_array[gpu_index].set_attributes(0, gpu_vertices, cl_offsetof(Vertex, position));
			prim_array[gpu_index].set_attributes(1, gpu_vertices, cl_offsetof(Vertex, color));
			prim_array[gpu_index].set_attributes(2, gpu_vertices, cl_offsetof(Vertex, texcoord));
		}

		gpu_vertices.upload_data(gc, 0, vertices);

		gc.set_blend_state(blend_state);
		gc.set_program_object(program_path);
		gc.set_texture(0, texture);
		gc.draw_primitives(clan::type_triangles, 6, prim_array[gpu_index]);
		gc.reset_texture(0);
		gc.reset_program_object();
		gc.reset_blend_state();
	}

	void PathRenderer::quadratic_bezier(float qcp0_x, float qcp0_y, float qcp1_x, float qcp1_y, float qcp2_x, float qcp2_y)
	{
		// Convert to cubic:
		float cp0_x = qcp0_x;
		float cp0_y = qcp0_y;
		float cp1_x = qcp0_x + 2.0f * (qcp1_x - qcp0_x) / 3.0f;
		float cp1_y = qcp0_y + 2.0f * (qcp1_y - qcp0_y) / 3.0f;
		float cp2_x = qcp1_x + (qcp2_x - qcp1_x) / 3.0f;
		float cp2_y = qcp1_y + (qcp2_y - qcp1_y) / 3.0f;
		float cp3_x = qcp2_x;
		float cp3_y = qcp2_y;
		cubic_bezier(cp0_x, cp0_y, cp1_x, cp1_y, cp2_x, cp2_y, cp3_x, cp3_y);
	}

	void PathRenderer::cubic_bezier(float cp0_x, float cp0_y, float cp1_x, float cp1_y, float cp2_x, float cp2_y, float cp3_x, float cp3_y)
	{
		float estimated_length =
			std::sqrt((cp1_x - cp0_x) * (cp1_x - cp0_x) + (cp1_y - cp0_y) * (cp1_y - cp0_y)) +
			std::sqrt((cp1_x - cp0_x) * (cp1_x - cp0_x) + (cp1_y - cp0_y) * (cp1_y - cp0_y)) +
			std::sqrt((cp1_x - cp0_x) * (cp1_x - cp0_x) + (cp1_y - cp0_y) * (cp1_y - cp0_y));

		float min_segs = 10.0f;
		float segs = estimated_length / 30.0f;
		int steps = (int)std::ceil(std::sqrt(segs * segs * 0.6f + min_segs));
		for (int i = 0; i < steps; i++)
		{
			clan::Pointf sp = point_on_bezier(cp0_x, cp0_y, cp1_x, cp1_y, cp2_x, cp2_y, cp3_x, cp3_y, i / (float)steps);
			clan::Pointf ep = point_on_bezier(cp0_x, cp0_y, cp1_x, cp1_y, cp2_x, cp2_y, cp3_x, cp3_y, (i + 1) / (float)steps);
			line(sp.x, sp.y, ep.x, ep.y);
		}

		// http://ciechanowski.me/blog/2014/02/18/drawing-bezier-curves/
		// http://antigrain.com/research/adaptive_bezier/  (best method, unfortunately GPL example code)
	}

	void PathRenderer::subdivide_bezier(int level, float cp0_x, float cp0_y, float cp1_x, float cp1_y, float cp2_x, float cp2_y, float cp3_x, float cp3_y, float t0, float t1)
	{
		const float split_angle_cos = 0.99f;

		float tc = (t0 + t1) * 0.5f;

		clan::Pointf sp = point_on_bezier(cp0_x, cp0_y, cp1_x, cp1_y, cp2_x, cp2_y, cp3_x, cp3_y, t0);
		clan::Pointf cp = point_on_bezier(cp0_x, cp0_y, cp1_x, cp1_y, cp2_x, cp2_y, cp3_x, cp3_y, tc);
		clan::Pointf ep = point_on_bezier(cp0_x, cp0_y, cp1_x, cp1_y, cp2_x, cp2_y, cp3_x, cp3_y, t1);

		clan::Vec2f sp2cp(cp.x - sp.x, cp.y - sp.y);
		clan::Vec2f cp2ep(ep.x - cp.x, ep.y - cp.y);

		float dot = clan::Vec2f::dot(sp2cp.normalize(), cp2ep.normalize());
		if (dot < split_angle_cos && level < 15)
		{
			subdivide_bezier(level + 1, cp0_x, cp0_y, cp1_x, cp1_y, cp2_x, cp2_y, cp3_x, cp3_y, t0, tc);
			subdivide_bezier(level + 1, cp0_x, cp0_y, cp1_x, cp1_y, cp2_x, cp2_y, cp3_x, cp3_y, tc, t1);
		}
		else
		{
			line(sp.x, sp.y, ep.x, ep.y);
		}
	}

	clan::Pointf PathRenderer::point_on_bezier(float cp0_x, float cp0_y, float cp1_x, float cp1_y, float cp2_x, float cp2_y, float cp3_x, float cp3_y, float t)
	{
		const int num_cp = 4;

		float cp_x[4] = { cp0_x, cp1_x, cp2_x, cp3_x };
		float cp_y[4] = { cp0_y, cp1_y, cp2_y, cp3_y };

		// Perform deCasteljau iterations:
		// (linear interpolate between the control points)
		float a = 1.0f - t;
		float b = t;
		for (int j = num_cp - 1; j > 0; j--)
		{
			for (int i = 0; i < j; i++)
			{
				cp_x[i] = a * cp_x[i] + b * cp_x[i + 1];
				cp_y[i] = a * cp_y[i] + b * cp_y[i + 1];
			}
		}

		return clan::Pointf(cp_x[0], cp_y[0]);
	}

	/////////////////////////////////////////////////////////////////////////////

	PathRasterRange::PathRasterRange(const PathScanline &scanline, PathFillMode mode) : scanline(scanline), mode(mode)
	{
	}

	void PathRasterRange::next()
	{
		if (mode == PathFillMode::alternate)
		{
			if (i + 1 >= scanline.edges.size())
			{
				found = false;
				return;
			}

			x0 = scanline.edges[i].x;
			x1 = scanline.edges[i + 1].x;

			i += 2;
		}
		else
		{
			while (i < scanline.edges.size() && !scanline.edges[i].up_direction) i++;
			if (i == scanline.edges.size())
			{
				found = false;
				return;
			}
			size_t start = i;

			while (i < scanline.edges.size() && scanline.edges[i].up_direction) i++;
			if (i == scanline.edges.size())
			{
				found = false;
				return;
			}
			size_t end = i;

			x0 = scanline.edges[start].x;
			x1 = scanline.edges[end].x;
		}

		found = true;
		return;
	}
}
