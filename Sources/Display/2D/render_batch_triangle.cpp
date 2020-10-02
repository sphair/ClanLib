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
#include "render_batch_triangle.h"
#include "sprite_impl.h"
#include "API/Display/Render/blend_state_description.h"
#include "API/Display/2D/canvas.h"
#include "API/Core/Math/quad.h"

namespace clan
{
	// For use by the GL1 target, so it can reduce the number of textures. Now also used by the GL3 target to increase the number. Global vars should be banned!
	// Warning: Ensure this number does not exceed RenderBatchTriangle::max_number_of_texture_coords
	int RenderBatchTriangle::max_textures = 4;

	RenderBatchTriangle::RenderBatchTriangle(GraphicContext &gc, RenderBatchBuffer *batch_buffer)
		: batch_buffer(batch_buffer)
	{
		vertices = (SpriteVertex *)batch_buffer->buffer;
	}

	void RenderBatchTriangle::draw_sprite(Canvas &canvas, const Pointf texture_position[4], const Pointf dest_position[4], const Texture2D &texture, const Colorf &color)
	{
		int texindex = set_batcher_active(canvas, texture);

		to_sprite_vertex(texture_position[0], dest_position[0], vertices[position++], texindex, color);
		to_sprite_vertex(texture_position[1], dest_position[1], vertices[position++], texindex, color);
		to_sprite_vertex(texture_position[2], dest_position[2], vertices[position++], texindex, color);
		to_sprite_vertex(texture_position[1], dest_position[1], vertices[position++], texindex, color);
		to_sprite_vertex(texture_position[3], dest_position[3], vertices[position++], texindex, color);
		to_sprite_vertex(texture_position[2], dest_position[2], vertices[position++], texindex, color);
	}

	void RenderBatchTriangle::fill_triangle(Canvas &canvas, const Vec2f *triangle_positions, const Vec4f *triangle_colors, int num_vertices)
	{
		int texindex = set_batcher_active(canvas, num_vertices);


		for (; num_vertices > 0; num_vertices--)
		{
			vertices[position].color = (*(triangle_colors++));
			vertices[position].position = to_position(triangle_positions->x, triangle_positions->y);
			triangle_positions++;
			vertices[position].texcoord = Vec2f(0.0f, 0.0f);
			vertices[position].texindex = texindex;
			position++;
		}
	}

	void RenderBatchTriangle::fill_triangle(Canvas &canvas, const Vec2f *triangle_positions, const Colorf &color, int num_vertices)
	{
		int texindex = set_batcher_active(canvas, num_vertices);


		for (; num_vertices > 0; num_vertices--)
		{
			vertices[position].color = color;
			vertices[position].position = to_position(triangle_positions->x, triangle_positions->y);
			triangle_positions++;
			vertices[position].texcoord = Vec2f(0.0f, 0.0f);
			vertices[position].texindex = texindex;
			position++;
		}
	}

	void RenderBatchTriangle::fill_triangles(Canvas &canvas, const Vec2f *positions, const Vec2f *texture_positions, int num_vertices, const Texture2D &texture, const Colorf &color)
	{
		int texindex = set_batcher_active(canvas, texture);

		for (; num_vertices > 0; num_vertices--)
		{
			vertices[position].color = color;
			vertices[position].position = to_position(positions->x, positions->y);
			positions++;
			vertices[position].texcoord = *(texture_positions++);
			vertices[position].texindex = texindex;
			position++;
		}

	}

	void RenderBatchTriangle::fill_triangles(Canvas &canvas, const Vec2f *positions, const Vec2f *texture_positions, int num_vertices, const Texture2D &texture, const Colorf *colors)
	{
		int texindex = set_batcher_active(canvas, texture);

		for (; num_vertices > 0; num_vertices--)
		{
			vertices[position].color = *(colors++);
			vertices[position].position = to_position(positions->x, positions->y);
			positions++;
			vertices[position].texcoord = *(texture_positions++);
			vertices[position].texindex = texindex;
			position++;
		}
	}

	inline void RenderBatchTriangle::to_sprite_vertex(const Pointf &texture_position, const Pointf &dest_position, RenderBatchTriangle::SpriteVertex &v, int texindex, const Colorf &color) const
	{
		v.position = to_position(dest_position.x, dest_position.y);
		v.color = color;
		v.texcoord = texture_position;
		v.texindex = texindex;
	}

