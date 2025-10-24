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

//! clanGL="Display 3D"
//! header=gl.h

#ifndef header_texture_unit
#define header_texture_unit

#ifdef CL_API_DLL
#ifdef CL_GL_EXPORT
#define CL_API_GL __declspec(dllexport)
#else
#define CL_API_GL __declspec(dllimport)
#endif
#else
#define CL_API_GL
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../Core/System/sharedptr.h"

class CL_TextureUnit_Generic;

enum CL_TextureUnitMode
{
	cl_unitmode_replace,
	cl_unitmode_modulate,
	cl_unitmode_decal,
	cl_unitmode_blend,
	cl_unitmode_add,
	cl_unitmode_combine
};

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

enum CL_TextureUnitAlphaMode
{
	cl_combinemode_alpha_replace,
	cl_combinemode_alpha_modulate,
	cl_combinemode_alpha_add,
	cl_combinemode_alpha_add_signed,
	cl_combinemode_alpha_interpolate,
	cl_combinemode_alpha_subtract
};

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

enum CL_TextureUnitRGBOperand
{
	cl_rgb_operand_src_color,
	cl_rgb_operand_one_minus_src_color,
	cl_rgb_operand_src_alpha,
	cl_rgb_operand_one_minus_src_alpha
};

enum CL_TextureUnitAlphaOperand
{
	cl_rgb_operand_src_alpha,
	cl_rgb_operand_one_minus_src_alpha
};

class CL_Color;

//: glTexEnv container.
class CL_API_GL CL_TextureUnit
{
//! Construction:
public:
	//: Constructs a texture unit.
	CL_TextureUnit();

	~CL_TextureUnit();

//! Attributes:
public:
	//: Returns the texture level of detail bias for texture unit.
	float get_lod_bias() const;

	//: Returns the constant color for texture unit.
	CL_Color get_color() const;

	void get_color(float &red, float &green, float &blue, float &alpha) const;

	void get_color(float color[4]) const;

	//: Returns the texture function for texture unit.
	CL_TextureUnitMode get_mode() const;

	//: Returns the texture function as an OpenGL constant.
	int get_mode_gl() const;

	//: Returns the RGB combine texture function.
	//- <p>The RGB combine texture function is only used when the mode is cl_unitmode_combine.</p>
	CL_TextureUnitRGBMode get_combine_rgb_mode() const;

	//: Returns the RGB combine texture function as OpenGL constant.
	//- <p>The RGB combine texture function is only used when the mode is cl_unitmode_combine.</p>
	int get_combine_rgb_mode_gl() const;

	//: Returns the alpha combine texture function.
	//- <p>The alpha combine texture function is only used when the mode is cl_unitmode_combine.</p>
	CL_TextureUnitAlphaMode get_combine_alpha_mode() const;

	//: Returns the alpha combine texture function as OpenGL constant.
	//- <p>The alpha combine texture function is only used when the mode is cl_unitmode_combine.</p>
	int get_combine_alpha_mode_gl() const;

	//: Returns the color source for the RGB combine function's specified argument.
	CL_TextureUnitSource get_combine_arg_src_rgb(int arg) const;

	//: Returns the color source for the RGB combine function's specified argument as OpenGL constant.
	int get_combine_arg_src_rgb_gl(int arg) const;

	//: Returns the operand for the RGB combine function's specified argument.
	CL_TextureUnitRGBOperand get_combine_arg_operand_rgb(int arg) const;

	//: Returns the operand for the alpha combine function's specified argument as OpenGL constant.
	int get_combine_arg_operand_rgb_gl(int arg) const;

	//: Returns the alpha source for the alpha combine function's specified argument.
	CL_TextureUnitSource get_combine_arg_src_alpha(int arg) const;

	//: Returns the alpha source for the alpha combine function's specified argument as OpenGL constant.
	int get_combine_arg_src_alpha_gl(int arg) const;

	//: Returns the operand for the alpha combine function's specified argument.
	CL_TextureUnitAlphaOperand get_combine_arg_operand_alpha(int arg) const;

	//: Returns the operand for the alpha combine function's specified argument as OpenGL constant.
	int get_combine_arg_operand_alpha_gl(int arg) const;

//! Operations:
public:
	//: Sets the texture level of detail bias for texture unit.
	void set_lod_bias(float bias);

	//: Sets the constant color for texture unit.
	void set_color(const CL_Color &color);

	void set_color(float red, float green, float blue, float alpha);

	void set_color(float color[4]);

	//: Sets the texture function for texture unit.
	void set_mode(CL_TextureUnitMode mode);

	//: Sets the texture function for texture unit with an OpenGL constant.
	void set_mode_gl(int mode);

	//: Sets the RGB and alpha combine texture functions.
	//- <p>The combine texture functions are only used when the mode is cl_unitmode_combine.</p>
	void set_combine_mode(CL_TextureUnitRGBMode rgb_mode, CL_TextureUnitAlphaMode alpha_mode);

	//: Sets the RGB and alpha combine texture functions with an OpenGL constant.
	//- <p>The combine texture functions are only used when the mode is cl_unitmode_combine.</p>
	void set_combine_mode_gl(int rgb_mode, int alpha_mode);

	//: Sets an arguement for the RGB and alpha combine texture functions.
	void set_combine_arg(
		int arg,
		CL_TextureUnitSource src_rgb,
		CL_TextureUnitRGBOperand operand_rgb,
		CL_TextureUnitSource src_alpha,
		CL_TextureUnitAlphaOperand operand_alpha);

	//: Sets an arguement for the RGB and alpha combine texture functions using OpenGL constants.
	void set_combine_arg_gl(
		int arg,
		int src_rgb,
		int operand_rgb,
		int src_alpha,
		int operand_alpha);

	//: Sets an arguement for the RGB combine texture function.
	void set_combine_rgb_arg(int arg, CL_TextureUnitSource src_rgb, CL_TextureUnitRGBOperand operand_rgb);

	//: Sets an arguement for the RGB combine texture function with an OpenGL constant.
	void set_combine_rgb_arg_gl(int arg, int src_rgb, int operand_rgb);

	//: Sets an arguement for the alpha combine texture function.
	void set_combine_alpha_arg(int arg, CL_TextureUnitSource src_alpha, CL_TextureUnitAlphaOperand operand_alpha);

	//: Sets an arguement for the alpha combine texture function with an OpenGL constant.
	void set_combine_alpha_arg_gl(int arg, int src_alpha, int operand_alpha);

	//: Send the texture unit configuration to OpenGL.
	void setup_unit(int unit);

//! Implementation:
private:
	CL_SharedPtr<CL_TextureUnit_Generic> impl;
};

#endif
