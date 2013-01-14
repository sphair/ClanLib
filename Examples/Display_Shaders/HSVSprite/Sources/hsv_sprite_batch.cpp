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

#include "precomp.h"
#include "hsv_sprite_batch.h"

HSVSpriteBatch::HSVSpriteBatch(Canvas &canvas)
: fill_position(0), texture_group(Size(256, 256)), program(create_shader_program(canvas)), vertices(0)
{
}

Subtexture HSVSpriteBatch::alloc_sprite(Canvas &canvas, const Size &size)
{
	return texture_group.add(canvas, size);
}

inline Vec4f HSVSpriteBatch::to_position(float x, float y) const
{
	return Vec4f(
		modelview_projection_matrix.matrix[0*4+0]*x + modelview_projection_matrix.matrix[1*4+0]*y + modelview_projection_matrix.matrix[3*4+0],
		modelview_projection_matrix.matrix[0*4+1]*x + modelview_projection_matrix.matrix[1*4+1]*y + modelview_projection_matrix.matrix[3*4+1],
		modelview_projection_matrix.matrix[0*4+2]*x + modelview_projection_matrix.matrix[1*4+2]*y + modelview_projection_matrix.matrix[3*4+2],
		modelview_projection_matrix.matrix[0*4+3]*x + modelview_projection_matrix.matrix[1*4+3]*y + modelview_projection_matrix.matrix[3*4+3]);
}

void HSVSpriteBatch::draw_sprite(Canvas &canvas, const Rectf &dest, const Rect &src, const Texture &texture, float hue_offset)
{
	canvas.set_batcher(this);
	if (current_texture != texture)
	{
		flush(canvas);
		current_texture = texture;
	}
	lock_transfer_buffer(canvas);

	vertices[fill_position+0].position = to_position(dest.left, dest.top);
	vertices[fill_position+1].position = to_position(dest.right, dest.top);
	vertices[fill_position+2].position = to_position(dest.left, dest.bottom);
	vertices[fill_position+3].position = to_position(dest.right, dest.top);
	vertices[fill_position+4].position = to_position(dest.left, dest.bottom);
	vertices[fill_position+5].position = to_position(dest.right, dest.bottom);

	vertices[fill_position+0].tex1_coord = Vec2f(src.left/256.f, src.top/256.f);
	vertices[fill_position+1].tex1_coord = Vec2f(src.right/256.f, src.top/256.f);
	vertices[fill_position+2].tex1_coord = Vec2f(src.left/256.f, src.bottom/256.f);
	vertices[fill_position+3].tex1_coord = Vec2f(src.right/256.f, src.top/256.f);
	vertices[fill_position+4].tex1_coord = Vec2f(src.left/256.f, src.bottom/256.f);
	vertices[fill_position+5].tex1_coord = Vec2f(src.right/256.f, src.bottom/256.f);

	for (int i=0; i<6; i++)
		vertices[fill_position+i].hue_offset = Vec1f(hue_offset);

	fill_position += 6;

	if (fill_position == max_vertices)
		flush(canvas);
}

void HSVSpriteBatch::lock_transfer_buffer(Canvas &canvas)
{
	if (vertices == 0)
	{
		GraphicContext &gc = canvas.get_gc();
		if(transfer_buffers.is_null())
		{
			transfer_buffers = TransferVector<SpriteVertex>(gc, max_vertices);
		}
		transfer_buffers.lock(gc, access_write_discard);
		vertices = transfer_buffers.get_data();
	}
}

void HSVSpriteBatch::flush(GraphicContext &gc)
{
	if (fill_position > 0)
	{
		gc.set_program_object(program);

		if (gpu_vertices.is_null())
		{
			gpu_vertices = VertexArrayVector<SpriteVertex>(gc, max_vertices);
			prim_array = PrimitivesArray(gc);
			prim_array.set_attributes(0, gpu_vertices, (Vec4f*)cl_offsetof(SpriteVertex, position));
			prim_array.set_attributes(1, gpu_vertices, (Vec2f*)cl_offsetof(SpriteVertex, tex1_coord));
			prim_array.set_attributes(2, gpu_vertices, (Vec1f*)cl_offsetof(SpriteVertex, hue_offset));
		}

		if (vertices)
		{
			vertices = 0;
			transfer_buffers.unlock();
		}
		gpu_vertices.copy_from(gc, transfer_buffers, 0, 0, fill_position);

		gc.set_texture(0, current_texture);
		gc.draw_primitives(type_triangles, fill_position, prim_array);
		gc.reset_program_object();
		gc.reset_texture(0);

		fill_position = 0;
		current_texture = Texture();
	}
}

void HSVSpriteBatch::matrix_changed(const Mat4f &new_modelview, const Mat4f &new_projection)
{
	modelview_projection_matrix = new_projection * new_modelview;
}

ProgramObject HSVSpriteBatch::create_shader_program(Canvas &canvas)
{
	ProgramObject program = ProgramObject::load(canvas, "Resources/vertex.glsl", "Resources/fragment.glsl");
	program.bind_attribute_location(0, "Position");
	program.bind_attribute_location(1, "HueOffset0");
	program.bind_attribute_location(2, "TexCoord0");
	if (!program.link())
		throw Exception("Unable to link program");
	return program;
}
