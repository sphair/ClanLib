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

class CL_TextureUnit_GL1_Impl;
class CL_Colorf;

/// \brief Texture unit modes.
///
/// \xmlonly !group=GL1/Display! !header=gl1.h! \endxmlonly
enum CL_TextureUnitMode
{
	cl_unitmode_replace,
	cl_unitmode_modulate,
	cl_unitmode_decal,
	cl_unitmode_blend,
	cl_unitmode_add,
	cl_unitmode_combine
};

/// \brief Texture unit RGB combine modes.
///
/// \xmlonly !group=GL1/Display! !header=gl1.h! \endxmlonly
enum CL_TextureUnitRGBMode
{
	cl_combinemode_rgb_replace,
	cl_combinemode_rgb_modulate,
	cl_combinemode_rgb_add,
	cl_combinemode_rgb_add_signed,
	cl_combinemode_rgb_interpolate,
	cl_combinemode_rgb_subtract,
	cl_combinemode_rgb_dot3_rgb,
	cl_combinemode_rgb_dot3_rgba
};

/// \brief Texture unit alpha combine modes.
///
/// \xmlonly !group=GL1/Display! !header=gl1.h! \endxmlonly
enum CL_TextureUnitAlphaMode
{
	cl_combinemode_alpha_replace,
	cl_combinemode_alpha_modulate,
	cl_combinemode_alpha_add,
	cl_combinemode_alpha_add_signed,
	cl_combinemode_alpha_interpolate,
	cl_combinemode_alpha_subtract
};

/// \brief Texture unit sources
///
/// \xmlonly !group=GL1/Display! !header=gl1.h! \endxmlonly
enum CL_TextureUnitSource
{
	cl_source_texture,
	cl_source_texture0,
	cl_source_texture1,
	cl_source_texture2,
	cl_source_texture3,
	cl_source_texture4,
	cl_source_texture5,
	cl_source_texture6,
	cl_source_texture7,
	cl_source_texture8,
	cl_source_texture9,
	cl_source_texture10,
	cl_source_texture11,
	cl_source_texture12,
	cl_source_texture13,
	cl_source_texture14,
	cl_source_texture15,
	cl_source_constant,
	cl_source_primary_color,
	cl_source_previous
};

/// \brief Texture unit RGB operands.
///
/// \xmlonly !group=GL1/Display! !header=gl1.h! \endxmlonly
enum CL_TextureUnitRGBOperand
{
	cl_rgb_operand_src_color,
	cl_rgb_operand_one_minus_src_color,
	cl_rgb_operand_src_alpha,
	cl_rgb_operand_one_minus_src_alpha
};

/// \brief Texture unit alpha operands.
///
/// \xmlonly !group=GL1/Display! !header=gl1.h! \endxmlonly
enum CL_TextureUnitAlphaOperand
{
	cl_alpha_operand_src_alpha,
	cl_alpha_operand_one_minus_src_alpha
};

class CL_Color;

/// \brief Texture Unit description.
///
/// \xmlonly !group=GL1/Display! !header=gl1.h! \endxmlonly
class CL_API_GL1 CL_TextureUnit_GL1
{
//!Construction
public:
	/// \brief  Constructs a texture unit.*/
	CL_TextureUnit_GL1();

	~CL_TextureUnit_GL1();

//!Attributes
public:
	/// \brief  Returns the texture level of detail bias for texture unit.
	double get_lod_bias() const;

	/// \brief  Returns the constant color for texture unit.
	CL_Colorf get_color() const;

	/// \brief  Returns the texture function for texture unit.
	CL_TextureUnitMode get_mode() const;

	/// \brief  Returns the RGB combine texture function.
	/// <p>The RGB combine texture function is only used when the mode is cl_unitmode_combine.</p>
	CL_TextureUnitRGBMode get_combine_rgb_mode() const;

	/// \brief  Returns the alpha combine texture function.
	/// <p>The alpha combine texture function is only used when the mode is cl_unitmode_combine.</p>
	CL_TextureUnitAlphaMode get_combine_alpha_mode() const;

	/// \brief  Returns the color source for the RGB combine function's specified argument.
	CL_TextureUnitSource get_combine_arg_src_rgb(int arg) const;

	/// \brief  Returns the operand for the RGB combine function's specified argument.
	CL_TextureUnitRGBOperand get_combine_arg_operand_rgb(int arg) const;

	/// \brief  Returns the alpha source for the alpha combine function's specified argument.
	CL_TextureUnitSource get_combine_arg_src_alpha(int arg) const;

	/// \brief  Returns the operand for the alpha combine function's specified argument.
	CL_TextureUnitAlphaOperand get_combine_arg_operand_alpha(int arg) const;

	/// \brief  Returns true if using point sprite texture coordinates.
	bool get_point_coord_replace() const;

//!Operations
public:
	/// \brief  Sets the texture level of detail bias for texture unit.
	void set_lod_bias(double bias);

	/// \brief  Sets the constant color for texture unit.
	void set_color(const CL_Colorf &color);

	/// \brief  Sets the texture function for texture unit.
	void set_mode(CL_TextureUnitMode mode);

	/// \brief  Sets the RGB and alpha combine texture functions.
	/// <p>The combine texture functions are only used when the mode is cl_unitmode_combine.</p>
	void set_combine_mode(CL_TextureUnitRGBMode rgb_mode, CL_TextureUnitAlphaMode alpha_mode);

	/// \brief  Sets an arguement for the RGB and alpha combine texture functions.
	void set_combine_arg(
		int arg,
		CL_TextureUnitSource src_rgb,
		CL_TextureUnitRGBOperand operand_rgb,
		CL_TextureUnitSource src_alpha,
		CL_TextureUnitAlphaOperand operand_alpha);

	/// \brief  Sets an arguement for the RGB combine texture function.
	void set_combine_rgb_arg(int arg, CL_TextureUnitSource src_rgb, CL_TextureUnitRGBOperand operand_rgb);

	/// \brief  Sets an arguement for the alpha combine texture function.
	void set_combine_alpha_arg(int arg, CL_TextureUnitSource src_alpha, CL_TextureUnitAlphaOperand operand_alpha);

	/// \brief  Enables whether texture coordinates are replaced with point sprite texture coordinates.
	void set_point_coord_replace(bool enable);

//!Implementation
private:
	CL_SharedPtr<CL_TextureUnit_GL1_Impl> impl;
};

/// \}