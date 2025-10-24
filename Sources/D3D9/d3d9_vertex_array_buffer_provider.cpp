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

#include "D3D9/precomp.h"
#include "d3d9_vertex_array_buffer_provider.h"
#include "d3d9_display_window_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9VertexArrayBufferProvider Construction:

CL_D3D9VertexArrayBufferProvider::CL_D3D9VertexArrayBufferProvider(CL_D3D9DisplayWindowProvider *window)
: window(window), data_ptr(0)
{
}

CL_D3D9VertexArrayBufferProvider::~CL_D3D9VertexArrayBufferProvider()
{
}

void CL_D3D9VertexArrayBufferProvider::create(int size, CL_BufferUsage usage)
{
	DWORD flags = D3DCREATE_MIXED_VERTEXPROCESSING;
	switch (usage)
	{
	case cl_usage_stream_draw:
		flags |= D3DUSAGE_WRITEONLY;
		break;
	case cl_usage_stream_read:
		break;
	case cl_usage_stream_copy:
		flags |= D3DUSAGE_WRITEONLY;
		break;
	case cl_usage_static_draw:
		flags |= D3DUSAGE_WRITEONLY;
		break;
	case cl_usage_static_read:
		break;
	case cl_usage_static_copy:
		flags |= D3DUSAGE_WRITEONLY;
		break;
	case cl_usage_dynamic_draw:
		flags |= D3DUSAGE_DYNAMIC;
		flags |= D3DUSAGE_WRITEONLY;
		break;
	case cl_usage_dynamic_read:
		flags |= D3DUSAGE_DYNAMIC;
		break;
	case cl_usage_dynamic_copy:
		flags |= D3DUSAGE_DYNAMIC;
		flags |= D3DUSAGE_WRITEONLY;
		break;
	}
	HRESULT result = window->get_device()->CreateVertexBuffer(size, flags, 0, D3DPOOL_DEFAULT, vertex_buffer.output_variable(), 0);
	if (FAILED(result))
		throw CL_Exception("Unable to create vertex buffer");
}

void CL_D3D9VertexArrayBufferProvider::create(void *data, int size, CL_BufferUsage usage)
{
	create(size, usage);
	lock(cl_access_write_only);
	memcpy(get_data(), data, size);
	unlock();
}

void CL_D3D9VertexArrayBufferProvider::destroy()
{
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9VertexArrayBufferProvider Attributes:

void *CL_D3D9VertexArrayBufferProvider::get_data()
{
	return data_ptr;
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9VertexArrayBufferProvider Operations:

void CL_D3D9VertexArrayBufferProvider::lock(CL_BufferAccess access)
{
	DWORD flags = 0;
	switch (access)
	{
	case cl_access_read_only:
		flags |= D3DLOCK_READONLY;
		break;
	case cl_access_write_only:
		break;
	case cl_access_read_write:
		break;
	}

	HRESULT result = vertex_buffer->Lock(0, 0, &data_ptr, flags);
	if (FAILED(result))
		throw CL_Exception("Unable to lock vertex buffer");
}

void CL_D3D9VertexArrayBufferProvider::unlock()
{
	HRESULT result = vertex_buffer->Unlock();
	if (FAILED(result))
		throw CL_Exception("Unable to unlock vertex buffer");
	data_ptr = 0;
}

void CL_D3D9VertexArrayBufferProvider::upload_data(int offset, void *data, int size)
{
	HRESULT result = vertex_buffer->Lock(offset, size, &data_ptr, 0);
	if (FAILED(result))
		throw CL_Exception("Unable to lock vertex buffer");

	memcpy(data_ptr, data, size);

	result = vertex_buffer->Unlock();
	if (FAILED(result))
		throw CL_Exception("Unable to unlock vertex buffer");
	data_ptr = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9VertexArrayBufferProvider Implementation:
