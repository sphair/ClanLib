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

#pragma once

#include "API/Display/Render/render_batcher.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Render/blend_state.h"

namespace clan
{

class RenderBatchLine : public RenderBatcher
{
public:
	RenderBatchLine();
	void draw_line_strip(Canvas &canvas, Vec2f *line_positions, const Vec4f &line_color, int num_vertices);

private:
	struct LineVertex
	{
		Vec4f position;
		Vec4f color;
	};

	inline Vec4f to_position(float x, float y) const;
	void set_batcher_active(Canvas &canvas, int num_vertices);
	void flush(GraphicContext &gc);
	void matrix_changed(const Mat4f &modelview, const Mat4f &projection);
	void lock_transfer_buffer(Canvas &canvas);

	enum { max_vertices = 4096, num_gpu_buffers = 4 };
	LineVertex *vertices;
	TransferVector<LineVertex> transfer_buffers[num_gpu_buffers];
	VertexArrayVector<LineVertex> gpu_vertices;
	PrimitivesArray prim_array;
	int current_gpu_buffer;
	int position;
	Mat4f modelview_projection_matrix;


};

}
