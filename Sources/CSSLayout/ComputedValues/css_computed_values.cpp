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
#include "API/CSSLayout/ComputedValues/css_computed_values.h"
#include "css_computed_values_impl.h"

namespace clan
{

CSSComputedValues::CSSComputedValues()
{
}

CSSComputedValues::CSSComputedValues(CSSResourceCache *resource_cache)
: impl(std::make_shared<CSSComputedValues_Impl>(resource_cache))
{
}

bool CSSComputedValues::is_null() const
{
	return !impl;
}

void CSSComputedValues::set_parent(const CSSComputedValues &parent)
{
	impl->set_parent(parent);
}

void CSSComputedValues::set_specified_values(const CSSSelectResult &selected_values)
{
	set_specified_values(selected_values, CSSStyleProperties());
}

void CSSComputedValues::set_specified_values(const CSSSelectResult &selected_values, const std::string &style_values)
{
	set_specified_values(selected_values, CSSStyleProperties(style_values));
}

void CSSComputedValues::set_specified_values(const CSSSelectResult &selected_values, const CSSStyleProperties &style_values)
{
	impl->selected_values = selected_values;
	impl->style_values = style_values;
	impl->set_specified_values_changed();
}

const CSSComputedBox &CSSComputedValues::get_box() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->box;
}

int CSSComputedValues::get_box_generation() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->box_generation;
}

const CSSComputedBackground &CSSComputedValues::get_background() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->background;
}

int CSSComputedValues::get_background_generation() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->background_generation;
}

const CSSComputedBorder &CSSComputedValues::get_border() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->border;
}

int CSSComputedValues::get_border_generation() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->border_generation;
}

const CSSComputedCounter &CSSComputedValues::get_counter() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->counter;
}

int CSSComputedValues::get_counter_generation() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->counter_generation;
}

const CSSComputedFlex &CSSComputedValues::get_flex() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->flex;
}

int CSSComputedValues::get_flex_generation() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->flex_generation;
}

const CSSComputedFont &CSSComputedValues::get_font() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->font;
}

int CSSComputedValues::get_font_generation() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->font_generation;
}

const CSSComputedGeneric &CSSComputedValues::get_generic() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->generic_values;
}

int CSSComputedValues::get_generic_generation() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->generic_generation;
}

const CSSComputedListStyle &CSSComputedValues::get_list_style() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->list_style;
}

int CSSComputedValues::get_list_style_generation() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->list_style_generation;
}

const CSSComputedMargin &CSSComputedValues::get_margin() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->margin;
}

int CSSComputedValues::get_margin_generation() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->margin_generation;
}

const CSSComputedMiscReset &CSSComputedValues::get_misc_reset() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->misc_reset;
}

int CSSComputedValues::get_misc_reset_generation() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->misc_reset_generation;
}

const CSSComputedMiscInherit &CSSComputedValues::get_misc_inherit() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->misc_inherit;
}

int CSSComputedValues::get_misc_inherit_generation() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->misc_inherit_generation;
}

const CSSComputedOutline &CSSComputedValues::get_outline() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->outline;
}

int CSSComputedValues::get_outline_generation() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->outline_generation;
}

const CSSComputedPadding &CSSComputedValues::get_padding() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->padding;
}

int CSSComputedValues::get_padding_generation() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->padding_generation;
}

const CSSComputedTableReset &CSSComputedValues::get_table_reset() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->table_reset;
}

int CSSComputedValues::get_table_reset_generation() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->table_reset_generation;
}

const CSSComputedTableInherit &CSSComputedValues::get_table_inherit() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->table_inherit;
}

int CSSComputedValues::get_table_inherit_generation() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->table_inherit_generation;
}

const CSSComputedTextReset &CSSComputedValues::get_text_reset() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->text_reset;
}

int CSSComputedValues::get_text_reset_generation() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->text_reset_generation;
}

const CSSComputedTextInherit &CSSComputedValues::get_text_inherit() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->text_inherit;
}

int CSSComputedValues::get_text_inherit_generation() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->text_inherit_generation;
}

}
