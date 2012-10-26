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

HSVSpriteBatch::HSVSpriteBatch(GraphicContext &gc)
: fill_position(0), texture_group(Size(256, 256)), program(create_shader_program(gc))
{
}

Subtexture HSVSpriteBatch::alloc_sprite(GraphicContext &gc, const Size &size)
{
	return texture_group.add(gc, size);
}

void HSVSpriteBatch::draw_sprite(GraphicContext &gc, const Rectf &dest, const Rect &src, const Texture &texture, float hue_offset)
{
	gc.set_batcher(this);
	if (current_texture != texture)
	{
		flush(gc);
		current_texture = texture;
	}

	positions[fill_position+0] = Vec2f(dest.left, dest.top);
	positions[fill_position+1] = Vec2f(dest.right, dest.top);
	positions[fill_position+2] = Vec2f(dest.left, dest.bottom);
	positions[fill_position+3] = Vec2f(dest.right, dest.top);
	positions[fill_position+4] = Vec2f(dest.left, dest.bottom);
	positions[fill_position+5] = Vec2f(dest.right, dest.bottom);

	tex1_coords[fill_position+0] = Vec2f(src.left/256.f, src.top/256.f);
	tex1_coords[fill_position+1] = Vec2f(src.right/256.f, src.top/256.f);
	tex1_coords[fill_position+2] = Vec2f(src.left/256.f, src.bottom/256.f);
	tex1_coords[fill_position+3] = Vec2f(src.right/256.f, src.top/256.f);
	tex1_coords[fill_position+4] = Vec2f(src.left/256.f, src.bottom/256.f);
	tex1_coords[fill_position+5] = Vec2f(src.right/256.f, src.bottom/256.f);

	for (int i=0; i<6; i++)
		hue_offsets[fill_position+i] = Vec1f(hue_offset);

	fill_position += 6;

	if (fill_position == num_vertices)
		flush(gc);
}

void HSVSpriteBatch::flush(GraphicContext &gc)
{
	if (fill_position > 0)
	{
		PrimitivesArray primarray(gc);
		primarray.set_attributes(0, positions);
		primarray.set_attributes(1, hue_offsets);
		primarray.set_attributes(2, tex1_coords);

		gc.set_texture(0, current_texture);
		gc.set_program_object(program, cl_program_matrix_modelview_projection);
		gc.draw_primitives(cl_triangles, fill_position, primarray);
		gc.reset_program_object();
		gc.reset_texture(0);

		fill_position = 0;
		current_texture = Texture();
	}
}

void HSVSpriteBatch::modelview_changed(const Mat4f &modelview)
{
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
