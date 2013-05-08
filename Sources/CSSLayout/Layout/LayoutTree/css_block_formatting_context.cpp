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
#include "css_block_formatting_context.h"
#include "css_layout_cursor.h"

namespace clan
{

CSSBlockFormattingContext::CSSBlockFormattingContext(CSSBlockFormattingContext *parent)
: parent(parent), x(0), y(0)
{
}

void CSSBlockFormattingContext::clear()
{
	left_floats.clear();
	right_floats.clear();
}

void CSSBlockFormattingContext::set_position(int new_x, int new_y)
{
	x = new_x;
	y = new_y;
}

int CSSBlockFormattingContext::get_x() const
{
	if (parent)
		return parent->get_x() + x;
	else
		return x;
}

int CSSBlockFormattingContext::get_y() const
{
	if (parent)
		return parent->get_y() + y;
	else
		return y;
}

Rect CSSBlockFormattingContext::place_left(Rect box, int width)
{
	while (true)
	{
		Rect left_box = find_relevant_left_box(box);
		Rect right_box = find_relevant_right_box(box, width);

		if (is_null(left_box) && is_null(right_box))
			break;

		Rect offset_box = box;
		if (offset_box.left < left_box.right)
		{
			int delta = left_box.right-offset_box.left;
			offset_box.left += delta;
			offset_box.right += delta;
		}
		if (offset_box.right <= right_box.left)
		{
			box = offset_box;
			break;
		}

		int delta = 0;
		if (!is_null(left_box) && !is_null(right_box))
			delta = min(left_box.bottom, right_box.bottom)-box.top;
		else if (is_null(left_box))
			delta = right_box.bottom-box.top;
		else if (is_null(right_box))
			delta = left_box.bottom-box.top;

		box.top += delta;
		box.bottom += delta;
	}
	return box;
}

Rect CSSBlockFormattingContext::place_right(Rect box, int width)
{
	while (true)
	{
		Rect left_box = find_relevant_left_box(box);
		Rect right_box = find_relevant_right_box(box, width);

		if (is_null(left_box) && is_null(right_box))
			break;

		Rect offset_box = box;
		if (offset_box.right > right_box.left)
		{
			int delta = right_box.left-offset_box.right;
			offset_box.left += delta;
			offset_box.right += delta;
		}
		if (offset_box.left >= left_box.right)
		{
			box = offset_box;
			break;
		}

		int delta = 0;
		if (!is_null(left_box) && !is_null(right_box))
			delta = min(left_box.bottom, right_box.bottom)-box.top;
		else if (is_null(left_box))
			delta = right_box.bottom-box.top;
		else if (is_null(right_box))
			delta = left_box.bottom-box.top;

		box.top += delta;
		box.bottom += delta;
	}
	return box;
}

Rect CSSBlockFormattingContext::place_right_shrink_to_fit(Rect box, int width)
{
	return place_left(box, width);
}

Rect CSSBlockFormattingContext::float_left(Rect box, int width)
{
	box = place_left(box, width);
	left_floats.push_back(CSSFloat(box, CSSFloat::type_left));
	return box;
}

Rect CSSBlockFormattingContext::float_right(Rect box, int width)
{
	box = place_right(box, width);
	right_floats.push_back(CSSFloat(box, CSSFloat::type_right));
	return box;
}

Rect CSSBlockFormattingContext::float_right_shrink_to_fit(Rect box, int width)
{
	box = place_left(box, width);
	left_floats.push_back(CSSFloat(box, CSSFloat::type_right));
	return box;
}

Rect CSSBlockFormattingContext::find_relevant_left_box(const Rect &box) const
{
	Rect left_box;
	for (size_t i = 0; i < left_floats.size(); i++)
	{
		int top = max(left_floats[i].box.top, box.top);
		int bottom = min(left_floats[i].box.bottom, box.bottom);
		if (bottom > top && left_box.right < left_floats[i].box.right)
			left_box = left_floats[i].box;
	}
	return left_box;
}

Rect CSSBlockFormattingContext::find_relevant_right_box(const Rect &box, int width) const
{
	Rect right_box(width, 0, width, 0);
	for (size_t i = 0; i < right_floats.size(); i++)
	{
		int top = max(right_floats[i].box.top, box.top);
		int bottom = min(right_floats[i].box.bottom, box.bottom);
		if (bottom > top && right_box.left > right_floats[i].box.left)
			right_box = right_floats[i].box;
	}
	return right_box;
}

bool CSSBlockFormattingContext::is_null(const Rect &box)
{
	return box.left == box.right || box.top == box.bottom;
}

Rect CSSBlockFormattingContext::find_line_box(int left, int right, int y, int height, int minimum_width) const
{
	Rect box(left, y, right, y+height);
	while (true)
	{
		Rect left_box = find_relevant_left_box(box);
		Rect right_box = find_relevant_right_box(box, right);
		if (!is_null(left_box))
			box.left = max(box.left, left_box.right);
		if (!is_null(right_box))
			box.right = min(box.right, right_box.left);
		if (box.get_width() >= minimum_width || (is_null(left_box) && is_null(right_box)))
			return box;

		if (!is_null(left_box))
			box.top = max(box.top, left_box.bottom);
		if (!is_null(right_box))
			box.top = max(box.top, right_box.bottom);
		box.bottom = box.top + height;
		box.left = left;
		box.right = right;
	}
}

int CSSBlockFormattingContext::find_left_clearance() const
{
	int y = 0;
	for (size_t i = 0; i < left_floats.size(); i++)
	{
		if (left_floats[i].type == CSSFloat::type_left)
			y = max(y, left_floats[i].box.bottom);
	}
	return y;
}

int CSSBlockFormattingContext::find_right_clearance() const
{
	int y = 0;
	for (size_t i = 0; i < left_floats.size(); i++)
	{
		if (left_floats[i].type == CSSFloat::type_right)
			y = max(y, left_floats[i].box.bottom);
	}
	for (size_t i = 0; i < right_floats.size(); i++)
	{
		if (right_floats[i].type == CSSFloat::type_right)
			y = max(y, right_floats[i].box.bottom);
	}
	return y;
}

}
