/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

class CL_CSSUsedWidth
{
public:
	CL_CSSUsedWidth() : value(0.0f), expanding(false) { }

	CL_CSSUsedValue value;
	bool expanding;
};

class CL_CSSUsedHeight
{
public:
	CL_CSSUsedHeight() : value(0.0f), use_content(true) { }

	CL_CSSUsedValue value;
	bool use_content;
};
/*

#include "css_containing_block.h"

class CL_CSSBoxProperties;
class CL_CSSBoxMarginWidth;
class CL_CSSBoxPaddingWidth;

class CL_CSSUsedValues
{
public:
	CL_CSSUsedValues();
	void set_replaced(bool replaced = true);
	void set_width(const CL_CSSBoxProperties &properties);
	void set_width(const CL_CSSBoxProperties &properties, CL_CSSUsedValue width);
	void set_height(const CL_CSSBoxProperties &properties);
	void set_height(const CL_CSSBoxProperties &properties, CL_CSSUsedValue height);

	struct Margin
	{
		Margin() : left(0.0f), right(0.0f), top(0.0f), bottom(0.0f) { }
		CL_CSSUsedValue left, right, top, bottom;
	};
	struct Border
	{
		Border() : left(0.0f), right(0.0f), top(0.0f), bottom(0.0f) { }
		CL_CSSUsedValue left, right, top, bottom;
	};
	struct Padding
	{
		Padding() : left(0.0f), right(0.0f), top(0.0f), bottom(0.0f) { }
		CL_CSSUsedValue left, right, top, bottom;
	};
	struct Offset
	{
		Offset() : x(0.0f), y(0.0f) { }
		CL_CSSUsedValue x, y;
	};

	bool replaced;
	CL_CSSIntrinsicValues intrinsic;
	CL_CSSContainingBlock containing;

	Margin margin;
	Border border;
	Padding padding;
	CL_CSSUsedValue width;
	CL_CSSUsedValue height;
	Offset offset;
	bool undetermined_width;
	bool undetermined_height;

	CL_CSSUsedValue min_width, preferred_width;
	bool min_width_calculated;
	bool preferred_width_calculated;

private:
	void calc_noncontent_width(const CL_CSSBoxProperties &properties);
	void calc_noncontent_height(const CL_CSSBoxProperties &properties);

	static bool is_inline(const CL_CSSBoxProperties &properties);
	static bool is_block(const CL_CSSBoxProperties &properties);
	static bool is_floating(const CL_CSSBoxProperties &properties);
	static bool is_inline_block(const CL_CSSBoxProperties &properties);
	static bool is_absolute(const CL_CSSBoxProperties &properties);
	CL_CSSUsedValue get_margin_width(const CL_CSSBoxMarginWidth &margin_width);
	CL_CSSUsedValue get_padding_width(const CL_CSSBoxPaddingWidth &padding_width);
};
*/
