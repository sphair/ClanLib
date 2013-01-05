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

#include "CSSLayout/precomp.h"
#include "css_table_size_grid.h"

namespace clan
{

CSSTableSizeGrid::CSSTableSizeGrid()
{
}

CSSTableSizeGrid::CSSTableSizeGrid(size_t height, size_t width)
{
	content_widths.resize(width, 0);
	content_heights.resize(height, 0);
	non_content_widths.resize(width+1, 0);
	non_content_heights.resize(height+1, 0);
}

void CSSTableSizeGrid::apply_width(size_t col, CSSUsedValue width)
{
	content_widths[col] = max(content_widths[col], width);
}

void CSSTableSizeGrid::apply_height(size_t row, CSSUsedValue height)
{
	content_heights[row] = max(content_heights[row], height);
}

void CSSTableSizeGrid::apply_width_span(size_t col, CSSUsedValue width, int span)
{
	CSSUsedValue available_width = 0.0f;
	for (size_t i = 0; i < span; i++)
		available_width += content_widths[col+i];

	if (available_width < width)
	{
		for (size_t i = 0; i < span; i++)
			content_widths[col+i] += (width-available_width)/span;
	}
}

void CSSTableSizeGrid::apply_height_span(size_t row, CSSUsedValue height, int span)
{
	CSSUsedValue available_height = 0.0f;
	for (size_t i = 0; i < span; i++)
		available_height += content_heights[row+i];

	if (available_height < height)
	{
		for (size_t i = 0; i < span; i++)
			content_heights[row+i] += (height-available_height)/span;
	}
}

void CSSTableSizeGrid::apply_collapsed_table_border(CSSUsedValue border_left, CSSUsedValue border_top, CSSUsedValue border_right, CSSUsedValue border_bottom)
{
	if (cols() > 0)
	{
		non_content_widths[0] = max(non_content_widths[0], border_left*0.5f);
		non_content_widths[cols()] = max(non_content_widths[cols()], border_right*0.5f);
	}
	if (rows() > 0)
	{
		non_content_heights[0] = max(non_content_heights[0], border_top*0.5f);
		non_content_heights[rows()] = max(non_content_heights[rows()], border_bottom*0.5f);
	}
}

void CSSTableSizeGrid::apply_collapsed_border(size_t row, size_t col, CSSUsedValue border_left, CSSUsedValue border_top, CSSUsedValue border_right, CSSUsedValue border_bottom)
{
	if (col == 0)
		border_left *= 0.5f;
	if (col+1 == cols())
		border_right *= 0.5f;

	if (row == 0)
		border_top *= 0.5f;
	if (row+1 == rows())
		border_bottom *= 0.5f;

	if (cols() > 0)
	{
		non_content_widths[col] = max(non_content_widths[col], border_left);
		non_content_widths[col+1] = max(non_content_widths[col+1], border_right);
	}
	if (rows() > 0)
	{
		non_content_heights[row] = max(non_content_heights[row], border_top);
		non_content_heights[row+1] = max(non_content_heights[row+1], border_bottom);
	}
}

void CSSTableSizeGrid::apply_separate_table_border(CSSUsedValue border_left, CSSUsedValue border_top, CSSUsedValue border_right, CSSUsedValue border_bottom)
{
	if (cols() > 0)
	{
		non_content_widths[0] = non_content_widths[0] + border_left;
		non_content_widths[cols()] = non_content_widths[cols()] + border_right;
	}
	if (rows() > 0)
	{
		non_content_heights[0] = non_content_heights[0] + border_top;
		non_content_heights[rows()] = non_content_heights[rows()] + border_bottom;
	}
}

void CSSTableSizeGrid::apply_separate_table_padding(CSSUsedValue padding_left, CSSUsedValue padding_top, CSSUsedValue padding_right, CSSUsedValue padding_bottom)
{
	if (cols() > 0)
	{
		non_content_widths[0] = non_content_widths[0] + padding_left;
		non_content_widths[cols()] = non_content_widths[cols()] + padding_right;
	}
	if (rows() > 0)
	{
		non_content_heights[0] = non_content_heights[0] + padding_top;
		non_content_heights[rows()] = non_content_heights[rows()] + padding_bottom;
	}
}

void CSSTableSizeGrid::apply_separate_spacing(CSSUsedValue spacing_width, CSSUsedValue spacing_height)
{
	for (size_t col = 1; col + 1 < non_content_widths.size(); col++)
		non_content_widths[col] += spacing_width;

	for (size_t row = 1; row + 1 < non_content_heights.size(); row++)
		non_content_heights[row] += spacing_height;
}

void CSSTableSizeGrid::expand_table_width(CSSUsedValue width)
{
	CSSUsedValue available_width = get_table_width();
	if (available_width < width)
	{
		for (size_t col = 0; col < content_widths.size(); col++)
			content_widths[col] += (width-available_width)/content_widths.size();
	}
}

CSSUsedValue CSSTableSizeGrid::get_table_width() const
{
	CSSUsedValue width = 0;
	for (size_t col = 0; col < content_widths.size(); col++)
	{
		width += non_content_widths[col];
		width += content_widths[col];
	}
	if (!content_widths.empty())
		width += non_content_widths[content_widths.size()];
	return width;
}

CSSUsedValue CSSTableSizeGrid::get_table_height() const
{
	CSSUsedValue height = 0;
	for (size_t row = 0; row < content_heights.size(); row++)
	{
		height += non_content_heights[row];
		height += content_heights[row];
	}
	if (!content_heights.empty())
		height += non_content_heights[content_heights.size()];
	return height;
}

CSSUsedValue CSSTableSizeGrid::get_width(size_t col) const
{
	return content_widths[col];
}

CSSUsedValue CSSTableSizeGrid::get_height(size_t row) const
{
	return content_heights[row];
}

CSSUsedValue CSSTableSizeGrid::get_non_content_width(size_t index) const
{
	return non_content_widths[index];
}

CSSUsedValue CSSTableSizeGrid::get_non_content_height(size_t index) const
{
	return non_content_heights[index];
}

}
