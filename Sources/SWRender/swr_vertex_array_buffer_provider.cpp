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
#include "swr_vertex_array_buffer_provider.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderVertexArrayBufferProvider Construction:

CL_SWRenderVertexArrayBufferProvider::CL_SWRenderVertexArrayBufferProvider()
{
}

CL_SWRenderVertexArrayBufferProvider::~CL_SWRenderVertexArrayBufferProvider()
{
}

void CL_SWRenderVertexArrayBufferProvider::create(int size, CL_BufferUsage usage)
{
}

void CL_SWRenderVertexArrayBufferProvider::create(void *data, int size, CL_BufferUsage usage)
{
}

void CL_SWRenderVertexArrayBufferProvider::destroy()
{
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderVertexArrayBufferProvider Attributes:

void *CL_SWRenderVertexArrayBufferProvider::get_data()
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderVertexArrayBufferProvider Operations:

void CL_SWRenderVertexArrayBufferProvider::lock(CL_BufferAccess access)
{
}

void CL_SWRenderVertexArrayBufferProvider::unlock()
{
}

void CL_SWRenderVertexArrayBufferProvider::upload_data(int offset, void *data, int size)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderVertexArrayBufferProvider Implementation:

}
