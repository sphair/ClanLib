/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Display/display_precomp.h"
#include "API/GL/light_source.h"
#include "API/GL/opengl_wrap.h"

/////////////////////////////////////////////////////////////////////////////
// CL_LightSource construction:

CL_LightSource::CL_LightSource() :
	spot_exponent(0.0f), spot_cutoff(180.0f),
	constant_attenuation(1.0f), linear_attenuation(0.0f), quadratic_attenuation(0.0f),
	position(0.0f, 0.0f, -1.0f, 0.0f), spot_direction(0.0f, 0.0f, -1.0f)
{
	ambient[0] = 0.0f;
	ambient[1] = 0.0f;
	ambient[2] = 0.0f;
	ambient[3] = 1.0f;
	diffuse[0] = 1.0f;
	diffuse[1] = 1.0f;
	diffuse[2] = 1.0f;
	diffuse[3] = 1.0f;
	specular[0] = 1.0f;
	specular[1] = 1.0f;
	specular[2] = 1.0f;
	specular[3] = 1.0f;
}

CL_LightSource::~CL_LightSource()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_LightSource attributes:

float CL_LightSource::get_spot_exponent() const
{
	return spot_exponent;
}

float CL_LightSource::get_spot_cutoff() const
{
	return spot_cutoff;
}

float CL_LightSource::get_constant_attenuation() const
{
	return constant_attenuation;
}

float CL_LightSource::get_linear_attenuation() const
{
	return linear_attenuation;
}

float CL_LightSource::get_quadratic_attenuation() const
{
	return quadratic_attenuation;
}

void CL_LightSource::get_ambient_intensity(float &red, float &green, float &blue, float &alpha) const
{
	red = ambient[0];
	green = ambient[1];
	blue = ambient[2];
	alpha = ambient[3];
}

void CL_LightSource::get_diffuse_intensity(float &red, float &green, float &blue, float &alpha) const
{
	red = diffuse[0];
	green = diffuse[1];
	blue = diffuse[2];
	alpha = diffuse[3];
}

void CL_LightSource::get_specular_intensity(float &red, float &green, float &blue, float &alpha) const
{
	red = specular[0];
	green = specular[1];
	blue = specular[2];
	alpha = specular[3];
}

CL_Vector CL_LightSource::get_position() const
{
	return position;
}

CL_Vector CL_LightSource::get_spot_direction() const
{
	return spot_direction;
}

/////////////////////////////////////////////////////////////////////////////
// CL_LightSource operations:

void CL_LightSource::set_spot_exponent(float new_spot_exponent)
{
	spot_exponent = new_spot_exponent;
}

void CL_LightSource::set_spot_cutoff(float new_spot_cutoff)
{
	spot_cutoff = new_spot_cutoff;
}

void CL_LightSource::set_constant_attenuation(float new_constant_attenuation)
{
	constant_attenuation = new_constant_attenuation;
}

void CL_LightSource::set_linear_attenuation(float new_linear_attenuation)
{
	linear_attenuation = new_linear_attenuation;
}

void CL_LightSource::set_quadratic_attenuation(float new_quadratic_attenuation)
{
	quadratic_attenuation = new_quadratic_attenuation;
}

void CL_LightSource::set_ambient_intensity(float red, float green, float blue, float alpha)
{
	ambient[0] = red;
	ambient[1] = green;
	ambient[2] = blue;
	ambient[3] = alpha;
}

void CL_LightSource::set_diffuse_intensity(float red, float green, float blue, float alpha)
{
	diffuse[0] = red;
	diffuse[1] = green;
	diffuse[2] = blue;
	diffuse[3] = alpha;
}

void CL_LightSource::set_specular_intensity(float red, float green, float blue, float alpha)
{
	specular[0] = red;
	specular[1] = green;
	specular[2] = blue;
	specular[3] = alpha;
}

void CL_LightSource::set_position(const CL_Vector &new_position)
{
	position = new_position;
}

void CL_LightSource::set_spot_direction(const CL_Vector &new_spot_direction)
{
	spot_direction = new_spot_direction;
}

void CL_LightSource::set_viewpoint(const CL_Viewpoint &viewpoint)
{
	position = viewpoint.pos;
	spot_direction = viewpoint.dir;
}

void CL_LightSource::setup_light(CLenum light_id)
{
	clLightf(light_id, CL_SPOT_EXPONENT, spot_exponent);
	clLightf(light_id, CL_SPOT_CUTOFF, spot_cutoff);
	clLightf(light_id, CL_CONSTANT_ATTENUATION, constant_attenuation);
	clLightf(light_id, CL_LINEAR_ATTENUATION, linear_attenuation);
	clLightf(light_id, CL_QUADRATIC_ATTENUATION, quadratic_attenuation);
	clLightfv(light_id, CL_AMBIENT, ambient);
	clLightfv(light_id, CL_DIFFUSE, diffuse);
	clLightfv(light_id, CL_SPECULAR, specular);
	CLfloat pos[4] = { position.x, position.y, position.z, position.w };
	clLightfv(light_id, CL_POSITION, pos);
	CLfloat spot_dir[3] = { spot_direction.x, spot_direction.y, spot_direction.z };
	clLightfv(light_id, CL_SPOT_DIRECTION, spot_dir);
}

/////////////////////////////////////////////////////////////////////////////
// CL_LightSource implementation:
