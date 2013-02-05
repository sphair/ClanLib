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
*/

#include "LegacyGL/precomp.h"
#include "API/LegacyGL/graphic_context_legacy_gl.h"
#include "gl1_graphic_context_provider.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_LegacyGL_Impl Class:

class GraphicContext_LegacyGL_Impl
{
public:
	GraphicContext_LegacyGL_Impl() : provider(0)
	{
	}

	~GraphicContext_LegacyGL_Impl()
	{
	}
	GL1GraphicContextProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_LegacyGL Construction:

GraphicContext_LegacyGL::GraphicContext_LegacyGL(GraphicContext &gc) : GraphicContext(gc),
 impl(new GraphicContext_LegacyGL_Impl)
{
	impl->provider = dynamic_cast <GL1GraphicContextProvider *> (GraphicContext::get_provider());
	if (!impl->provider)
	{
			throw Exception("Graphic Context is not from a LegacyGL target");
	}

}

GraphicContext_LegacyGL::~GraphicContext_LegacyGL()
{
}

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_LegacyGL Attributes:

void GraphicContext_LegacyGL::throw_if_null() const
{
	if (!impl)
		throw Exception("GraphicContext_LegacyGL is null");
}

int GraphicContext_LegacyGL::get_max_texture_coords()
{
	return impl->provider->get_max_texture_coords();
}

void GraphicContext_LegacyGL::get_opengl_version(int &version_major, int &version_minor, int &version_release)
{
	impl->provider->get_opengl_version(version_major, version_minor, version_release);
}

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_LegacyGL Operations:
void GraphicContext_LegacyGL::set_active()
{
	impl->provider->set_active();
}

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_LegacyGL Implementation:

}
