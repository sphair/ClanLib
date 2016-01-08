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
*/

#pragma once

class HSVSpriteBatch : public RenderBatcher
{
public:
	HSVSpriteBatch(GraphicContext &gc);

	Subtexture alloc_sprite(Canvas &canvas, const Size &size);
	void draw_sprite(Canvas &canvas, const Rectf &dest, const Rect &src, const Texture &texture, float hue_offset);

	void flush(GraphicContext &gc) override;
	void matrix_changed(const Mat4f &modelview, const Mat4f &projection, TextureImageYAxis image_yaxis, float dpi) override;

private:
	static ProgramObject create_shader_program(GraphicContext &gc);
	inline Vec4f to_position(float x, float y) const;

	struct SpriteVertex
	{
		Vec4f position;
		Vec2f tex1_coord;
		float hue_offset;
	};

	enum { max_vertices = 6*256 };

	static const int num_vertex_buffers = 4;
	VertexArrayVector<SpriteVertex> gpu_vertices[num_vertex_buffers];
	PrimitivesArray prim_array[num_vertex_buffers];
	SpriteVertex vertices[max_vertices];
	int current_vertex_buffer;

	int fill_position;
	Texture current_texture;
	Mat4f modelview_projection_matrix;

	TextureGroup texture_group;
	ProgramObject program;

};
