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
#include "API/CSSLayout/ComputedValues/css_computed_text.h"
#include "API/CSSLayout/ComputedValues/css_computed_values.h"

namespace clan
{

void CSSComputedTextReset::compute(const CSSComputedValues &parent, CSSResourceCache *layout, float em_size, float ex_size, const CSSValueLineHeight &computed_line_height)
{
	if (!parent.is_null())
	{
		const CSSComputedTextReset &parent_text_reset = parent.get_text_reset();

		text_decoration.compute(&parent_text_reset.text_decoration, layout, em_size, ex_size);
		vertical_align.compute(&parent_text_reset.vertical_align, layout, em_size, ex_size, computed_line_height);
		unicode_bidi.compute(&parent_text_reset.unicode_bidi, layout, em_size, ex_size);
	}
	else
	{
		text_decoration.compute(0, layout, em_size, ex_size);
		vertical_align.compute(0, layout, em_size, ex_size, computed_line_height);
		unicode_bidi.compute(0, layout, em_size, ex_size);
	}
}

void CSSComputedTextInherit::compute(const CSSComputedValues &parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (!parent.is_null())
	{
		const CSSComputedTextInherit &parent_text_inherit = parent.get_text_inherit();

		white_space.compute(&parent_text_inherit.white_space, layout, em_size, ex_size);
		color.compute(&parent_text_inherit.color, layout, em_size, ex_size);
		text_indent.compute(&parent_text_inherit.text_indent, layout, em_size, ex_size);
		text_transform.compute(&parent_text_inherit.text_transform, layout, em_size, ex_size);
		text_align.compute(&parent_text_inherit.text_align, layout, em_size, ex_size);
		letter_spacing.compute(&parent_text_inherit.letter_spacing, layout, em_size, ex_size);
		word_spacing.compute(&parent_text_inherit.word_spacing, layout, em_size, ex_size);
	}
	else
	{
		white_space.compute(0, layout, em_size, ex_size);
		color.compute(0, layout, em_size, ex_size);
		text_indent.compute(0, layout, em_size, ex_size);
		text_transform.compute(0, layout, em_size, ex_size);
		text_align.compute(0, layout, em_size, ex_size);
		letter_spacing.compute(0, layout, em_size, ex_size);
		word_spacing.compute(0, layout, em_size, ex_size);
	}
}

}
