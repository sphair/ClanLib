/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#include "SWRender/precomp.h"
#include "swr_element_array_buffer_provider.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SWRenderElementArrayBufferProvider Construction:

SWRenderElementArrayBufferProvider::SWRenderElementArrayBufferProvider()
{
}

SWRenderElementArrayBufferProvider::~SWRenderElementArrayBufferProvider()
{
}

void SWRenderElementArrayBufferProvider::create(int size, BufferUsage usage)
{
}

void SWRenderElementArrayBufferProvider::create(void *data, int size, BufferUsage usage)
{
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderElementArrayBufferProvider Attributes:

void *SWRenderElementArrayBufferProvider::get_data()
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderElementArrayBufferProvider Operations:

void SWRenderElementArrayBufferProvider::lock(BufferAccess access)
{
}

void SWRenderElementArrayBufferProvider::unlock()
{
}

void SWRenderElementArrayBufferProvider::upload_data(GraphicContext &gc, const void *data, int size)
{

}
void SWRenderElementArrayBufferProvider::copy_from(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int size)
{
}
void SWRenderElementArrayBufferProvider::copy_to(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int size)
{
}


/////////////////////////////////////////////////////////////////////////////
// SWRenderElementArrayBufferProvider Implementation:

}
