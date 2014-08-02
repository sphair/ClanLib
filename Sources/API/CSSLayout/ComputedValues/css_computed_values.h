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

#include "css_computed_background.h"
#include "css_computed_border.h"
#include "css_computed_box.h"
#include "css_computed_counter.h"
#include "css_computed_flex.h"
#include "css_computed_font.h"
#include "css_computed_generic.h"
#include "css_computed_list_style.h"
#include "css_computed_margin.h"
#include "css_computed_misc.h"
#include "css_computed_outline.h"
#include "css_computed_padding.h"
#include "css_computed_table.h"
#include "css_computed_text.h"

namespace clan
{
/// \addtogroup clanCSSLayout_ComputedValues clanCSSLayout Computed Values
/// \{

class CSSSelectResult;
class CSSStyleProperties;
class CSSPropertyValue;
class CSSComputedValues_Impl;
class CSSResourceCache;

class CSSComputedValues
{
public:
	CSSComputedValues();
	CSSComputedValues(CSSResourceCache *resource_cache);

	bool is_null() const;

	void set_parent(const CSSComputedValues &parent);
	void set_specified_values(const CSSSelectResult &selected_values);
	void set_specified_values(const CSSSelectResult &selected_values, const std::string &style_values);
	void set_specified_values(const CSSSelectResult &selected_values, const CSSStyleProperties &style_values);

	const CSSComputedBox &get_box() const;
	int get_box_generation() const;

	const CSSComputedBackground &get_background() const;
	int get_background_generation() const;

	const CSSComputedBorder &get_border() const;
	int get_border_generation() const;

	const CSSComputedCounter &get_counter() const;
	int get_counter_generation() const;

	const CSSComputedFlex &get_flex() const;
	int get_flex_generation() const;

	const CSSComputedFont &get_font() const;
	int get_font_generation() const;

	const CSSComputedGeneric &get_generic() const;
	int get_generic_generation() const;

	const CSSComputedListStyle &get_list_style() const;
	int get_list_style_generation() const;

	const CSSComputedMargin &get_margin() const;
	int get_margin_generation() const;

	const CSSComputedMiscReset &get_misc_reset() const;
	int get_misc_reset_generation() const;

	const CSSComputedMiscInherit &get_misc_inherit() const;
	int get_misc_inherit_generation() const;

	const CSSComputedOutline &get_outline() const;
	int get_outline_generation() const;

	const CSSComputedPadding &get_padding() const;
	int get_padding_generation() const;

	const CSSComputedTableReset &get_table_reset() const;
	int get_table_reset_generation() const;

	const CSSComputedTableInherit &get_table_inherit() const;
	int get_table_inherit_generation() const;

	const CSSComputedTextReset &get_text_reset() const;
	int get_text_reset_generation() const;

	const CSSComputedTextInherit &get_text_inherit() const;
	int get_text_inherit_generation() const;

private:
	std::shared_ptr<CSSComputedValues_Impl> impl;

	friend class CSSComputedValues_Impl;
};

/// \}
}
