/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "UI/precomp.h"
#include "API/UI/Style/text_style.h"
#include "API/UI/UIThread/ui_thread.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/2D/color.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Font/font.h"
#include "text_style_impl.h"

namespace clan
{
	TextStyle::TextStyle() : impl(std::make_shared<TextStyleImpl>())
	{
	}

	void TextStyle::set_font(const std::string &family, float size)
	{
		set_font(family, size, 0.0f);
	}

	void TextStyle::set_font(const std::string &family, float size, float line_height)
	{
		set_font_family(family);
		set_size(size);
		set_line_height(line_height);
	}

	void TextStyle::set_font_family(const std::string &family)
	{
		impl->family = family;
	}

	std::string TextStyle::family() const
	{
		return impl->family;
	}

	void TextStyle::set_size(float size)
	{
		impl->size = size;
	}

	float TextStyle::size() const
	{
		return impl->size;
	}

	void TextStyle::set_line_height_normal()
	{
		set_line_height(0.0f);
	}

	void TextStyle::set_line_height(float height)
	{
		impl->line_height = height;
	}

	float TextStyle::line_height() const
	{
		return impl->line_height;
	}

	void TextStyle::set_weight_normal()
	{
		set_weight(400);
	}

	void TextStyle::set_weight_bold()
	{
		set_weight(700);
	}

	void TextStyle::set_weight(int weight)
	{
		impl->weight = weight;
	}

	int TextStyle::weight() const
	{
		return impl->weight;
	}

	void TextStyle::set_style_normal()
	{
		impl->style = FontStyle::normal;
	}

	void TextStyle::set_style_italic()
	{
		impl->style = FontStyle::italic;
	}

	void TextStyle::set_style_oblique()
	{
		impl->style = FontStyle::oblique;
	}

	bool TextStyle::is_style_normal() const
	{
		return impl->style == FontStyle::normal;
	}

	bool TextStyle::is_style_italic() const
	{
		return impl->style == FontStyle::italic;
	}

	bool TextStyle::is_style_oblique() const
	{
		return impl->style == FontStyle::oblique;
	}

	void TextStyle::set_color(const Colorf &color)
	{
		impl->color = color;
	}

	Colorf TextStyle::color() const
	{
		return impl->color;
	}

	void TextStyle::set_shadow_none()
	{
		set_shadow(0.0f, 0.0f, 0.0f, Colorf::transparent);
	}

	void TextStyle::set_shadow(float horz_offset, float vert_offset, float blur_radius, const Colorf &color)
	{
		impl->shadow.horz_offset = horz_offset;
		impl->shadow.vert_offset = vert_offset;
		impl->shadow.blur_radius = blur_radius;
		impl->shadow.color = color;
	}

	bool TextStyle::has_shadow() const
	{
		return impl->shadow.blur_radius > 0.0f && impl->shadow.color.a > 0.0f;
	}

	float TextStyle::shadow_horz_offset() const
	{
		return impl->shadow.horz_offset;
	}

	float TextStyle::shadow_vert_offset() const
	{
		return impl->shadow.vert_offset;
	}

	float TextStyle::shadow_blur_radius() const
	{
		return impl->shadow.blur_radius;
	}

	Colorf TextStyle::shadow_color() const
	{
		return impl->shadow.color;
	}

	void TextStyle::set_transform_none()
	{
		impl->transform = TextTransform::none;
	}

	void TextStyle::set_transform_uppercase()
	{
		impl->transform = TextTransform::uppercase;
	}

	void TextStyle::set_transform_lowercase()
	{
		impl->transform = TextTransform::lowercase;
	}

	bool TextStyle::is_transform_none() const
	{
		return impl->transform == TextTransform::none;
	}

	bool TextStyle::is_transform_uppercase() const
	{
		return impl->transform == TextTransform::uppercase;
	}

	bool TextStyle::is_transform_lowercase() const
	{
		return impl->transform == TextTransform::lowercase;
	}

	void TextStyle::set_target_opaque()
	{
		impl->subpixel = true;
	}

	void TextStyle::set_target_transparent()
	{
		impl->subpixel = false;
	}

	bool TextStyle::is_target_opaque() const
	{
		return impl->subpixel;
	}

	bool TextStyle::is_target_transparent() const
	{
		return !impl->subpixel;
	}

	Font TextStyle::get_font(Canvas &canvas)
	{
		FontDescription font_desc;
		font_desc.set_typeface_name(impl->family);
		font_desc.set_height(impl->size);
		font_desc.set_line_height(impl->line_height);
		font_desc.set_weight(impl->weight);
		font_desc.set_italic(is_style_italic() || is_style_oblique());
		font_desc.set_subpixel(impl->subpixel);
		return Font::resource(canvas, font_desc, UIThread::get_resources());
	}
}
