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

class CL_LightModel_GL1_Impl;
class CL_Colorf;

/// \brief Light model color control
///
/// \xmlonly !group=GL1/Display! !header=gl1.h! \endxmlonly
enum CL_ColorControl_GL1
{
	cl_color_control_single_color,
	cl_color_control_separate_specular_color
};

/// \brief Light model description.
///
/// \xmlonly !group=GL1/Display! !header=gl1.h! \endxmlonly
class CL_API_GL1 CL_LightModel_GL1
{
//!Construction
public:
	/// \brief  Constructs a light model.
	CL_LightModel_GL1();

	~CL_LightModel_GL1();

//!Attributes
public:
	/// \brief  Returns true if lighting is enabled.
	bool is_lighting_enabled() const;

	/// \brief  Returns true if local viewer is enabled.
	bool is_local_viewer() const;

	/// \brief  Returns true if two sided materials are being used.
	bool get_two_sided_materials() const;

	/// \brief  Returns the current color control being used.
	CL_ColorControl_GL1 get_color_control() const;

	/// \brief  Returns true if flat shading is enabled.
	bool get_flat_shading() const;

	/// \brief  Returns the scene global ambient light.
	CL_Colorf get_scene_ambient_light() const;

//!Operations
public:
	/// \brief  Enables or disables lighting calculations.
	void enable_lighting(bool value);

	/// \brief  Enables or disables local viewer light model.
	void set_local_viewer(bool value);

	/// \brief  Controls if the lighting model uses different materials for front and back facing faces.
	void set_two_sided_materials(bool value);

	/// \brief  Controls how primary and secondary colors are calculated.
	void set_color_control(CL_ColorControl_GL1 value);

	/// \brief  Enables if polygons are to be flat shaded from lighting calculations.
	void set_flat_shading(bool value);

	/// \brief  Sets the scene global ambient light.
	void set_scene_ambient_light(CL_Colorf color);

//!Implementation
private:
	CL_SharedPtr<CL_LightModel_GL1_Impl> impl;
};

/// \}