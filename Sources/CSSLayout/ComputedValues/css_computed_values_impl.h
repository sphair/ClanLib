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

#pragma once

#include "API/CSSLayout/ComputedValues/css_computed_background.h"
#include "API/CSSLayout/ComputedValues/css_computed_border.h"
#include "API/CSSLayout/ComputedValues/css_computed_box.h"
#include "API/CSSLayout/ComputedValues/css_computed_counter.h"
#include "API/CSSLayout/ComputedValues/css_computed_flex.h"
#include "API/CSSLayout/ComputedValues/css_computed_font.h"
#include "API/CSSLayout/ComputedValues/css_computed_generic.h"
#include "API/CSSLayout/ComputedValues/css_computed_list_style.h"
#include "API/CSSLayout/ComputedValues/css_computed_margin.h"
#include "API/CSSLayout/ComputedValues/css_computed_misc.h"
#include "API/CSSLayout/ComputedValues/css_computed_outline.h"
#include "API/CSSLayout/ComputedValues/css_computed_padding.h"
#include "API/CSSLayout/ComputedValues/css_computed_table.h"
#include "API/CSSLayout/ComputedValues/css_computed_text.h"
#include "API/CSSLayout/ComputedValues/css_computed_values.h"
#include "API/CSSLayout/CSSDocument/css_select_result.h"
#include "API/CSSLayout/CSSDocument/css_style_properties.h"
#include <algorithm>

namespace clan
{

class CSSComputedValues_Impl
{
public:
	CSSComputedValues_Impl(CSSResourceCache *resource_cache);
	~CSSComputedValues_Impl();

	void set_parent(const CSSComputedValues &parent);
	void set_specified_values_changed();
	void update_if_changed();

	CSSComputedValues parent;
	std::vector<CSSComputedValues_Impl *> children;

	bool specified_values_changed;

	CSSComputedBox box;
	int box_generation;

	CSSComputedBackground background;
	int background_generation;

	CSSComputedBorder border;
	int border_generation;

	CSSComputedCounter counter;
	int counter_generation;

	CSSComputedFlex flex;
	int flex_generation;

	CSSComputedFont font;
	int font_generation;

	CSSComputedGeneric generic_values;
	int generic_generation;

	CSSComputedListStyle list_style;
	int list_style_generation;

	CSSComputedMargin margin;
	int margin_generation;

	CSSComputedMiscReset misc_reset;
	int misc_reset_generation;

	CSSComputedMiscInherit misc_inherit;
	int misc_inherit_generation;

	CSSComputedOutline outline;
	int outline_generation;

	CSSComputedPadding padding;
	int padding_generation;

	CSSComputedTableReset table_reset;
	int table_reset_generation;

	CSSComputedTableInherit table_inherit;
	int table_inherit_generation;

	CSSComputedTextReset text_reset;
	int text_reset_generation;

	CSSComputedTextInherit text_inherit;
	int text_inherit_generation;

	CSSSelectResult selected_values;
	CSSStyleProperties style_values;

	CSSResourceCache *resource_cache;

private:
	void detach_from_parent();
};

inline CSSComputedValues_Impl::CSSComputedValues_Impl(CSSResourceCache *resource_cache) :
	specified_values_changed(true),
	box_generation(0),
	background_generation(0),
	border_generation(0),
	counter_generation(0),
	flex_generation(0),
	font_generation(0),
	generic_generation(0),
	list_style_generation(0),
	margin_generation(0),
	misc_reset_generation(0),
	misc_inherit_generation(0),
	outline_generation(0),
	padding_generation(0),
	table_reset_generation(0),
	table_inherit_generation(0),
	text_reset_generation(0),
	text_inherit_generation(0),
	resource_cache(resource_cache)
{
}

inline CSSComputedValues_Impl::~CSSComputedValues_Impl()
{
	detach_from_parent();
}

inline void CSSComputedValues_Impl::set_parent(const CSSComputedValues &new_parent)
{
	detach_from_parent();

	parent = new_parent;

	if (parent.impl)
		parent.impl->children.push_back(this);

	set_specified_values_changed();
}

inline void CSSComputedValues_Impl::detach_from_parent()
{
	if (parent.impl)
		parent.impl->children.erase(std::find(parent.impl->children.begin(), parent.impl->children.end(), this));
}

inline void CSSComputedValues_Impl::set_specified_values_changed()
{
	specified_values_changed = true;
	for (size_t i = 0; i < children.size(); i++)
		children[i]->set_specified_values_changed();
}

inline void CSSComputedValues_Impl::update_if_changed()
{
	if (specified_values_changed)
	{
		const CSSComputedBox *parent_computed_box_values = (!parent.is_null()) ? &parent.get_box() : nullptr;

		box = CSSComputedBox();
		if (!selected_values.is_null())
		{
			const std::vector<CSSPropertyValue *> &selected_values_vector = selected_values.get_values();
			for (size_t i = selected_values_vector.size(); i > 0; --i)
			{
				selected_values_vector[i-1]->apply_to_box(box);
			}
		}

		if (!style_values.is_null())
		{
			const std::vector<std::unique_ptr<CSSPropertyValue> > &style_values_vector = style_values.get_values();
			for (size_t i = style_values_vector.size(); i > 0; --i)
			{
				style_values_vector[i-1]->apply_to_box(box);
			}
		}
		box.compute(parent_computed_box_values, resource_cache);

		box_generation++;
		specified_values_changed = false;
	}
}

}
