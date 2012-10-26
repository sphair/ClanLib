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
**    Mark Page
*/

#include "Display/precomp.h"
#include "render_batch_point.h"
#include "sprite_impl.h"
#include "API/Display/Render/blend_state_description.h"
#include "API/Display/2D/canvas.h"

namespace clan
{

RenderBatchPoint::RenderBatchPoint()
: current_gpu_buffer(0), position(0), vertices(0)
{
}

void RenderBatchPoint::draw_point(Canvas &canvas, Vec2f *line_positions, const Vec4f &point_color, int num_vertices)
{
	set_batcher_active(canvas, num_vertices);
	lock_transfer_buffer(canvas);

	for (; num_vertices > 0; num_vertices--)
	{
		vertices[position].color = point_color;
		vertices[position].position = to_position(line_positions->x, line_positions->y);
		line_positions++;
		position++;
	}

}

inline Vec3f RenderBatchPoint::to_position(float x, float y) const
{
	return Vec3f(
		modelview_projection_matrix.matrix[0*4+0]*x + modelview_projection_matrix.matrix[1*4+0]*y + modelview_projection_matrix.matrix[3*4+0],
		modelview_projection_matrix.matrix[0*4+1]*x + modelview_projection_matrix.matrix[1*4+1]*y + modelview_projection_matrix.matrix[3*4+1],
		modelview_projection_matrix.matrix[0*4+2]*x + modelview_projection_matrix.matrix[1*4+2]*y + modelview_projection_matrix.matrix[3*4+2]);
}

void RenderBatchPoint::set_batcher_active(Canvas &canvas, int num_vertices)
{
	if (position+num_vertices > max_vertices)
		canvas.flush();

	if (num_vertices > max_vertices)
		throw Exception("Too many vertices for RenderBatchPoint");

	canvas.set_batcher(this);
}

void RenderBatchPoint::lock_transfer_buffer(Canvas &canvas)
{
	if (vertices == 0)
	{
		GraphicContext &gc = canvas.get_gc();
		if (transfer_buffers[current_gpu_buffer].is_null())
			transfer_buffers[current_gpu_buffer] = TransferVector<PointVertex>(gc, max_vertices, usage_stream_draw);
		transfer_buffers[current_gpu_buffer].lock(gc, access_write_discard);
		vertices = transfer_buffers[current_gpu_buffer].get_data();
	}
}

void RenderBatchPoint::flush(GraphicContext &gc)
{
	if (position > 0)
	{
		gc.set_program_object(program_color_only);

		if (gpu_vertices.is_null())
		{
			gpu_vertices = VertexArrayVector<PointVertex>(gc, max_vertices);
			prim_array = PrimitivesArray(gc);
			prim_array.set_attributes(0, gpu_vertices, (Vec2f*)offsetof(PointVertex, position));
			prim_array.set_attributes(1, gpu_vertices, (Vec4f*)offsetof(PointVertex, color));
		}

		if (vertices)
		{
			vertices = 0;
			transfer_buffers[current_gpu_buffer].unlock();
		}

		gpu_vertices.copy_from(gc, transfer_buffers[current_gpu_buffer], 0, 0, position);

		gc.draw_primitives(type_points, position, prim_array);

		gc.reset_program_object();

		position = 0;

		current_gpu_buffer++;
		if (current_gpu_buffer == num_gpu_buffers)
			current_gpu_buffer = 0;
	}
}

void RenderBatchPoint::matrix_changed(const Mat4f &new_modelview, const Mat4f &new_projection)
{
	modelview_projection_matrix = new_projection * new_modelview;
}

}
