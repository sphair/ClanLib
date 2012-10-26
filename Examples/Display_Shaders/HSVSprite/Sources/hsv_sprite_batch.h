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

#pragma once

class HSVSpriteBatch : public RenderBatcher
{
public:
	HSVSpriteBatch(GraphicContext &gc);

	Subtexture alloc_sprite(GraphicContext &gc, const Size &size);
	void draw_sprite(GraphicContext &gc, const Rectf &dest, const Rect &src, const Texture &texture, float hue_offset);

	void flush(GraphicContext &gc);
	void modelview_changed(const Mat4f &modelview);

private:
	static ProgramObject create_shader_program(GraphicContext &gc);

	enum { num_vertices = 6*256 };
	Vec2f positions[num_vertices];
	Vec2f tex1_coords[num_vertices];
	Vec1f hue_offsets[num_vertices];
	int fill_position;
	Texture current_texture;

	TextureGroup texture_group;
	ProgramObject program;
};
