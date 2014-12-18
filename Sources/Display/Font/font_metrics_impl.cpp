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
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "font_metrics_impl.h"

namespace clan
{
	FontMetrics_Impl::FontMetrics_Impl()
	{
	}

	FontMetrics_Impl::FontMetrics_Impl(float height, float ascent, float descent, float internal_leading, float external_leading, float line_height) : height(height), ascent(ascent), descent(descent), internal_leading(internal_leading), external_leading(external_leading), line_height(line_height)
	{
		if (line_height = 0.0f)
			line_height = height + external_leading;
	}

	FontMetrics_Impl::~FontMetrics_Impl()
	{
	}
}
