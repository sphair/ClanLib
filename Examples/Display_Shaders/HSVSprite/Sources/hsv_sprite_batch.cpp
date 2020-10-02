/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

HSVSpriteBatch::HSVSpriteBatch(GraphicContext &gc)
: fill_position(0), texture_group(Size(256, 256)), program(create_shader_program(gc)), current_vertex_buffer(0)
{
	for (int index=0; index < num_vertex_buffers; index++)
	{
		gpu_vertices[index] = VertexArrayVector<SpriteVertex>(gc, max_vertices, BufferUsage::stream_draw);
		prim_array[index] = PrimitivesArray(gc);
		prim_array[index].set_attributes(0, gpu_vertices[index], cl_offsetof(SpriteVertex, position));
		prim_array[index].set_attributes(1, gpu_vertices[index], cl_offsetof(SpriteVertex, hue_offset));
		prim_array[index].set_attributes(2, gpu_vertices[index], cl_offsetof(SpriteVertex, tex1_coord));
	}
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
		vertices[fill_position+i].hue_offset = hue_offset;

	fill_position += 6;

	if (fill_position == max_vertices)
		flush(canvas);
}

void HSVSpriteBatch::flush(GraphicContext &gc)
{
	if (fill_position > 0)
	{
		gc.set_program_object(program);

		gpu_vertices[current_vertex_buffer].upload_data(gc, 0, vertices, fill_position);

		gc.set_texture(0, current_texture);
		gc.draw_primitives(PrimitivesType::triangles, fill_position, prim_array[current_vertex_buffer]);
		gc.reset_program_object();
		gc.reset_texture(0);

		current_vertex_buffer++;
		if (current_vertex_buffer >= num_vertex_buffers)
			current_vertex_buffer = 0;

		fill_position = 0;
		current_texture = Texture();
	}
}

void HSVSpriteBatch::matrix_changed(const Mat4f &modelview, const Mat4f &projection, TextureImageYAxis image_yaxis, float dpi)
{
	modelview_projection_matrix = projection * modelview;
}

ProgramObject HSVSpriteBatch::create_shader_program(GraphicContext &gc)
{
	ProgramObject program = ProgramObject::load(gc, "Resources/vertex.glsl", "Resources/fragment.glsl");
	program.bind_attribute_location(0, "Position");
	program.bind_attribute_location(1, "HueOffset0");
	program.bind_attribute_location(2, "TexCoord0");
	if (!program.link())
		throw Exception("Unable to link program");
	return program;
}