	void RenderBatchTriangle::draw_image(Canvas &canvas, const Rectf &src, const Rectf &dest, const Colorf &color, const Texture2D &texture)
	{
		int texindex = set_batcher_active(canvas, texture);

		vertices[position + 0].position = to_position(dest.left, dest.top);
		vertices[position + 1].position = to_position(dest.right, dest.top);
		vertices[position + 2].position = to_position(dest.left, dest.bottom);
		vertices[position + 3].position = to_position(dest.right, dest.top);
		vertices[position + 4].position = to_position(dest.right, dest.bottom);
		vertices[position + 5].position = to_position(dest.left, dest.bottom);
		float src_left = (src.left) / tex_sizes[texindex].width;
		float src_top = (src.top) / tex_sizes[texindex].height;
		float src_right = (src.right) / tex_sizes[texindex].width;
		float src_bottom = (src.bottom) / tex_sizes[texindex].height;
		vertices[position + 0].texcoord = Vec2f(src_left, src_top);
		vertices[position + 1].texcoord = Vec2f(src_right, src_top);
		vertices[position + 2].texcoord = Vec2f(src_left, src_bottom);
		vertices[position + 3].texcoord = Vec2f(src_right, src_top);
		vertices[position + 4].texcoord = Vec2f(src_right, src_bottom);
		vertices[position + 5].texcoord = Vec2f(src_left, src_bottom);
		for (int i = 0; i < 6; i++)
		{
			vertices[position + i].color = Vec4f(color.r, color.g, color.b, color.a);
			vertices[position + i].texindex = texindex;
		}
		position += 6;
	}

	void RenderBatchTriangle::draw_image(Canvas &canvas, const Rectf &src, const Quadf &dest, const Colorf &color, const Texture2D &texture)
	{
		int texindex = set_batcher_active(canvas, texture);

		vertices[position + 0].position = to_position(dest.p.x, dest.p.y);
		vertices[position + 1].position = to_position(dest.q.x, dest.q.y);
		vertices[position + 2].position = to_position(dest.s.x, dest.s.y);
		vertices[position + 3].position = to_position(dest.q.x, dest.q.y);
		vertices[position + 4].position = to_position(dest.r.x, dest.r.y);
		vertices[position + 5].position = to_position(dest.s.x, dest.s.y);
		float src_left = (src.left) / tex_sizes[texindex].width;
		float src_top = (src.top) / tex_sizes[texindex].height;
		float src_right = (src.right) / tex_sizes[texindex].width;
		float src_bottom = (src.bottom) / tex_sizes[texindex].height;
		vertices[position + 0].texcoord = Vec2f(src_left, src_top);
		vertices[position + 1].texcoord = Vec2f(src_right, src_top);
		vertices[position + 2].texcoord = Vec2f(src_left, src_bottom);
		vertices[position + 3].texcoord = Vec2f(src_right, src_top);
		vertices[position + 4].texcoord = Vec2f(src_right, src_bottom);
		vertices[position + 5].texcoord = Vec2f(src_left, src_bottom);
		for (int i = 0; i < 6; i++)
		{
			vertices[position + i].color = Vec4f(color.r, color.g, color.b, color.a);
			vertices[position + i].texindex = texindex;
		}
		position += 6;
	}

	void RenderBatchTriangle::draw_glyph_subpixel(Canvas &canvas, const Rectf &src, const Rectf &dest, const Colorf &color, const Texture2D &texture)
	{
		int texindex = set_batcher_active(canvas, texture, true, color);

		vertices[position + 0].position = to_position(dest.left, dest.top);
		vertices[position + 1].position = to_position(dest.right, dest.top);
		vertices[position + 2].position = to_position(dest.left, dest.bottom);
		vertices[position + 3].position = to_position(dest.right, dest.top);
		vertices[position + 4].position = to_position(dest.right, dest.bottom);
		vertices[position + 5].position = to_position(dest.left, dest.bottom);
		float src_left = (src.left) / tex_sizes[texindex].width;
		float src_top = (src.top) / tex_sizes[texindex].height;
		float src_right = (src.right) / tex_sizes[texindex].width;
		float src_bottom = (src.bottom) / tex_sizes[texindex].height;
		vertices[position + 0].texcoord = Vec2f(src_left, src_top);
		vertices[position + 1].texcoord = Vec2f(src_right, src_top);
		vertices[position + 2].texcoord = Vec2f(src_left, src_bottom);
		vertices[position + 3].texcoord = Vec2f(src_right, src_top);
		vertices[position + 4].texcoord = Vec2f(src_right, src_bottom);
		vertices[position + 5].texcoord = Vec2f(src_left, src_bottom);
		for (int i = 0; i < 6; i++)
		{
			vertices[position + i].color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
			vertices[position + i].texindex = texindex;
		}
		position += 6;
	}

	void RenderBatchTriangle::fill(Canvas &canvas, float x1, float y1, float x2, float y2, const Colorf &color)
	{
		int texindex = set_batcher_active(canvas);

		vertices[position + 0].position = to_position(x1, y1);
		vertices[position + 1].position = to_position(x2, y1);
		vertices[position + 2].position = to_position(x1, y2);
		vertices[position + 3].position = to_position(x2, y1);
		vertices[position + 4].position = to_position(x2, y2);
		vertices[position + 5].position = to_position(x1, y2);
		for (int i = 0; i < 6; i++)
		{
			vertices[position + i].color = Vec4f(color.r, color.g, color.b, color.a);
			vertices[position + i].texcoord = Vec2f(0.0f, 0.0f);
			vertices[position + i].texindex = texindex;
		}
		position += 6;
	}

