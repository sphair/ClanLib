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

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/ComputedValues/css_computed_font.h"
#include "API/CSSLayout/ComputedValues/css_computed_values.h"

namespace clan
{

void CSSComputedFont::compute(const CSSComputedValues &parent, CSSResourceCache *layout)
{
	if (!parent.is_null())
	{
		const CSSComputedFont &parent_font = parent.get_font();

		float parent_em_size = parent_font.font_size.length.value;
		float parent_ex_size = parent_em_size * 0.5f; // Allowed by the standard, but should ideally be queried from the font somehow

		font_size.compute(&parent_font.font_size, layout, parent_em_size, parent_ex_size);

		float em_size = font_size.length.value;
		float ex_size = em_size * 0.5f;

		font_family.compute(&parent_font.font_family, layout, em_size, ex_size);
		font_style.compute(&parent_font.font_style, layout, em_size, ex_size);
		font_variant.compute(&parent_font.font_variant, layout, em_size, ex_size);
		font_weight.compute(&parent_font.font_weight, layout, em_size, ex_size);
		line_height.compute(&parent_font.line_height, layout, em_size, ex_size);
	}
	else
	{
		float initial_em_size = 16.0f;
		float initial_ex_size = initial_em_size * 0.5f;

		font_size.compute(nullptr, layout, initial_em_size, initial_ex_size);

		float em_size = font_size.length.value;
		float ex_size = em_size * 0.5f;

		font_family.compute(nullptr, layout, em_size, ex_size);
		font_style.compute(nullptr, layout, em_size, ex_size);
		font_variant.compute(nullptr, layout, em_size, ex_size);
		font_weight.compute(nullptr, layout, em_size, ex_size);
		line_height.compute(nullptr, layout, em_size, ex_size);
	}
}

}
