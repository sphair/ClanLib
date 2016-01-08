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
**    Mark Page
*/

#pragma once

#include "API/Display/Render/texture.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Render/blend_state.h"
#include "API/Display/Render/render_batcher.h"
#include "API/Display/Render/texture_2d.h"
#include "render_batch_buffer.h"

namespace clan
{
	struct Surface_DrawParams1;
	class RenderBatchBuffer;
	class Quadf;

	class RenderBatchTriangle : public RenderBatcher
	{
	public:
		RenderBatchTriangle(GraphicContext &gc, RenderBatchBuffer *batch_buffer);
		void draw_sprite(Canvas &canvas, const Pointf texture_position[4], const Pointf dest_position[4], const Texture2D &texture, const Colorf &color);
		void draw_image(Canvas &canvas, const Rectf &src, const Rectf &dest, const Colorf &color, const Texture2D &texture);
		void draw_image(Canvas &canvas, const Rectf &src, const Quadf &dest, const Colorf &color, const Texture2D &texture);
		void draw_glyph_subpixel(Canvas &canvas, const Rectf &src, const Rectf &dest, const Colorf &color, const Texture2D &texture);
		void fill_triangle(Canvas &canvas, const Vec2f *triangle_positions, const Vec4f *triangle_colors, int num_vertices);
		void fill_triangle(Canvas &canvas, const Vec2f *triangle_positions, const Colorf &color, int num_vertices);
		void fill_triangles(Canvas &canvas, const Vec2f *positions, const Vec2f *texture_positions, int num_vertices, const Texture2D &texture, const Colorf &color);
		void fill_triangles(Canvas &canvas, const Vec2f *positions, const Vec2f *texture_positions, int num_vertices, const Texture2D &texture, const Colorf *colors);
		void fill(Canvas &canvas, float x1, float y1, float x2, float y2, const Colorf &color);

	public:
		static int max_textures;	// For use by the GL1 target, so it can reduce the number of textures

	private:
		struct SpriteVertex
		{
			Vec4f position;
			Vec2f texcoord;
			Vec4f color;
			int texindex;
		};

		int set_batcher_active(Canvas &canvas, const Texture2D &texture, bool glyph_program = false, const Colorf &constant_color = Colorf::black);
		int set_batcher_active(Canvas &canvas);
		int set_batcher_active(Canvas &canvas, int num_vertices);
		void flush(GraphicContext &gc) override;
		void matrix_changed(const Mat4f &modelview, const Mat4f &projection, TextureImageYAxis image_yaxis, float pixel_ratio) override;

		inline void to_sprite_vertex(const Pointf &texture_position, const Pointf &dest_position, RenderBatchTriangle::SpriteVertex &v, int texindex, const Colorf &color) const;
		inline Vec4f to_position(float x, float y) const;

		Mat4f modelview_projection_matrix;
		int position = 0;
		enum { max_vertices = RenderBatchBuffer::vertex_buffer_size / sizeof(SpriteVertex) };
		SpriteVertex *vertices;

		RenderBatchBuffer *batch_buffer;

		PrimitivesArray prim_array[RenderBatchBuffer::num_vertex_buffers];

		static const int max_number_of_texture_coords = 32;

		Texture2D current_textures[max_number_of_texture_coords];
		int num_current_textures = 0;
		Sizef tex_sizes[max_number_of_texture_coords];
		bool use_glyph_program = false;
		Colorf constant_color;
		BlendState glyph_blend;
	};
}
