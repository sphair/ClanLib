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

#include "render_batch_sprite.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Render/blend_mode.h"

struct CL_Surface_DrawParams1;

class CL_RenderBatch2D : public CL_RenderBatcherSprite
{
public:
	CL_RenderBatch2D();
	void draw_sprite(CL_GraphicContext &gc, const CL_Surface_DrawParams1 *params, const CL_Texture &texture);
	void draw_image(CL_GraphicContext &gc, const CL_Rectf &src, const CL_Rectf &dest, const CL_Colorf &color, const CL_Texture &texture);
	void fill(CL_GraphicContext &gc, float x1, float y1, float x2, float y2, const CL_Colorf &color);

public:
	static int max_textures;	// For use by the GL1 target, so it can reduce the number of textures

private:
	struct SpriteVertex
	{
		CL_Vec3f position;
		CL_Vec2f texcoord;
		CL_Vec4f color;
		CL_Vec1f texindex;
	};

	int set_batcher_active(CL_GraphicContext &gc, const CL_Texture &texture);
	int set_batcher_active(CL_GraphicContext &gc);
	void flush(CL_GraphicContext &gc);
	void modelview_changed(const CL_Mat4f &modelview);
	inline void to_sprite_vertex(const CL_Surface_DrawParams1 *params, int index, CL_RenderBatch2D::SpriteVertex &v, int texindex) const;
	inline CL_Vec3f to_position(float x, float y) const;

	CL_Mat4f modelview;
	CL_Vec2f origin;
	CL_Vec2f x_dir, y_dir;
	int position;
	enum { max_vertices = 4096*3*2 };
	SpriteVertex vertices[max_vertices];
	CL_Texture current_textures[4];
	int num_current_textures;
	CL_Sizef tex_sizes[4];
};
