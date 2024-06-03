/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "gl3_buffer_object_provider.h"
#include "gl3_graphic_context_provider.h"
#include "gl3_transfer_buffer_provider.h"
#include "API/GL/opengl_wrap.h"
#include "API/Display/Render/shared_gc_data.h"

namespace clan
{
	GL3BufferObjectProvider::GL3BufferObjectProvider()
		: handle(0), data_ptr(nullptr)
	{
		SharedGCData::add_disposable(this);
		OpenGL::set_active();

		glGenBuffers(1, &handle);
	}

	GL3BufferObjectProvider::~GL3BufferObjectProvider()
	{
		dispose();
		SharedGCData::remove_disposable(this);
	}

	void GL3BufferObjectProvider::on_dispose()
	{
		if (handle)
		{
			if (OpenGL::set_active())
			{
				glDeleteBuffers(1, &handle);
				if (fence_object)
				{
					glDeleteSync(fence_object);
					fence_object = 0;
				}
			}
		}
	}

	void GL3BufferObjectProvider::create(const void *data, int new_size, BufferUsage usage, GLenum new_binding, GLenum new_target)
	{
		throw_if_disposed();

		binding = new_binding;
		target = new_target;
		buffer_size = new_size;

		OpenGL::set_active();

		GLint last_buffer = 0;
		if (binding)
			glGetIntegerv(binding, &last_buffer);
		glBindBuffer(target, handle);
		glBufferData(target, buffer_size, data, OpenGL::to_enum(usage));
		glBindBuffer(target, last_buffer);
	}

	void *GL3BufferObjectProvider::get_data()
	{
		if (data_ptr == nullptr)
			throw Exception("PixelBuffer was not locked");
		return data_ptr;
	}

	void GL3BufferObjectProvider::lock(GraphicContext &gc, BufferAccess access)
	{
		throw_if_disposed();
		lock_gc = gc;
		OpenGL::set_active(lock_gc);
		GLint last_buffer = 0;
		if (binding)
			glGetIntegerv(binding, &last_buffer);
		glBindBuffer(target, handle);

#ifdef CLANLIB_OPENGL_ES3
		data_ptr = (void*)glMapBufferRange(target, 0, buffer_size, OpenGL::to_enum(access));
#else
		data_ptr = (void*)glMapBuffer(target, OpenGL::to_enum(access));
#endif
		glBindBuffer(target, last_buffer);
	}

	void GL3BufferObjectProvider::unlock()
	{
		throw_if_disposed();
		OpenGL::set_active(lock_gc);
		GLint last_buffer = 0;
		if (binding)
			glGetIntegerv(binding, &last_buffer);
		glBindBuffer(target, handle);
		glUnmapBuffer(target);
		glBindBuffer(target, last_buffer);
		data_ptr = nullptr;
		lock_gc = GraphicContext();
	}

	void GL3BufferObjectProvider::upload_data(GraphicContext &gc, int offset, const void *data, int size)
	{
		throw_if_disposed();
		OpenGL::set_active(gc);
		GLint last_buffer = 0;
		if (binding)
			glGetIntegerv(binding, &last_buffer);
		glBindBuffer(target, handle);

		// This fence_object is required to fix a bug in the Intel OpenGL driver.
		// It appears the Intel driver is not waiting for completion of rendering, before the data is modified by the glBufferSubData() call.
		// For research, reduce "RenderBatchBuffer::vertex_buffer_size to 1024*10, and run Examples/Display/Path using OpenGL Windows with Intel GPU
		//
		// Maybe an interpretation of:
		// If any rendering in the pipeline makes reference to data in the buffer object being updated by glBufferSubData, especially from the specific region being updated, that rendering must drain from the pipeline before the data store can be updated.
		// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferSubData.xhtml
		if (fence_object)
		{
			GLenum result = glClientWaitSync(fence_object, GL_SYNC_FLUSH_COMMANDS_BIT, 5000000000);
			if (result == GL_TIMEOUT_EXPIRED)
			{
				throw clan::Exception("OpenGL Buffer 5 second timeout expired");
			}
			glDeleteSync(fence_object);
			fence_object = 0;
		}

		glBufferSubData(target, offset, size, data);

		if (glFenceSync)
			fence_object = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

		glBindBuffer(target, last_buffer);
	}

	void GL3BufferObjectProvider::upload_data(GraphicContext &gc, const void *data, int size)
	{
		upload_data(gc, 0, data, size);
	}

	void GL3BufferObjectProvider::copy_from(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int size)
	{
		throw_if_disposed();
		OpenGL::set_active(gc);

		glBindBuffer(GL_COPY_WRITE_BUFFER, handle);
		glBindBuffer(GL_COPY_READ_BUFFER, static_cast<GL3TransferBufferProvider*>(buffer.get_provider())->get_handle());

		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, src_pos, dest_pos, size);

		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
		glBindBuffer(GL_COPY_READ_BUFFER, 0);
	}

	void GL3BufferObjectProvider::copy_to(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int size)
	{
		throw_if_disposed();
		OpenGL::set_active(gc);

		glBindBuffer(GL_COPY_WRITE_BUFFER, static_cast<GL3TransferBufferProvider*>(buffer.get_provider())->get_handle());
		glBindBuffer(GL_COPY_READ_BUFFER, handle);

		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, dest_pos, src_pos, size);

		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
		glBindBuffer(GL_COPY_READ_BUFFER, 0);
	}
}
