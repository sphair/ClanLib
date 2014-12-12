/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "API/GL/opengl.h"
#include "API/GL/opengl_wrap.h"
#include "opengl_graphic_context_provider.h"
#include "GL3/gl3_graphic_context_provider.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_GL_Impl Class:

class GraphicContext_GL_Impl
{
public:
	GraphicContext_GL_Impl() : provider(nullptr)
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
 impl(std::make_shared<GraphicContext_GL_Impl>())
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

void GraphicContext_GL::get_opengl_version(int &version_major, int &version_minor, int &version_release)
{
	impl->provider->get_opengl_version(version_major, version_minor, version_release);
}

void GraphicContext_GL::get_opengl_shading_language_version(int &version_major, int &version_minor)
{
	impl->provider->get_opengl_shading_language_version(version_major, version_minor);
}

std::string GraphicContext_GL::get_renderer_string()
{
	set_active();
	std::string renderer = (char*)glGetString(GL_RENDERER);
	return renderer;
}

std::string GraphicContext_GL::get_vendor_string()
{
	set_active();
	std::string vendor = (char*)glGetString(GL_VENDOR);
	return vendor;
}

std::vector<std::string> GraphicContext_GL::get_extensions()
{
	set_active();
	std::string extension_string = (char*)glGetString(GL_EXTENSIONS);
	std::vector<std::string> tmp = StringHelp::split_text(extension_string, " ");
	std::vector<std::string> extensions;
	for (auto & elem : tmp)
		extensions.push_back(elem);
	return extensions;
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
