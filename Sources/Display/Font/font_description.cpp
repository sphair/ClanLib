/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
**    Mark Page
**    Harry Storbacka
*/
	
#include "Display/precomp.h"
#include "API/Display/Font/font_description.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "font_description_impl.h"

namespace clan
{
	FontDescription::FontDescription() : impl(std::make_shared<FontDescription_Impl>())
	{
	}

	FontDescription::~FontDescription()
	{
	}

	FontDescription FontDescription::create_null_object()
	{
		FontDescription fd;
		fd.impl = std::shared_ptr<FontDescription_Impl>();
		return fd;
	}

	void FontDescription::throw_if_null() const
	{
		if (!impl)
			throw Exception("is null");
	}

	float FontDescription::get_height() const
	{
		return impl->height;
	}

	float FontDescription::get_line_height() const
	{
		return impl->line_height;
	}

	float FontDescription::get_average_width() const
	{
		return impl->average_width;
	}

	float FontDescription::get_escapement() const
	{
		return impl->escapement;
	}

	float FontDescription::get_orientation() const
	{
		return impl->orientation;
	}

	FontWeight FontDescription::get_weight() const
	{
		return impl->weight;
	}

	FontStyle FontDescription::get_style() const
	{
		return impl->style;
	}

	bool FontDescription::get_anti_alias() const
	{
		return impl->anti_alias;
	}

	bool FontDescription::get_subpixel() const
	{
		return impl->subpixel;
	}

	FontDescription::Charset FontDescription::get_charset() const
	{
		return impl->charset;
	}

	std::string FontDescription::get_unique_id() const
	{
		StringFormat format("%1-%2-%3-%4-%5-%6-%7-%8-%9");
		format.set_arg(1, impl->anti_alias ? 1 : 0);
		format.set_arg(2, impl->subpixel ? 1 : 0);
		format.set_arg(3, static_cast<int>(impl->height * 10.0f + 0.5f));
		format.set_arg(4, static_cast<int>(impl->average_width * 10.0f + 0.5f));
		format.set_arg(5, static_cast<int>(impl->escapement * 10.0f + 0.5f));
		format.set_arg(6, static_cast<int>(impl->orientation * 10.0f + 0.5f));
		format.set_arg(7, static_cast<int>(impl->weight));
		format.set_arg(8, static_cast<int>(impl->style));
		format.set_arg(9, impl->charset);
		return format.get_result();
	}

	bool FontDescription::operator==(const FontDescription &other) const
	{
		return 	impl->anti_alias == other.impl->anti_alias &&
			impl->subpixel == other.impl->subpixel &&
			impl->height == other.impl->height &&
			impl->average_width == other.impl->average_width &&
			impl->escapement == other.impl->escapement &&
			impl->orientation == other.impl->orientation &&
			impl->weight == other.impl->weight &&
			impl->style == other.impl->style &&
			impl->charset == other.impl->charset;
	}

	FontDescription &FontDescription::operator =(const FontDescription &copy)
	{
		impl = copy.impl;
		return *this;
	}

	FontDescription FontDescription::clone() const
	{
		FontDescription copy;
		*copy.impl = *impl;
		return copy;
	}

	void FontDescription::set_height(float value)
	{
		impl->height = value;
	}

	void FontDescription::set_line_height(float value)
	{
		impl->line_height = value;
	}

	void FontDescription::set_average_width(float value)
	{
		impl->average_width = value;
	}

	void FontDescription::set_escapement(float value)
	{
		impl->escapement = value;
	}

	void FontDescription::set_orientation(float value)
	{
		impl->orientation = value;
	}

	void FontDescription::set_weight(FontWeight value)
	{
		impl->weight = value;
	}

	void FontDescription::set_style(FontStyle setting)
	{
		impl->style = setting;
	}

	void FontDescription::set_anti_alias(bool setting)
	{
		impl->anti_alias = setting;
	}

	void FontDescription::set_subpixel(bool setting)
	{
		impl->subpixel = setting;
	}

	void FontDescription::set_charset(Charset new_charset)
	{
		impl->charset = new_charset;
	}
}
