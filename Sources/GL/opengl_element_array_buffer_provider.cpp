/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
#include "opengl_element_array_buffer_provider.h"
#include "opengl_graphic_context_provider.h"
#include "API/GL/opengl_wrap.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLElementArrayBufferProvider Construction:

CL_OpenGLElementArrayBufferProvider::CL_OpenGLElementArrayBufferProvider(CL_OpenGLGraphicContextProvider *gc_provider)
: gc_provider(gc_provider), handle(0), data_ptr(0)
{
	CL_OpenGL::set_active(gc_provider);
	clGenBuffers(1, &handle);
}

CL_OpenGLElementArrayBufferProvider::~CL_OpenGLElementArrayBufferProvider()
{
	CL_OpenGL::set_active(gc_provider);
	clDeleteBuffers(1, &handle);
}

void CL_OpenGLElementArrayBufferProvider::create(int size, CL_BufferUsage usage)
{
	create(0, size, usage);
}

void CL_OpenGLElementArrayBufferProvider::create(void *data, int size, CL_BufferUsage usage)
{
	CL_OpenGL::set_active(gc_provider);
	CLint last_buffer = 0;
	clGetIntegerv(CL_ELEMENT_ARRAY_BUFFER_BINDING, &last_buffer);
	clBindBuffer(CL_ELEMENT_ARRAY_BUFFER, handle);
	clBufferData(CL_ELEMENT_ARRAY_BUFFER, size, data, to_enum(usage));
	clBindBuffer(CL_ELEMENT_ARRAY_BUFFER, last_buffer);
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLElementArrayBufferProvider Attributes:

void *CL_OpenGLElementArrayBufferProvider::get_data()
{
	return data_ptr;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLElementArrayBufferProvider Operations:

void CL_OpenGLElementArrayBufferProvider::lock(CL_BufferAccess access)
{
	CL_OpenGL::set_active(gc_provider);
	CLint last_buffer = 0;
	clGetIntegerv(CL_ELEMENT_ARRAY_BUFFER_BINDING, &last_buffer);
	clBindBuffer(CL_ELEMENT_ARRAY_BUFFER, handle);
	data_ptr = (void *) clMapBuffer(CL_ELEMENT_ARRAY_BUFFER, to_enum(access));
	clBindBuffer(CL_ELEMENT_ARRAY_BUFFER, last_buffer);
}

void CL_OpenGLElementArrayBufferProvider::unlock()
{
	CL_OpenGL::set_active(gc_provider);
	CLint last_buffer = 0;
	clGetIntegerv(CL_ELEMENT_ARRAY_BUFFER_BINDING, &last_buffer);
	clBindBuffer(CL_ELEMENT_ARRAY_BUFFER, handle);
	clUnmapBuffer(CL_ELEMENT_ARRAY_BUFFER);
	clBindBuffer(CL_ELEMENT_ARRAY_BUFFER, last_buffer);
	data_ptr = 0;
}

void CL_OpenGLElementArrayBufferProvider::upload_data(int offset, void *data, int size)
{
	CL_OpenGL::set_active(gc_provider);
	CLint last_buffer = 0;
	clGetIntegerv(CL_ELEMENT_ARRAY_BUFFER_BINDING, &last_buffer);
	clBindBuffer(CL_ELEMENT_ARRAY_BUFFER, handle);
	clBufferSubData(CL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	clBindBuffer(CL_ELEMENT_ARRAY_BUFFER, last_buffer);
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLElementArrayBufferProvider Implementation:

CLenum CL_OpenGLElementArrayBufferProvider::to_enum(CL_BufferUsage usage) const
{
	switch (usage)
	{
	case cl_usage_stream_draw:
		return CL_STREAM_DRAW;
	case cl_usage_stream_read:
		return CL_STREAM_READ;
	case cl_usage_stream_copy:
		return CL_STREAM_COPY;
	case cl_usage_static_draw:
		return CL_STATIC_DRAW;
	case cl_usage_static_read:
		return CL_STATIC_READ;
	case cl_usage_static_copy:
		return CL_STATIC_COPY;
	case cl_usage_dynamic_draw:
		return CL_DYNAMIC_DRAW;
	case cl_usage_dynamic_read:
		return CL_DYNAMIC_READ;
	case cl_usage_dynamic_copy:
		return CL_DYNAMIC_COPY;
	default:
		return CL_STATIC_DRAW;
	}
}

CLenum CL_OpenGLElementArrayBufferProvider::to_enum(CL_BufferAccess access) const
{
	switch (access)
	{
	case cl_access_read_only:
		return CL_READ_ONLY;
	case cl_access_write_only:
		return CL_WRITE_ONLY;
	case cl_access_read_write:
		return CL_READ_WRITE;
	default:
		return CL_READ_WRITE;
	}
};
