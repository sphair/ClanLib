/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
