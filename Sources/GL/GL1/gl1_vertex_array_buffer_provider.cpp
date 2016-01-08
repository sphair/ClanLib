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
*/

#include "GL/precomp.h"
#include "gl1_vertex_array_buffer_provider.h"
#include "API/Display/Render/transfer_buffer.h"

namespace clan
{
	GL1VertexArrayBufferProvider::GL1VertexArrayBufferProvider()
		: data(nullptr), size(0)
	{
	}

	GL1VertexArrayBufferProvider::~GL1VertexArrayBufferProvider()
	{
		delete[] data;
	}

	void GL1VertexArrayBufferProvider::create(int new_size, BufferUsage usage)
	{
		delete[] data;
		data = nullptr;
		size = 0;
		data = new char[new_size];
		size = new_size;
	}

	void GL1VertexArrayBufferProvider::create(void *init_data, int new_size, BufferUsage usage)
	{
		delete[] data;
		data = nullptr;
		size = 0;
		data = new char[new_size];
		size = new_size;
		memcpy(data, init_data, size);
	}

	void GL1VertexArrayBufferProvider::upload_data(GraphicContext &gc, int offset, const void *new_data, int new_size)
	{
		if ((offset < 0) || (new_size < 0) || ((new_size + offset) > size))
			throw Exception("Vertex array buffer, invalid size");

		memcpy(data + offset, new_data, new_size);
	}

	void GL1VertexArrayBufferProvider::copy_from(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int size)
	{
		buffer.lock(gc, access_read_only);
		memcpy(this->data + dest_pos, (char *)buffer.get_data() + src_pos, size);
		buffer.unlock();
	}

	void GL1VertexArrayBufferProvider::copy_to(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int size)
	{
		buffer.upload_data(gc, dest_pos, this->data + src_pos, size);
	}
}
