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
#include "API/CSSLayout/ComputedValues/css_computed_flex.h"
#include "API/CSSLayout/ComputedValues/css_computed_values.h"

namespace clan
{

void CSSComputedFlex::compute(const CSSComputedValues &parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (!parent.is_null())
	{
		const CSSComputedFlex &parent_flex = parent.get_flex();

		flex_direction.compute(&parent_flex.flex_direction, layout, em_size, ex_size);
		flex_wrap.compute(&parent_flex.flex_wrap, layout, em_size, ex_size);
		order.compute(&parent_flex.order, layout, em_size, ex_size);
		flex_grow.compute(&parent_flex.flex_grow, layout, em_size, ex_size);
		flex_shrink.compute(&parent_flex.flex_shrink, layout, em_size, ex_size);
		flex_basis.compute(&parent_flex.flex_basis, layout, em_size, ex_size);
		justify_content.compute(&parent_flex.justify_content, layout, em_size, ex_size);
		align_items.compute(&parent_flex.align_items, layout, em_size, ex_size);
		align_self.compute(&parent_flex.align_self, layout, em_size, ex_size, &parent_flex.align_items);
		align_content.compute(&parent_flex.align_content, layout, em_size, ex_size);
	}
	else
	{
		flex_direction.compute(nullptr, layout, em_size, ex_size);
		flex_wrap.compute(nullptr, layout, em_size, ex_size);
		order.compute(nullptr, layout, em_size, ex_size);
		flex_grow.compute(nullptr, layout, em_size, ex_size);
		flex_shrink.compute(nullptr, layout, em_size, ex_size);
		flex_basis.compute(nullptr, layout, em_size, ex_size);
		justify_content.compute(nullptr, layout, em_size, ex_size);
		align_items.compute(nullptr, layout, em_size, ex_size);
		align_self.compute(nullptr, layout, em_size, ex_size, nullptr);
		align_content.compute(nullptr, layout, em_size, ex_size);
	}
}

}