	inline Vec4f RenderBatchTriangle::to_position(float x, float y) const
	{
		return Vec4f(
			modelview_projection_matrix.matrix[0 * 4 + 0] * x + modelview_projection_matrix.matrix[1 * 4 + 0] * y + modelview_projection_matrix.matrix[3 * 4 + 0],
			modelview_projection_matrix.matrix[0 * 4 + 1] * x + modelview_projection_matrix.matrix[1 * 4 + 1] * y + modelview_projection_matrix.matrix[3 * 4 + 1],
			modelview_projection_matrix.matrix[0 * 4 + 2] * x + modelview_projection_matrix.matrix[1 * 4 + 2] * y + modelview_projection_matrix.matrix[3 * 4 + 2],
			modelview_projection_matrix.matrix[0 * 4 + 3] * x + modelview_projection_matrix.matrix[1 * 4 + 3] * y + modelview_projection_matrix.matrix[3 * 4 + 3]);
	}


	int RenderBatchTriangle::set_batcher_active(Canvas &canvas, const Texture2D &texture, bool glyph_program, const Colorf &new_constant_color)
	{
		if (use_glyph_program != glyph_program || constant_color != new_constant_color)
		{
			canvas.flush();
			use_glyph_program = glyph_program;
			constant_color = new_constant_color;
		}

		int texindex = -1;
		for (int i = 0; i < num_current_textures; i++)
		{
			if (current_textures[i] == texture)
			{
				texindex = i;
				break;
			}
		}
		if (texindex == -1 && num_current_textures < max_textures)
		{
			texindex = num_current_textures;
			current_textures[num_current_textures++] = texture;
			tex_sizes[texindex] = Sizef((float)current_textures[texindex].get_width(), (float)current_textures[texindex].get_height());
		}

		if (position == 0 || position + 6 > max_vertices || texindex == -1)
		{
			canvas.flush();
			texindex = 0;
			current_textures[texindex] = texture;
			num_current_textures = 1;
			tex_sizes[texindex] = Sizef((float)current_textures[texindex].get_width(), (float)current_textures[texindex].get_height());
		}
		canvas.set_batcher(this);
		return texindex;
	}

	int RenderBatchTriangle::set_batcher_active(Canvas &canvas)
	{
		if (use_glyph_program != false)
		{
			canvas.flush();
			use_glyph_program = false;
		}

		if (position == 0 || position + 6 > max_vertices)
			canvas.flush();
		canvas.set_batcher(this);
		return RenderBatchTriangle::max_textures;
	}

	int RenderBatchTriangle::set_batcher_active(Canvas &canvas, int num_vertices)
	{
		if (use_glyph_program != false)
		{
			canvas.flush();
			use_glyph_program = false;
		}

		if (position + num_vertices > max_vertices)
			canvas.flush();

		if (num_vertices > max_vertices)
			throw Exception("Too many vertices for RenderBatchTriangle");

		canvas.set_batcher(this);
		return RenderBatchTriangle::max_textures;
	}

	void RenderBatchTriangle::flush(GraphicContext &gc)
	{
		if (position > 0)
		{
			gc.set_program_object(StandardProgram::sprite);

			int gpu_index;
			VertexArrayVector<SpriteVertex> gpu_vertices(batch_buffer->get_vertex_buffer(gc, gpu_index));

			if (prim_array[gpu_index].is_null())
			{
				prim_array[gpu_index] = PrimitivesArray(gc);
				prim_array[gpu_index].set_attributes(0, gpu_vertices, cl_offsetof(SpriteVertex, position));
				prim_array[gpu_index].set_attributes(1, gpu_vertices, cl_offsetof(SpriteVertex, color));
				prim_array[gpu_index].set_attributes(2, gpu_vertices, cl_offsetof(SpriteVertex, texcoord));
				prim_array[gpu_index].set_attributes(3, gpu_vertices, cl_offsetof(SpriteVertex, texindex));

				if (glyph_blend.is_null())
				{
					BlendStateDescription blend_desc;
					blend_desc.set_blend_function(BlendFunc::constant_color, BlendFunc::one_minus_src_color, BlendFunc::zero, BlendFunc::one);
					glyph_blend = BlendState(gc, blend_desc);
				}
			}

			gpu_vertices.upload_data(gc, 0, vertices, position);

			for (int i = 0; i < num_current_textures; i++)
				gc.set_texture(i, current_textures[i]);

			if (use_glyph_program)
			{
				gc.set_blend_state(glyph_blend, constant_color);
				gc.draw_primitives(PrimitivesType::triangles, position, prim_array[gpu_index]);
				gc.reset_blend_state();
			}
			else
			{
				gc.draw_primitives(PrimitivesType::triangles, position, prim_array[gpu_index]);
			}

			for (int i = 0; i < num_current_textures; i++)
				gc.reset_texture(i);

			gc.reset_program_object();

			position = 0;
			for (int i = 0; i < num_current_textures; i++)
				current_textures[i] = Texture2D();
			num_current_textures = 0;

		}
	}

	void RenderBatchTriangle::matrix_changed(const Mat4f &new_modelview, const Mat4f &new_projection, TextureImageYAxis image_yaxis, float pixel_ratio)
	{
		modelview_projection_matrix = new_projection * new_modelview;
	}
}
