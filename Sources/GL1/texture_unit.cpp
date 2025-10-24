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

#include "GL1/precomp.h"
#include "API/GL1/texture_unit.h"
#include "API/Display/2D/color.h"

/////////////////////////////////////////////////////////////////////////////
// CL_TextureUnit_GL1_Impl Class:

class CL_TextureUnit_GL1_Impl
{
public:
	CL_TextureUnit_GL1_Impl()
	{
		lod_bias = 0.0;
		color = CL_Colorf(0.0f, 0.0f, 0.0f, 0.0f);
		mode = cl_unitmode_modulate;
		combine_rgb_mode = cl_combinemode_rgb_modulate;
		combine_alpha_mode = cl_combinemode_alpha_modulate;
		combine_arg_src_rgb[0] = cl_source_texture;
		combine_arg_src_rgb[1] = cl_source_previous;
		combine_arg_src_rgb[2] = cl_source_constant;
		combine_arg_operand_rgb[0] = cl_rgb_operand_src_color;
		combine_arg_operand_rgb[1] = cl_rgb_operand_src_color;
		combine_arg_operand_rgb[2] = cl_rgb_operand_src_alpha;
		combine_arg_src_alpha[0] = cl_source_texture;
		combine_arg_src_alpha[1] = cl_source_previous;
		combine_arg_src_alpha[2] = cl_source_constant;
		combine_arg_operand_alpha[0] = cl_alpha_operand_src_alpha;
		combine_arg_operand_alpha[1] = cl_alpha_operand_src_alpha;
		combine_arg_operand_alpha[2] = cl_alpha_operand_src_alpha;
		point_coord_replace = false;
	}

	~CL_TextureUnit_GL1_Impl()
	{
	}

	double lod_bias;

	CL_Colorf color;

	CL_TextureUnitMode mode;

	CL_TextureUnitRGBMode combine_rgb_mode;

	CL_TextureUnitAlphaMode combine_alpha_mode;

	CL_TextureUnitSource combine_arg_src_rgb[3];

	CL_TextureUnitRGBOperand combine_arg_operand_rgb[3];

	CL_TextureUnitSource combine_arg_src_alpha[3];

	CL_TextureUnitAlphaOperand combine_arg_operand_alpha[3];

	bool point_coord_replace;
};

/////////////////////////////////////////////////////////////////////////////
// CL_TextureUnit_GL1 Construction:

CL_TextureUnit_GL1::CL_TextureUnit_GL1()
: impl(new CL_TextureUnit_GL1_Impl)
{
}

CL_TextureUnit_GL1::~CL_TextureUnit_GL1()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_TextureUnit_GL1 Attributes:

double CL_TextureUnit_GL1::get_lod_bias() const
{
	return impl->lod_bias;
}

CL_Colorf CL_TextureUnit_GL1::get_color() const
{
	return impl->color;
}

CL_TextureUnitMode CL_TextureUnit_GL1::get_mode() const
{
	return impl->mode;
}

CL_TextureUnitRGBMode CL_TextureUnit_GL1::get_combine_rgb_mode() const
{
	return impl->combine_rgb_mode;
}

CL_TextureUnitAlphaMode CL_TextureUnit_GL1::get_combine_alpha_mode() const
{
	return impl->combine_alpha_mode;
}

CL_TextureUnitSource CL_TextureUnit_GL1::get_combine_arg_src_rgb(int arg) const
{
	return impl->combine_arg_src_rgb[arg];
}

CL_TextureUnitRGBOperand CL_TextureUnit_GL1::get_combine_arg_operand_rgb(int arg) const
{
	return impl->combine_arg_operand_rgb[arg];
}

CL_TextureUnitSource CL_TextureUnit_GL1::get_combine_arg_src_alpha(int arg) const
{
	return impl->combine_arg_src_alpha[arg];
}

CL_TextureUnitAlphaOperand CL_TextureUnit_GL1::get_combine_arg_operand_alpha(int arg) const
{
	return impl->combine_arg_operand_alpha[arg];
}

bool CL_TextureUnit_GL1::get_point_coord_replace() const
{
	return impl->point_coord_replace;
}

/////////////////////////////////////////////////////////////////////////////
// CL_TextureUnit_GL1 Operations:

void CL_TextureUnit_GL1::set_lod_bias(double bias)
{
	impl->lod_bias = bias;
}

void CL_TextureUnit_GL1::set_color(const CL_Colorf &color)
{
	impl->color = color;
}

void CL_TextureUnit_GL1::set_mode(CL_TextureUnitMode mode)
{
	impl->mode = mode;
}

void CL_TextureUnit_GL1::set_combine_mode(CL_TextureUnitRGBMode rgb_mode, CL_TextureUnitAlphaMode alpha_mode)
{
	impl->combine_rgb_mode = rgb_mode;
	impl->combine_alpha_mode = alpha_mode;
}

void CL_TextureUnit_GL1::set_combine_arg(
	int arg,
	CL_TextureUnitSource src_rgb,
	CL_TextureUnitRGBOperand operand_rgb,
	CL_TextureUnitSource src_alpha,
	CL_TextureUnitAlphaOperand operand_alpha)
{
	impl->combine_arg_src_rgb[arg] = src_rgb;
	impl->combine_arg_operand_rgb[arg] = operand_rgb;
	impl->combine_arg_src_alpha[arg] = src_alpha;
	impl->combine_arg_operand_alpha[arg] = operand_alpha;
}

void CL_TextureUnit_GL1::set_combine_rgb_arg(
	int arg, CL_TextureUnitSource src_rgb, CL_TextureUnitRGBOperand operand_rgb)
{
	impl->combine_arg_src_rgb[arg] = src_rgb;
	impl->combine_arg_operand_rgb[arg] = operand_rgb;
}

void CL_TextureUnit_GL1::set_combine_alpha_arg(
	int arg, CL_TextureUnitSource src_alpha, CL_TextureUnitAlphaOperand operand_alpha)
{
	impl->combine_arg_src_alpha[arg] = src_alpha;
	impl->combine_arg_operand_alpha[arg] = operand_alpha;
}

void CL_TextureUnit_GL1::set_point_coord_replace(bool enable)
{
	impl->point_coord_replace = enable;
}

/////////////////////////////////////////////////////////////////////////////
// CL_TextureUnit_GL1 Implementation:
