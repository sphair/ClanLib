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

#include "../api_csslayout.h"
#include "../PropertyValues/css_value_text_align.h"
#include "../PropertyValues/css_value_text_decoration.h"
#include "../PropertyValues/css_value_text_indent.h"
#include "../PropertyValues/css_value_text_transform.h"
#include "../PropertyValues/css_value_vertical_align.h"
#include "../PropertyValues/css_value_white_space.h"
#include "../PropertyValues/css_value_color.h"
#include "../PropertyValues/css_value_letter_spacing.h"
#include "../PropertyValues/css_value_word_spacing.h"
#include "../PropertyValues/css_value_unicode_bidi.h"

namespace clan
{

class CSSComputedValues;

class CL_API_CSSLAYOUT CSSComputedTextReset
{
public:
	CSSValueTextDecoration text_decoration; // complex

	CSSValueVerticalAlign vertical_align;
	CSSValueUnicodeBidi unicode_bidi;

	void compute(const CSSComputedValues &parent, CSSResourceCache *layout, float em_size, float ex_size, const CSSValueLineHeight &computed_line_height);
};

class CL_API_CSSLAYOUT CSSComputedTextInherit
{
public:
	CSSValueWhiteSpace white_space; // inherited
	CSSValueColor color; // inherited
	CSSValueTextIndent text_indent; // inherited
	CSSValueTextTransform text_transform; // inherited
	CSSValueTextAlign text_align; // inherited
	CSSValueLetterSpacing letter_spacing; // inherited
	CSSValueWordSpacing word_spacing; // inherited

	void compute(const CSSComputedValues &parent, CSSResourceCache *layout, float em_size, float ex_size);
};

}
