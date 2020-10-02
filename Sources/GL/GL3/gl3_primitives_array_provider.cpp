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
#include "gl3_primitives_array_provider.h"
#include "gl3_vertex_array_buffer_provider.h"
#include "gl3_graphic_context_provider.h"
#include "API/Display/Render/shared_gc_data.h"

namespace clan
{
	GL3PrimitivesArrayProvider::GL3PrimitivesArrayProvider(GL3GraphicContextProvider *gc_provider)
		: handle(0), gc_provider(gc_provider)
	{
		gc_provider->add_disposable(this);
		OpenGL::set_active(gc_provider);
		glGenVertexArrays(1, &handle);
	}

	GL3PrimitivesArrayProvider::~GL3PrimitivesArrayProvider()
	{
		dispose();
	}

	void GL3PrimitivesArrayProvider::set_attribute(int attrib_index, const VertexData &attribute, bool normalize)
	{
		PrimitivesArrayStateTracker tracker(gc_provider, handle);

		glBindBuffer(GL_ARRAY_BUFFER, static_cast<GL3VertexArrayBufferProvider *>(attribute.array_provider)->get_handle());
		glEnableVertexAttribArray(attrib_index);

		if (attribute.type == VertexAttributeDataType::type_float)
		{
			glVertexAttribPointer(attrib_index, attribute.size, OpenGL::to_enum(attribute.type),
				normalize ? GL_TRUE : GL_FALSE, attribute.stride, (GLvoid *)attribute.offset);
		}
		else
		{
			glVertexAttribIPointer(attrib_index, attribute.size, OpenGL::to_enum(attribute.type), attribute.stride, (GLvoid *)attribute.offset);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GL3PrimitivesArrayProvider::on_dispose()
	{
		if (handle)
		{
			OpenGL::set_active(gc_provider);
			glDeleteVertexArrays(1, &handle);
		}
		gc_provider->remove_disposable(this);
	}

	PrimitivesArrayStateTracker::PrimitivesArrayStateTracker(GL3GraphicContextProvider *gc_provider, GLuint handle) : vao_set(false)
	{
		OpenGL::set_active(gc_provider);

		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint *)&last_vao);
		if (handle != last_vao)
		{
			vao_set = true;
			glBindVertexArray(handle);
		}
	}

	PrimitivesArrayStateTracker::~PrimitivesArrayStateTracker()
	{
		if (vao_set)
			glBindVertexArray(last_vao);
	}
}
