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
**    Mark Page
*/

#include "Display/precomp.h"
#include "render_batch_line_texture.h"
#include "sprite_impl.h"
#include "API/Display/Render/blend_state_description.h"
#include "API/Display/2D/canvas.h"

namespace clan
{
	RenderBatchLineTexture::RenderBatchLineTexture(GraphicContext &gc, RenderBatchBuffer *batch_buffer)
		: batch_buffer(batch_buffer)
	{
		vertices = (LineTextureVertex *)batch_buffer->buffer;
	}

	inline Vec4f RenderBatchLineTexture::to_position(float x, float y) const
	{
		return Vec4f(
			modelview_projection_matrix.matrix[0 * 4 + 0] * x + modelview_projection_matrix.matrix[1 * 4 + 0] * y + modelview_projection_matrix.matrix[3 * 4 + 0],
			modelview_projection_matrix.matrix[0 * 4 + 1] * x + modelview_projection_matrix.matrix[1 * 4 + 1] * y + modelview_projection_matrix.matrix[3 * 4 + 1],
			modelview_projection_matrix.matrix[0 * 4 + 2] * x + modelview_projection_matrix.matrix[1 * 4 + 2] * y + modelview_projection_matrix.matrix[3 * 4 + 2],
			modelview_projection_matrix.matrix[0 * 4 + 3] * x + modelview_projection_matrix.matrix[1 * 4 + 3] * y + modelview_projection_matrix.matrix[3 * 4 + 3]);
	}

	void RenderBatchLineTexture::draw_lines(Canvas &canvas, const Vec2f *line_positions, const Vec2f *texture_positions, int num_vertices, const Texture2D &texture, const Vec4f &line_color)
	{
		if (num_vertices < 2)
		{
			return;	// Invalid line, we ignore this. It could be null call to this function
		}

		// We convert a line strip to a line
		set_batcher_active(canvas, num_vertices, texture);


		for (; num_vertices > 0; num_vertices--)
		{
			vertices[position].color = line_color;
			vertices[position].position = to_position(line_positions->x, line_positions->y);
			vertices[position].texcoord = *texture_positions;
			line_positions++;
			texture_positions++;
			position++;
		}

	}


	void RenderBatchLineTexture::set_batcher_active(Canvas &canvas, int num_vertices, const Texture2D &texture)
	{
		if (position + num_vertices > max_vertices)
			canvas.flush();

		if (num_vertices > max_vertices)
			throw Exception("Too many vertices for RenderBatchLineTexture");


		if (!current_texture.is_null())
		{
			if (current_texture != texture)
				canvas.flush();
		}

		current_texture = texture;

		canvas.set_batcher(this);
	}

	void RenderBatchLineTexture::flush(GraphicContext &gc)
	{
		if (position > 0)
		{
			gc.set_program_object(StandardProgram::single_texture);

			int gpu_index;
			VertexArrayVector<LineTextureVertex> gpu_vertices(batch_buffer->get_vertex_buffer(gc, gpu_index));

			if (prim_array[gpu_index].is_null())
			{
				prim_array[gpu_index] = PrimitivesArray(gc);
				prim_array[gpu_index].set_attributes(0, gpu_vertices, cl_offsetof(LineTextureVertex, position));
				prim_array[gpu_index].set_attributes(1, gpu_vertices, cl_offsetof(LineTextureVertex, color));
				prim_array[gpu_index].set_attributes(2, gpu_vertices, cl_offsetof(LineTextureVertex, texcoord));
			}


			gpu_vertices.upload_data(gc, 0, vertices, position);

			gc.set_texture(0, current_texture);

			gc.draw_primitives(PrimitivesType::lines, position, prim_array[gpu_index]);

			gc.reset_program_object();

			gc.reset_texture(0);
			current_texture = Texture2D();

			position = 0;
		}

	}

	void RenderBatchLineTexture::matrix_changed(const Mat4f &new_modelview, const Mat4f &new_projection, TextureImageYAxis image_yaxis, float pixel_ratio)
	{
		modelview_projection_matrix = new_projection * new_modelview;
	}
}
