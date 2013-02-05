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
**    Harry Storbacka
*/

#include "GL/precomp.h"
#include "opengl_occlusion_query_provider.h"
#include "API/GL/opengl_wrap.h"
#include "API/Display/Render/shared_gc_data.h"
#include "opengl_graphic_context_provider.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// OpenGLOcclusionQueryProvider Construction:

OpenGLOcclusionQueryProvider::OpenGLOcclusionQueryProvider(OpenGLGraphicContextProvider *gc_provider)
: handle(0), gc_provider(gc_provider)
{
	SharedGCData::add_disposable(this);
	create();
}

OpenGLOcclusionQueryProvider::~OpenGLOcclusionQueryProvider()
{
	dispose();
	SharedGCData::remove_disposable(this);
}

void OpenGLOcclusionQueryProvider::on_dispose()
{
	if (handle)
	{
		if (OpenGL::set_active())
		{
			glDeleteQueries(1, &handle);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// OpenGLOcclusionQueryProvider Attributes:

bool OpenGLOcclusionQueryProvider::is_result_ready() const
{
	OpenGL::set_active(gc_provider);
	int available;
	glGetQueryObjectiv(handle, GL_QUERY_RESULT_AVAILABLE, &available);
	return (available != 0);
}

GLint OpenGLOcclusionQueryProvider::get_result() const
{
	OpenGL::set_active(gc_provider);
	GLint result;
	glGetQueryObjectiv(handle, GL_QUERY_RESULT, &result);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// OpenGLOcclusionQueryProvider Operations:

void OpenGLOcclusionQueryProvider::create()
{
	OpenGL::set_active(gc_provider);

	if (handle)
	{
		glDeleteQueries(1, &handle);
		handle = 0;
	}

	glGenQueries(1, &handle);
}

void OpenGLOcclusionQueryProvider::begin()
{
	OpenGL::set_active(gc_provider);
	glBeginQuery(GL_SAMPLES_PASSED, handle);
}

void OpenGLOcclusionQueryProvider::end()
{
	OpenGL::set_active(gc_provider);
	glEndQuery(GL_SAMPLES_PASSED);
}


/////////////////////////////////////////////////////////////////////////////
// OpenGLOcclusionQueryProvider Implementation:

}
