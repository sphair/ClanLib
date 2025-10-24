
#include "precomp.h"
#include "hsv_sprite_batch.h"

HSVSpriteBatch::HSVSpriteBatch(CL_GraphicContext &gc)
: fill_position(0), texture_group(gc, CL_Size(256, 256)), program(create_shader_program(gc))
{
}

CL_Subtexture HSVSpriteBatch::alloc_sprite(CL_GraphicContext &gc, const CL_Size &size)
{
	return texture_group.add(gc, size);
}

void HSVSpriteBatch::draw_sprite(CL_GraphicContext &gc, const CL_Rectf &dest, const CL_Rect &src, const CL_Texture &texture, float hue_offset)
{
	gc.set_batcher(this);
	if (current_texture != texture)
	{
		flush(gc);
		current_texture = texture;
	}

	positions[fill_position+0] = CL_Vec2f(dest.left, dest.top);
	positions[fill_position+1] = CL_Vec2f(dest.right, dest.top);
	positions[fill_position+2] = CL_Vec2f(dest.left, dest.bottom);
	positions[fill_position+3] = CL_Vec2f(dest.right, dest.top);
	positions[fill_position+4] = CL_Vec2f(dest.left, dest.bottom);
	positions[fill_position+5] = CL_Vec2f(dest.right, dest.bottom);

	tex1_coords[fill_position+0] = CL_Vec2f(src.left/256.f, src.top/256.f);
	tex1_coords[fill_position+1] = CL_Vec2f(src.right/256.f, src.top/256.f);
	tex1_coords[fill_position+2] = CL_Vec2f(src.left/256.f, src.bottom/256.f);
	tex1_coords[fill_position+3] = CL_Vec2f(src.right/256.f, src.top/256.f);
	tex1_coords[fill_position+4] = CL_Vec2f(src.left/256.f, src.bottom/256.f);
	tex1_coords[fill_position+5] = CL_Vec2f(src.right/256.f, src.bottom/256.f);

	for (int i=0; i<6; i++)
		hue_offsets[fill_position+i] = hue_offset;

	fill_position += 6;

	if (fill_position == num_vertices)
		flush(gc);
}

void HSVSpriteBatch::flush(CL_GraphicContext &gc)
{
	if (fill_position > 0)
	{
		CL_PrimitivesArray primarray(gc);
		primarray.set_attributes(0, positions);
		primarray.set_attributes(1, hue_offsets);
		primarray.set_attributes(2, tex1_coords);

		gc.set_texture(0, current_texture);
		gc.set_program_object(program);
		gc.draw_primitives(cl_triangles, fill_position, primarray);
		gc.reset_program_object();
		gc.reset_texture(0);

		fill_position = 0;
		current_texture = CL_Texture();
	}
}

void HSVSpriteBatch::modelview_changed(const CL_Mat4f &modelview)
{
}

CL_ProgramObject HSVSpriteBatch::create_shader_program(CL_GraphicContext &gc)
{
	CL_ProgramObject program = CL_ProgramObject::load(gc, "vertex.glsl", "fragment.glsl");
	program.bind_attribute_location(0, "Position");
	program.bind_attribute_location(1, "HueOffset0");
	program.bind_attribute_location(2, "TexCoord0");
	if (!program.link())
		throw CL_Exception("Unable to link program");
	return program;
}
