/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

/// \addtogroup clanGL1_Display clanGL1 Display
/// \{

#pragma once

#include "api_gl1.h"
#include "../Core/System/sharedptr.h"
#include "../Core/Math/vec3.h"

class CL_Colorf;
class CL_LightSource_GL1_Impl;

/// \brief Light Source description.
///
/// \xmlonly !group=GL1/Display! !header=gl1.h! \endxmlonly
class CL_API_GL1 CL_LightSource_GL1
{
//!Construction
public:
	/// \brief  Constructs a light source container.
	CL_LightSource_GL1();

	~CL_LightSource_GL1();

//!Attributes
public:
	/// \brief  Returns the intensity distribution of the light.
	/// <p>Only values in the range [0,128] are accepted. The default spot exponent is 0,
	///    resulting in uniform light distribution.</p>
	float get_spot_exponent() const;

	/// \brief  Returns the spot cutoff angle (the maximum spread angle of a light source).
	/// <p>The default spot cutoff is 180, resulting in uniform light distribution.</p>
	float get_spot_cutoff() const;

	/// \brief  Returns constant attenuation of the light.
	/// <p>The default attenuation factors are (1,0,0), resulting in no attenuation.</p>
	float get_constant_attenuation() const;

	/// \brief  Returns linear attenuation of the light.
	/// <p>The default attenuation factors are (1,0,0), resulting in no attenuation.</p>
	float get_linear_attenuation() const;

	/// \brief  Returns quadratic attenuation of the light.
	/// <p>The default attenuation factors are (1,0,0), resulting in no attenuation.</p>
	float get_quadratic_attenuation() const;

	/// \brief  Returns the ambient (global light) intensity of the light.
	CL_Colorf get_ambient_intensity() const;

	/// \brief  Returns the diffuse (spread over wide area) intensity of the light.
	CL_Colorf get_diffuse_intensity() const;

	/// \brief  Returns the specular (mirror reflective) intensity of the light.
	CL_Colorf get_specular_intensity() const;

	/// \brief  Returns the light position.
	CL_Vec4f get_position() const;

	/// \brief  Returns the light spot direction.
	CL_Vec4f get_spot_direction() const;

//!Operations
public:
	/// \brief  Sets the intensity distribution of the light.
	/// <p>Only values in the range [0,128] are accepted. The default spot exponent is 0,
	///    resulting in uniform light distribution.</p>
	void set_spot_exponent(float spot_exponent);

	/// \brief  Sets the spot cutoff angle (the maximum spread angle of a light source).
	/// <p>The default spot cutoff is 180, resulting in uniform light distribution.</p>
	void set_spot_cutoff(float spot_cutoff);

	/// \brief  Sets the constant attenuation of the light.
	/// <p>The default attenuation factors are (1,0,0), resulting in no attenuation.</p>
	void set_constant_attenuation(float constant_attenuation);

	/// \brief  Sets linear attenuation of the light.
	/// <p>The default attenuation factors are (1,0,0), resulting in no attenuation.</p>
	void set_linear_attenuation(float linear_attenuation);

	/// \brief  Sets quadratic attenuation of the light.
	/// <p>The default attenuation factors are (1,0,0), resulting in no attenuation.</p>
	void set_quadratic_attenuation(float quadratic_attenuation);

	/// \brief  Sets the ambient (global light) intensity of the light.
	void set_ambient_intensity(const CL_Colorf &color);

	/// \brief  Sets the diffuse (spread over wide area) intensity of the light.
	void set_diffuse_intensity(const CL_Colorf &color);

	/// \brief  Sets the specular (mirror reflective) intensity of the light.
	void set_specular_intensity(const CL_Colorf &color);

	/// \brief  Sets the light position.
	/// <p>The position is transformed by the modelview matrix when glLight is called
	///    (just as if it were a point), and it is stored in eye coordinates. If the w
	///    component of the position is 0.0, the light is treated as a directional source.</p>
	///    <p>Diffuse and specular lighting calculations take the lights direction, but not
	///    its actual position, into account, and attenuation is disabled. Otherwise, diffuse
	///    and specular lighting calculations are based on the actual location of the light
	///    in eye coordinates, and attenuation is enabled.</p>
	///    <p>The default position is (0,0,1,0); thus, the default light source is directional,
	///    parallel to, and in the direction of the x axis.</p>
	void set_position(const CL_Vec4f &position);

	/// \brief  Sets the light spot direction.
	void set_spot_direction(const CL_Vec3f &spot_direction);

//!Implementation
private:
	CL_SharedPtr<CL_LightSource_GL1_Impl> impl;
};

/// \}