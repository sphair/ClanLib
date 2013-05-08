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

namespace clan
{

class CSSFloat;
class CSSLayoutTreeNode;
class CSSLayoutCursor;

class CSSBlockFormattingContext
{
public:
	CSSBlockFormattingContext(CSSBlockFormattingContext *parent);

	void clear();
	void set_position(int x, int y);
	Rect place_left(Rect box, int width);
	Rect place_right(Rect box, int width);
	Rect place_right_shrink_to_fit(Rect box, int width);
	Rect float_left(Rect box, int width);
	Rect float_right(Rect box, int width);
	Rect float_right_shrink_to_fit(Rect box, int width);
	Rect find_line_box(int left, int right, int y, int height, int minimum_width) const;
	int find_left_clearance() const;
	int find_right_clearance() const;

	int get_x() const;
	int get_y() const;
	int get_local_x() const { return x; }
	int get_local_y() const { return y; }
	CSSBlockFormattingContext *get_parent() { return parent; }

private:
	static bool is_null(const Rect &box);
	Rect find_relevant_left_box(const Rect &box) const;
	Rect find_relevant_right_box(const Rect &box, int width) const;

	CSSBlockFormattingContext *parent;
	int x, y;
	std::vector<CSSFloat> left_floats;
	std::vector<CSSFloat> right_floats;
};

class CSSFloat
{
public:
	enum Type
	{
		type_left,
		type_right
	};

	CSSFloat(const Rect &box, Type type) : box(box), type(type) { }

	Rect box;
	Type type;
};

}
