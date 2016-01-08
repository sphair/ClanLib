/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "API/Core/System/system.h"
#include <algorithm>

#if !defined __ANDROID__ && ! defined CL_DISABLE_SSE2
#include <xmmintrin.h>
#endif

using namespace clan::PathConstants;

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
		clear();

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
		current_instance_offset = instances.push(canvas, brush, transform);
		if (!current_instance_offset)
		{
			flush(canvas);
			initialise_buffers(canvas);
			current_instance_offset = instances.push(canvas, brush, transform);
		}

		int max_width = canvas.get_gc().get_width() * antialias_level;

		int start_y = first_scanline / scanline_block_size * scanline_block_size;
		int end_y = (last_scanline + scanline_block_size - 1) / scanline_block_size * scanline_block_size;

		for (size_t y = start_y; y < end_y; y += scanline_block_size)
		{
			mask_blocks.begin_row(&scanlines[y], mode);
			Extent extent = find_extent(&scanlines[y], max_width);

			for (int xpos = extent.left; xpos < extent.right; xpos += scanline_block_size)
			{
				if (vertices.is_full() || mask_blocks.is_full())
				{
					flush(canvas);
					initialise_buffers(canvas);
					current_instance_offset = instances.push(canvas, brush, transform);
				}

				if (mask_blocks.fill_block(xpos))
				{
					vertices.push(xpos / antialias_level, y / antialias_level, current_instance_offset, mask_blocks.block_index);
				}
			}
		}
	}

	PathFillRenderer::Extent PathFillRenderer::find_extent(const PathScanline *scanline, int max_width)
	{
		// Find scanline extents
		Extent extent;
		for (unsigned int cnt = 0; cnt < scanline_block_size; cnt++, scanline++)
		{
			if (scanline->edges.empty())
				continue;

			if (scanline->edges[0].x < extent.left)
				extent.left = scanline->edges[0].x;

			if (scanline->edges[scanline->edges.size() - 1].x > extent.right)
				extent.right = scanline->edges[scanline->edges.size() - 1].x;
		}
		if (extent.left < 0)
			extent.left = 0;
		if (extent.right > max_width)
			extent.right = max_width;

		return extent;
	}

	void PathFillRenderer::flush(GraphicContext &gc)
	{
		if (mask_blocks.next_block == 0) // Nothing to flush
			return;

		mask_blocks.flush_block();
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

		int block_y = (((mask_blocks.next_block-1) * mask_block_size) / mask_texture_size)* mask_block_size;
		mask_texture.set_subimage(gc, 0, 0, mask_buffer, Rect(Point(0, 0), Size(mask_texture_size, block_y + mask_block_size)));

		instance_texture.set_subimage(gc, 0, 0, instance_buffer, Rect(Point(0, 0), Size(instance_buffer_width, (instances.get_position() + instance_buffer_width - 1) / instance_buffer_width)));

		gc.set_blend_state(blend_state);
		gc.set_program_object(program_path);

		ProgramObject obj = gc.get_program_object();
		obj.set_uniform1f("ypos_scale", image_yaxis == y_axis_top_down ? 1.0f : -1.0f);

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

		// Set nothing more to flush.
		// Although this is cleared in PathMaskBuffer::reset() called by initialise_buffers(), It is still possible for this function to be called without reinitialising the buffers
		mask_blocks.next_block = 0;

		// Finished with the buffers
		mask_buffer = TransferTexture();
		mask_texture = Texture2D();
		instance_buffer = TransferTexture();
		instance_texture = Texture2D();
	}

	void PathFillRenderer::initialise_buffers(Canvas &canvas)
	{
		if (mask_texture.is_null())
		{
			GraphicContext gc = canvas.get_gc();
			mask_texture = batch_buffer->get_texture_r8(gc);
			mask_buffer = batch_buffer->get_transfer_r8(gc, mask_buffer_id);
			instance_texture = batch_buffer->get_texture_rgba32f(gc);
			instance_buffer = batch_buffer->get_transfer_rgba32f(gc);

			mask_buffer.lock(gc, access_write_discard);
			instance_buffer.lock(gc, access_write_discard);

			instances.reset(gc, instance_buffer.get_data<Vec4f>(), instance_buffer_width * instance_buffer_height);
			vertices.reset((Vec4i *)batch_buffer->buffer, max_vertices);

			mask_blocks.reset(mask_buffer.get_data_uint8(), mask_buffer.get_pitch());
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

	PathMaskBuffer::PathMaskBuffer()
	{
#ifdef __SSE2__
		mask_row_block_data = (unsigned char*)System::aligned_alloc(mask_texture_size * mask_block_size);
#endif
	}

	PathMaskBuffer::~PathMaskBuffer()
	{
#ifdef __SSE2__
		System::aligned_free(mask_row_block_data);
#endif
	}

	bool PathMaskBuffer::is_full() const
	{
		return next_block == max_blocks;
	}

	void PathMaskBuffer::reset(unsigned char *new_mask_buffer_data, int new_mask_buffer_pitch)
	{
		found_filled_block = false;
		filled_block_index = 0;
		block_index = 0;
		next_block = 0;
		mask_buffer_data = new_mask_buffer_data;
		mask_buffer_pitch = new_mask_buffer_pitch;
	}

	void PathMaskBuffer::flush_block()
	{
#ifdef __SSE2__
		int block_x = (next_block * mask_block_size) % mask_texture_size;
		if (block_x != 0)
		{
			int block_y = ((next_block * mask_block_size) / mask_texture_size) * mask_block_size;
			for (int y = 0; y < mask_block_size; y++)
			{
				__m128i *input = (__m128i*)(mask_row_block_data + mask_texture_size * y);
				__m128i *output = (__m128i*)(mask_buffer_data + mask_buffer_pitch * (block_y + y));
				for (int sse_x = 0; sse_x < mask_texture_size / 16; sse_x++)
				{
					_mm_stream_si128(&output[sse_x], _mm_load_si128(&input[sse_x]));
				}
			}
		}
#endif
	}

	void PathMaskBuffer::begin_row(PathScanline *scanlines, PathFillMode mode)
	{
		for (unsigned int cnt = 0; cnt < scanline_block_size; cnt++)
		{
			range[cnt].begin(&scanlines[cnt], mode);
		}
	}

#ifdef __SSE2__
	bool PathMaskBuffer::fill_block(int xpos)
	{
		if (is_full_block(xpos))
		{
			fill_full_block();
			return true;
		}

		const int block_size = mask_block_size / 16 * mask_block_size;
		__m128i block[block_size];

		for (auto & elem : block)
			elem = _mm_setzero_si128();

		for (unsigned int cnt = 0; cnt < scanline_block_size; cnt++)
		{
			__m128i *line = &block[mask_block_size / 16 * (cnt / antialias_level)];

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
							__m128i add_value = _mm_and_si128(mask, _mm_set1_epi8(256 / (antialias_level*antialias_level)));

							line[sse_block] = _mm_adds_epu8(line[sse_block], add_value);
						}
					}

					range[cnt].x0 = x1;	// For next time
				}
			}
		}

		__m128i empty_status = _mm_setzero_si128();
		for (auto & elem : block)
			empty_status = _mm_or_si128(empty_status, elem);

		bool empty_block = _mm_movemask_epi8(_mm_cmpeq_epi32(empty_status, _mm_setzero_si128())) == 0xffff;
		if (empty_block) return false;

		int block_x = (next_block * mask_block_size) % mask_texture_size;
		int block_y = ((next_block * mask_block_size) / mask_texture_size)* mask_block_size;

		for (unsigned int cnt = 0; cnt < mask_block_size; cnt++)
		{
			__m128i *input = &block[mask_block_size / 16 * cnt];
			__m128i *output = (__m128i*)(mask_row_block_data + cnt * mask_texture_size + block_x);

			for (int sse_block = 0; sse_block < mask_block_size / 16; sse_block++)
				_mm_store_si128(&output[sse_block], input[sse_block]);
		}

		if (((next_block + 1) % (mask_texture_size / mask_block_size) == 0))
			flush_block();

		block_index = next_block++;
		return true;
	}

	void PathMaskBuffer::fill_full_block()
	{
		if (!found_filled_block)
		{
			int block_x = (next_block * mask_block_size) % mask_texture_size;

			for (unsigned int cnt = 0; cnt < mask_block_size; cnt++)
			{
				__m128i *line = (__m128i*)(mask_row_block_data + mask_texture_size * cnt + block_x);
				for (int sse_block = 0; sse_block < mask_block_size / 16; sse_block++)
				{
					_mm_store_si128(&line[sse_block], _mm_set1_epi32(-1));
				}
			}
			if (((next_block + 1) % (mask_texture_size / mask_block_size) == 0))
				flush_block();

			found_filled_block = true;
			filled_block_index = next_block++;
		}

		block_index = filled_block_index;
	}

