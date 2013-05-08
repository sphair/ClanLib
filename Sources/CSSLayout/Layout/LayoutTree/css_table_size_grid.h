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

#include "css_used_value.h"

namespace clan
{

class CSSTableSizeGrid
{
public:
	CSSTableSizeGrid();
	CSSTableSizeGrid(size_t rows, size_t cols);

	void apply_width(size_t col, CSSUsedValue width);
	void apply_height(size_t row, CSSUsedValue height);
	void apply_width_span(size_t col, CSSUsedValue width, int span);
	void apply_height_span(size_t row, CSSUsedValue height, int span);
	void apply_collapsed_table_border(CSSUsedValue border_left, CSSUsedValue border_top, CSSUsedValue border_right, CSSUsedValue border_bottom);
	void apply_collapsed_border(size_t row, size_t col, CSSUsedValue border_left, CSSUsedValue border_top, CSSUsedValue border_right, CSSUsedValue border_bottom);
	void apply_separate_table_border(CSSUsedValue border_left, CSSUsedValue border_top, CSSUsedValue border_right, CSSUsedValue border_bottom);
	void apply_separate_table_padding(CSSUsedValue padding_left, CSSUsedValue padding_top, CSSUsedValue padding_right, CSSUsedValue padding_bottom);
	void apply_separate_spacing(CSSUsedValue spacing_width, CSSUsedValue spacing_height);
	void expand_table_width(CSSUsedValue width);

	CSSUsedValue get_table_width() const;
	CSSUsedValue get_table_height() const;
	CSSUsedValue get_width(size_t col) const;
	CSSUsedValue get_height(size_t col) const;
	CSSUsedValue get_non_content_width(size_t index) const;
	CSSUsedValue get_non_content_height(size_t index) const;

	size_t rows() const { return content_heights.size(); }
	size_t cols() const { return content_widths.size(); }

	std::vector<CSSUsedValue> non_content_widths;
	std::vector<CSSUsedValue> content_widths;

	std::vector<CSSUsedValue> non_content_heights;
	std::vector<CSSUsedValue> content_heights;
};

}
