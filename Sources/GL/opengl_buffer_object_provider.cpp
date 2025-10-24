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
**    Mark Page
*/

#include "GL/precomp.h"
#include "opengl_buffer_object_provider.h"
#include "opengl_graphic_context_provider.h"
#include "API/GL/opengl_wrap.h"
#include "API/Display/Render/shared_gc_data.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLBufferObjectProvider Construction:

CL_OpenGLBufferObjectProvider::CL_OpenGLBufferObjectProvider()
: handle(0), data_ptr(0)
{
	CL_SharedGCData::add_disposable(this);
	CL_OpenGL::set_active();

	glGenBuffers(1, &handle);
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
			glDeleteBuffers(1, &handle);
		}
	}
}

void CL_OpenGLBufferObjectProvider::create(const void *data, int size, CL_BufferUsage usage, GLenum new_binding, GLenum new_target)
{
	throw_if_disposed();

	binding = new_binding;
	target = new_target;

	CL_OpenGL::set_active();

	GLint last_buffer = 0;
	glGetIntegerv(binding, &last_buffer);
	glBindBuffer(target, handle);
	glBufferData(target, size, data, to_enum(usage));
	glBindBuffer(target, last_buffer);
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
	CL_OpenGL::set_active();
	GLint last_buffer = 0;
	glGetIntegerv(binding, &last_buffer);
	glBindBuffer(target, handle);
	data_ptr = (void *) glMapBuffer(target, to_enum(access));
	glBindBuffer(target, last_buffer);
}

void CL_OpenGLBufferObjectProvider::unlock()
{
	throw_if_disposed();
	CL_OpenGL::set_active();
	GLint last_buffer = 0;
	glGetIntegerv(binding, &last_buffer);
	glBindBuffer(target, handle);
	glUnmapBuffer(target);
	glBindBuffer(target, last_buffer);
	data_ptr = 0;
}

void CL_OpenGLBufferObjectProvider::upload_data(int offset, const void *data, int size)
{
	throw_if_disposed();
	CL_OpenGL::set_active();
	GLint last_buffer = 0;
	glGetIntegerv(binding, &last_buffer);
	glBindBuffer(target, handle);
	glBufferSubData(target, offset, size, data);
	glBindBuffer(target, last_buffer);
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLBufferObjectProvider Implementation:

GLenum CL_OpenGLBufferObjectProvider::to_enum(CL_BufferUsage usage) const
{
	switch (usage)
	{
	case cl_usage_stream_draw:
		return GL_STREAM_DRAW;
	case cl_usage_stream_read:
		return GL_STREAM_READ;
	case cl_usage_stream_copy:
		return GL_STREAM_COPY;
	case cl_usage_static_draw:
		return GL_STATIC_DRAW;
	case cl_usage_static_read:
		return GL_STATIC_READ;
	case cl_usage_static_copy:
		return GL_STATIC_COPY;
	case cl_usage_dynamic_draw:
		return GL_DYNAMIC_DRAW;
	case cl_usage_dynamic_read:
		return GL_DYNAMIC_READ;
	case cl_usage_dynamic_copy:
		return GL_DYNAMIC_COPY;
	default:
		return GL_STATIC_DRAW;
	}
}

GLenum CL_OpenGLBufferObjectProvider::to_enum(CL_BufferAccess access) const
{
	switch (access)
	{
	case cl_access_read_only:
		return GL_READ_ONLY;
	case cl_access_write_only:
		return GL_WRITE_ONLY;
	case cl_access_read_write:
		return GL_READ_WRITE;
	default:
		return GL_READ_WRITE;
	}
};
