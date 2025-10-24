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

/// \addtogroup clanGL1_Display clanGL1 Display
/// \{

#pragma once

#include "api_gl1.h"
#include "../Core/System/sharedptr.h"

/// \brief Color Material
///
/// \xmlonly !group=GL1/Display! !header=gl1.h! \endxmlonly

/// \brief Color Material_ GL1
///
/// \xmlonly !group=GL1/Display! !header=gl1.h! \endxmlonly
enum CL_ColorMaterial_GL1
{
	cl_color_material_emission,
	cl_color_material_ambient,
	cl_color_material_diffuse,
	cl_color_material_specular,
	cl_color_material_ambient_and_diffuse
};

class CL_Colorf;
class CL_Material_GL1_Impl;

/// \brief Material description.
///
/// \xmlonly !group=GL1/Display! !header=gl1.h! \endxmlonly
class CL_API_GL1 CL_Material_GL1
{
//!Construction
public:
	/// \brief  Constructs a material.*/
	CL_Material_GL1();

	~CL_Material_GL1();

///!Attributes
public:
	/// \brief  Returns the ambient intensity of the material.
	CL_Colorf get_ambient() const;

	/// \brief  Returns the diffuse (spread over wide area) intensity of the material.
	CL_Colorf get_diffuse() const;

	/// \brief  Returns the specular (mirror reflective) intensity of the material.
	CL_Colorf get_specular() const;

	/// \brief  Returns the emission of the material.
	CL_Colorf get_emission() const;

	/// \brief  Returns the shininess of the material.
	float get_shininess() const;

//!Operations
public:
	/// \brief  Set the ambient intensity of the material.
	void set_ambient(const CL_Colorf &color);

	/// \brief  Set the diffuse (spread over wide area) intensity of the material.
	void set_diffuse(const CL_Colorf &color);

	/// \brief  Set the specular (mirror reflective) intensity of the material.
	void set_specular(const CL_Colorf &color);

	/// \brief  Set the combined ambient and diffuse intensity of the material.
	void set_ambient_and_diffuse(const CL_Colorf &color);

	/// \brief  Set the emission of the material.
	void set_emission(const CL_Colorf &color);

	/// \brief  Set the shininess of the material.
	void set_shininess(float value);

//!Implementation
private:
	CL_SharedPtr<CL_Material_GL1_Impl> impl;
};

/// \}
