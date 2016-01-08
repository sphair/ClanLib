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

#include "Display/precomp.h"
#include "render_batch_buffer.h"
#include "sprite_impl.h"
#include "API/Display/Render/blend_state_description.h"
#include "API/Display/2D/canvas.h"

namespace clan
{
	RenderBatchBuffer::RenderBatchBuffer(GraphicContext &gc)
	{
		for (auto & elem : vertex_buffers)
		{
			elem = VertexArrayBuffer(gc, vertex_buffer_size, usage_stream_draw);
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

	Texture2D RenderBatchBuffer::get_texture_rgba32f(GraphicContext &gc)
	{
		current_rgba32f_texture++;
		if (current_rgba32f_texture == num_r8_buffers)
			current_rgba32f_texture = 0;

		if (textures_rgba32f[current_rgba32f_texture].is_null())
		{
			textures_rgba32f[current_rgba32f_texture] = Texture2D(gc, rgba32f_width, rgba32f_height, tf_rgba32f);
			textures_rgba32f[current_rgba32f_texture].set_min_filter(filter_nearest);
			textures_rgba32f[current_rgba32f_texture].set_mag_filter(filter_nearest);

		}

		return textures_rgba32f[current_rgba32f_texture];
	}

	Texture2D RenderBatchBuffer::get_texture_r8(GraphicContext &gc)
	{
		current_r8_texture++;
		if (current_r8_texture == num_r8_buffers)
			current_r8_texture = 0;

		if (textures_r8[current_r8_texture].is_null())
		{
			textures_r8[current_r8_texture] = Texture2D(gc, r8_size, r8_size, tf_r8);
			textures_r8[current_r8_texture].set_min_filter(filter_nearest);
			textures_r8[current_r8_texture].set_mag_filter(filter_nearest);
		}
		return textures_r8[current_r8_texture];
	}

	TransferTexture RenderBatchBuffer::get_transfer_rgba32f(GraphicContext &gc)
	{
		current_rgba32f_transfer++;
		if (current_rgba32f_transfer == num_r8_buffers)
			current_rgba32f_transfer = 0;

		if (transfers_rgba32f[current_rgba32f_transfer].is_null())
			transfers_rgba32f[current_rgba32f_transfer] = TransferTexture(gc, rgba32f_width, rgba32f_height, data_to_gpu, tf_rgba32f);

		return transfers_rgba32f[current_rgba32f_transfer];
	}

	TransferTexture RenderBatchBuffer::get_transfer_r8(GraphicContext &gc, int &out_index)
	{
		current_r8_transfer++;
		if (current_r8_transfer == num_r8_buffers)
			current_r8_transfer = 0;

		if (transfers_r8[current_r8_transfer].is_null())
		{
			transfers_r8[current_r8_transfer] = TransferTexture(gc, r8_size, r8_size, data_to_gpu, tf_r8);
		}
		out_index = current_r8_transfer;

		return transfers_r8[current_r8_transfer];
	}
}
