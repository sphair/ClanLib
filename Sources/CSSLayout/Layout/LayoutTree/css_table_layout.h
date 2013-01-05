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

#include "css_layout_tree_node.h"
#include "css_table_size_grid.h"

namespace clan
{

class CSSBoxElement;
class CSSTableColumn;
class CSSTableRow;
class CSSBoxText;
class CSSInlineLayout;

class CSSTableLayout : public CSSLayoutTreeNode
{
public:
	CSSTableLayout(CSSBoxElement *block_element);
	~CSSTableLayout();

	void add_row(CSSBoxElement *row_element);
	void add_cell(CSSLayoutTreeNode *cell_layout, int col_span, int row_span);
	void add_caption(CSSLayoutTreeNode *caption_layout);

	void render_layer_background(CSSLayoutGraphics *graphics, CSSResourceCache *resources, bool root);
	void render_layer_non_inline(CSSLayoutGraphics *graphics, CSSResourceCache *resources);
	void render_layer_floats(CSSLayoutGraphics *graphics, CSSResourceCache *resources);
	void render_layer_inline(CSSLayoutGraphics *graphics, CSSResourceCache *resources);
	void render_layer_positioned(CSSLayoutGraphics *graphics, CSSResourceCache *resources);

	void set_component_geometry();
	int get_first_line_baseline();
	int get_last_line_baseline();

	void calculate_content_top_down_heights();
	CSSLayoutHitTestResult hit_test(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache, const Point &pos) const;
	CSSInlineLayout *find_inline_layout(CSSBoxText *text_node);

	bool find_content_box(CSSBoxElement *element, Rect &out_rect);

private:
	void prepare_children();
	void layout_content(CSSLayoutGraphics *graphics, CSSLayoutCursor &cursor, LayoutStrategy strategy);
	void position_cells(CSSLayoutCursor &cursor);
	void layout_cells(CSSLayoutGraphics *graphics, CSSLayoutCursor & cursor);
	void calculate_cell_widths(CSSLayoutGraphics *graphics, CSSLayoutCursor & cursor, LayoutStrategy strategy);
	void calculate_preferred_cell_widths(CSSLayoutGraphics *graphics, CSSLayoutCursor & cursor);
	void calculate_minimum_cell_widths(CSSLayoutGraphics *graphics, CSSLayoutCursor & cursor);
	Rect get_cell_border_box(size_t row, size_t col);
	void render_cell_non_content(CSSLayoutGraphics *graphics, CSSResourceCache *resources, size_t row, size_t col);

	CSSTableSizeGrid create_preferred_width_grid(CSSLayoutGraphics *graphics, CSSLayoutCursor & cursor);
	CSSTableSizeGrid create_minimum_width_grid(CSSLayoutGraphics *graphics, CSSLayoutCursor & cursor);
	void apply_non_content(CSSTableSizeGrid &size_grid);

	int get_row_span(int col, int row);
	int get_column_span(int col, int row);
	CSSLayoutTreeNode *get_layout(int col, int row);
	const CSSLayoutTreeNode *get_layout(int col, int row) const;

	std::vector<CSSTableColumn> columns;
	std::vector<CSSTableRow> rows;
	std::vector<CSSLayoutTreeNode *> captions;
	size_t next_column;

	CSSTableSizeGrid size_grid;
	CSSActualValue table_x, table_y;
	Rect border_box;
};

}
