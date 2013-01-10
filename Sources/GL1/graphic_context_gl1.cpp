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

#include "GL1/precomp.h"
#include "API/GL1/graphic_context_gl1.h"
#include "gl1_graphic_context_provider.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_GL1_Impl Class:

class GraphicContext_GL1_Impl
{
public:
	GraphicContext_GL1_Impl() : provider(0)
	{
	}

	~GraphicContext_GL1_Impl()
	{
	}
	GL1GraphicContextProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_GL1 Construction:

GraphicContext_GL1::GraphicContext_GL1(GraphicContext &gc) : GraphicContext(gc),
 impl(new GraphicContext_GL1_Impl)
{
	impl->provider = dynamic_cast <GL1GraphicContextProvider *> (GraphicContext::get_provider());
	if (!impl->provider)
	{
			throw Exception("Graphic Context is not from a GL1 target");
	}

}

GraphicContext_GL1::~GraphicContext_GL1()
{
}

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_GL1 Attributes:

void GraphicContext_GL1::throw_if_null() const
{
	if (!impl)
		throw Exception("GraphicContext_GL1 is null");
}

int GraphicContext_GL1::get_max_texture_coords()
{
	return impl->provider->get_max_texture_coords();
}

void GraphicContext_GL1::get_opengl_version(int &version_major, int &version_minor, int &version_release)
{
	impl->provider->get_opengl_version(version_major, version_minor, version_release);
}

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_GL1 Operations:
void GraphicContext_GL1::set_active()
{
	impl->provider->set_active();
}

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_GL1 Implementation:

}
