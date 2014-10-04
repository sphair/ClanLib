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

#pragma once
#include "API/Display/Font/font_metrics.h"

namespace clan
{
	class LineMetrics
	{
	public:
		//LineMetrics() { }
		//LineMetrics(float ascent, float descent, float leading_top, float text_height, float line_height) : ascent(ascent), descent(descent), leading_top(leading_top), text_height(text_height), line_height(line_height) { }
		LineMetrics(Font &font)
		{
			FontMetrics metrics = font.get_font_metrics();

			float clan_ascent = metrics.get_ascent();
			float clan_descent = metrics.get_descent();
			float clan_cap_height = metrics.get_internal_leading();
			float clan_line_gap = metrics.get_external_leading();
			float clan_height = clan_ascent + clan_descent;

			line_height = metrics.get_line_height();
			float leading = line_height - clan_height;
			float half_leading = std::round(leading * 0.5f);
			ascent = clan_ascent + half_leading;
			descent = line_height - ascent;
			text_height = clan_height;
		}

		float ascent = 0.0f;
		float descent = 0.0f;
		float leading_top = 0.0f;
		float text_height = 0.0f;
		float line_height = 0.0f;
	};
}
