/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include "Display/precomp.h"
#include "render_batch_path.h"
#include "sprite_impl.h"
#include "API/Display/Render/blend_state_description.h"
#include "API/Display/2D/canvas.h"
#include "API/Core/Math/quad.h"
#include "path_impl.h"
#include "render_batch_buffer.h"

namespace clan
{

RenderBatchPath::RenderBatchPath(GraphicContext &gc, RenderBatchBuffer *batch_buffer) : batch_buffer(batch_buffer)
{
	path_renderer = std::make_shared<PathRenderer>(gc);
}

inline Pointf RenderBatchPath::to_position(const clan::Pointf &point) const
{
	return Pointf(
		modelview_matrix.matrix[0 * 4 + 0] * point.x + modelview_matrix.matrix[1 * 4 + 0] * point.y + modelview_matrix.matrix[3 * 4 + 0],
		modelview_matrix.matrix[0 * 4 + 1] * point.x + modelview_matrix.matrix[1 * 4 + 1] * point.y + modelview_matrix.matrix[3 * 4 + 1]);
}

void RenderBatchPath::draw_path(Canvas &canvas, const Path &path, const Pen &pen, const Brush &brush, bool stroke, bool fill)
{
	canvas.flush();
	canvas.set_batcher(this);

	if (fill)
	{
		path_renderer->set_size(canvas.get_width(), canvas.get_height());

		path_renderer->clear();

		for (const auto &subpath : path.get_impl()->subpaths)
		{
			clan::Pointf previous_point = to_position(subpath.points[0]);
			clan::Pointf next_point;

			size_t i = 1;
			for (PathCommand command : subpath.commands)
			{
				if (command == PathCommand::line)
				{
					next_point = to_position(subpath.points[i]);
					i++;

					path_renderer->line(previous_point.x, previous_point.y, next_point.x, next_point.y);
				}
				else if (command == PathCommand::quadradic)
				{
					clan::Pointf control = to_position(subpath.points[i]);
					next_point = to_position(subpath.points[i + 1]);
					i += 2;

					path_renderer->quadratic_bezier(previous_point.x, previous_point.y, control.x, control.y, next_point.x, next_point.y);
				}
				else if (command == PathCommand::cubic)
				{
					clan::Pointf control1 = to_position(subpath.points[i]);
					clan::Pointf control2 = to_position(subpath.points[i + 1]);
					next_point = to_position(subpath.points[i + 2]);
					i += 3;

					path_renderer->cubic_bezier(previous_point.x, previous_point.y, control1.x, control1.y, control2.x, control2.y, next_point.x, next_point.y);
				}

				previous_point = next_point;
			}

			if (subpath.closed)
			{
				next_point = to_position(subpath.points[0]);
				path_renderer->line(previous_point.x, previous_point.y, next_point.x, next_point.y);
			}
		}

		path_renderer->fill(batch_buffer, canvas, path.get_impl()->fill_mode, brush);
	}

	// To do: add stroking
}


void RenderBatchPath::flush(GraphicContext &gc)
{
	// Path does not support batching at the moment
}

void RenderBatchPath::matrix_changed(const Mat4f &new_modelview, const Mat4f &new_projection)
{
	// We ignore the projection
	modelview_matrix = new_modelview;
}

}
