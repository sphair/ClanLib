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

#pragma once

#include "API/Display/Render/render_batcher.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Render/blend_state.h"
#include "render_batch_buffer.h"

namespace clan
{
	class RenderBatchBuffer;

	class RenderBatchPoint : public RenderBatcher
	{
	public:
		RenderBatchPoint(GraphicContext &gc, RenderBatchBuffer *batch_buffer);
		void draw_point(Canvas &canvas, Vec2f *line_positions, const Vec4f &point_color, int num_vertices);

	private:
		struct PointVertex
		{
			Vec4f position;
			Vec4f color;
		};

		inline Vec4f to_position(float x, float y) const;
		void set_batcher_active(Canvas &canvas, int num_vertices);
		void flush(GraphicContext &gc) override;
		void matrix_changed(const Mat4f &modelview, const Mat4f &projection, TextureImageYAxis image_yaxis, float pixel_ratio) override;

		enum { max_vertices = RenderBatchBuffer::vertex_buffer_size / sizeof(PointVertex) };
		PointVertex *vertices;
		RenderBatchBuffer *batch_buffer;
		PrimitivesArray prim_array[RenderBatchBuffer::num_vertex_buffers];
		int position = 0;
		Mat4f modelview_projection_matrix;
	};
}
