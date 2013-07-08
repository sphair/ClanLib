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
#include "render_batch_buffer.h"
#include "sprite_impl.h"
#include "API/Display/Render/blend_state_description.h"
#include "API/Display/2D/canvas.h"

namespace clan
{

RenderBatchBuffer::RenderBatchBuffer(GraphicContext &gc) : current_vertex_buffer(0)
{
	for (int index=0; index < num_vertex_buffers; index++)
	{
		vertex_buffers[index] = VertexArrayBuffer(gc, vertex_buffer_size, usage_stream_draw);
	}
}

VertexArrayBuffer RenderBatchBuffer::get_vertex_buffer(GraphicContext &gc, int &out_index)
{
	out_index = current_vertex_buffer;

	current_vertex_buffer++;
	if (current_vertex_buffer == num_vertex_buffers)
		current_vertex_buffer = 0;

	return vertex_buffers[out_index];
}

}
