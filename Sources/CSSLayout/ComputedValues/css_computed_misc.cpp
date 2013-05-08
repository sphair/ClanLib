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

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/ComputedValues/css_computed_misc.h"
#include "API/CSSLayout/ComputedValues/css_computed_values.h"

namespace clan
{

void CSSComputedMiscReset::compute(const CSSComputedValues &parent, CSSResourceCache *layout, float em_size, float ex_size, bool is_before_or_after_pseudo_element)
{
	if (!parent.is_null())
	{
		const CSSComputedMiscReset &parent_misc_reset = parent.get_misc_reset();

		decoration_break.compute(&parent_misc_reset.decoration_break, layout, em_size, ex_size);
		page_break_before.compute(&parent_misc_reset.page_break_before, layout, em_size, ex_size);
		page_break_after.compute(&parent_misc_reset.page_break_after, layout, em_size, ex_size);
		page_break_inside.compute(&parent_misc_reset.page_break_inside, layout, em_size, ex_size);
		content.compute(&parent_misc_reset.content, layout, em_size, ex_size, is_before_or_after_pseudo_element);
		z_index.compute(&parent_misc_reset.z_index, layout, em_size, ex_size);
		clip.compute(&parent_misc_reset.clip, layout, em_size, ex_size);
		shadow.compute(&parent_misc_reset.shadow, layout, em_size, ex_size);
	}
	else
	{
		decoration_break.compute(0, layout, em_size, ex_size);
		page_break_before.compute(0, layout, em_size, ex_size);
		page_break_after.compute(0, layout, em_size, ex_size);
		page_break_inside.compute(0, layout, em_size, ex_size);
		content.compute(0, layout, em_size, ex_size, is_before_or_after_pseudo_element);
		z_index.compute(0, layout, em_size, ex_size);
		clip.compute(0, layout, em_size, ex_size);
		shadow.compute(0, layout, em_size, ex_size);
	}
}

void CSSComputedMiscInherit::compute(const CSSComputedValues &parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (!parent.is_null())
	{
		const CSSComputedMiscInherit &parent_misc_inherit = parent.get_misc_inherit();

		direction.compute(&parent_misc_inherit.direction, layout, em_size, ex_size);
		orphans.compute(&parent_misc_inherit.orphans, layout, em_size, ex_size);
		widows.compute(&parent_misc_inherit.widows, layout, em_size, ex_size);
		quotes.compute(&parent_misc_inherit.quotes, layout, em_size, ex_size);
		visibility.compute(&parent_misc_inherit.visibility, layout, em_size, ex_size);
		cursor.compute(&parent_misc_inherit.cursor, layout, em_size, ex_size);
	}
	else
	{
		direction.compute(0, layout, em_size, ex_size);
		orphans.compute(0, layout, em_size, ex_size);
		widows.compute(0, layout, em_size, ex_size);
		quotes.compute(0, layout, em_size, ex_size);
		visibility.compute(0, layout, em_size, ex_size);
		cursor.compute(0, layout, em_size, ex_size);
	}
}

}
