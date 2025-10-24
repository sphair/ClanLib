/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
#include "render_batch3d.h"
#include "sprite_impl.h"

int CL_RenderBatch3D::max_textures = 4;	// For use by the GL1 target, so it can reduce the number of textures

CL_RenderBatch3D::CL_RenderBatch3D()
: modelview(CL_Mat4f::identity()), position(0), num_current_textures(0), use_glyph_program(false)
{
}

void CL_RenderBatch3D::draw_sprite(CL_GraphicContext &gc, const CL_Surface_DrawParams1 *params, const CL_Texture &texture)
{
	int texindex = set_batcher_active(gc, texture);

	to_sprite_vertex(params, 0, vertices[position++], texindex);
	to_sprite_vertex(params, 1, vertices[position++], texindex);
	to_sprite_vertex(params, 2, vertices[position++], texindex);
	to_sprite_vertex(params, 1, vertices[position++], texindex);
	to_sprite_vertex(params, 3, vertices[position++], texindex);
	to_sprite_vertex(params, 2, vertices[position++], texindex);
}

inline void CL_RenderBatch3D::to_sprite_vertex(const CL_Surface_DrawParams1 *params, int index, CL_RenderBatch3D::SpriteVertex &v, int texindex) const
{
	float x = params->dest_position[index].x;
	float y = params->dest_position[index].y;
	float z = params->destZ;
	CL_Vec3f result(
		modelview.matrix[0*4+0]*x + modelview.matrix[1*4+0]*y + modelview.matrix[2*4+0]*z + modelview.matrix[3*4+0],
		modelview.matrix[0*4+1]*x + modelview.matrix[1*4+1]*y + modelview.matrix[2*4+0]*z + modelview.matrix[3*4+1],
		modelview.matrix[0*4+2]*x + modelview.matrix[1*4+2]*y + modelview.matrix[2*4+0]*z + modelview.matrix[3*4+2]);

	v.position.x = result.x;
	v.position.y = result.y;
	v.position.z = result.z;
	v.color.r = params->color[index].r;
	v.color.g = params->color[index].g;
	v.color.b = params->color[index].b;
	v.color.a = params->color[index].a;
	v.texcoord.s = params->texture_position[index].x;
	v.texcoord.t = params->texture_position[index].y;
	v.texindex.x = (float) texindex;
}

void CL_RenderBatch3D::draw_image(CL_GraphicContext &gc, const CL_Rectf &src, const CL_Rectf &dest, const CL_Colorf &color, const CL_Texture &texture)
{
	int texindex = set_batcher_active(gc, texture);
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
	vertices[position+0].texcoord = CL_Vec2f(src_left, src_top);
	vertices[position+1].texcoord = CL_Vec2f(src_right, src_top);
	vertices[position+2].texcoord = CL_Vec2f(src_left, src_bottom);
	vertices[position+3].texcoord = CL_Vec2f(src_right, src_top);
	vertices[position+4].texcoord = CL_Vec2f(src_right, src_bottom);
	vertices[position+5].texcoord = CL_Vec2f(src_left, src_bottom);
	for (int i=0; i<6; i++)
	{
		vertices[position+i].color = CL_Vec4f(color.r, color.g, color.b, color.a);
		vertices[position+i].texindex.x = (float)texindex;
	}
	position += 6;
}

void CL_RenderBatch3D::draw_glyph_subpixel(CL_GraphicContext &gc, const CL_Rectf &src, const CL_Rectf &dest, const CL_Colorf &color, const CL_Texture &texture)
{
	int texindex = set_batcher_active(gc, texture, true, color);
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
	vertices[position+0].texcoord = CL_Vec2f(src_left, src_top);
	vertices[position+1].texcoord = CL_Vec2f(src_right, src_top);
	vertices[position+2].texcoord = CL_Vec2f(src_left, src_bottom);
	vertices[position+3].texcoord = CL_Vec2f(src_right, src_top);
	vertices[position+4].texcoord = CL_Vec2f(src_right, src_bottom);
	vertices[position+5].texcoord = CL_Vec2f(src_left, src_bottom);
	for (int i=0; i<6; i++)
	{
		vertices[position+i].color = CL_Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
		vertices[position+i].texindex.x = (float)texindex;
	}
	position += 6;
}

