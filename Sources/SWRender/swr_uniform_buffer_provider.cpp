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
**    Mark Page
*/

#include "SWRender/precomp.h"
#include "swr_uniform_buffer_provider.h"
#include "API/Display/Render/transfer_buffer.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SWRenderUniformBufferProvider Construction:

SWRenderUniformBufferProvider::SWRenderUniformBufferProvider()
: data(0), size(0)
{
}

SWRenderUniformBufferProvider::~SWRenderUniformBufferProvider()
{
	delete[] data;
}

void SWRenderUniformBufferProvider::create(int new_size, BufferUsage usage)
{
	delete[] data;
	data = 0;
	size = 0;
	data = new char[new_size];
	size = new_size;
}

void SWRenderUniformBufferProvider::create(const void *init_data, int new_size, BufferUsage usage)
{
	delete[] data;
	data = 0;
	size = 0;
	data = new char[new_size];
	size = new_size;
	memcpy(data, init_data, size);
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderUniformBufferProvider Attributes:

/////////////////////////////////////////////////////////////////////////////
// SWRenderUniformBufferProvider Operations:

void SWRenderUniformBufferProvider::upload_data(GraphicContext &gc, const void *data, int size)
{
	if ( (size < 0) || (size > this->size) )
		throw Exception("Uniform buffer, invalid size");

	memcpy(this->data, data, size);
}

void SWRenderUniformBufferProvider::copy_from(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int size)
{
	buffer.lock(gc, access_read_only);
	memcpy(this->data + dest_pos, (char *) buffer.get_data() + src_pos, size);
	buffer.unlock();
}

void SWRenderUniformBufferProvider::copy_to(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int size)
{
	buffer.upload_data(gc, dest_pos, this->data + src_pos, size);
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderUniformBufferProvider Implementation:

}
