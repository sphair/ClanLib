/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "API/Core/Text/string_help.h"
#include "text_style_impl.h"

namespace clan
{
	TextStyle::TextStyle() : impl(new TextStyleImpl())
	{
	}

	void TextStyle::set_font(const std::string &family, float size)
	{
	}

	void TextStyle::set_font(const std::string &family, float size, float line_height)
	{
	}

	void TextStyle::set_font_family(const std::string &family)
	{
	}

	void TextStyle::set_size(float size)
	{
	}

	void TextStyle::set_line_height_normal()
	{
	}

	void TextStyle::set_line_height(float height)
	{
	}

	void TextStyle::set_weight_normal()
	{
	}

	void TextStyle::set_weight_bold()
	{
	}

	void TextStyle::set_weight(int weight)
	{
	}

	void TextStyle::set_style_normal()
	{
	}

	void TextStyle::set_style_italic()
	{
	}

	void TextStyle::set_color(const Colorf &color)
	{
	}

	void TextStyle::set_shadow_none()
	{
	}

	void TextStyle::set_shadow(float horz_offset, float vert_offset, float blur_radius, const Colorf &color)
	{
	}

	void TextStyle::set_align_left()
	{
	}

	void TextStyle::set_align_right()
	{
	}

	void TextStyle::set_align_center()
	{
	}

	void TextStyle::set_align_justify()
	{
	}

	void TextStyle::set_transform_none()
	{
	}

	void TextStyle::set_transform_uppercase()
	{
	}

	void TextStyle::set_transform_lowercase()
	{
	}
}
