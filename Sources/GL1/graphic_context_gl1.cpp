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
int GraphicContext_GL1::get_light_count()
{
	return impl->provider->get_light_count();
}

void GraphicContext_GL1::get_opengl_version(int &version_major, int &version_minor, int &version_release)
{
	impl->provider->get_opengl_version(version_major, version_minor, version_release);
}

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_GL1 Operations:
void GraphicContext_GL1::push_texture_matrix(int unit_index)
{
	impl->provider->push_texture_matrix(unit_index);
}
void GraphicContext_GL1::set_texture_matrix(int unit_index, const Mat4f &matrix)
{
	impl->provider->set_texture_matrix(unit_index, matrix);
}
void GraphicContext_GL1::pop_texture_matrix(int unit_index)
{
	impl->provider->pop_texture_matrix(unit_index);
}
void GraphicContext_GL1::set_light_model(const LightModel_GL1 &light_model)
{
	impl->provider->set_light_model(light_model);
}
void GraphicContext_GL1::reset_light_model()
{
	impl->provider->reset_light_model();
}
void GraphicContext_GL1::set_material(const Material_GL1 &material)
{
	impl->provider->set_material(material);
}
void GraphicContext_GL1::set_front_material(const Material_GL1 &material)
{
	impl->provider->set_front_material(material);
}
void GraphicContext_GL1::set_back_material(const Material_GL1 &material)
{
	impl->provider->set_back_material(material);
}
void GraphicContext_GL1::reset_front_material()
{
	impl->provider->reset_front_material();
}
void GraphicContext_GL1::reset_back_material()
{
	impl->provider->reset_back_material();
}
void GraphicContext_GL1::set_material(unsigned int face, const Material_GL1 &material)
{
	impl->provider->set_material(face, material);
}
void GraphicContext_GL1::reset_material()
{
	impl->provider->reset_material();
}
void GraphicContext_GL1::set_color_material(ColorMaterial_GL1 color)
{
	impl->provider->set_color_material(color);
}
void GraphicContext_GL1::set_color_material_front(ColorMaterial_GL1 color)
{
	impl->provider->set_color_material_front(color);
}
void GraphicContext_GL1::set_color_material_back(ColorMaterial_GL1 color)
{
	impl->provider->set_color_material_back(color);
}
void GraphicContext_GL1::reset_color_material()
{
	impl->provider->reset_color_material();
}
void GraphicContext_GL1::set_color_material(unsigned int face, ColorMaterial_GL1 color)
{
	impl->provider->set_color_material(color);
}
void GraphicContext_GL1::set_light(int light_index, const LightSource_GL1 &light)
{
	impl->provider->set_light(light_index, light);
}
void GraphicContext_GL1::reset_light(int light_index)
{
	impl->provider->reset_light(light_index);
}
void GraphicContext_GL1::set_texture_unit(int unit_index, const TextureUnit_GL1 &unit)
{
	impl->provider->set_texture_unit(unit_index, unit);
}

void GraphicContext_GL1::set_active()
{
	impl->provider->set_active();
}

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_GL1 Implementation:

}
