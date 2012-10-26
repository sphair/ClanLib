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
#include "API/GL1/texture_unit.h"
#include "API/Display/2D/color.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// TextureUnit_GL1_Impl Class:

class TextureUnit_GL1_Impl
{
public:
	TextureUnit_GL1_Impl()
	{
		lod_bias = 0.0;
		color = Colorf(0.0f, 0.0f, 0.0f, 0.0f);
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

	~TextureUnit_GL1_Impl()
	{
	}

	double lod_bias;

	Colorf color;

	TextureUnitMode mode;

	TextureUnitRGBMode combine_rgb_mode;

	TextureUnitAlphaMode combine_alpha_mode;

	TextureUnitSource combine_arg_src_rgb[3];

	TextureUnitRGBOperand combine_arg_operand_rgb[3];

	TextureUnitSource combine_arg_src_alpha[3];

	TextureUnitAlphaOperand combine_arg_operand_alpha[3];

	bool point_coord_replace;
};

/////////////////////////////////////////////////////////////////////////////
// TextureUnit_GL1 Construction:

TextureUnit_GL1::TextureUnit_GL1()
: impl(new TextureUnit_GL1_Impl)
{
}

TextureUnit_GL1::~TextureUnit_GL1()
{
}

/////////////////////////////////////////////////////////////////////////////
// TextureUnit_GL1 Attributes:

double TextureUnit_GL1::get_lod_bias() const
{
	return impl->lod_bias;
}

Colorf TextureUnit_GL1::get_color() const
{
	return impl->color;
}

TextureUnitMode TextureUnit_GL1::get_mode() const
{
	return impl->mode;
}

TextureUnitRGBMode TextureUnit_GL1::get_combine_rgb_mode() const
{
	return impl->combine_rgb_mode;
}

TextureUnitAlphaMode TextureUnit_GL1::get_combine_alpha_mode() const
{
	return impl->combine_alpha_mode;
}

TextureUnitSource TextureUnit_GL1::get_combine_arg_src_rgb(int arg) const
{
	return impl->combine_arg_src_rgb[arg];
}

TextureUnitRGBOperand TextureUnit_GL1::get_combine_arg_operand_rgb(int arg) const
{
	return impl->combine_arg_operand_rgb[arg];
}

TextureUnitSource TextureUnit_GL1::get_combine_arg_src_alpha(int arg) const
{
	return impl->combine_arg_src_alpha[arg];
}

TextureUnitAlphaOperand TextureUnit_GL1::get_combine_arg_operand_alpha(int arg) const
{
	return impl->combine_arg_operand_alpha[arg];
}

bool TextureUnit_GL1::get_point_coord_replace() const
{
	return impl->point_coord_replace;
}

/////////////////////////////////////////////////////////////////////////////
// TextureUnit_GL1 Operations:

void TextureUnit_GL1::set_lod_bias(double bias)
{
	impl->lod_bias = bias;
}

void TextureUnit_GL1::set_color(const Colorf &color)
{
	impl->color = color;
}

void TextureUnit_GL1::set_mode(TextureUnitMode mode)
{
	impl->mode = mode;
}

void TextureUnit_GL1::set_combine_mode(TextureUnitRGBMode rgb_mode, TextureUnitAlphaMode alpha_mode)
{
	impl->combine_rgb_mode = rgb_mode;
	impl->combine_alpha_mode = alpha_mode;
}

void TextureUnit_GL1::set_combine_arg(
	int arg,
	TextureUnitSource src_rgb,
	TextureUnitRGBOperand operand_rgb,
	TextureUnitSource src_alpha,
	TextureUnitAlphaOperand operand_alpha)
{
	impl->combine_arg_src_rgb[arg] = src_rgb;
	impl->combine_arg_operand_rgb[arg] = operand_rgb;
	impl->combine_arg_src_alpha[arg] = src_alpha;
	impl->combine_arg_operand_alpha[arg] = operand_alpha;
}

void TextureUnit_GL1::set_combine_rgb_arg(
	int arg, TextureUnitSource src_rgb, TextureUnitRGBOperand operand_rgb)
{
	impl->combine_arg_src_rgb[arg] = src_rgb;
	impl->combine_arg_operand_rgb[arg] = operand_rgb;
}

void TextureUnit_GL1::set_combine_alpha_arg(
	int arg, TextureUnitSource src_alpha, TextureUnitAlphaOperand operand_alpha)
{
	impl->combine_arg_src_alpha[arg] = src_alpha;
	impl->combine_arg_operand_alpha[arg] = operand_alpha;
}

void TextureUnit_GL1::set_point_coord_replace(bool enable)
{
	impl->point_coord_replace = enable;
}

/////////////////////////////////////////////////////////////////////////////
// TextureUnit_GL1 Implementation:

}