#else
	bool PathMaskBuffer::fill_block(int xpos)
	{
		if (is_full_block(xpos))
		{
			fill_full_block();
			return true;
		}

		int block_x = (next_block * mask_block_size) % mask_texture_size;
		int block_y = ((next_block * mask_block_size) / mask_texture_size)* mask_block_size;

		for (unsigned int cnt = 0; cnt < mask_block_size; cnt++)
		{
			unsigned char *line = mask_buffer_data + mask_buffer_pitch * (block_y + cnt) + block_x;
			memset(line, 0, mask_block_size);
		}

		bool empty_block = true;
		for (unsigned int cnt = 0; cnt < scanline_block_size; cnt++)
		{
			unsigned char *line = mask_buffer_data + mask_buffer_pitch * (block_y + cnt / antialias_level) + block_x;
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
					empty_block = false;
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
			block_index = next_block++;
		return !empty_block;
	}

	void PathMaskBuffer::fill_full_block()
	{
		if (!found_filled_block)
		{
			int block_x = (next_block * mask_block_size) % mask_texture_size;
			int block_y = ((next_block * mask_block_size) / mask_texture_size)* mask_block_size;

			for (unsigned int cnt = 0; cnt < mask_block_size; cnt++)
			{
				unsigned char *line = mask_buffer_data + mask_buffer_pitch * (block_y + cnt) + block_x;
				for (unsigned int i = 0; i < mask_block_size; i++)
					line[i] = 255;
			}

			found_filled_block = true;
			filled_block_index = next_block++;
		}

		block_index = filled_block_index;
	}
#endif

	bool PathMaskBuffer::is_full_block(int xpos) const
	{
		for (auto & elem : range)
		{
			if (!elem.found)
			{
				return false;
			}
			if ((elem.x0 > xpos) || (elem.x1 < (xpos + scanline_block_size - 1)))
			{
				return false;
			}
		}
		return true;
	}

	/////////////////////////////////////////////////////////////////////////

	void PathInstanceBuffer::reset(GraphicContext &gc, Vec4f *new_buffer, int new_max_entries)
	{
		buffer = new_buffer;
		max_entries = new_max_entries;
		current_texture = Texture2D();

		buffer[0] = Vec4f(gc.get_width(), gc.get_height(), 0, 0);
		end_position = 1;
	}

	// Return 0 when buffer is full or requires flushing, else it is the instance offset
	int PathInstanceBuffer::push(Canvas &canvas, const Brush &brush, const Mat4f &transform)
	{
		int instance_position;
		switch (brush.type)
		{
		case BrushType::solid:
			instance_position = store_solid(canvas, brush, transform);
			break;
		case BrushType::linear:
			instance_position = store_linear(canvas, brush, transform);
			break;
		case BrushType::radial:
			instance_position = store_radial(canvas, brush, transform);
			break;
		case BrushType::image:
			instance_position = store_image(canvas, brush, transform);
			break;
		default:
			throw Exception("Unknown brush");
			break;
		}
		return instance_position;
	}

	int PathInstanceBuffer::next_position(int size)
	{
		if (size > instance_buffer_width)
			throw Exception("Brush too complex");		// If brush will not exist on a single row

		// Ensure entire brush will fit in a horizontal row (to assist the pixel shader)
		if (((end_position % instance_buffer_width) + size) > instance_buffer_width)
		{
			end_position = ((end_position / instance_buffer_width) + 1) * instance_buffer_width;	// Next line
		}

		if (end_position + size > max_entries)
			return 0;		// Buffer exceeded, must flush

		int instance_position = end_position;
		end_position += size;
		return instance_position;
	}

	int PathInstanceBuffer::store_linear(Canvas &canvas, const Brush &brush, const Mat4f &transform)
	{
		int num_stops = brush.stops.size();
		int instance_position = next_position(num_stops * 2 + 3);
		if (!instance_position)
			return 0;
		int position = instance_position;

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
		brush_data2.y = 3;
		brush_data2.z = 3 + num_stops * 2;

		brush_data3.set_xy(start_point);

		buffer[position++] = brush_data1;
		buffer[position++] = brush_data2;
		buffer[position++] = brush_data3;
		for (unsigned int cnt = 0; cnt < num_stops; cnt++)
		{
			auto &color = brush.stops[cnt].color;
			buffer[position++] = Vec4f(color.r * color.a, color.g * color.a, color.b * color.a, color.a);
			buffer[position++] = Vec4f(brush.stops[cnt].position, 0.0f, 0.0f, 0.0f);
		}
		return instance_position;
	}

	int PathInstanceBuffer::store_radial(Canvas &canvas, const Brush &brush, const Mat4f &transform)
	{
		int num_stops = brush.stops.size();
		int instance_position = next_position(num_stops * 2 + 3);
		if (!instance_position)
			return 0;
		int position = instance_position;

		Pointf center_point = transform_point(brush.center_point, brush.transform, transform);
		//Pointf radius = transform_point(Pointf(brush.radius_x, brush.radius_y), brush.transform, transform) - transform_point(Pointf(), brush.transform, transform);

		Vec4f brush_data1;
		Vec4f brush_data2;
		Vec4f brush_data3;
		brush_data1.x = (float)PathShaderDrawMode::radial;
		brush_data2.x = 1.0f / brush.radius_x;
		brush_data2.y = 3;
		brush_data2.z = 3 + num_stops * 2;

		brush_data3.set_xy(center_point);

		buffer[position++] = brush_data1;
		buffer[position++] = brush_data2;
		buffer[position++] = brush_data3;
		for (unsigned int cnt = 0; cnt < num_stops; cnt++)
		{
			auto &color = brush.stops[cnt].color;
			buffer[position++] = Vec4f(color.r * color.a, color.g * color.a, color.b * color.a, color.a);
			buffer[position++] = Vec4f(brush.stops[cnt].position, 0.0f, 0.0f, 0.0f);
		}
		return instance_position;
	}

	int PathInstanceBuffer::store_image(Canvas &canvas, const Brush &brush, const Mat4f &transform)
	{
		Subtexture subtexture = brush.image.get_texture();
		if (subtexture.is_null())
			throw Exception("BrushType::image used without a valid texture");

		if (!current_texture.is_null() && subtexture.get_texture() != current_texture)
			return 0;		// Change in texture, must flush now

		int instance_position = next_position(6);
		if (!instance_position)
			return 0;
		int position = instance_position;


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
	
		buffer[position++] = Vec4f(matrix.matrix[0], matrix.matrix[1], matrix.matrix[2], matrix.matrix[3]);
		buffer[position++] = Vec4f(matrix.matrix[4], matrix.matrix[5], matrix.matrix[6], matrix.matrix[7]);
		buffer[position++] = Vec4f(matrix.matrix[8], matrix.matrix[9], matrix.matrix[10], matrix.matrix[11]);
		buffer[position++] = Vec4f(matrix.matrix[12], matrix.matrix[13], matrix.matrix[14], matrix.matrix[15]);
		return instance_position;
	}

	int PathInstanceBuffer::store_solid(Canvas &canvas, const Brush &brush, const Mat4f &transform)
	{
		int instance_position = next_position(2);
		if (!instance_position)
			return 0;
		int position = instance_position;

		Vec4f brush_data1;
		Vec4f brush_data2;
		brush_data1.x = (float)PathShaderDrawMode::solid;
		brush_data2 = brush.color;
		brush_data2.r *= brush_data2.a;
		brush_data2.g *= brush_data2.a;
		brush_data2.b *= brush_data2.a;
		buffer[position++] = brush_data1;
		buffer[position++] = brush_data2;
		return instance_position;
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
