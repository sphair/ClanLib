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

	void PathFillRenderer::set_size(Canvas &canvas, int new_width, int new_height)
	{
		if (width != new_width || height != new_height)
		{
			width = new_width;
			height = new_height;
			scanlines.resize(height * 2);
			if (width != 0 && height != 0)
			{
				GraphicContext gc = canvas.get_gc();
				mask_buffer = TransferTexture(gc, width, height, data_to_gpu, tf_r8);
				mask_texture = Texture2D(gc, width, height, tf_r8);
				mask_texture.set_min_filter(filter_nearest);
				mask_texture.set_mag_filter(filter_nearest);
			}
		}
	}

	void PathFillRenderer::clear()
	{
		for (size_t y = 0; y < scanlines.size(); y++)
		{
			auto &scanline = scanlines[y];
			if (!scanline.edges.empty())
			{
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

		Mat3f inv_brush_transform = Mat3f::inverse(brush.transform);
		Mat4f inv_transform = Mat4f::inverse(transform);

		float canvas_width = (float)canvas.get_width();
		float canvas_height = (float)canvas.get_height();

		Rectf mask_extent(canvas_width*2.0f, canvas_height * 2.0f, 0.0f, 0.0f);		// Dummy initial values

		GraphicContext gc = canvas.get_gc();
		mask_buffer.lock(gc, access_read_write);
		memset(mask_buffer.get_data(), 0, mask_buffer.get_height() * mask_buffer.get_pitch());

		for (size_t y = 0; y < scanlines.size(); y++)
		{
			auto &scanline = scanlines[y];
			if (scanline.edges.empty())
				continue;

			std::sort(scanline.edges.begin(), scanline.edges.end(), [](const PathScanlineEdge &a, const PathScanlineEdge &b) { return a.x < b.x; });

			// Calculate mask extent
			if (y < mask_extent.top)
				mask_extent.top = y;

			if (y > mask_extent.bottom)
				mask_extent.bottom = y;	
			
			if (scanline.edges[0].x < mask_extent.left)
				mask_extent.left = scanline.edges[0].x;

			if (scanline.edges[scanline.edges.size()-1].x > mask_extent.right)
				mask_extent.right = scanline.edges[scanline.edges.size() - 1].x;

			unsigned char *line = mask_buffer.get_line_uint8(y / 2);

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

		mask_buffer.unlock();
		Rectf canvas_extent((int) (mask_extent.left / 2.0f), (int) ( mask_extent.top / 2.0f) , (int) (mask_extent.right / 2.0f), (int) (mask_extent.bottom / 2.0f));
		canvas_extent.clip(clan::Sizef(width, height));
		if ((canvas_extent.get_width() > 0) && (canvas_extent.get_height() > 0) )
			mask_texture.set_subimage(canvas, canvas_extent.left, canvas_extent.top, mask_buffer, canvas_extent);
	
		Rectf canvas_extent_normalised(canvas_extent);
		canvas_extent_normalised.left = (2.0f * canvas_extent_normalised.left / canvas_width) - 1.0f;
		canvas_extent_normalised.right = (2.0f * canvas_extent_normalised.right / canvas_width) - 1.0f;
		canvas_extent_normalised.top = (2.0f * canvas_extent_normalised.top / canvas_height) - 1.0f;
		canvas_extent_normalised.bottom = (2.0f * canvas_extent_normalised.bottom / canvas_height) - 1.0f;

		Rectf texture_extent_normalised(canvas_extent);
		texture_extent_normalised.left = (texture_extent_normalised.left / width);
		texture_extent_normalised.right = (texture_extent_normalised.right /width);
		texture_extent_normalised.top = (texture_extent_normalised.top / height);
		texture_extent_normalised.bottom = (texture_extent_normalised.bottom / height);

		std::vector<Vertex> vertices;
		Vec4f brush_data2;
		int draw_mode;

		int num_stops = max(brush.stops.size(), 8);
		PixelBuffer gradient_pixelbuffer(num_stops * 2, 1, tf_rgba32f);
		Vec4f *gradient_ptr = gradient_pixelbuffer.get_data<Vec4f>();
		for (unsigned int cnt = 0; cnt < brush.stops.size(); cnt++)
		{
			*(gradient_ptr++) = brush.stops[cnt].color;
			*(gradient_ptr++) = Vec4f(brush.stops[cnt].position, 0.0f, 0.0f, 0.0f);
		}

		Texture2D gradient_texture(gc, gradient_pixelbuffer);
		gradient_texture.set_min_filter(filter_nearest);
		gradient_texture.set_mag_filter(filter_nearest);

		Vec4f brush_data1_bottom_left;
		Vec4f brush_data1_bottom_right;
		Vec4f brush_data1_top_left;
		Vec4f brush_data1_top_right;

		if (brush.type == BrushType::linear)
		{
			draw_mode = 1;
			Pointf end_point = transform_point(brush.end_point, brush.transform, transform);
			Pointf start_point = transform_point(brush.start_point, brush.transform, transform);
			Pointf dir = end_point - start_point;
			Pointf dir_normed = Pointf::normalize(dir);
			brush_data1_bottom_left.set_zw(dir_normed);
			brush_data1_bottom_right.set_zw(dir_normed);
			brush_data1_top_left.set_zw(dir_normed);
			brush_data1_top_right.set_zw(dir_normed);

			brush_data2.x = 1.0f / dir.length();
			brush_data2.y = 0.0f;
			brush_data2.z = brush.stops.size();

			brush_data1_bottom_left.set_xy(canvas_extent.get_bottom_left() - start_point);
			brush_data1_bottom_right.set_xy(canvas_extent.get_bottom_right() - start_point);
			brush_data1_top_left.set_xy(canvas_extent.get_top_left() - start_point);
			brush_data1_top_right.set_xy(canvas_extent.get_top_right() - start_point);
		}
		else if (brush.type == BrushType::radial)
		{
			Pointf center_point = transform_point(brush.center_point, brush.transform, transform);
			Pointf radius = transform_point(Pointf(brush.radius_x, brush.radius_y), brush.transform, transform) - transform_point(Pointf(), brush.transform, transform);

			draw_mode = 2;
			brush_data2.x = 1.0f / brush.radius_x;
			brush_data2.y = 0.0f;
			brush_data2.z = brush.stops.size();

			brush_data1_bottom_left.set_xy(canvas_extent.get_bottom_left() - center_point);
			brush_data1_bottom_right.set_xy(canvas_extent.get_bottom_right() - center_point);
			brush_data1_top_left.set_xy(canvas_extent.get_top_left() - center_point);
			brush_data1_top_right.set_xy(canvas_extent.get_top_right() - center_point);
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

			// Find transformed UV coordinates for image covering the entire mask texture:
			Pointf image_tl = transform_point(canvas_extent.get_top_left(), inv_brush_transform, inv_transform);
			Pointf image_br = transform_point(canvas_extent.get_bottom_right(), inv_brush_transform, inv_transform);

			// Convert to subtexture coordinates:
			Sizef tex_size = Sizef((float)image_texture.get_width(), (float)image_texture.get_height());
			float src_left = (src.left + image_tl.x) / tex_size.width;
			float src_right = (src.left + image_br.x) / tex_size.width;
			float src_top = (src.top + image_tl.y) / tex_size.height;
			float src_bottom = (src.top + image_br.y) / tex_size.height;

			brush_data1_bottom_left.set_xy(Vec2f(src_left, src_bottom));
			brush_data1_bottom_right.set_xy(Vec2f(src_right, src_bottom));
			brush_data1_top_left.set_xy(Vec2f(src_left, src_top));
			brush_data1_top_right.set_xy(Vec2f(src_right, src_top));

			gc.set_texture(2, image_texture);
		}
		else
		{
			draw_mode = 0;
			brush_data2 = brush.color;
		}

		//vertices.push_back(Vertex(Vec4f(canvas_extent_normalised.left, -canvas_extent_normalised.bottom, 0.0f, 1.0f), brush_data1_bottom_left, brush_data2, Vec2f(texture_extent_normalised.left, texture_extent_normalised.top), draw_mode));
		//vertices.push_back(Vertex(Vec4f(canvas_extent_normalised.right, -canvas_extent_normalised.bottom, 0.0f, 1.0f), brush_data1_bottom_right, brush_data2, Vec2f(texture_extent_normalised.right, texture_extent_normalised.top), draw_mode));
		//vertices.push_back(Vertex(Vec4f(canvas_extent_normalised.left, -canvas_extent_normalised.top, 0.0f, 1.0f), brush_data1_top_left, brush_data2, Vec2f(texture_extent_normalised.left, texture_extent_normalised.bottom), draw_mode));
		//vertices.push_back(Vertex(Vec4f(canvas_extent_normalised.right, -canvas_extent_normalised.bottom, 0.0f, 1.0f), brush_data1_bottom_right, brush_data2, Vec2f(texture_extent_normalised.right, texture_extent_normalised.top), draw_mode));
		//vertices.push_back(Vertex(Vec4f(canvas_extent_normalised.right, -canvas_extent_normalised.top, 0.0f, 1.0f), brush_data1_top_right, brush_data2, Vec2f(texture_extent_normalised.right, texture_extent_normalised.bottom), draw_mode));
		//vertices.push_back(Vertex(Vec4f(canvas_extent_normalised.left, -canvas_extent_normalised.top, 0.0f, 1.0f), brush_data1_top_left, brush_data2, Vec2f(texture_extent_normalised.left, texture_extent_normalised.bottom), draw_mode));

		vertices.push_back(Vertex(Vec4f(canvas_extent_normalised.left, -canvas_extent_normalised.top, 0.0f, 1.0f), brush_data1_top_left, brush_data2, Vec2f(texture_extent_normalised.left, texture_extent_normalised.top), draw_mode));
		vertices.push_back(Vertex(Vec4f(canvas_extent_normalised.right, -canvas_extent_normalised.top, 0.0f, 1.0f), brush_data1_top_right, brush_data2, Vec2f(texture_extent_normalised.right, texture_extent_normalised.top), draw_mode));
		vertices.push_back(Vertex(Vec4f(canvas_extent_normalised.left, -canvas_extent_normalised.bottom, 0.0f, 1.0f), brush_data1_bottom_left, brush_data2, Vec2f(texture_extent_normalised.left, texture_extent_normalised.bottom), draw_mode));
		vertices.push_back(Vertex(Vec4f(canvas_extent_normalised.right, -canvas_extent_normalised.top, 0.0f, 1.0f), brush_data1_top_right, brush_data2, Vec2f(texture_extent_normalised.right, texture_extent_normalised.top), draw_mode));
		vertices.push_back(Vertex(Vec4f(canvas_extent_normalised.right, -canvas_extent_normalised.bottom, 0.0f, 1.0f), brush_data1_bottom_right, brush_data2, Vec2f(texture_extent_normalised.right, texture_extent_normalised.bottom), draw_mode));
		vertices.push_back(Vertex(Vec4f(canvas_extent_normalised.left, -canvas_extent_normalised.bottom, 0.0f, 1.0f), brush_data1_bottom_left, brush_data2, Vec2f(texture_extent_normalised.left, texture_extent_normalised.bottom), draw_mode));

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
		gc.set_texture(0, mask_texture);
		gc.set_texture(1, gradient_texture);
		gc.draw_primitives(type_triangles, 6, prim_array[gpu_index]);
		gc.reset_texture(2);
		gc.reset_texture(1);
		gc.reset_texture(0);
		gc.reset_program_object();
		gc.reset_blend_state();
	}

	inline Pointf PathFillRenderer::transform_point(Pointf point, const Mat3f &brush_transform, const Mat4f &transform) const
	{
		point = Pointf(
			brush_transform.matrix[0 * 3 + 0] * point.x + brush_transform.matrix[1 * 3 + 0] * point.y + brush_transform.matrix[2 * 3 + 0],
			brush_transform.matrix[0 * 3 + 1] * point.x + brush_transform.matrix[1 * 3 + 1] * point.y + brush_transform.matrix[2 * 3 + 1]);

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
