/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
	FontMetrics::FontMetrics() : impl(std::make_shared<FontMetrics_Impl>())
	{
	}

	FontMetrics::FontMetrics(float height, float ascent, float descent, float internal_leading, float external_leading, float line_height, float pixel_ratio)
		: impl(std::make_shared<FontMetrics_Impl>(height, ascent, descent, internal_leading, external_leading, line_height, pixel_ratio))
	{

	}

	FontMetrics::~FontMetrics()
	{
	}

	float FontMetrics::get_height() const
	{
		return impl->height;
	}

	float FontMetrics::get_line_height() const
	{
		return impl->line_height;
	}

	float FontMetrics::get_baseline_offset() const
	{
		return impl->baseline_offset;
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
}
