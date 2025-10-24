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
**    Mark Page
*/

#include "GL1/precomp.h"
#include "API/GL1/graphic_context_gl1.h"
#include "gl1_graphic_context_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_GL1_Impl Class:

class CL_GraphicContext_GL1_Impl
{
public:
	CL_GraphicContext_GL1_Impl() : provider(0)
	{
	}

	~CL_GraphicContext_GL1_Impl()
	{
	}
	CL_GL1GraphicContextProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_GL1 Construction:

CL_GraphicContext_GL1::CL_GraphicContext_GL1(CL_GraphicContext &gc) : CL_GraphicContext(gc),
 impl(new CL_GraphicContext_GL1_Impl)
{
	impl->provider = dynamic_cast <CL_GL1GraphicContextProvider *> (CL_GraphicContext::get_provider());
	if (!impl->provider)
	{
			throw CL_Exception("Graphic Context is not from a GL1 target");
	}

}

CL_GraphicContext_GL1::~CL_GraphicContext_GL1()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_GL1 Attributes:

void CL_GraphicContext_GL1::throw_if_null() const
{
	if (!impl)
		throw CL_Exception("CL_GraphicContext_GL1 is null");
}

int CL_GraphicContext_GL1::get_max_texture_coords()
{
	return impl->provider->get_max_texture_coords();
}
int CL_GraphicContext_GL1::get_light_count()
{
	return impl->provider->get_light_count();
}

void CL_GraphicContext_GL1::get_opengl_version(int &version_major, int &version_minor, int &version_release)
{
	impl->provider->get_opengl_version(version_major, version_minor, version_release);
}

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_GL1 Operations:
void CL_GraphicContext_GL1::push_texture_matrix(int unit_index)
{
	flush_batcher();
	impl->provider->push_texture_matrix(unit_index);
}
void CL_GraphicContext_GL1::set_texture_matrix(int unit_index, const CL_Mat4f &matrix)
{
	flush_batcher();
	impl->provider->set_texture_matrix(unit_index, matrix);
}
void CL_GraphicContext_GL1::pop_texture_matrix(int unit_index)
{
	flush_batcher();
	impl->provider->pop_texture_matrix(unit_index);
}
void CL_GraphicContext_GL1::set_light_model(const CL_LightModel_GL1 &light_model)
{
	flush_batcher();
	impl->provider->set_light_model(light_model);
}
void CL_GraphicContext_GL1::reset_light_model()
{
	flush_batcher();
	impl->provider->reset_light_model();
}
void CL_GraphicContext_GL1::set_material(const CL_Material_GL1 &material)
{
	flush_batcher();
	impl->provider->set_material(material);
}
void CL_GraphicContext_GL1::set_front_material(const CL_Material_GL1 &material)
{
	flush_batcher();
	impl->provider->set_front_material(material);
}
void CL_GraphicContext_GL1::set_back_material(const CL_Material_GL1 &material)
{
	flush_batcher();
	impl->provider->set_back_material(material);
}
void CL_GraphicContext_GL1::reset_front_material()
{
	flush_batcher();
	impl->provider->reset_front_material();
}
void CL_GraphicContext_GL1::reset_back_material()
{
	flush_batcher();
	impl->provider->reset_back_material();
}
void CL_GraphicContext_GL1::set_material(GLenum face, const CL_Material_GL1 &material)
{
	flush_batcher();
	impl->provider->set_material(face, material);
}
void CL_GraphicContext_GL1::reset_material()
{
	flush_batcher();
	impl->provider->reset_material();
}
void CL_GraphicContext_GL1::set_color_material(CL_ColorMaterial_GL1 color)
{
	flush_batcher();
	impl->provider->set_color_material(color);
}
void CL_GraphicContext_GL1::set_color_material_front(CL_ColorMaterial_GL1 color)
{
	flush_batcher();
	impl->provider->set_color_material_front(color);
}
void CL_GraphicContext_GL1::set_color_material_back(CL_ColorMaterial_GL1 color)
{
	flush_batcher();
	impl->provider->set_color_material_back(color);
}
void CL_GraphicContext_GL1::reset_color_material()
{
	flush_batcher();
	impl->provider->reset_color_material();
}
void CL_GraphicContext_GL1::set_color_material(GLenum face, CL_ColorMaterial_GL1 color)
{
	flush_batcher();
	impl->provider->set_color_material(color);
}
void CL_GraphicContext_GL1::set_light(int light_index, const CL_LightSource_GL1 &light)
{
	flush_batcher();
	impl->provider->set_light(light_index, light);
}
void CL_GraphicContext_GL1::reset_light(int light_index)
{
	flush_batcher();
	impl->provider->reset_light(light_index);
}
void CL_GraphicContext_GL1::set_texture_unit(int unit_index, const CL_TextureUnit_GL1 &unit)
{
	flush_batcher();
	impl->provider->set_texture_unit(unit_index, unit);
}

void CL_GraphicContext_GL1::set_active()
{
	impl->provider->set_active();
}

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_GL1 Implementation:
