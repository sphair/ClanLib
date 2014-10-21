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
	PathFillRenderer::PathFillRenderer(GraphicContext &gc, RenderBatchBuffer *batch_buffer) : batch_buffer(batch_buffer)
	{
		BlendStateDescription blend_desc;
		blend_desc.set_blend_function(blend_one, blend_one_minus_src_alpha, blend_one, blend_one_minus_src_alpha);
		blend_state = BlendState(gc, blend_desc);
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
			first_scanline = scanlines.size();
			last_scanline = 0;
		}
	}

	void PathFillRenderer::clear()
	{
		for (size_t y = first_scanline; y < last_scanline; y++)
		{
			auto &scanline = scanlines[y];
			if (!scanline.edges.empty())
			{
				scanline.edges.clear();
			}
		}

		first_scanline = scanlines.size();
		last_scanline = 0;
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

			first_scanline = std::min(first_scanline, start_y);
			last_scanline = std::max(last_scanline, end_y);

			for (int y = start_y; y < end_y; y++)
			{
				float ypos = y + 0.5f;
				float x = x0 + (x1 - x0) * (ypos - y0) * rcp_dy;
				scanlines[y].insert_sorted(PathScanlineEdge(x, up_direction));
			}
		}
	}

	void PathFillRenderer::fill(Canvas &canvas, PathFillMode mode, const Brush &brush, const Mat4f &transform)
	{
		if (scanlines.empty()) return;

		initialise_buffers(canvas);
		if (instances.is_full(brush))
		{
			flush(canvas);
			initialise_buffers(canvas);
		}
		current_instance_offset = instances.push(canvas, brush, transform);

		float canvas_width = (float)canvas.get_width();
		float canvas_height = (float)canvas.get_height();

		Rectf mask_extent = sort_and_find_extents(canvas_width, canvas_height);
	
		found_filled_block = false;

		unsigned char *mask_buffer_data = mask_buffer.get_data_uint8();
		int mask_buffer_pitch = mask_buffer.get_pitch();

		PathRasterRange range[scanline_block_size];

		int start_y = first_scanline / scanline_block_size * scanline_block_size;
		int end_y = (last_scanline + scanline_block_size - 1) / scanline_block_size * scanline_block_size;

		for (size_t y = start_y; y < end_y; y += scanline_block_size)
		{
			auto &scanline = scanlines[y];

			for (unsigned int cnt = 0; cnt < scanline_block_size; cnt++)
			{
				range[cnt].begin(&scanlines[y + cnt], mode);
			}

			for (int xpos = mask_extent.left; xpos < mask_extent.right; xpos += scanline_block_size)
			{
				if (vertices.is_full() || next_block == max_blocks)
				{
					flush(canvas);
					initialise_buffers(canvas);
					mask_buffer_data = mask_buffer.get_data_uint8();
					mask_buffer_pitch = mask_buffer.get_pitch();
					found_filled_block = false;
					current_instance_offset = instances.push(canvas, brush, transform);
				}

				int block_x = (next_block * mask_block_size) % mask_texture_size;
				int block_y = ((next_block * mask_block_size) / mask_texture_size)* mask_block_size;

				// Identify filled blocks
				bool full_block = true;
				for (unsigned int filled_cnt = 0; filled_cnt < scanline_block_size; filled_cnt++)
				{
					if (!range[filled_cnt].found)
					{
						full_block = false;
						break;
					}
					if ((range[filled_cnt].x0 > xpos) || (range[filled_cnt].x1 < (xpos + scanline_block_size)))
					{
						full_block = false;
						break;
					}
				}
				if (full_block)
				{
					if (!found_filled_block)
					{
						found_filled_block = true;
						filled_block_index = next_block;
					}
					else
					{
						vertices.push(xpos / antialias_level, y / antialias_level, current_instance_offset, filled_block_index);
						continue;
					}
				}

				bool empty_block = true;
				for (unsigned int cnt = 0; cnt < scanline_block_size; cnt++)
				{
					unsigned char *line = mask_buffer_data + mask_buffer_pitch * (block_y + cnt / antialias_level) + block_x;
					if (range[cnt].found)
					{
						empty_block = false;
					}

#ifdef __SSE2__

					__m128i pixels[mask_block_size / 16];

					for (int sse_block = 0; sse_block < mask_block_size / 16; sse_block++)
						pixels[sse_block] = _mm_loadu_si128((const __m128i*)(&line[16 * sse_block]));

					while (range[cnt].found)
					{
						int x0 = range[cnt].x0;
						if (x0 >= xpos + scanline_block_size)
							break;
						int x1 = range[cnt].x1;

						x0 = max(x0, xpos);
						x1 = min(x1, xpos + scanline_block_size);

						if (x0 >= x1)	// Done segment
						{
							range[cnt].next();
						}
						else
						{
							for (int sse_block = 0; sse_block < mask_block_size / 16; sse_block++)
							{
								for (int alias_cnt = 0; alias_cnt < (antialias_level); alias_cnt++)
								{
									__m128i start = _mm_set1_epi8((x0 + alias_cnt - xpos) / antialias_level - 16 * sse_block);
									__m128i end = _mm_set1_epi8((x1 + alias_cnt - xpos) / antialias_level - 16 * sse_block);
									__m128i x = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);

									__m128i left = _mm_cmplt_epi8(x, start);
									__m128i right = _mm_cmplt_epi8(x, end);
									__m128i mask = _mm_andnot_si128(left, right);
									__m128i add_value = _mm_set1_epi8(256 / (antialias_level*antialias_level));

									pixels[sse_block] = _mm_adds_epu8(pixels[sse_block], _mm_and_si128(mask, add_value));
								}
							}

							range[cnt].x0 = x1;	// For next time
						}
					}

					for (int sse_block = 0; sse_block < mask_block_size / 16; sse_block++)
						_mm_storeu_si128((__m128i*)(&line[16 * sse_block]), pixels[sse_block]);

#else
					while (range[cnt].found)
					{
						int x0 = range[cnt].x0;
						if (x0 >= xpos + scanline_block_size)
							break;
						int x1 = range[cnt].x1;

						x0 = max(x0, xpos);
						x1 = min(x1, xpos + scanline_block_size);

						if (x0 >= x1)	// Done segment
						{
							range[cnt].next();
						}
						else
						{
							for (int x = x0 - xpos; x < x1 - xpos; x++)
							{
								int pixel = line[x / antialias_level];
								pixel = min(pixel + (256 / (antialias_level*antialias_level)), 255);
								line[x / antialias_level] = pixel;
							}
							range[cnt].x0 = x1;	// For next time
						}
					}
#endif
				}
				if (!empty_block)
				{
					vertices.push(xpos / antialias_level, y / antialias_level, current_instance_offset, next_block++);
				}
			}
		}
		//PNGProvider::save(mask_buffer, "c:\\development\\test.png");
	}

	Rectf PathFillRenderer::sort_and_find_extents(float canvas_width, float canvas_height)
	{
		Rectf mask_extent(canvas_width*static_cast<float>(antialias_level), canvas_height * static_cast<float>(antialias_level), 0.0f, 0.0f);		// Dummy initial values

		// Precalculation to determine the extents
		for (size_t y = first_scanline; y < last_scanline; y++)
		{
			auto &scanline = scanlines[y];
			if (scanline.edges.empty())
				continue;

			//std::sort(scanline.edges.begin(), scanline.edges.end(), [](const PathScanlineEdge &a, const PathScanlineEdge &b) { return a.x < b.x; });

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

		mask_extent.clip(Sizef(canvas_width * antialias_level, canvas_height * antialias_level));

		return mask_extent;
	}

	void PathFillRenderer::flush(GraphicContext &gc)
	{
		if (instances.get_position() == 0 && vertices.get_position() == 0 && next_block == 0) // Nothing to flush
			return;

		// To do: fix that we lock in one file and unlocks in another (too ugly way of doing it)
		mask_buffer.unlock();
		instance_buffer.unlock();

		int gpu_index;
		VertexArrayVector<Vec4i> gpu_vertices(batch_buffer->get_vertex_buffer(gc, gpu_index));

		if (prim_array[gpu_index].is_null())
		{
			prim_array[gpu_index] = PrimitivesArray(gc);
			prim_array[gpu_index].set_attributes(0, gpu_vertices);
		}

		gpu_vertices.upload_data(gc, 0, vertices.get_vertices(), vertices.get_position());

		int block_y = (((next_block-1) * mask_block_size) / mask_texture_size)* mask_block_size;
		mask_texture.set_subimage(gc, 0, 0, mask_buffer, Rect(Point(0, 0), Size(mask_texture_size, block_y + mask_block_size)));

		instance_texture.set_subimage(gc, 0, 0, instance_buffer, Rect(Point(0, 0), Size(instance_buffer_width, (instances.get_position() + instance_buffer_width - 1) / instance_buffer_width)));

		gc.set_blend_state(blend_state);
		gc.set_program_object(program_path);
		gc.set_texture(0, mask_texture);
		gc.set_texture(1, instance_texture);

		Texture2D current_texture = instances.get_texture();

		if (!current_texture.is_null())
			gc.set_texture(2, current_texture);
		gc.draw_primitives(type_triangles, vertices.get_position(), prim_array[gpu_index]);
		if (!current_texture.is_null())
		{
			gc.reset_texture(2);
		}
		gc.reset_texture(1);
		gc.reset_texture(0);
		gc.reset_program_object();
		gc.reset_blend_state();

		// Finishedwith the buffers
		mask_buffer = TransferTexture();
		mask_texture = Texture2D();
		instance_buffer = TransferTexture();
		instance_texture = Texture2D();

		batch_buffer->set_transfer_r8_used(mask_buffer_id, block_y + mask_block_size);
		next_block = 0;
	}

	void PathFillRenderer::initialise_buffers(Canvas &canvas)
	{
		if (mask_texture.is_null())
		{
			GraphicContext gc = canvas.get_gc();
			mask_texture = batch_buffer->get_texture_r8(gc);
			mask_buffer = batch_buffer->get_transfer_r8(gc, mask_buffer_id, access_read_write);
			instance_texture = batch_buffer->get_texture_rgba32f(gc);
			instance_buffer = batch_buffer->get_transfer_rgba32f(gc, access_write_only);

			instances.reset(gc, instance_buffer.get_data<Vec4f>(), instance_buffer_width * instance_buffer_height);
			vertices.reset((Vec4i *)batch_buffer->buffer, max_vertices);
		}
	}
	/////////////////////////////////////////////////////////////////////////////

	void PathRasterRange::begin(const PathScanline *new_scanline, PathFillMode new_mode)
	{
		scanline = new_scanline;
		mode = new_mode;
		found = false;
		i = 0;
		nonzero_rule = 0;
		next();
	}

	void PathRasterRange::next()
	{
		if (i + 1 >= scanline->edges.size())
		{
			found = false;
			return;
		}

		if (mode == PathFillMode::alternate)
		{
			x0 = static_cast<int>(scanline->edges[i].x + 0.5f);
			x1 = static_cast<int>(scanline->edges[i + 1].x - 0.5f) + 1;
			i += 2;
			found = true;
		}
		else
		{
			x0 = static_cast<int>(scanline->edges[i].x + 0.5f);
			nonzero_rule += scanline->edges[i].up_direction ? 1 : -1;
			i++;

			while (i < scanline->edges.size())
			{
				nonzero_rule += scanline->edges[i].up_direction ? 1 : -1;
				x1 = static_cast<int>(scanline->edges[i].x - 0.5f) + 1;
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

	/////////////////////////////////////////////////////////////////////////

	//void PathMaskBuffer::foo()
	//{
	//}

	/////////////////////////////////////////////////////////////////////////

	void PathInstanceBuffer::reset(GraphicContext &gc, Vec4f *new_buffer, int new_max_entries)
	{
		buffer = new_buffer;
		max_entries = new_max_entries;
		current_texture = Texture2D();

		buffer[0] = Vec4f(gc.get_width(), gc.get_height(), 0, 0);
		position = 1;
	}

	bool PathInstanceBuffer::is_full(const Brush &brush) const
	{
		if (brush.type == BrushType::image) // Currently only support a single path texture
		{
			Subtexture subtexture = brush.image.get_texture();
			if (!subtexture.is_null() && !current_texture.is_null() && subtexture.get_texture() != current_texture)
				return true;
		}

		return position + brush.stops.size() * 2 + 2 > max_entries;
	}

	int PathInstanceBuffer::push(Canvas &canvas, const Brush &brush, const Mat4f &transform)
	{
		if (is_full(brush)) return 0;

		int instance_pos = position;

		int num_stops = brush.stops.size();

		if (brush.type == BrushType::linear)
		{
			Pointf end_point = transform_point(brush.end_point, brush.transform, transform);
			Pointf start_point = transform_point(brush.start_point, brush.transform, transform);
			Pointf dir = end_point - start_point;
			Pointf dir_normed = Pointf::normalize(dir);

			Vec4f brush_data1;
			Vec4f brush_data2;
			Vec4f brush_data3;
			brush_data1.x = (float)PathShaderDrawMode::linear;
			brush_data1.set_zw(dir_normed);

			brush_data2.x = 1.0f / dir.length();
			brush_data2.y = instance_pos + 3;
			brush_data2.z = instance_pos + 3 + num_stops * 2;

			brush_data3.set_xy(start_point);

			buffer[position++] = brush_data1;
			buffer[position++] = brush_data2;
			buffer[position++] = brush_data3;
		}
		else if (brush.type == BrushType::radial)
		{
			Pointf center_point = transform_point(brush.center_point, brush.transform, transform);
			Pointf radius = transform_point(Pointf(brush.radius_x, brush.radius_y), brush.transform, transform) - transform_point(Pointf(), brush.transform, transform);

			Vec4f brush_data1;
			Vec4f brush_data2;
			Vec4f brush_data3;
			brush_data1.x = (float)PathShaderDrawMode::radial;
			brush_data2.x = 1.0f / brush.radius_x;
			brush_data2.y = instance_pos + 3;
			brush_data2.z = instance_pos + 3 + num_stops * 2;

			brush_data3.set_xy(center_point);

			buffer[position++] = brush_data1;
			buffer[position++] = brush_data2;
			buffer[position++] = brush_data3;

		}
		else if (brush.type == BrushType::image)
		{
			Subtexture subtexture = brush.image.get_texture();
			if (subtexture.is_null())
				return 0;

			current_texture = subtexture.get_texture();
			if (current_texture.is_null())
				throw Exception("BrushType::image used without a valid texture");

			Mat3f inv_brush_transform = Mat3f::inverse(brush.transform);
			Mat4f inv_transform = Mat4f::inverse(transform);

			Rectf src = subtexture.get_geometry();

			Vec4f brush_data1(src.left, src.top, src.right, src.bottom);
			Vec4f brush_data2(current_texture.get_width(), current_texture.get_height(), 0, 0);

			brush_data1.x = (float)PathShaderDrawMode::image;
			buffer[position++] = brush_data1;
			buffer[position++] = brush_data2;

			Mat4f matrix = Mat4f(inv_brush_transform) * inv_transform;	//TODO: Is this the correct multication order
			matrix.transpose();
	
			buffer[position++] = Vec4f(matrix.matrix[0], matrix.matrix[1], matrix.matrix[2], matrix.matrix[3]);
			buffer[position++] = Vec4f(matrix.matrix[4], matrix.matrix[5], matrix.matrix[6], matrix.matrix[7]);
			buffer[position++] = Vec4f(matrix.matrix[8], matrix.matrix[9], matrix.matrix[10], matrix.matrix[11]);
			buffer[position++] = Vec4f(matrix.matrix[12], matrix.matrix[13], matrix.matrix[14], matrix.matrix[15]);
		}
		else
		{
			Vec4f brush_data1;
			Vec4f brush_data2;
			brush_data1.x = (float)PathShaderDrawMode::solid;
			brush_data2 = brush.color;
			buffer[position++] = brush_data1;
			buffer[position++] = brush_data2;

		}


		for (unsigned int cnt = 0; cnt < num_stops; cnt++)
		{
			buffer[position++] = brush.stops[cnt].color;
			buffer[position++] = Vec4f(brush.stops[cnt].position, 0.0f, 0.0f, 0.0f);
		}

		return instance_pos;
	}

	inline Pointf PathInstanceBuffer::transform_point(Pointf point, const Mat3f &brush_transform, const Mat4f &transform)
	{
		point = Pointf(
			brush_transform.matrix[0 * 3 + 0] * point.x + brush_transform.matrix[1 * 3 + 0] * point.y + brush_transform.matrix[2 * 3 + 0],
			brush_transform.matrix[0 * 3 + 1] * point.x + brush_transform.matrix[1 * 3 + 1] * point.y + brush_transform.matrix[2 * 3 + 1]);

		return Pointf(
			transform.matrix[0 * 4 + 0] * point.x + transform.matrix[1 * 4 + 0] * point.y + transform.matrix[3 * 4 + 0],
			transform.matrix[0 * 4 + 1] * point.x + transform.matrix[1 * 4 + 1] * point.y + transform.matrix[3 * 4 + 1]);
	}

	/////////////////////////////////////////////////////////////////////////

	void PathVertexBuffer::reset(Vec4i *new_vertices, int new_max_vertices)
	{
		vertices = new_vertices;
		max_vertices = new_max_vertices;
		position = 0;
	}

	bool PathVertexBuffer::is_full() const
	{
		return position + 6 > max_vertices;
	}

	void PathVertexBuffer::push(int x, int y, int instance_offset, int mask_offset)
	{
		if (is_full()) return;

		int w = instance_offset * 65536 + mask_offset;

		vertices[position++] = Vec4i(x, y, (0) + (0 * 2), w);
		vertices[position++] = Vec4i(x, y, (1) + (0 * 2), w);
		vertices[position++] = Vec4i(x, y, (0) + (1 * 2), w);
		vertices[position++] = Vec4i(x, y, (1) + (0 * 2), w);
		vertices[position++] = Vec4i(x, y, (1) + (1 * 2), w);
		vertices[position++] = Vec4i(x, y, (0) + (1 * 2), w);
	}
}
