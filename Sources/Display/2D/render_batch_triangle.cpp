/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include "Display/precomp.h"
#include "render_batch_triangle.h"
#include "sprite_impl.h"
#include "API/Display/Render/blend_state_description.h"
#include "API/Display/2D/canvas.h"

namespace clan
{

int RenderBatchTriangle::max_textures = 4;	// For use by the GL1 target, so it can reduce the number of textures

RenderBatchTriangle::RenderBatchTriangle()
: position(0), vertices(0), current_gpu_buffer(0), num_current_textures(0), use_glyph_program(false)
{
}

void RenderBatchTriangle::draw_sprite(Canvas &canvas, const Surface_DrawParams1 *params, const Texture2D &texture)
{
	int texindex = set_batcher_active(canvas, texture);
	lock_transfer_buffer(canvas);
	to_sprite_vertex(params, 0, vertices[position++], texindex);
	to_sprite_vertex(params, 1, vertices[position++], texindex);
	to_sprite_vertex(params, 2, vertices[position++], texindex);
	to_sprite_vertex(params, 1, vertices[position++], texindex);
	to_sprite_vertex(params, 3, vertices[position++], texindex);
	to_sprite_vertex(params, 2, vertices[position++], texindex);
}

void RenderBatchTriangle::draw_triangle(Canvas &canvas, Vec2f *triangle_positions, Vec4f *triangle_colors, int num_vertices)
{
	int texindex = set_batcher_active(canvas, num_vertices);
	lock_transfer_buffer(canvas);

	for (; num_vertices > 0; num_vertices--)
	{
		vertices[position].color = (*(triangle_colors++));
		vertices[position].position = to_position(triangle_positions->x, triangle_positions->y);
		triangle_positions++;
		vertices[position].texcoord = Vec2f(0.0f, 0.0f);
		vertices[position].texindex.x = (float)texindex;
		position++;
	}
}

void RenderBatchTriangle::draw_triangle(Canvas &canvas, Vec2f *triangle_positions, const Colorf &color, int num_vertices)
{
	int texindex = set_batcher_active(canvas, num_vertices);
	lock_transfer_buffer(canvas);

	for (; num_vertices > 0; num_vertices--)
	{
		vertices[position].color = color;
		vertices[position].position = to_position(triangle_positions->x, triangle_positions->y);
		triangle_positions++;
		vertices[position].texcoord = Vec2f(0.0f, 0.0f);
		vertices[position].texindex.x = (float)texindex;
		position++;
	}
}
inline void RenderBatchTriangle::to_sprite_vertex(const Surface_DrawParams1 *params, int index, RenderBatchTriangle::SpriteVertex &v, int texindex) const
{
	v.position = to_position(params->dest_position[index].x, params->dest_position[index].y);
	v.color = params->color;

	v.texcoord.s = params->texture_position[index].x;
	v.texcoord.t = params->texture_position[index].y;
	v.texindex.x = (float) texindex;
}

void RenderBatchTriangle::draw_image(Canvas &canvas, const Rectf &src, const Rectf &dest, const Colorf &color, const Texture2D &texture)
{
	int texindex = set_batcher_active(canvas, texture);
	lock_transfer_buffer(canvas);
	vertices[position+0].position = to_position(dest.left, dest.top);
	vertices[position+1].position = to_position(dest.right, dest.top);
	vertices[position+2].position = to_position(dest.left, dest.bottom);
	vertices[position+3].position = to_position(dest.right, dest.top);
	vertices[position+4].position = to_position(dest.right, dest.bottom);
	vertices[position+5].position = to_position(dest.left, dest.bottom);
	float src_left = (src.left)/tex_sizes[texindex].width;
	float src_top = (src.top) / tex_sizes[texindex].height;
	float src_right = (src.right)/tex_sizes[texindex].width;
	float src_bottom = (src.bottom) / tex_sizes[texindex].height;
	vertices[position+0].texcoord = Vec2f(src_left, src_top);
	vertices[position+1].texcoord = Vec2f(src_right, src_top);
	vertices[position+2].texcoord = Vec2f(src_left, src_bottom);
	vertices[position+3].texcoord = Vec2f(src_right, src_top);
	vertices[position+4].texcoord = Vec2f(src_right, src_bottom);
	vertices[position+5].texcoord = Vec2f(src_left, src_bottom);
	for (int i=0; i<6; i++)
	{
		vertices[position+i].color = Vec4f(color.r, color.g, color.b, color.a);
		vertices[position+i].texindex.x = (float)texindex;
	}
	position += 6;
}

void RenderBatchTriangle::draw_glyph_subpixel(Canvas &canvas, const Rectf &src, const Rectf &dest, const Colorf &color, const Texture2D &texture)
{
	int texindex = set_batcher_active(canvas, texture, true, color);
	lock_transfer_buffer(canvas);
	vertices[position+0].position = to_position(dest.left, dest.top);
	vertices[position+1].position = to_position(dest.right, dest.top);
	vertices[position+2].position = to_position(dest.left, dest.bottom);
	vertices[position+3].position = to_position(dest.right, dest.top);
	vertices[position+4].position = to_position(dest.right, dest.bottom);
	vertices[position+5].position = to_position(dest.left, dest.bottom);
	float src_left = (src.left)/tex_sizes[texindex].width;
	float src_top = (src.top) / tex_sizes[texindex].height;
	float src_right = (src.right)/tex_sizes[texindex].width;
	float src_bottom = (src.bottom) / tex_sizes[texindex].height;
	vertices[position+0].texcoord = Vec2f(src_left, src_top);
	vertices[position+1].texcoord = Vec2f(src_right, src_top);
	vertices[position+2].texcoord = Vec2f(src_left, src_bottom);
	vertices[position+3].texcoord = Vec2f(src_right, src_top);
	vertices[position+4].texcoord = Vec2f(src_right, src_bottom);
	vertices[position+5].texcoord = Vec2f(src_left, src_bottom);
	for (int i=0; i<6; i++)
	{
		vertices[position+i].color = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
		vertices[position+i].texindex.x = (float)texindex;
	}
	position += 6;
}

void RenderBatchTriangle::fill(Canvas &canvas, float x1, float y1, float x2, float y2, const Colorf &color)
{
	int texindex = set_batcher_active(canvas);
	lock_transfer_buffer(canvas);
	vertices[position+0].position = to_position(x1, y1);
	vertices[position+1].position = to_position(x2, y1);
	vertices[position+2].position = to_position(x1, y2);
	vertices[position+3].position = to_position(x2, y1);
	vertices[position+4].position = to_position(x2, y2);
	vertices[position+5].position = to_position(x1, y2);
	for (int i=0; i<6; i++)
	{
		vertices[position+i].color = Vec4f(color.r, color.g, color.b, color.a);
		vertices[position+i].texcoord = Vec2f(0.0f, 0.0f);
		vertices[position+i].texindex.x = (float)texindex;
	}
	position += 6;
}

inline Vec4f RenderBatchTriangle::to_position(float x, float y) const
{
	return Vec4f(
		modelview_projection_matrix.matrix[0*4+0]*x + modelview_projection_matrix.matrix[1*4+0]*y + modelview_projection_matrix.matrix[3*4+0],
		modelview_projection_matrix.matrix[0*4+1]*x + modelview_projection_matrix.matrix[1*4+1]*y + modelview_projection_matrix.matrix[3*4+1],
		modelview_projection_matrix.matrix[0*4+2]*x + modelview_projection_matrix.matrix[1*4+2]*y + modelview_projection_matrix.matrix[3*4+2],
		modelview_projection_matrix.matrix[0*4+3]*x + modelview_projection_matrix.matrix[1*4+3]*y + modelview_projection_matrix.matrix[3*4+3]);
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

	if (position == 0 || position+6 > max_vertices || texindex == -1)
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

	if (position == 0 || position+6 > max_vertices)
		canvas.flush();
	canvas.set_batcher(this);
	return 4;
}

int RenderBatchTriangle::set_batcher_active(Canvas &canvas, int num_vertices)
{
	if (use_glyph_program != false)
	{
		canvas.flush();
		use_glyph_program = false;
	}

	if (position+num_vertices > max_vertices)
		canvas.flush();

	if (num_vertices > max_vertices)
		throw Exception("Too many vertices for RenderBatchTriangle");

	canvas.set_batcher(this);
	return 4;
}

void RenderBatchTriangle::lock_transfer_buffer(Canvas &canvas)
{
	if (vertices == 0)
	{
		GraphicContext &gc = canvas.get_gc();
		if (transfer_buffers[current_gpu_buffer].is_null())
			transfer_buffers[current_gpu_buffer] = TransferVector<SpriteVertex>(gc, max_vertices, usage_stream_draw);
		transfer_buffers[current_gpu_buffer].lock(gc, access_write_discard);
		vertices = transfer_buffers[current_gpu_buffer].get_data();
	}
}

void RenderBatchTriangle::flush(GraphicContext &gc)
{
	if (position > 0)
	{
		gc.set_program_object(program_sprite);

		if (gpu_vertices.is_null())
		{
			gpu_vertices = VertexArrayVector<SpriteVertex>(gc, max_vertices);
			prim_array = PrimitivesArray(gc);
			prim_array.set_attributes(0, gpu_vertices, (Vec4f*)cl_offsetof(SpriteVertex, position));
			prim_array.set_attributes(1, gpu_vertices, (Vec4f*)cl_offsetof(SpriteVertex, color));
			prim_array.set_attributes(2, gpu_vertices, (Vec2f*)cl_offsetof(SpriteVertex, texcoord));
			prim_array.set_attributes(3, gpu_vertices, (Vec1f*)cl_offsetof(SpriteVertex, texindex));

			BlendStateDescription blend_desc;
			blend_desc.set_blend_function(blend_constant_color, blend_one_minus_src_color, blend_zero, blend_one);
			glyph_blend = BlendState(gc, blend_desc);
		}

		if (vertices)
		{
			vertices = 0;
			transfer_buffers[current_gpu_buffer].unlock();
		}

		gpu_vertices.copy_from(gc, transfer_buffers[current_gpu_buffer], 0, 0, position);

		for (int i = 0; i < num_current_textures; i++)
			gc.set_texture(i, current_textures[i]);

		if (use_glyph_program)
		{
			gc.set_blend_state(glyph_blend, constant_color);
			gc.draw_primitives(type_triangles, position, prim_array);
			gc.reset_blend_state();
		}
		else
		{
			gc.draw_primitives(type_triangles, position, prim_array);
		}

		for (int i = 0; i < num_current_textures; i++)
			gc.reset_texture(i);

		gc.reset_program_object();

		position = 0;
		for (int i = 0; i < num_current_textures; i++)
			current_textures[i] = Texture2D();
		num_current_textures = 0;

		current_gpu_buffer++;
		if (current_gpu_buffer == num_gpu_buffers)
			current_gpu_buffer = 0;
	}
}

void RenderBatchTriangle::matrix_changed(const Mat4f &new_modelview, const Mat4f &new_projection)
{
	modelview_projection_matrix = new_projection * new_modelview;
}

}
