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
**    Mark Page
**    Harry Storbacka
*/

#include "GL/precomp.h"
#include "API/GL/opengl_graphic_context.h"
#include "opengl_graphic_context_provider.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_GL_Impl Class:

class GraphicContext_GL_Impl
{
public:
	GraphicContext_GL_Impl() : provider(0)
	{
	}

	~GraphicContext_GL_Impl()
	{
	}
	OpenGLGraphicContextProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_GL Construction:

GraphicContext_GL::GraphicContext_GL(GraphicContext &gc) : GraphicContext(gc),
 impl(new GraphicContext_GL_Impl)
{
	impl->provider = dynamic_cast <OpenGLGraphicContextProvider *> (GraphicContext::get_provider());
	if (!impl->provider)
	{
			throw Exception("Graphic Context is not from a GL target");
	}

}

GraphicContext_GL::~GraphicContext_GL()
{
}

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_GL Attributes:

void GraphicContext_GL::throw_if_null() const
{
	if (!impl)
		throw Exception("GraphicContext_GL is null");
}

void GraphicContext_GL::get_opengl_version(int &version_major, int &version_minor)
{
	impl->provider->get_opengl_version(version_major, version_minor);
}

void GraphicContext_GL::get_opengl_shading_language_version(int &version_major, int &version_minor)
{
	impl->provider->get_opengl_shading_language_version(version_major, version_minor);
}

std::string GraphicContext_GL::get_renderer_string()
{
	return impl->provider->get_renderer_string();
}

std::string GraphicContext_GL::get_vendor_string()
{
	return impl->provider->get_vendor_string();
}

std::vector<std::string> GraphicContext_GL::get_extensions()
{
	return impl->provider->get_extensions();
}

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_GL Operations:

void GraphicContext_GL::set_active()
{
	OpenGL::set_active(impl->provider);
}

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_GL Implementation:

}
