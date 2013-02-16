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
#include "API/CSSLayout/ComputedValues/css_computed_table.h"
#include "API/CSSLayout/ComputedValues/css_computed_values.h"

namespace clan
{

void CSSComputedTableReset::compute(const CSSComputedValues &parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (!parent.is_null())
	{
		const CSSComputedTableReset &parent_table_reset = parent.get_table_reset();

		table_layout.compute(&parent_table_reset.table_layout, layout, em_size, ex_size);
	}
	else
	{
		table_layout.compute(nullptr, layout, em_size, ex_size);
	}
}

void CSSComputedTableInherit::compute(const CSSComputedValues &parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (!parent.is_null())
	{
		const CSSComputedTableInherit &parent_table_inherit = parent.get_table_inherit();

		caption_side.compute(&parent_table_inherit.caption_side, layout, em_size, ex_size);
		border_collapse.compute(&parent_table_inherit.border_collapse, layout, em_size, ex_size);
		border_spacing.compute(&parent_table_inherit.border_spacing, layout, em_size, ex_size);
		empty_cells.compute(&parent_table_inherit.empty_cells, layout, em_size, ex_size);
	}
	else
	{
		caption_side.compute(nullptr, layout, em_size, ex_size);
		border_collapse.compute(nullptr, layout, em_size, ex_size);
		border_spacing.compute(nullptr, layout, em_size, ex_size);
		empty_cells.compute(nullptr, layout, em_size, ex_size);
	}
}

}
