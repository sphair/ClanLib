/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#include "css_layout_tree_node.h"

class CL_CSSBoxElement;
class CL_CSSBlockLayout;
class CL_CSSTableColumn;
class CL_CSSTableRow;

class CL_CSSTableLayout : public CL_CSSLayoutTreeNode
{
public:
	CL_CSSTableLayout(CL_CSSBoxElement *block_element);
	~CL_CSSTableLayout();

	void add_row(CL_CSSBoxElement *row_element);
	void add_cell(CL_CSSLayoutTreeNode *cell_layout);

	void render(CL_GraphicContext &gc, CL_CSSResourceCache *resources);
	void set_component_geometry();
	int get_first_line_baseline();
	int get_last_line_baseline();

private:
	void calculate_top_down_sizes();
	void prepare_children();
	void layout_content(CL_GraphicContext &gc, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy);
	void position_cells(CL_CSSLayoutCursor &cursor);
	void layout_cells(CL_GraphicContext & gc, CL_CSSLayoutCursor & cursor);
	void calculate_cell_widths(CL_GraphicContext & gc, CL_CSSLayoutCursor & cursor);
	void calculate_preferred_cell_widths(CL_GraphicContext & gc, CL_CSSLayoutCursor & cursor);
	void calculate_minimum_cell_widths(CL_GraphicContext & gc, CL_CSSLayoutCursor & cursor);
	CL_Rect get_cell_border_box(size_t row, size_t col);
	void render_cell_non_content(CL_GraphicContext &gc, CL_CSSResourceCache *resources, size_t row, size_t col);

	std::vector<CL_CSSTableColumn> columns;
	std::vector<CL_CSSTableRow> rows;
	size_t next_column;
};
