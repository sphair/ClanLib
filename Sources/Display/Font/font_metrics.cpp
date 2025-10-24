/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
*/

#include "Display/precomp.h"
#include "API/Display/Font/font_metrics.h"

#include "font_metrics_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_FontMetrics Construction:

CL_FontMetrics::CL_FontMetrics()
{
	return;
}

CL_FontMetrics::CL_FontMetrics(
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
	: impl( new CL_FontMetrics_Impl(
		height, ascent, descent,
		internal_leading, external_leading, average_character_width,
		max_character_width, weight, overhang,
		digitized_aspect_x, digitized_aspect_y, italic, underline, struck_out, fixed_pitch ) )
{
	return;
}

CL_FontMetrics::~CL_FontMetrics()
{
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontMetrics Attributes:

float CL_FontMetrics::get_height() const
{
	return impl->height;
}

float CL_FontMetrics::get_ascent() const
{
	return impl->ascent;
}

float CL_FontMetrics::get_descent() const
{
	return impl->descent;
}

float CL_FontMetrics::get_internal_leading() const
{
	return impl->internal_leading;
}

float CL_FontMetrics::get_external_leading() const
{
	return impl->external_leading;
}

float CL_FontMetrics::get_average_character_width() const
{
	return impl->average_character_width;
}

float CL_FontMetrics::get_max_character_width() const
{
	return impl->max_character_width;
}

float CL_FontMetrics::get_weight() const
{
	return impl->weight;
}

float CL_FontMetrics::get_overhang() const
{
	return impl->overhang;
}

float CL_FontMetrics::get_digitized_aspect_x() const
{
	return impl->digitized_aspect_x;
}

float CL_FontMetrics::get_digitized_aspect_y() const
{
	return impl->digitized_aspect_y;
}

CL_String::char_type CL_FontMetrics::get_first_char() const
{
	return impl->get_first_char();
}

CL_String::char_type CL_FontMetrics::get_last_char() const
{
	return impl->get_last_char();
}

CL_String::char_type CL_FontMetrics::get_default_char() const
{
	return impl->get_default_char();
}

CL_String::char_type CL_FontMetrics::get_word_break_char() const
{
	return impl->get_word_break_char();
}

bool CL_FontMetrics::is_italic() const
{
	return impl->italic;
}

bool CL_FontMetrics::is_underlined() const
{
	return impl->underline;
}

bool CL_FontMetrics::is_struck_out() const
{
	return impl->struck_out;
}

bool CL_FontMetrics::is_fixed_pitch() const
{
	return impl->fixed_pitch;
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontMetrics Operations:

/////////////////////////////////////////////////////////////////////////////
// CL_FontMetrics Implementation:
