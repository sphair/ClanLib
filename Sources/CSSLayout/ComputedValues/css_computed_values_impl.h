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
#include "API/CSSLayout/ComputedValues/css_computed_values_updater.h"
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

class CSSComputedValuesUpdateSession : public CSSComputedValuesUpdater
{
public:
	CSSComputedValuesUpdateSession(CSSComputedValues_Impl *values)
		: values(values), box_updated(false), background_updated(false), border_updated(false), counter_updated(false),
		flex_updated(false), font_updated(false), generic_updated(false), list_style_updated(false), margin_updated(false),
		misc_reset_updated(false), misc_inherit_updated(false), outline_updated(false), padding_updated(false),
		table_reset_updated(false), table_inherit_updated(false), text_reset_updated(false), text_inherit_updated(false)
	{
	}

	CSSComputedBox &get_box()
	{
		if (!box_updated)
		{
			values->box = CSSComputedBox();
			box_updated = true;
		}
		return values->box;
	}

	CSSComputedBackground &get_background()
	{
		if (!background_updated)
		{
			values->background = CSSComputedBackground();
			background_updated = true;
		}
		return values->background;
	}

	CSSComputedBorder &get_border()
	{
		if (!border_updated)
		{
			values->border = CSSComputedBorder();
			border_updated = true;
		}
		return values->border;
	}

	CSSComputedCounter &get_counter()
	{
		if (!counter_updated)
		{
			values->counter = CSSComputedCounter();
			counter_updated = true;
		}
		return values->counter;
	}

	CSSComputedFlex &get_flex()
	{
		if (!flex_updated)
		{
			values->flex = CSSComputedFlex();
			flex_updated = true;
		}
		return values->flex;
	}

	CSSComputedFont &get_font()
	{
		if (!font_updated)
		{
			values->font = CSSComputedFont();
			font_updated = true;
		}
		return values->font;
	}

	CSSComputedGeneric &get_generic()
	{
		if (!generic_updated)
		{
			values->generic_values = CSSComputedGeneric();
			generic_updated = true;
		}
		return values->generic_values;
	}

	CSSComputedListStyle &get_list_style()
	{
		if (!list_style_updated)
		{
			values->list_style = CSSComputedListStyle();
			list_style_updated = true;
		}
		return values->list_style;
	}

	CSSComputedMargin &get_margin()
	{
		if (!margin_updated)
		{
			values->margin = CSSComputedMargin();
			margin_updated = true;
		}
		return values->margin;
	}

	CSSComputedMiscReset &get_misc_reset()
	{
		if (!misc_reset_updated)
		{
			values->misc_reset = CSSComputedMiscReset();
			misc_reset_updated = true;
		}
		return values->misc_reset;
	}

	CSSComputedMiscInherit &get_misc_inherit()
	{
		if (!misc_inherit_updated)
		{
			values->misc_inherit = CSSComputedMiscInherit();
			misc_inherit_updated = true;
		}
		return values->misc_inherit;
	}

	CSSComputedOutline &get_outline()
	{
		if (!outline_updated)
		{
			values->outline = CSSComputedOutline();
			outline_updated = true;
		}
		return values->outline;
	}

	CSSComputedPadding &get_padding()
	{
		if (!padding_updated)
		{
			values->padding = CSSComputedPadding();
			padding_updated = true;
		}
		return values->padding;
	}

	CSSComputedTableReset &get_table_reset()
	{
		if (!table_reset_updated)
		{
			values->table_reset = CSSComputedTableReset();
			table_reset_updated = true;
		}
		return values->table_reset;
	}

	CSSComputedTableInherit &get_table_inherit()
	{
		if (!table_inherit_updated)
		{
			values->table_inherit = CSSComputedTableInherit();
			table_inherit_updated = true;
		}
		return values->table_inherit;
	}

	CSSComputedTextReset &get_text_reset()
	{
		if (!text_reset_updated)
		{
			values->text_reset = CSSComputedTextReset();
			text_reset_updated = true;
		}
		return values->text_reset;
	}

	CSSComputedTextInherit &get_text_inherit()
	{
		if (!text_inherit_updated)
		{
			values->text_inherit = CSSComputedTextInherit();
			text_inherit_updated = true;
		}
		return values->text_inherit;
	}

	void compute()
	{
		// To do: use the updated booleans to only partially compute properties

		// Until the above to do has been implemented we must initialize all existing structures:

		if (!box_updated)
			values->box = CSSComputedBox();
		if (!background_updated)
			values->background = CSSComputedBackground();
		if (!border_updated)
			values->border = CSSComputedBorder();
		if (!counter_updated)
			values->counter = CSSComputedCounter();
		if (!flex_updated)
			values->flex = CSSComputedFlex();
		if (!font_updated)
			values->font = CSSComputedFont();
		if (!generic_updated)
			values->generic_values = CSSComputedGeneric();
		if (!list_style_updated)
			values->list_style = CSSComputedListStyle();
		if (!margin_updated)
			values->margin = CSSComputedMargin();
		if (!misc_reset_updated)
			values->misc_reset = CSSComputedMiscReset();
		if (!misc_inherit_updated)
			values->misc_inherit = CSSComputedMiscInherit();
		if (!outline_updated)
			values->outline = CSSComputedOutline();
		if (!padding_updated)
			values->padding = CSSComputedPadding();
		if (!table_reset_updated)
			values->table_reset = CSSComputedTableReset();
		if (!table_inherit_updated)
			values->table_inherit = CSSComputedTableInherit();
		if (!text_reset_updated)
			values->text_reset = CSSComputedTextReset();
		if (!text_inherit_updated)
			values->text_inherit = CSSComputedTextInherit();

		// Compute everything:

		values->font.compute(values->parent, values->resource_cache);
		values->font_generation++;

		float em_size = values->font.font_size.length.value;
		float ex_size = em_size * 0.5f;

		bool is_before_or_after_pseudo_element = false;

		values->box.compute(values->parent, values->resource_cache, em_size, ex_size);
		values->box_generation++;

		values->background.compute(values->parent, values->resource_cache, em_size, ex_size);
		values->background_generation++;

		values->text_reset.compute(values->parent, values->resource_cache, em_size, ex_size, values->font.line_height);
		values->text_reset_generation++;

		values->text_inherit.compute(values->parent, values->resource_cache, em_size, ex_size);
		values->text_inherit_generation++;

		values->border.compute(values->parent, values->resource_cache, em_size, ex_size, values->text_inherit.color.color);
		values->border_generation++;

		values->counter.compute(values->parent, values->resource_cache, em_size, ex_size);
		values->counter_generation++;

		values->flex.compute(values->parent, values->resource_cache, em_size, ex_size);
		values->flex_generation++;

		values->generic_values.compute(values->parent, values->resource_cache, em_size, ex_size);
		values->generic_generation++;

		values->list_style.compute(values->parent, values->resource_cache, em_size, ex_size);
		values->list_style_generation++;

		values->margin.compute(values->parent, values->resource_cache, em_size, ex_size);
		values->margin_generation++;

		values->misc_reset.compute(values->parent, values->resource_cache, em_size, ex_size, is_before_or_after_pseudo_element);
		values->misc_reset_generation++;

		values->misc_inherit.compute(values->parent, values->resource_cache, em_size, ex_size);
		values->misc_inherit_generation++;

		values->outline.compute(values->parent, values->resource_cache, em_size, ex_size);
		values->outline_generation++;

		values->padding.compute(values->parent, values->resource_cache, em_size, ex_size);
		values->padding_generation++;

		values->table_reset.compute(values->parent, values->resource_cache, em_size, ex_size);
		values->table_reset_generation++;

		values->table_inherit.compute(values->parent, values->resource_cache, em_size, ex_size);
		values->table_inherit_generation++;
	}

	CSSComputedValues_Impl *values;
	bool box_updated;
	bool background_updated;
	bool border_updated;
	bool counter_updated;
	bool flex_updated;
	bool font_updated;
	bool generic_updated;
	bool list_style_updated;
	bool margin_updated;
	bool misc_reset_updated;
	bool misc_inherit_updated;
	bool outline_updated;
	bool padding_updated;
	bool table_reset_updated;
	bool table_inherit_updated;
	bool text_reset_updated;
	bool text_inherit_updated;
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
		CSSComputedValuesUpdateSession updater(this);

		if (!selected_values.is_null())
		{
			const std::vector<CSSPropertyValue *> &selected_values_vector = selected_values.get_values();
			for (size_t i = selected_values_vector.size(); i > 0; --i)
			{
				selected_values_vector[i-1]->apply(&updater);
			}
		}

		if (!style_values.is_null())
		{
			const std::vector<std::unique_ptr<CSSPropertyValue> > &style_values_vector = style_values.get_values();
			for (size_t i = style_values_vector.size(); i > 0; --i)
			{
				style_values_vector[i-1]->apply(&updater);
			}
		}

		updater.compute();

		specified_values_changed = false;
	}
}

}
