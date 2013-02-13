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
**    Magnus Norddahl
*/

#include "Scene3D/precomp.h"
#include "instances_buffer.h"

namespace clan
{


InstancesBuffer::InstancesBuffer()
	: max_offset_indexes(0), next_offset_index(0), max_vectors(0), num_vectors(0), current_buffer(0)
{
}

int InstancesBuffer::new_offset_index()
{
	return next_offset_index++;
}

void InstancesBuffer::clear()
{
	num_vectors = 0;
	current_buffer = (current_buffer + 1) % num_buffers;
}

void InstancesBuffer::add(int vectors_count)
{
	num_vectors += vectors_count;
}

void InstancesBuffer::lock(GraphicContext &gc)
{
	if (num_vectors > max_vectors)
	{
		max_vectors = num_vectors * 2;
		int pixels_width = min(max_vectors, 8192);
		int pixels_height = (max_vectors + 8191) / 8192;
		for (int i = 0; i < num_buffers; i++)
		{
			vectors[i] = Texture2D(gc, pixels_width, pixels_height, tf_rgba32f);
			vectors_transfer[i] = TransferTexture(gc, pixels_width, pixels_height, data_to_gpu, tf_rgba32f, 0, usage_stream_draw);
		}
	}

	if (next_offset_index > max_offset_indexes)
	{
		max_offset_indexes = next_offset_index * 2;
		int pixels_width = min(max_offset_indexes, 1024);
		int pixels_height = (max_offset_indexes + 1024) / 1024;
		for (int i = 0; i < num_buffers; i++)
		{
			indexes[i] = Texture2D(gc, pixels_width, pixels_height, tf_r32f);
			indexes_transfer[i] = TransferTexture(gc, pixels_width, pixels_height, data_to_gpu, tf_r32f, 0, usage_stream_draw);
		}
	}

	indexes_transfer[current_buffer].lock(gc, access_write_discard);
	vectors_transfer[current_buffer].lock(gc, access_write_discard);

	num_vectors = 0;
}

Vec4f *InstancesBuffer::upload(int offset_index, int vectors)
{
	indexes_transfer[current_buffer].get_data<float>()[offset_index] = (float)num_vectors;
	Vec4f *v = vectors_transfer[current_buffer].get_data<Vec4f>() + num_vectors;
	num_vectors += vectors;
	return v;
}

void InstancesBuffer::unlock(GraphicContext &gc)
{
	indexes_transfer[current_buffer].unlock();
	vectors_transfer[current_buffer].unlock();

	indexes[current_buffer].set_image(gc, indexes_transfer[current_buffer]);
	vectors[current_buffer].set_image(gc, vectors_transfer[current_buffer]);
}

}

