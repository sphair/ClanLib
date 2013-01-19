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

#include "css_intrinsic_values.h"

namespace clan
{

class CSSUsedWidth
{
public:
	CSSUsedWidth() : value(0.0f), expanding(false) { }

	CSSUsedValue value;
	bool expanding;
};

class CSSUsedHeight
{
public:
	CSSUsedHeight() : value(0.0f), use_content(true) { }

	CSSUsedValue value;
	bool use_content;
};
/*

#include "css_containing_block.h"

class CSSComputedBox;
class CSSValueMarginWidth;
class CSSValuePaddingWidth;

class CSSUsedValues
{
public:
	CSSUsedValues();
	void set_replaced(bool replaced = true);
	void set_width(const CSSComputedBox &properties);
	void set_width(const CSSComputedBox &properties, CSSUsedValue width);
	void set_height(const CSSComputedBox &properties);
	void set_height(const CSSComputedBox &properties, CSSUsedValue height);

	struct Margin
	{
		Margin() : left(0.0f), right(0.0f), top(0.0f), bottom(0.0f) { }
		CSSUsedValue left, right, top, bottom;
	};
	struct Border
	{
		Border() : left(0.0f), right(0.0f), top(0.0f), bottom(0.0f) { }
		CSSUsedValue left, right, top, bottom;
	};
	struct Padding
	{
		Padding() : left(0.0f), right(0.0f), top(0.0f), bottom(0.0f) { }
		CSSUsedValue left, right, top, bottom;
	};
	struct Offset
	{
		Offset() : x(0.0f), y(0.0f) { }
		CSSUsedValue x, y;
	};

	bool replaced;
	CSSIntrinsicValues intrinsic;
	CSSContainingBlock containing;

	Margin margin;
	Border border;
	Padding padding;
	CSSUsedValue width;
	CSSUsedValue height;
	Offset offset;
	bool undetermined_width;
	bool undetermined_height;

	CSSUsedValue min_width, preferred_width;
	bool min_width_calculated;
	bool preferred_width_calculated;

private:
	void calc_noncontent_width(const CSSComputedBox &properties);
	void calc_noncontent_height(const CSSComputedBox &properties);

	static bool is_inline(const CSSComputedBox &properties);
	static bool is_block(const CSSComputedBox &properties);
	static bool is_floating(const CSSComputedBox &properties);
	static bool is_inline_block(const CSSComputedBox &properties);
	static bool is_absolute(const CSSComputedBox &properties);
	CSSUsedValue get_margin_width(const CSSValueMarginWidth &margin_width);
	CSSUsedValue get_padding_width(const CSSValuePaddingWidth &padding_width);
};
*/

}
