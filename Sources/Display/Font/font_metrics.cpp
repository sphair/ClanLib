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
**    Harry Storbacka
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Display/Font/font_metrics.h"
#include "font_metrics_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// FontMetrics Construction:

FontMetrics::FontMetrics()
{

}

FontMetrics::FontMetrics(
	float height,
	float ascent,
	float descent,
	float internal_leading,
	float external_leading,
	float average_character_width,
	float max_character_width,
	float weight,
	float overhang,
	float digitized_aspect_x,
	float digitized_aspect_y,		
	bool italic,
	bool underline,
	bool struck_out,
	bool fixed_pitch )
	: impl( new FontMetrics_Impl(
		height, ascent, descent,
		internal_leading, external_leading, average_character_width,
		max_character_width, weight, overhang,
		digitized_aspect_x, digitized_aspect_y, italic, underline, struck_out, fixed_pitch ) )
{

}

FontMetrics::~FontMetrics()
{

}

/////////////////////////////////////////////////////////////////////////////
// FontMetrics Attributes:

void FontMetrics::throw_if_null() const
{
	if (!impl)
		throw Exception("FontMetrics is null");
}

float FontMetrics::get_height() const
{
	return impl->height;
}

float FontMetrics::get_ascent() const
{
	return impl->ascent;
}

float FontMetrics::get_descent() const
{
	return impl->descent;
}

float FontMetrics::get_internal_leading() const
{
	return impl->internal_leading;
}

float FontMetrics::get_external_leading() const
{
	return impl->external_leading;
}

float FontMetrics::get_average_character_width() const
{
	return impl->average_character_width;
}

float FontMetrics::get_max_character_width() const
{
	return impl->max_character_width;
}

float FontMetrics::get_weight() const
{
	return impl->weight;
}

float FontMetrics::get_overhang() const
{
	return impl->overhang;
}

float FontMetrics::get_digitized_aspect_x() const
{
	return impl->digitized_aspect_x;
}

float FontMetrics::get_digitized_aspect_y() const
{
	return impl->digitized_aspect_y;
}

std::string::value_type FontMetrics::get_first_char() const
{
	return impl->get_first_char();
}

std::string::value_type FontMetrics::get_last_char() const
{
	return impl->get_last_char();
}

std::string::value_type FontMetrics::get_default_char() const
{
	return impl->get_default_char();
}

std::string::value_type FontMetrics::get_word_break_char() const
{
	return impl->get_word_break_char();
}

bool FontMetrics::is_italic() const
{
	return impl->italic;
}

bool FontMetrics::is_underlined() const
{
	return impl->underlined;
}

bool FontMetrics::is_struck_out() const
{
	return impl->struck_out;
}

bool FontMetrics::is_fixed_pitch() const
{
	return impl->fixed_pitch;
}

/////////////////////////////////////////////////////////////////////////////
// FontMetrics Operations:

void FontMetrics::set_height(float value)
{
	impl->height = value;
}

void FontMetrics::set_ascent(float value)
{
	impl->ascent = value;
}

void FontMetrics::set_descent(float value)
{
	impl->descent = value;
}

void FontMetrics::set_internal_leading(float value)
{
	impl->internal_leading = value;
}

void FontMetrics::set_external_leading(float value)
{
	impl->external_leading = value;
}

void FontMetrics::set_average_character_width(float value)
{
	impl->average_character_width = value;
}

void FontMetrics::set_max_character_width(float value)
{
	impl->max_character_width = value;
}

void FontMetrics::set_weight(float value)
{
	impl->weight = value;
}

void FontMetrics::set_overhang(float value)
{
	impl->overhang = value;
}

void FontMetrics::set_digitized_aspect_x(float value)
{
	impl->digitized_aspect_x = value;
}

void FontMetrics::set_digitized_aspect_y(float value)
{
	impl->digitized_aspect_y = value;
}

void FontMetrics::set_italic(bool value)
{
	impl->italic = value;
}

void FontMetrics::set_underlined(bool value)
{
	impl->underlined = value;
}

void FontMetrics::set_struck_out(bool value)
{
	impl->struck_out = value;
}

void FontMetrics::set_fixed_pitch(bool value)
{
	impl->fixed_pitch = value;
}

/////////////////////////////////////////////////////////////////////////////
// FontMetrics Implementation:

}
