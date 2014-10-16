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
#include "API/Display/ImageProviders/png_provider.h"
#include <algorithm>

namespace clan
{
	PathFillRenderer::PathFillRenderer(GraphicContext &gc)
	{
		BlendStateDescription blend_desc;
		blend_desc.set_blend_function(blend_one, blend_one_minus_src_alpha, blend_one, blend_one_minus_src_alpha);
		blend_state = BlendState(gc, blend_desc);

		mask_buffer = TransferTexture(gc, mask_texture_size, mask_texture_size, data_to_gpu, tf_r8);
		mask_texture = Texture2D(gc, mask_texture_size, mask_texture_size, tf_r8);
		mask_texture.set_min_filter(filter_nearest);
		mask_texture.set_mag_filter(filter_nearest);
	}

	void PathFillRenderer::set_size(Canvas &canvas, int new_width, int new_height)
	{
		// For simplicity of the code, ensure the mask is always a multiple of mask_block_size
		new_width = mask_block_size * ((new_width + mask_block_size - 1) / mask_block_size);
		new_height = mask_block_size * ((new_height + mask_block_size - 1) / mask_block_size);

		if (width != new_width || height != new_height)
		{
			width = new_width;
			height = new_height;
			scanlines.resize(height * antialias_level);
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

		x0 *= static_cast<float>(antialias_level);
		x1 *= static_cast<float>(antialias_level);
		y0 *= static_cast<float>(antialias_level);
		y1 *= static_cast<float>(antialias_level);

		bool up_direction = y1 < y0;
		float dy = y1 - y0;

		const float epsilon = std::numeric_limits<float>::epsilon();
		if (dy < -epsilon || dy > epsilon)
		{
			int start_y = static_cast<int>(std::floor(min(y0, y1) + 0.5f));
			int end_y = static_cast<int>(std::floor(max(y0, y1) - 0.5f)) + 1;

			start_y = max(start_y, 0);
			end_y = min(end_y, height * antialias_level);

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

		Rectf mask_extent = sort_and_find_extents(canvas_width, canvas_height);
		build_upload_list(canvas, mask_extent, mode);
		upload_and_draw(batch_buffer, canvas, brush, transform);
	}

	Rectf PathFillRenderer::sort_and_find_extents(float canvas_width, float canvas_height)
	{
		Rectf mask_extent(canvas_width*static_cast<float>(antialias_level), canvas_height * static_cast<float>(antialias_level), 0.0f, 0.0f);		// Dummy initial values

		// Precalculation to determine the extents
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

			if (scanline.edges[scanline.edges.size() - 1].x > mask_extent.right)
				mask_extent.right = scanline.edges[scanline.edges.size() - 1].x;
		}

		mask_extent.clip(Sizef(width * antialias_level, height * antialias_level));

		return mask_extent;
	}

	void PathFillRenderer::build_upload_list(Canvas &canvas, const Rectf &mask_extent, PathFillMode mode)
	{
		GraphicContext gc = canvas.get_gc();

		int empty_blocks = 0;
		int full_blocks = 0;

		// To do: remove the upload reset to allow batching across multiple path fills
		upload_list.clear();
		next_block = 0;

		mask_buffer.lock(gc, access_read_write); // To do: maybe keep this locked?
		memset(mask_buffer.get_data(), 0, mask_buffer.get_height() * mask_buffer.get_pitch()); // To do: this clear needs to be smarter - doesn't work with batching this way

		range.clear();
		range.reserve(scanline_block_size);

		for (size_t y = 0; y < scanlines.size(); y += scanline_block_size)
		{
			auto &scanline = scanlines[y];

			range.clear();
			for (unsigned int cnt = 0; cnt < scanline_block_size; cnt++)
			{
				range.push_back(PathRasterRange(scanlines[y + cnt], mode));
				range[cnt].next();
			}

			for (int xpos = mask_extent.left; xpos < mask_extent.right; xpos += scanline_block_size)
			{
				int block_x = (next_block * mask_block_size) % mask_texture_size;
				int block_y = ((next_block * mask_block_size) / mask_texture_size)* mask_block_size;

				bool empty_block = true;
				bool full_block = true;
				for (unsigned int cnt = 0; cnt < scanline_block_size; cnt++)
				{
					unsigned char *line = mask_buffer.get_line_uint8(block_y + cnt / antialias_level) + block_x;
					if (range[cnt].found)
					{
						full_block = false;
						empty_block = false;
					}

					while (range[cnt].found)
					{
						int x0 = static_cast<int>(range[cnt].x0 + 0.5f);
						if (x0 >= xpos + scanline_block_size)
							break;
						int x1 = static_cast<int>(range[cnt].x1 - 0.5f) + 1;
						if (x0 == x1)	// Done segment
						{
							range[cnt].next();
						}
						else
						{
							if ((x0 > xpos) || (x1 < (xpos + scanline_block_size)))
								full_block = false;

							x0 = max(x0, xpos);
							x1 = min(x1, xpos + scanline_block_size);
							for (int x = x0 - xpos; x < x1 - xpos; x++)
							{
								int pixel = line[x / antialias_level];
								pixel = min(pixel + (256 / (antialias_level*antialias_level)), 255);
								line[x / antialias_level] = pixel;
							}
							range[cnt].x0 = x1;	// For next time
						}
					}
				}
				if (!empty_block)
				{
					upload_list.push_back(Point(xpos / antialias_level, y / antialias_level));
					next_block++;
					if (next_block == max_blocks)
						flush();
				}
				else
				{
					empty_blocks++;
				}
				if (full_block)
					full_blocks++;
			}
		}

		//PNGProvider::save(mask_buffer, "c:\\development\\test.png");
		mask_buffer.unlock();
	}

	void PathFillRenderer::flush()
	{
		// To do: Texture mask buff is full. Render what we have.
		//        This function is supposed to do what upload_and_draw is actually doing

		next_block = 0;
		upload_list.clear();
	}

	void PathFillRenderer::upload_and_draw(RenderBatchBuffer *batch_buffer, Canvas &canvas, const Brush &brush, const Mat4f &transform)
	{
		GraphicContext gc = canvas.get_gc();

		float canvas_width = (float)canvas.get_width();
		float canvas_height = (float)canvas.get_height();

		size_t blocks_height = (upload_list.size() + mask_texture_size - 1) / mask_texture_size * mask_texture_size;
		int block_y = ((next_block * mask_block_size) / mask_texture_size)* mask_block_size;
		mask_texture.set_subimage(canvas, 0, 0, mask_buffer, Rect(Point(0, 0), Size(mask_texture_size, block_y + mask_block_size)));

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

		std::vector<Vertex> vertices;

		Mat3f inv_brush_transform;
		Mat4f inv_transform;
		if (brush.type == BrushType::image)
		{
			inv_brush_transform = Mat3f::inverse(brush.transform);
			inv_transform = Mat4f::inverse(transform);

		}

		for (unsigned int upload_index = 0; upload_index < upload_list.size(); upload_index++)
		{
			Point upload_point = upload_list[upload_index];

			Rectf upload_rect(Pointf(upload_list[upload_index]), Sizef(mask_block_size, mask_block_size));
			Rectf upload_rect_normalised(upload_rect);
			upload_rect_normalised.left = (2.0f * upload_rect_normalised.left / canvas_width) - 1.0f;
			upload_rect_normalised.right = (2.0f * upload_rect_normalised.right / canvas_width) - 1.0f;
			upload_rect_normalised.top = (2.0f * upload_rect_normalised.top / canvas_height) - 1.0f;
			upload_rect_normalised.bottom = (2.0f * upload_rect_normalised.bottom / canvas_height) - 1.0f;

			int block_x = (upload_index* mask_block_size) % mask_texture_size;
			int block_y = ((upload_index* mask_block_size) / mask_texture_size) * mask_block_size;
			Rectf block_normalised(Pointf(block_x, block_y), Sizef(mask_block_size, mask_block_size));
			block_normalised.left = (block_normalised.left / mask_texture_size);
			block_normalised.right = (block_normalised.right / mask_texture_size);
			block_normalised.top = (block_normalised.top / mask_texture_size);
			block_normalised.bottom = (block_normalised.bottom / mask_texture_size);

			Vec4f brush_data2;
			int draw_mode;
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

				brush_data1_bottom_left.set_xy(upload_rect.get_bottom_left() - start_point);
				brush_data1_bottom_right.set_xy(upload_rect.get_bottom_right() - start_point);
				brush_data1_top_left.set_xy(upload_rect.get_top_left() - start_point);
				brush_data1_top_right.set_xy(upload_rect.get_top_right() - start_point);
			}
			else if (brush.type == BrushType::radial)
			{
				Pointf center_point = transform_point(brush.center_point, brush.transform, transform);
				Pointf radius = transform_point(Pointf(brush.radius_x, brush.radius_y), brush.transform, transform) - transform_point(Pointf(), brush.transform, transform);

				draw_mode = 2;
				brush_data2.x = 1.0f / brush.radius_x;
				brush_data2.y = 0.0f;
				brush_data2.z = brush.stops.size();

				brush_data1_bottom_left.set_xy(upload_rect.get_bottom_left() - center_point);
				brush_data1_bottom_right.set_xy(upload_rect.get_bottom_right() - center_point);
				brush_data1_top_left.set_xy(upload_rect.get_top_left() - center_point);
				brush_data1_top_right.set_xy(upload_rect.get_top_right() - center_point);
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
				Pointf image_tl = transform_point(upload_rect.get_top_left(), inv_brush_transform, inv_transform);
				Pointf image_br = transform_point(upload_rect.get_bottom_right(), inv_brush_transform, inv_transform);

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

			vertices.push_back(Vertex(Vec4f(upload_rect_normalised.left, -upload_rect_normalised.top, 0.0f, 1.0f), brush_data1_top_left, brush_data2, Vec2f(block_normalised.left, block_normalised.top), draw_mode));
			vertices.push_back(Vertex(Vec4f(upload_rect_normalised.right, -upload_rect_normalised.top, 0.0f, 1.0f), brush_data1_top_right, brush_data2, Vec2f(block_normalised.right, block_normalised.top), draw_mode));
			vertices.push_back(Vertex(Vec4f(upload_rect_normalised.left, -upload_rect_normalised.bottom, 0.0f, 1.0f), brush_data1_bottom_left, brush_data2, Vec2f(block_normalised.left, block_normalised.bottom), draw_mode));
			vertices.push_back(Vertex(Vec4f(upload_rect_normalised.right, -upload_rect_normalised.top, 0.0f, 1.0f), brush_data1_top_right, brush_data2, Vec2f(block_normalised.right, block_normalised.top), draw_mode));
			vertices.push_back(Vertex(Vec4f(upload_rect_normalised.right, -upload_rect_normalised.bottom, 0.0f, 1.0f), brush_data1_bottom_right, brush_data2, Vec2f(block_normalised.right, block_normalised.bottom), draw_mode));
			vertices.push_back(Vertex(Vec4f(upload_rect_normalised.left, -upload_rect_normalised.bottom, 0.0f, 1.0f), brush_data1_bottom_left, brush_data2, Vec2f(block_normalised.left, block_normalised.bottom), draw_mode));
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
		gc.set_texture(0, mask_texture);
		gc.set_texture(1, gradient_texture);
		gc.draw_primitives(type_triangles, vertices.size(), prim_array[gpu_index]);
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
