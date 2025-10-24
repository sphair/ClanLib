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
**    Magnus Norddahl
*/

#include "GL1/precomp.h"
#include "API/GL1/light_source.h"
#include "API/Core/Math/vec3.h"
#include "API/Display/2D/color.h"

/////////////////////////////////////////////////////////////////////////////
// CL_LightSource_GL1_Impl Class:

class CL_LightSource_GL1_Impl
{
public:
	CL_LightSource_GL1_Impl()
	: spot_exponent(0.0), spot_cutoff(180.0),
	  constant_attenuation(1.0), linear_attenuation(0.0), quadratic_attenuation(0.0),
	  position(0.0, 0.0, -1.0, 0.0), spot_direction(0.0, 0.0, -1.0)
	{
		ambient = CL_Colorf(0.0f, 0.0f, 0.0f, 0.0f);
		diffuse = CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f);
		specular = CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f);
	}

	float spot_exponent;

	float spot_cutoff;

	float constant_attenuation;

	float linear_attenuation;

	float quadratic_attenuation;

	CL_Colorf ambient;

	CL_Colorf diffuse;

	CL_Colorf specular;

	CL_Vec4f position;

	CL_Vec3f spot_direction;
};

/////////////////////////////////////////////////////////////////////////////
// CL_LightSource_GL1 construction:

CL_LightSource_GL1::CL_LightSource_GL1()
: impl(new CL_LightSource_GL1_Impl)
{
}

CL_LightSource_GL1::~CL_LightSource_GL1()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_LightSource_GL1 attributes:

float CL_LightSource_GL1::get_spot_exponent() const
{
	return impl->spot_exponent;
}

float CL_LightSource_GL1::get_spot_cutoff() const
{
	return impl->spot_cutoff;
}

float CL_LightSource_GL1::get_constant_attenuation() const
{
	return impl->constant_attenuation;
}

float CL_LightSource_GL1::get_linear_attenuation() const
{
	return impl->linear_attenuation;
}

float CL_LightSource_GL1::get_quadratic_attenuation() const
{
	return impl->quadratic_attenuation;
}

CL_Colorf CL_LightSource_GL1::get_ambient_intensity() const
{
	return impl->ambient;
}

CL_Colorf CL_LightSource_GL1::get_diffuse_intensity() const
{
	return impl->diffuse;
}

CL_Colorf CL_LightSource_GL1::get_specular_intensity() const
{
	return impl->specular;
}

CL_Vec4f CL_LightSource_GL1::get_position() const
{
	return impl->position;
}

CL_Vec4f CL_LightSource_GL1::get_spot_direction() const
{
	return impl->spot_direction;
}

/////////////////////////////////////////////////////////////////////////////
// CL_LightSource_GL1 operations:

void CL_LightSource_GL1::set_spot_exponent(float new_spot_exponent)
{
	impl->spot_exponent = new_spot_exponent;
}

void CL_LightSource_GL1::set_spot_cutoff(float new_spot_cutoff)
{
	impl->spot_cutoff = new_spot_cutoff;
}

void CL_LightSource_GL1::set_constant_attenuation(float new_constant_attenuation)
{
	impl->constant_attenuation = new_constant_attenuation;
}

void CL_LightSource_GL1::set_linear_attenuation(float new_linear_attenuation)
{
	impl->linear_attenuation = new_linear_attenuation;
}

void CL_LightSource_GL1::set_quadratic_attenuation(float new_quadratic_attenuation)
{
	impl->quadratic_attenuation = new_quadratic_attenuation;
}

void CL_LightSource_GL1::set_ambient_intensity(const CL_Colorf &color)
{
	impl->ambient = color;
}

void CL_LightSource_GL1::set_diffuse_intensity(const CL_Colorf &color)
{
	impl->diffuse = color;
}

void CL_LightSource_GL1::set_specular_intensity(const CL_Colorf &color)
{
	impl->specular = color;
}

void CL_LightSource_GL1::set_position(const CL_Vec4f &new_position)
{
	impl->position = new_position;
}

void CL_LightSource_GL1::set_spot_direction(const CL_Vec3f &new_spot_direction)
{
	impl->spot_direction = new_spot_direction;
}
/*
void CL_LightSource_GL1::set_viewpoint(const CL_Viewpoint &viewpoint)
{
	impl->position = viewpoint.pos;
	impl->spot_direction = viewpoint.dir;
}
*/
/////////////////////////////////////////////////////////////////////////////
// CL_LightSource_GL1 implementation:
