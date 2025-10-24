
#pragma once

class HSVSpriteBatch : public CL_RenderBatcher
{
public:
	HSVSpriteBatch(CL_GraphicContext &gc);

	CL_Subtexture alloc_sprite(CL_GraphicContext &gc, const CL_Size &size);
	void draw_sprite(CL_GraphicContext &gc, const CL_Rectf &dest, const CL_Rect &src, const CL_Texture &texture, float hue_offset);

	void flush(CL_GraphicContext &gc);
	void modelview_changed(const CL_Mat4f &modelview);

private:
	static CL_ProgramObject create_shader_program(CL_GraphicContext &gc);

	enum { num_vertices = 6*256 };
	CL_Vec2f positions[num_vertices];
	CL_Vec2f tex1_coords[num_vertices];
	CL_Vec1f hue_offsets[num_vertices];
	int fill_position;
	CL_Texture current_texture;

	CL_TextureGroup texture_group;
	CL_ProgramObject program;
};