void CL_RenderBatch3D::fill(CL_GraphicContext &gc, float x1, float y1, float x2, float y2, const CL_Colorf &color)
{
	int texindex = set_batcher_active(gc);
	vertices[position+0].position = to_position(x1, y1);
	vertices[position+1].position = to_position(x2, y1);
	vertices[position+2].position = to_position(x1, y2);
	vertices[position+3].position = to_position(x2, y1);
	vertices[position+4].position = to_position(x2, y2);
	vertices[position+5].position = to_position(x1, y2);
	for (int i=0; i<6; i++)
	{
		vertices[position+i].color = CL_Vec4f(color.r, color.g, color.b, color.a);
		vertices[position+i].texcoord = CL_Vec2f(0.0f, 0.0f);
		vertices[position+i].texindex.x = (float)texindex;
	}
	position += 6;
}

inline CL_Vec3f CL_RenderBatch3D::to_position(float x, float y) const
{
	return CL_Vec3f(
		modelview.matrix[0*4+0]*x + modelview.matrix[1*4+0]*y + modelview.matrix[3*4+0],
		modelview.matrix[0*4+1]*x + modelview.matrix[1*4+1]*y + modelview.matrix[3*4+1],
		modelview.matrix[0*4+2]*x + modelview.matrix[1*4+2]*y + modelview.matrix[3*4+2]);
}

int CL_RenderBatch3D::set_batcher_active(CL_GraphicContext &gc, const CL_Texture &texture, bool glyph_program, const CL_Colorf &new_constant_color)
{
	if (use_glyph_program != glyph_program || constant_color != new_constant_color)
	{
		gc.flush_batcher();
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
		tex_sizes[texindex] = CL_Sizef((float)current_textures[texindex].get_width(), (float)current_textures[texindex].get_height());
	}

	if (position == 0 || position+6 > max_vertices || texindex == -1)
	{
		gc.flush_batcher();
		texindex = 0;
		current_textures[texindex] = texture;
		num_current_textures = 1;
		tex_sizes[texindex] = CL_Sizef((float)current_textures[texindex].get_width(), (float)current_textures[texindex].get_height());
	}
	gc.set_batcher(this);
	return texindex;
}


int CL_RenderBatch3D::set_batcher_active(CL_GraphicContext &gc)
{
	if (use_glyph_program != false)
	{
		gc.flush_batcher();
		use_glyph_program = false;
	}

	if (position == 0 || position+6 > max_vertices)
		gc.flush_batcher();
	gc.set_batcher(this);
	return 4;
}

void CL_RenderBatch3D::flush(CL_GraphicContext &gc)
{
	if (position > 0)
	{
		gc.set_modelview(CL_Mat4f::identity());
		gc.set_program_object(cl_program_sprite);

		if (use_glyph_program)
		{
			CL_BlendMode old_blend_mode = gc.get_blend_mode();
			CL_BlendMode blend_mode;
			blend_mode.set_blend_color(constant_color);
			blend_mode.set_blend_function(cl_blend_constant_color, cl_blend_one_minus_src_color, cl_blend_zero, cl_blend_one);
			gc.set_blend_mode(blend_mode);

			for (int i = 0; i < num_current_textures; i++)
				gc.set_texture(i, current_textures[i]);
			CL_PrimitivesArray prim_array(gc);
			prim_array.set_attributes(0, &vertices[0].position, sizeof(SpriteVertex));
			prim_array.set_attributes(1, &vertices[0].color, sizeof(SpriteVertex));
			prim_array.set_attributes(2, &vertices[0].texcoord, sizeof(SpriteVertex));
			prim_array.set_attributes(3, &vertices[0].texindex, sizeof(SpriteVertex));
			gc.draw_primitives(cl_triangles, position, prim_array);
			for (int i = 0; i < num_current_textures; i++)
				gc.reset_texture(i);

			gc.set_blend_mode(old_blend_mode);
		}
		else
		{
			for (int i = 0; i < num_current_textures; i++)
				gc.set_texture(i, current_textures[i]);
			CL_PrimitivesArray prim_array(gc);
			prim_array.set_attributes(0, &vertices[0].position, sizeof(SpriteVertex));
			prim_array.set_attributes(1, &vertices[0].color, sizeof(SpriteVertex));
			prim_array.set_attributes(2, &vertices[0].texcoord, sizeof(SpriteVertex));
			prim_array.set_attributes(3, &vertices[0].texindex, sizeof(SpriteVertex));
			gc.draw_primitives(cl_triangles, position, prim_array);
			for (int i = 0; i < num_current_textures; i++)
				gc.reset_texture(i);
		}

		gc.reset_program_object();
		gc.set_modelview(modelview);
		position = 0;
		for (int i = 0; i < num_current_textures; i++)
			current_textures[i] = CL_Texture();
		num_current_textures = 0;
	}
}

void CL_RenderBatch3D::modelview_changed(const CL_Mat4f &new_modelview)
{
	modelview = new_modelview;

}
