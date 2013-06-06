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

#include "../api_csslayout.h"
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

class CL_API_CSSLAYOUT CSSComputedValuesUpdater
{
public:
	virtual CSSComputedBox &get_box() = 0;
	virtual CSSComputedBackground &get_background() = 0;
	virtual CSSComputedBorder &get_border() = 0;
	virtual CSSComputedCounter &get_counter() = 0;
	virtual CSSComputedFlex &get_flex() = 0;
	virtual CSSComputedFont &get_font() = 0;
	virtual CSSComputedGeneric &get_generic() = 0;
	virtual CSSComputedListStyle &get_list_style() = 0;
	virtual CSSComputedMargin &get_margin() = 0;
	virtual CSSComputedMiscReset &get_misc_reset() = 0;
	virtual CSSComputedMiscInherit &get_misc_inherit() = 0;
	virtual CSSComputedOutline &get_outline() = 0;
	virtual CSSComputedPadding &get_padding() = 0;
	virtual CSSComputedTableReset &get_table_reset() = 0;
	virtual CSSComputedTableInherit &get_table_inherit() = 0;
	virtual CSSComputedTextReset &get_text_reset() = 0;
	virtual CSSComputedTextInherit &get_text_inherit() = 0;
};

/// \}
}
