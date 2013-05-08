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
#include "../PropertyValues/css_value_align_content.h"
#include "../PropertyValues/css_value_align_items.h"
#include "../PropertyValues/css_value_align_self.h"
#include "../PropertyValues/css_value_flex_basis.h"
#include "../PropertyValues/css_value_flex_direction.h"
#include "../PropertyValues/css_value_flex_wrap.h"
#include "../PropertyValues/css_value_flex_grow.h"
#include "../PropertyValues/css_value_flex_shrink.h"
#include "../PropertyValues/css_value_order.h"
#include "../PropertyValues/css_value_justify_content.h"

namespace clan
{

class CSSComputedValues;

class CL_API_CSSLAYOUT CSSComputedFlex
{
public:
	CSSValueFlexDirection flex_direction;
	CSSValueFlexWrap flex_wrap;
	CSSValueOrder order;

	CSSValueFlexGrow flex_grow;
	CSSValueFlexShrink flex_shrink;
	CSSValueFlexBasis flex_basis;

	CSSValueJustifyContent justify_content;
	CSSValueAlignItems align_items;
	CSSValueAlignSelf align_self;
	CSSValueAlignContent align_content;

	void compute(const CSSComputedValues &parent, CSSResourceCache *layout, float em_size, float ex_size);
};

}
