/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#include "GL/precomp.h"
#include "opengl_buffer_object_provider.h"
#include "opengl_graphic_context_provider.h"
#include "API/GL/opengl_wrap.h"
#include "API/Display/Render/shared_gc_data.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLBufferObjectProvider Construction:

CL_OpenGLBufferObjectProvider::CL_OpenGLBufferObjectProvider(CL_OpenGLGraphicContextProvider *gc_provider)
: gc_provider(gc_provider), handle(0), data_ptr(0)
{
	CL_SharedGCData::add_disposable(this);
	CL_OpenGL::set_active(gc_provider);

	clGenBuffers(1, &handle);
}

CL_OpenGLBufferObjectProvider::~CL_OpenGLBufferObjectProvider()
{
	dispose();
	CL_SharedGCData::remove_disposable(this);
}

void CL_OpenGLBufferObjectProvider::on_dispose()
{
	if (handle)
	{
		if (CL_OpenGL::set_active())
		{
			clDeleteBuffers(1, &handle);
		}
	}
}

void CL_OpenGLBufferObjectProvider::create(const void *data, int size, CL_BufferUsage usage, CLenum new_binding, CLenum new_target)
{
	throw_if_disposed();

	binding = new_binding;
	target = new_target;

	CL_OpenGL::set_active(gc_provider);

	CLint last_buffer = 0;
	clGetIntegerv(binding, &last_buffer);
	clBindBuffer(target, handle);
	clBufferData(target, size, data, to_enum(usage));
	clBindBuffer(target, last_buffer);
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLBufferObjectProvider Attributes:

void *CL_OpenGLBufferObjectProvider::get_data()
{
	if (data_ptr == NULL)
		throw CL_Exception("PixelBuffer was not locked");
	return data_ptr;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLBufferObjectProvider Operations:

void CL_OpenGLBufferObjectProvider::lock(CL_BufferAccess access)
{
	throw_if_disposed();
	CL_OpenGL::set_active(gc_provider);
	CLint last_buffer = 0;
	clGetIntegerv(binding, &last_buffer);
	clBindBuffer(target, handle);
	data_ptr = (void *) clMapBuffer(target, to_enum(access));
	clBindBuffer(target, last_buffer);
}

void CL_OpenGLBufferObjectProvider::unlock()
{
	throw_if_disposed();
	CL_OpenGL::set_active(gc_provider);
	CLint last_buffer = 0;
	clGetIntegerv(binding, &last_buffer);
	clBindBuffer(target, handle);
	clUnmapBuffer(target);
	clBindBuffer(target, last_buffer);
	data_ptr = 0;
}

void CL_OpenGLBufferObjectProvider::upload_data(int offset, const void *data, int size)
{
	throw_if_disposed();
	CL_OpenGL::set_active(gc_provider);
	CLint last_buffer = 0;
	clGetIntegerv(binding, &last_buffer);
	clBindBuffer(target, handle);
	clBufferSubData(target, offset, size, data);
	clBindBuffer(target, last_buffer);
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLBufferObjectProvider Implementation:

CLenum CL_OpenGLBufferObjectProvider::to_enum(CL_BufferUsage usage) const
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

CLenum CL_OpenGLBufferObjectProvider::to_enum(CL_BufferAccess access) const
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
