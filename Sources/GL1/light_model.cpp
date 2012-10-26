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
**    Magnus Norddahl
*/

#include "GL1/precomp.h"
#include "API/GL1/light_model.h"
#include "API/Display/2D/color.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// LightModel_GL1_Impl Class:

class LightModel_GL1_Impl
{
public:
	LightModel_GL1_Impl()
	: lighting_enabled(false), local_viewer(false), two_sided_materials(false),
	  color_control(cl_color_control_single_color), flat_shading(false)
	{
		scene_ambient_light = Colorf(0.2f, 0.2f, 0.2f, 1.0f);
	}

	~LightModel_GL1_Impl()
	{
	}

	bool lighting_enabled;

	bool local_viewer;

	bool two_sided_materials;

	ColorControl_GL1 color_control;

	bool flat_shading;

	Colorf scene_ambient_light;
};

/////////////////////////////////////////////////////////////////////////////
// LightModel_GL1 Construction:

LightModel_GL1::LightModel_GL1()
: impl(new LightModel_GL1_Impl)
{
}

LightModel_GL1::~LightModel_GL1()
{
}

/////////////////////////////////////////////////////////////////////////////
// LightModel_GL1 Attributes:

bool LightModel_GL1::is_lighting_enabled() const
{
	return impl->lighting_enabled;
}

bool LightModel_GL1::is_local_viewer() const
{
	return impl->local_viewer;
}

bool LightModel_GL1::get_two_sided_materials() const
{
	return impl->two_sided_materials;
}

ColorControl_GL1 LightModel_GL1::get_color_control() const
{
	return impl->color_control;
}

bool LightModel_GL1::get_flat_shading() const
{
	return impl->flat_shading;
}

Colorf LightModel_GL1::get_scene_ambient_light() const
{
	return impl->scene_ambient_light;
}

/////////////////////////////////////////////////////////////////////////////
// LightModel_GL1 Operations:

void LightModel_GL1::enable_lighting(bool value)
{
	impl->lighting_enabled = value;
}

void LightModel_GL1::set_local_viewer(bool value)
{
	impl->local_viewer = value;
}

void LightModel_GL1::set_two_sided_materials(bool value)
{
	impl->two_sided_materials = value;
}

void LightModel_GL1::set_color_control(ColorControl_GL1 value)
{
	impl->color_control = value;
}

void LightModel_GL1::set_flat_shading(bool value)
{
	impl->flat_shading = value;
}

void LightModel_GL1::set_scene_ambient_light(Colorf color)
{
	impl->scene_ambient_light = color;
}

/////////////////////////////////////////////////////////////////////////////
// LightModel_GL1 Implementation:

}
