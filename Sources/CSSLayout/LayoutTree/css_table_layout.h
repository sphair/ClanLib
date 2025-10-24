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

#include "css_layout_tree_node.h"
#include "css_table_size_grid.h"

class CL_CSSBoxElement;
class CL_CSSTableColumn;
class CL_CSSTableRow;
class CL_CSSBoxText;
class CL_CSSInlineLayout;

class CL_CSSTableLayout : public CL_CSSLayoutTreeNode
{
public:
	CL_CSSTableLayout(CL_CSSBoxElement *block_element);
	~CL_CSSTableLayout();

	void add_row(CL_CSSBoxElement *row_element);
	void add_cell(CL_CSSLayoutTreeNode *cell_layout, int col_span, int row_span);
	void add_caption(CL_CSSLayoutTreeNode *caption_layout);

	void render_layer_background(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources, bool root);
	void render_layer_non_inline(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources);
	void render_layer_floats(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources);
	void render_layer_inline(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources);
	void render_layer_positioned(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources);

	void set_component_geometry();
	int get_first_line_baseline();
	int get_last_line_baseline();

	void calculate_content_top_down_heights();
	CL_CSSLayoutHitTestResult hit_test(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resource_cache, const CL_Point &pos) const;
	CL_CSSInlineLayout *find_inline_layout(CL_CSSBoxText *text_node);

	bool find_content_box(CL_CSSBoxElement *element, CL_Rect &out_rect);

private:
	void prepare_children();
	void layout_content(CL_CSSLayoutGraphics *graphics, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy);
	void position_cells(CL_CSSLayoutCursor &cursor);
	void layout_cells(CL_CSSLayoutGraphics *graphics, CL_CSSLayoutCursor & cursor);
	void calculate_cell_widths(CL_CSSLayoutGraphics *graphics, CL_CSSLayoutCursor & cursor, LayoutStrategy strategy);
	void calculate_preferred_cell_widths(CL_CSSLayoutGraphics *graphics, CL_CSSLayoutCursor & cursor);
	void calculate_minimum_cell_widths(CL_CSSLayoutGraphics *graphics, CL_CSSLayoutCursor & cursor);
	CL_Rect get_cell_border_box(size_t row, size_t col);
	void render_cell_non_content(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources, size_t row, size_t col);

	CL_CSSTableSizeGrid create_preferred_width_grid(CL_CSSLayoutGraphics *graphics, CL_CSSLayoutCursor & cursor);
	CL_CSSTableSizeGrid create_minimum_width_grid(CL_CSSLayoutGraphics *graphics, CL_CSSLayoutCursor & cursor);
	void apply_non_content(CL_CSSTableSizeGrid &size_grid);

	int get_row_span(int col, int row);
	int get_column_span(int col, int row);
	CL_CSSLayoutTreeNode *get_layout(int col, int row);
	const CL_CSSLayoutTreeNode *get_layout(int col, int row) const;

	std::vector<CL_CSSTableColumn> columns;
	std::vector<CL_CSSTableRow> rows;
	std::vector<CL_CSSLayoutTreeNode *> captions;
	size_t next_column;

	CL_CSSTableSizeGrid size_grid;
	CL_CSSActualValue table_x, table_y;
	CL_Rect border_box;
};
