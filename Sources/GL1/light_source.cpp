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
#include "API/GL1/light_source.h"
#include "API/Core/Math/vec3.h"
#include "API/Display/2D/color.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// LightSource_GL1_Impl Class:

class LightSource_GL1_Impl
{
public:
	LightSource_GL1_Impl()
	: spot_exponent(0.0), spot_cutoff(180.0),
	  constant_attenuation(1.0), linear_attenuation(0.0), quadratic_attenuation(0.0),
	  position(0.0, 0.0, -1.0, 0.0), spot_direction(0.0, 0.0, -1.0)
	{
		ambient = Colorf(0.0f, 0.0f, 0.0f, 0.0f);
		diffuse = Colorf(1.0f, 1.0f, 1.0f, 1.0f);
		specular = Colorf(1.0f, 1.0f, 1.0f, 1.0f);
	}

	float spot_exponent;

	float spot_cutoff;

	float constant_attenuation;

	float linear_attenuation;

	float quadratic_attenuation;

	Colorf ambient;

	Colorf diffuse;

	Colorf specular;

	Vec4f position;

	Vec3f spot_direction;
};

/////////////////////////////////////////////////////////////////////////////
// LightSource_GL1 construction:

LightSource_GL1::LightSource_GL1()
: impl(new LightSource_GL1_Impl)
{
}

LightSource_GL1::~LightSource_GL1()
{
}

/////////////////////////////////////////////////////////////////////////////
// LightSource_GL1 attributes:

float LightSource_GL1::get_spot_exponent() const
{
	return impl->spot_exponent;
}

float LightSource_GL1::get_spot_cutoff() const
{
	return impl->spot_cutoff;
}

float LightSource_GL1::get_constant_attenuation() const
{
	return impl->constant_attenuation;
}

float LightSource_GL1::get_linear_attenuation() const
{
	return impl->linear_attenuation;
}

float LightSource_GL1::get_quadratic_attenuation() const
{
	return impl->quadratic_attenuation;
}

Colorf LightSource_GL1::get_ambient_intensity() const
{
	return impl->ambient;
}

Colorf LightSource_GL1::get_diffuse_intensity() const
{
	return impl->diffuse;
}

Colorf LightSource_GL1::get_specular_intensity() const
{
	return impl->specular;
}

Vec4f LightSource_GL1::get_position() const
{
	return impl->position;
}

Vec3f LightSource_GL1::get_spot_direction() const
{
	return impl->spot_direction;
}

/////////////////////////////////////////////////////////////////////////////
// LightSource_GL1 operations:

void LightSource_GL1::set_spot_exponent(float new_spot_exponent)
{
	impl->spot_exponent = new_spot_exponent;
}

void LightSource_GL1::set_spot_cutoff(float new_spot_cutoff)
{
	impl->spot_cutoff = new_spot_cutoff;
}

void LightSource_GL1::set_constant_attenuation(float new_constant_attenuation)
{
	impl->constant_attenuation = new_constant_attenuation;
}

void LightSource_GL1::set_linear_attenuation(float new_linear_attenuation)
{
	impl->linear_attenuation = new_linear_attenuation;
}

void LightSource_GL1::set_quadratic_attenuation(float new_quadratic_attenuation)
{
	impl->quadratic_attenuation = new_quadratic_attenuation;
}

void LightSource_GL1::set_ambient_intensity(const Colorf &color)
{
	impl->ambient = color;
}

void LightSource_GL1::set_diffuse_intensity(const Colorf &color)
{
	impl->diffuse = color;
}

void LightSource_GL1::set_specular_intensity(const Colorf &color)
{
	impl->specular = color;
}

void LightSource_GL1::set_position(const Vec4f &new_position)
{
	impl->position = new_position;
}

void LightSource_GL1::set_spot_direction(const Vec3f &new_spot_direction)
{
	impl->spot_direction = new_spot_direction;
}
/*
void LightSource_GL1::set_viewpoint(const Viewpoint &viewpoint)
{
	impl->position = viewpoint.pos;
	impl->spot_direction = viewpoint.dir;
}
*/
/////////////////////////////////////////////////////////////////////////////
// LightSource_GL1 implementation:

}
