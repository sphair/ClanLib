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
#include "path_fill_renderer.h"
#include "API/Display/Render/texture_1d.h"
#include "API/Display/2D/subtexture.h"
#include <algorithm>

namespace clan
{
	PathFillRenderer::PathFillRenderer(GraphicContext &gc)
	{
		BlendStateDescription blend_desc;
		blend_desc.set_blend_function(blend_one, blend_one_minus_src_alpha, blend_one, blend_one_minus_src_alpha);
		blend_state = BlendState(gc, blend_desc);
	}

	void PathFillRenderer::set_size(int new_width, int new_height)
	{
		if (width != new_width || height != new_height)
		{
			width = new_width;
			height = new_height;
			scanlines.resize(height * 2);
			mask = PixelBuffer(width, height, tf_r8);
			memset(mask.get_data(), 0, mask.get_height() * mask.get_pitch());
		}
	}

	void PathFillRenderer::clear()
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

	void PathFillRenderer::end(bool close)
	{
		if (close)
		{
			line(start_x, start_y);
		}
	}

	void PathFillRenderer::line(float x1, float y1)
	{
		float x0 = last_x;
		float y0 = last_y;

		last_x = x1;
		last_y = y1;

		x0 *= 2.0f;
		x1 *= 2.0f;
		y0 *= 2.0f;
		y1 *= 2.0f;

		bool up_direction = y1 < y0;
		float dy = y1 - y0;

		const float epsilon = std::numeric_limits<float>::epsilon();
		if (dy < -epsilon || dy > epsilon)
		{
			int start_y = static_cast<int>(std::floor(min(y0, y1) + 0.5f));
			int end_y = static_cast<int>(std::floor(max(y0, y1) - 0.5f)) + 1;

			start_y = max(start_y, 0);
			end_y = min(end_y, height * 2);

			float rcp_dy = 1.0f / dy;

			for (int y = start_y; y < end_y; y++)
			{
				float ypos = y + 0.5f;
				float x = x0 + (x1 - x0) * (ypos - y0) * rcp_dy;
				scanlines[y].edges.push_back(PathScanlineEdge(x, up_direction));
			}
		}
	}

	void PathFillRenderer::fill(RenderBatchBuffer *batch_buffer, Canvas &canvas, PathFillMode mode, const Brush &brush, const Mat4f &transform)
	{
		if (scanlines.empty()) return;

		canvas.flush();

		float canvas_width = (float)canvas.get_width();
		float canvas_height = (float)canvas.get_height();

		for (size_t y = 0; y < scanlines.size(); y++)
		{
			auto &scanline = scanlines[y];
			std::sort(scanline.edges.begin(), scanline.edges.end(), [](const PathScanlineEdge &a, const PathScanlineEdge &b) { return a.x < b.x; });

			unsigned char *line = mask.get_line_uint8(y / 2);

			PathRasterRange range(scanline, mode);
			while (true)
			{
				range.next();
				if (!range.found) break;

				int x0 = static_cast<int>(range.x0 + 0.5f);
				int x1 = static_cast<int>(range.x1 - 0.5f) + 1;

				x0 = max(x0, 0);
				x1 = min(x1, width * 2);

				for (int x = x0; x < x1; x++)
				{
					int pixel = line[x / 2];
					pixel = min(pixel + 64, 255);
					line[x / 2] = pixel;
				}
			}
		}

		GraphicContext gc = canvas.get_gc();
		Texture2D texture(gc, mask);
		texture.set_min_filter(filter_nearest);
		texture.set_mag_filter(filter_nearest);

		std::vector<Vertex> vertices;
		Vec4f brush_data1;
		Vec4f brush_data2;
		int draw_mode;

		int num_stops = max(brush.stops.size(), 8);
		PixelBuffer gradient_pixelbuffer(num_stops * 2 * 4, 1, tf_rgba32f);
		Vec4f *gradient_ptr = gradient_pixelbuffer.get_data<Vec4f>();
		for (unsigned int cnt = 0; cnt < brush.stops.size(); cnt++)
		{
			*(gradient_ptr++) = brush.stops[cnt].color;
			*(gradient_ptr++) = Vec4f(brush.stops[cnt].position, 0.0f, 0.0f, 0.0f);
		}

		Texture2D gradient_texture(gc, gradient_pixelbuffer);
		gradient_texture.set_min_filter(filter_nearest);
		gradient_texture.set_mag_filter(filter_nearest);

		if (brush.type == BrushType::linear)
		{
			draw_mode = 1;
			Pointf end_point = path_to_world(brush.end_point, brush, transform);
			Pointf start_point = path_to_world(brush.start_point, brush, transform);
			Pointf dir = end_point - start_point;
			Pointf dir_normed = Pointf::normalize(dir);
			brush_data1.x = start_point.x;
			brush_data1.y = start_point.y;
			brush_data1.z = dir_normed.x;
			brush_data1.w = dir_normed.y;
			brush_data2.x = 1.0f / dir.length();
			brush_data2.y = 0.0f;
			brush_data2.z = brush.stops.size();

			vertices.push_back(Vertex(Vec4f(-1.0f, -1.0f, 0.0f, 0.0f), brush_data1, brush_data2, Vec2f(0.0f, 1.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(1.0f, -1.0f, canvas_width, 0.0f), brush_data1, brush_data2, Vec2f(1.0f, 1.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(-1.0f, 1.0f, 0.0f, canvas_height), brush_data1, brush_data2, Vec2f(0.0f, 0.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(1.0f, -1.0f, canvas_width, 0.0f), brush_data1, brush_data2, Vec2f(1.0f, 1.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(1.0f, 1.0f, canvas_width, canvas_height), brush_data1, brush_data2, Vec2f(1.0f, 0.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(-1.0f, 1.0f, 0.0f, canvas_height), brush_data1, brush_data2, Vec2f(0.0f, 0.0f), draw_mode));
		}
		else if (brush.type == BrushType::radial)
		{
			Pointf center_point = path_to_world(brush.center_point, brush, transform);
			Pointf radius = path_to_world(Pointf(brush.radius_x, brush.radius_y), brush, transform) - path_to_world(Pointf(), brush, transform);

			draw_mode = 2;
			brush_data1.x = center_point.x;
			brush_data1.y = center_point.y;
			brush_data2.x = 1.0f / brush.radius_x;
			brush_data2.y = 0.0f;
			brush_data2.z = brush.stops.size();

			vertices.push_back(Vertex(Vec4f(-1.0f, -1.0f, 0.0f, 0.0f), brush_data1, brush_data2, Vec2f(0.0f, 1.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(1.0f, -1.0f, canvas_width, 0.0f), brush_data1, brush_data2, Vec2f(1.0f, 1.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(-1.0f, 1.0f, 0.0f, canvas_height), brush_data1, brush_data2, Vec2f(0.0f, 0.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(1.0f, -1.0f, canvas_width, 0.0f), brush_data1, brush_data2, Vec2f(1.0f, 1.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(1.0f, 1.0f, canvas_width, canvas_height), brush_data1, brush_data2, Vec2f(1.0f, 0.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(-1.0f, 1.0f, 0.0f, canvas_height), brush_data1, brush_data2, Vec2f(0.0f, 0.0f), draw_mode));
		}
		else if (brush.type == BrushType::image)
		{
			draw_mode = 3;
			Subtexture subtexture = brush.image.get_texture();
			if (subtexture.is_null())
				return;
			Texture2D image_texture = subtexture.get_texture();
			if (image_texture.is_null())
				return;

			Rectf src = subtexture.get_geometry();

			Pointf image_tl = path_to_world(Pointf(), brush, transform);
			Pointf image_br = path_to_world(Pointf(src.get_width(), src.get_height()), brush, transform);

			// Convert to subtexture coordinates and normalize
			// To do: this has to be done in the shader for clamp/repeat/mirror to work with atlas

			image_tl.x += src.left;
			image_tl.y += src.top;
			image_br.x += src.left;
			image_br.y += src.top;

			Sizef tex_size = Sizef((float)image_texture.get_width(), (float)image_texture.get_height());
			float src_left = (image_tl.x) / tex_size.width;
			float src_top = (image_tl.y) / tex_size.height;
			float src_right = (image_br.x) / tex_size.width;
			float src_bottom = (image_br.y) / tex_size.height;

			gc.set_texture(2, image_texture);
			vertices.push_back(Vertex(Vec4f(-1.0f, -1.0f, 0.0f, 0.0f), Vec4f(src_left, src_top, 0.0f, 0.0f), brush_data2, Vec2f(0.0f, 1.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(1.0f, -1.0f, canvas_width, 1.0f), Vec4f(src_right, src_top, 0.0f, 0.0f), brush_data2, Vec2f(1.0f, 1.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(-1.0f, 1.0f, 0.0f, canvas_height), Vec4f(src_left, src_bottom, 0.0f, 0.0f), brush_data2, Vec2f(0.0f, 0.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(1.0f, -1.0f, canvas_width, 1.0f), Vec4f(src_right, src_top, 0.0f, 0.0f), brush_data2, Vec2f(1.0f, 1.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(1.0f, 1.0f, canvas_width, canvas_height), Vec4f(src_right, src_bottom, 0.0f, 0.0f), brush_data2, Vec2f(1.0f, 0.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(-1.0f, 1.0f, 0.0f, canvas_height), Vec4f(src_left, src_bottom, 0.0f, 0.0f), brush_data2, Vec2f(0.0f, 0.0f), draw_mode));
		}
		else
		{
			draw_mode = 0;
			brush_data1 = brush.color;

			vertices.push_back(Vertex(Vec4f(-1.0f, -1.0f, 0.0f, 0.0f), brush_data1, brush_data2, Vec2f(0.0f, 1.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(1.0f, -1.0f, canvas_width, 0.0f), brush_data1, brush_data2, Vec2f(1.0f, 1.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(-1.0f, 1.0f, 0.0f, canvas_height), brush_data1, brush_data2, Vec2f(0.0f, 0.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(1.0f, -1.0f, canvas_width, 0.0f), brush_data1, brush_data2, Vec2f(1.0f, 1.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(1.0f, 1.0f, canvas_width, canvas_height), brush_data1, brush_data2, Vec2f(1.0f, 0.0f), draw_mode));
			vertices.push_back(Vertex(Vec4f(-1.0f, 1.0f, 0.0f, canvas_height), brush_data1, brush_data2, Vec2f(0.0f, 0.0f), draw_mode));
		}

		int gpu_index;
		VertexArrayVector<Vertex> gpu_vertices(batch_buffer->get_vertex_buffer(gc, gpu_index));

		if (prim_array[gpu_index].is_null())
		{
			prim_array[gpu_index] = PrimitivesArray(gc);
			prim_array[gpu_index].set_attributes(0, gpu_vertices, cl_offsetof(Vertex, Position));
			prim_array[gpu_index].set_attributes(1, gpu_vertices, cl_offsetof(Vertex, BrushData1));
			prim_array[gpu_index].set_attributes(2, gpu_vertices, cl_offsetof(Vertex, BrushData2));
			prim_array[gpu_index].set_attributes(3, gpu_vertices, cl_offsetof(Vertex, TexCoord0));
			prim_array[gpu_index].set_attributes(4, gpu_vertices, cl_offsetof(Vertex, Mode));

		}

		gpu_vertices.upload_data(gc, 0, vertices);

		gc.set_blend_state(blend_state);
		gc.set_program_object(program_path);
		gc.set_texture(0, texture);
		gc.set_texture(1, gradient_texture);
		gc.draw_primitives(type_triangles, 6, prim_array[gpu_index]);
		gc.reset_texture(2);
		gc.reset_texture(1);
		gc.reset_texture(0);
		gc.reset_program_object();
		gc.reset_blend_state();
	}

	inline Pointf PathFillRenderer::path_to_world(Pointf point, const Brush &brush, const Mat4f &transform) const
	{
		point = Pointf(
			brush.transform.matrix[0 * 3 + 0] * point.x + brush.transform.matrix[1 * 3 + 0] * point.y + brush.transform.matrix[2 * 3 + 0],
			brush.transform.matrix[0 * 3 + 1] * point.x + brush.transform.matrix[1 * 3 + 1] * point.y + brush.transform.matrix[2 * 3 + 1]);

		return Pointf(
			transform.matrix[0 * 4 + 0] * point.x + transform.matrix[1 * 4 + 0] * point.y + transform.matrix[3 * 4 + 0],
			transform.matrix[0 * 4 + 1] * point.x + transform.matrix[1 * 4 + 1] * point.y + transform.matrix[3 * 4 + 1]);
	}

	/////////////////////////////////////////////////////////////////////////////

	PathRasterRange::PathRasterRange(const PathScanline &scanline, PathFillMode mode) : scanline(scanline), mode(mode)
	{
	}

	void PathRasterRange::next()
	{
		if (i + 1 >= scanline.edges.size())
		{
			found = false;
			return;
		}

		if (mode == PathFillMode::alternate)
		{
			x0 = scanline.edges[i].x;
			x1 = scanline.edges[i + 1].x;

			i += 2;
			found = true;
		}
		else
		{
			x0 = scanline.edges[i].x;
			nonzero_rule += scanline.edges[i].up_direction ? 1 : -1;
			i++;

			while (i < scanline.edges.size())
			{
				nonzero_rule += scanline.edges[i].up_direction ? 1 : -1;
				x1 = scanline.edges[i].x;
				i++;

				if (nonzero_rule == 0)
				{
					found = true;
					return;
				}
			}
			found = false;
		}
	}
}
