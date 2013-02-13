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
#include "css_table_layout.h"
#include "css_table_column.h"
#include "css_table_row.h"
#include "InlineLayout/css_inline_layout.h"
#include "css_layout_cursor.h"
#include "css_block_formatting_context.h"
#include "css_stacking_context.h"
#include "css_background_renderer.h"
#include "css_border_renderer.h"
#include "../../css_resource_cache.h"
#include "../BoxTree/css_box_element.h"
#include "../BoxTree/css_box_text.h"
#include "clan_image_stretch.h"
#include "css_layout_graphics.h"

namespace clan
{

CSSTableLayout::CSSTableLayout(CSSBoxElement *block_element)
: CSSLayoutTreeNode(block_element), next_column(0)
{
}

CSSTableLayout::~CSSTableLayout()
{
	for (size_t i = 0; i < columns.size(); i++)
	{
		for (size_t j = 0; j < columns[i].rows.size(); j++)
		{
			delete columns[i].rows[j].layout;
		}
	}

	for (size_t i = 0; i < captions.size(); i++)
	{
		delete captions[i];
	}
}

void CSSTableLayout::add_row(CSSBoxElement *row_element)
{
	next_column = 0;
	CSSTableRow row;
	row.element = row_element;
	rows.push_back(row);
}

void CSSTableLayout::add_cell(CSSLayoutTreeNode *cell_layout, int col_span, int row_span)
{
	// To do: Implement row and cell span

	if (columns.size() < next_column + 1)
		columns.resize(next_column + 1);

	if (columns[next_column].rows.size() + 1 < rows.size())
		columns[next_column].rows.resize(rows.size() - 1);

	CSSTableCell cell;
	cell.layout = cell_layout;
	cell.col_span = col_span;
	cell.row_span = row_span;
	columns[next_column].rows.push_back(cell);
	next_column++;
}

void CSSTableLayout::add_caption(CSSLayoutTreeNode *caption_layout)
{
	captions.push_back(caption_layout);
}

int CSSTableLayout::get_row_span(int col, int row)
{
	if (columns[col].rows.size() > row)
		return columns[col].rows[row].row_span;
	else
		return 0;
}

int CSSTableLayout::get_column_span(int col, int row)
{
	if (columns[col].rows.size() > row)
		return columns[col].rows[row].col_span;
	else
		return 0;
}

CSSLayoutTreeNode *CSSTableLayout::get_layout(int col, int row)
{
	if (columns[col].rows.size() > row)
		return columns[col].rows[row].layout;
	else
		return 0;
}

const CSSLayoutTreeNode *CSSTableLayout::get_layout(int col, int row) const
{
	if (columns[col].rows.size() > row)
		return columns[col].rows[row].layout;
	else
		return 0;
}

void CSSTableLayout::calculate_content_top_down_heights()
{
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (get_layout(cell, row))
			{
				get_layout(cell, row)->containing_height = height;
				get_layout(cell, row)->calculate_content_top_down_heights();
			}
		}
	}
}

void CSSTableLayout::layout_content(CSSLayoutGraphics *graphics, CSSLayoutCursor &cursor, LayoutStrategy strategy)
{
	CSSUsedValue start_y = cursor.y;

	for (size_t i = 0; i < captions.size(); i++)
	{
		if (captions[i]->get_element_node()->computed_values.get_table_inherit().caption_side.type == CSSValueCaptionSide::type_top)
		{
			captions[i]->containing_height = height;
			captions[i]->calculate_content_top_down_heights();
			captions[i]->containing_width = width;
			captions[i]->layout_normal(graphics, cursor, strategy);
		}
	}
	cursor.apply_margin();

	calculate_cell_widths(graphics, cursor, strategy);
	layout_cells(graphics, cursor);
	position_cells(cursor);

	for (size_t i = 0; i < captions.size(); i++)
	{
		if (captions[i]->get_element_node()->computed_values.get_table_inherit().caption_side.type == CSSValueCaptionSide::type_bottom)
		{
			captions[i]->containing_height = height;
			captions[i]->calculate_content_top_down_heights();
			captions[i]->containing_width = width;
			captions[i]->layout_normal(graphics, cursor, strategy);
		}
	}

	height.value = max(height.value, cursor.y-start_y);
}

Rect CSSTableLayout::get_cell_border_box(size_t row, size_t col)
{
	CSSUsedValue left = 0.0f;
	CSSUsedValue top = 0.0f;
	CSSUsedValue right = 0.0f;
	CSSUsedValue bottom = 0.0f;

	for (size_t i = 0; i < col; i++)
		left += size_grid.get_non_content_width(i) + size_grid.get_width(i);
	left += size_grid.get_non_content_width(col);

	for (size_t i = 0; i < row; i++)
		top += size_grid.get_non_content_height(i) + size_grid.get_height(i);
	top += size_grid.get_non_content_height(row);

	right = left + size_grid.get_width(col);
	bottom = top + size_grid.get_height(row);

	if (element_node->computed_values.get_table_inherit().border_collapse.type == CSSValueBorderCollapse::type_collapse)
	{
		left -= get_layout(col, row)->border.left;
		right += get_layout(col, row)->border.right;
		top -= get_layout(col, row)->border.top;
		bottom += get_layout(col, row)->border.bottom;
	}

	Rect box(used_to_actual(left),used_to_actual(top),used_to_actual(right),used_to_actual(bottom));
	box.translate(used_to_actual(relative_x) + formatting_context->get_x(), used_to_actual(relative_y) + formatting_context->get_y());
	return box;
}

void CSSTableLayout::render_cell_non_content(CSSLayoutGraphics *graphics, CSSResourceCache *resources, size_t row, size_t col)
{
	CSSLayoutTreeNode *cell = get_layout(col, row);

	Rect border_box = get_cell_border_box(row, col);

	CSSBackgroundRenderer background(graphics, resources, cell->get_element_node()->computed_values.get_background());
	background.set_initial_containing_box(Rect(0, 0, used_to_actual(containing_width.value), used_to_actual(containing_height.value))); // Bug: this is wrong
	background.set_content_box(border_box); // Bug: this is wrong
	background.set_padding_box(border_box); // Bug: this is wrong
	background.set_border_box(border_box);
	background.render();

	if (cell->get_element_node()->computed_values.get_border().border_style_top.type == CSSValueBorderStyle::type_solid)
		graphics->fill(Rect(border_box.left, border_box.top, border_box.right, border_box.top+cell->border.top), cell->get_element_node()->computed_values.get_border().border_color_top.color);

	if (cell->get_element_node()->computed_values.get_border().border_style_bottom.type == CSSValueBorderStyle::type_solid)
		graphics->fill(Rect(border_box.left, (border_box.bottom-cell->border.bottom), border_box.right, border_box.bottom), cell->get_element_node()->computed_values.get_border().border_color_bottom.color);

	if (cell->get_element_node()->computed_values.get_border().border_style_left.type == CSSValueBorderStyle::type_solid)
		graphics->fill(Rect(border_box.left, (border_box.top+cell->border.top), (border_box.left+cell->border.left), (border_box.bottom-cell->border.bottom)), cell->get_element_node()->computed_values.get_border().border_color_left.color);

	if (cell->get_element_node()->computed_values.get_border().border_style_right.type == CSSValueBorderStyle::type_solid)
		graphics->fill(Rect((border_box.right-cell->border.right), (border_box.top+cell->border.top), border_box.right, (border_box.bottom-cell->border.bottom)), cell->get_element_node()->computed_values.get_border().border_color_right.color);
}

void CSSTableLayout::set_component_geometry()
{
	for (size_t row = 0; row < rows.size(); row++)
		for (size_t cell = 0; cell < columns.size(); cell++)
			if (get_layout(cell, row))
				get_layout(cell, row)->set_component_geometry();
}

int CSSTableLayout::get_first_line_baseline()
{
	int first_line_baseline = -1;
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (get_layout(cell, row))
			{
				int baseline = get_layout(cell, row)->get_first_line_baseline();
				if (baseline != -1)
					baseline += get_layout(cell, row)->get_formatting_context()->get_local_y();

				if (first_line_baseline == -1)
					first_line_baseline = baseline;
				else if (baseline != -1)
					first_line_baseline = min(first_line_baseline, baseline);
			}
		}
	}
	return first_line_baseline;
}

int CSSTableLayout::get_last_line_baseline()
{
	int last_line_baseline = -1;
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (get_layout(cell, row))
			{
				int baseline = get_layout(cell, row)->get_last_line_baseline();
				if (baseline != -1)
					baseline += get_layout(cell, row)->get_formatting_context()->get_local_y();

				if (last_line_baseline == -1)
					last_line_baseline = baseline;
				else if (baseline != -1)
					last_line_baseline = max(last_line_baseline, baseline);
			}
		}
	}
	return last_line_baseline;
}

CSSLayoutHitTestResult CSSTableLayout::hit_test(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache, const Point &pos) const
{
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			Rect r = get_layout(cell, row)->content_box;
			r.translate(formatting_context->get_x(), formatting_context->get_y());
			if (r.contains(pos))
			{
				CSSLayoutHitTestResult result = get_layout(cell, row)->hit_test(graphics, resource_cache, pos);
				if (result.node)
					return result;
			}
		}
	}
	return CSSLayoutHitTestResult();
}
/*
CSSInlineLayout *CSSTableLayout::find_inline_layout(CSSBoxText *text_node)
{
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			CSSBlockLayout *block_child = dynamic_cast<CSSBlockLayout*>(columns[cell].rows[row]);
			CSSInlineLayout *inline_child = dynamic_cast<CSSInlineLayout*>(columns[cell].rows[row]);
			CSSTableLayout *table_child = dynamic_cast<CSSTableLayout*>(columns[cell].rows[row]);
			if (block_child)
			{
				CSSInlineLayout *inline_layout = block_child->find_inline_layout(text_node);
				if (inline_layout)
					return inline_layout;
			}
			else if (inline_child)
			{
				if (inline_child->contains_node(text_node))
					return inline_child;
			}
			else if (table_child)
			{
				CSSInlineLayout *inline_layout = table_child->find_inline_layout(text_node);
				if (inline_layout)
					return inline_layout;
			}
		}
	}
	return 0;
}
*/
void CSSTableLayout::prepare_children()
{
	for (size_t i = 0; i < captions.size(); i++)
	{
		captions[i]->prepare(formatting_context, stacking_context);
	}

	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (get_layout(cell, row))
			{
				get_layout(cell, row)->prepare(formatting_context, stacking_context);
			}
		}
	}
}

void CSSTableLayout::calculate_minimum_cell_widths(CSSLayoutGraphics *graphics, CSSLayoutCursor & cursor)
{
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (row == 0)
				columns[cell].minimum_width = 0;

			if (get_layout(cell, row))
			{
				get_layout(cell, row)->calc_minimum(graphics, cursor.resources);
				CSSUsedValue cell_width = get_layout(cell, row)->min_width;
				cell_width += get_layout(cell, row)->padding.left + get_layout(cell, row)->padding.right;
				if (cell == 0)
					cell_width += padding.left;
				else if (cell + 1 == columns.size())
					cell_width += padding.right;

				if (element_node->computed_values.get_table_inherit().border_collapse.type == CSSValueBorderCollapse::type_separate)
					cell_width += get_layout(cell, row)->border.left + get_layout(cell, row)->border.right;

				columns[cell].rows[row].minimum_width = cell_width;
				if (get_column_span(cell, row) == 1)
					columns[cell].minimum_width = max(columns[cell].minimum_width, cell_width);
			}
		}
	}
}

void CSSTableLayout::calculate_preferred_cell_widths(CSSLayoutGraphics *graphics, CSSLayoutCursor & cursor)
{
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (row == 0)
				columns[cell].maximum_width = 0;

			if (get_layout(cell, row))
			{
				get_layout(cell, row)->calc_preferred(graphics, cursor.resources);
				CSSUsedValue cell_width = get_layout(cell, row)->preferred_width;
				cell_width += get_layout(cell, row)->padding.left + get_layout(cell, row)->padding.right;
				if (cell == 0)
					cell_width += padding.left;
				else if (cell + 1 == columns.size())
					cell_width += padding.right;

				if (element_node->computed_values.get_table_inherit().border_collapse.type == CSSValueBorderCollapse::type_separate)
					cell_width += get_layout(cell, row)->border.left + get_layout(cell, row)->border.right;

				columns[cell].rows[row].maximum_width = cell_width;
				if (get_column_span(cell, row) == 1)
					columns[cell].maximum_width = max(columns[cell].maximum_width, cell_width);
			}
		}
	}
}

CSSTableSizeGrid CSSTableLayout::create_preferred_width_grid(CSSLayoutGraphics *graphics, CSSLayoutCursor & cursor)
{
	calculate_preferred_cell_widths(graphics, cursor);
	CSSTableSizeGrid grid(rows.size(), columns.size());
	apply_non_content(grid);

	for (size_t col = 0; col < columns.size(); col++)
	{
		grid.apply_width(col, columns[col].maximum_width);
	}

	for (size_t row = 0; row < columns.size(); row++)
	{
		for (size_t col = 0; col < columns.size(); col++)
		{
			if (get_column_span(col, row) > 1)
				grid.apply_width_span(col, columns[row].rows[col].maximum_width, get_column_span(col, row));
		}
	}

	return grid;
}

CSSTableSizeGrid CSSTableLayout::create_minimum_width_grid(CSSLayoutGraphics *graphics, CSSLayoutCursor & cursor)
{
	calculate_minimum_cell_widths(graphics, cursor);
	CSSTableSizeGrid grid(rows.size(), columns.size());
	apply_non_content(grid);

	for (size_t col = 0; col < columns.size(); col++)
	{
		grid.apply_width(col, columns[col].minimum_width);
	}

	for (size_t row = 0; row < columns.size(); row++)
	{
		for (size_t col = 0; col < columns.size(); col++)
		{
			if (get_column_span(col, row) > 1)
				grid.apply_width_span(col, columns[row].rows[col].minimum_width, get_column_span(col, row));
		}
	}

	return grid;
}

void CSSTableLayout::apply_non_content(CSSTableSizeGrid &grid)
{
	if (element_node->computed_values.get_table_inherit().border_collapse.type == CSSValueBorderCollapse::type_separate)
	{
		grid.apply_separate_table_border(border.left, border.top, border.right, border.bottom);
		grid.apply_separate_table_padding(padding.left, padding.top, padding.right, padding.bottom);
		grid.apply_separate_spacing(element_node->computed_values.get_table_inherit().border_spacing.length1.value, element_node->computed_values.get_table_inherit().border_spacing.length2.value);
	}
	else
	{
		grid.apply_collapsed_table_border(border.left, border.top, border.right, border.bottom);

		for (size_t col = 0; col < columns.size(); col++)
		{
			for (size_t row = 0; row < rows.size(); row++)
			{
				CSSLayoutTreeNode *cell_node = get_layout(col, row);
				if (cell_node)
					grid.apply_collapsed_border(row, col, cell_node->border.left, cell_node->border.top, cell_node->border.right, cell_node->border.bottom);
			}
		}
	}
}

void CSSTableLayout::calculate_cell_widths(CSSLayoutGraphics *graphics, CSSLayoutCursor & cursor, LayoutStrategy strategy)
{
	if (strategy == minimum_strategy)
		size_grid = create_minimum_width_grid(graphics, cursor);
	else
		size_grid = create_preferred_width_grid(graphics, cursor);

	if (element_node->computed_values.get_box().width.type == CSSValueWidth::type_auto)
	{
		if (!containing_width.expanding && size_grid.get_table_width() > containing_width.value)
		{
			if (strategy != minimum_strategy)
				size_grid = create_minimum_width_grid(graphics, cursor);
			if (size_grid.get_table_width() < containing_width.value)
				size_grid.expand_table_width(containing_width.value);
		}
	}
	else
	{
		if (size_grid.get_table_width() < width.value)
		{
			size_grid.expand_table_width(width.value);
		}
		else
		{
			if (strategy != minimum_strategy)
			{
				size_grid = create_minimum_width_grid(graphics, cursor);
				if (size_grid.get_table_width() < width.value)
					size_grid.expand_table_width(width.value);
			}
		}
	}

	for (size_t col = 0; col < columns.size(); col++)
	{
		columns[col].cell_width = size_grid.get_width(col);
	}

	width.value = size_grid.get_table_width();
}

void CSSTableLayout::layout_cells(CSSLayoutGraphics *graphics, CSSLayoutCursor & cursor)
{
	CSSUsedValue height_used = 0;
	for (size_t row = 0; row < rows.size(); row++)
	{
		switch (rows[row].element->computed_values.get_box().height.type)
		{
		default:
		case CSSValueHeight::type_auto:
			rows[row].height = 0.0f;
			break;
		case CSSValueHeight::type_length:
			rows[row].height = rows[row].element->computed_values.get_box().height.length.value;
			break;
		case CSSValueHeight::type_percentage:
			rows[row].height = height.use_content ? 0.0f : (height.value * rows[row].element->computed_values.get_box().height.percentage / 100.0f);
			break;
		}

		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (get_layout(cell, row))
			{
				CSSUsedValue cell_content_width = 0.0f;
				for (int span = 0; span < get_column_span(cell, row); span++)
					cell_content_width += columns[cell+span].cell_width;
				cell_content_width -= get_layout(cell, row)->padding.left + get_layout(cell, row)->padding.right;
				if (element_node->computed_values.get_table_inherit().border_collapse.type == CSSValueBorderCollapse::type_separate)
					cell_content_width -= get_layout(cell, row)->border.left + get_layout(cell, row)->border.right;
				get_layout(cell, row)->containing_width = width;
				get_layout(cell, row)->calculate_top_down_widths(normal_strategy);
				get_layout(cell, row)->calculate_top_down_heights();
				get_layout(cell, row)->width.value = cell_content_width;
				get_layout(cell, row)->relative_x = relative_x + get_layout(cell, row)->get_local_relative_x();
				get_layout(cell, row)->relative_y = relative_y + get_layout(cell, row)->get_local_relative_y();
				get_layout(cell, row)->layout_formatting_root_helper(graphics, cursor.resources, normal_strategy);
				CSSUsedValue cell_height = get_layout(cell, row)->height.value + get_layout(cell, row)->padding.top + get_layout(cell, row)->padding.bottom;
				if (row == 0)
					cell_height += padding.top;
				else if (row + 1 == rows.size())
					cell_height += padding.bottom;
				if (element_node->computed_values.get_table_inherit().border_collapse.type == CSSValueBorderCollapse::type_separate)
					cell_height += get_layout(cell, row)->border.top + get_layout(cell, row)->border.bottom;
				columns[cell].rows[row].height = cell_height;
				rows[row].height = max(rows[row].height, cell_height);
			}
		}
		height_used += rows[row].height;
	}

	for (size_t row = 0; row < rows.size(); row++)
	{
		size_grid.apply_height(row, rows[row].height);
	}

	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t col = 0; col < columns.size(); col++)
		{
			if (get_row_span(col, row) > 1)
				size_grid.apply_height_span(row, columns[col].rows[row].height, get_row_span(col, row));
		}
	}
}

void CSSTableLayout::position_cells(CSSLayoutCursor &cursor)
{
	table_x = cursor.x;
	table_y = cursor.y + cursor.get_total_margin();

	CSSUsedValue y = cursor.y + cursor.get_total_margin();
	for (size_t row = 0; row < rows.size(); row++)
	{
		y += size_grid.get_non_content_height(row);

		CSSUsedValue x = cursor.x;
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			x += size_grid.get_non_content_width(cell);

			if (get_layout(cell, row))
			{
				CSSUsedValue cell_padding_top = get_layout(cell, row)->padding.top;
				CSSUsedValue cell_padding_bottom = get_layout(cell, row)->padding.top;
				CSSUsedValue cell_padding_left = get_layout(cell, row)->padding.left;
				// CSSUsedValue cell_padding_right = get_layout(cell, row)->padding.right;
				if (row == 0)
					cell_padding_top += padding.top;
				if (row + 1 == rows.size())
					cell_padding_bottom += padding.bottom;

				CSSUsedValue offset = 0.0f;
				if (get_layout(cell, row)->get_element_node()->computed_values.get_text_reset().vertical_align.type == CSSValueVerticalAlign::type_middle)
				{
					CSSUsedValue center_height = get_layout(cell, row)->height.value + cell_padding_top + cell_padding_bottom;
					offset = cell_padding_top + (size_grid.get_height(row) - center_height)/2.0f;
				}
				else
				{
					offset = cell_padding_top;
				}

				int xx = used_to_actual(x + cell_padding_left);
				int yy = used_to_actual(y + offset);
				get_layout(cell, row)->set_root_content_position(xx, yy);
			}

			x += size_grid.get_width(cell);
		}

		y += size_grid.get_height(row);
	}
	if (!rows.empty())
		y += size_grid.get_non_content_height(rows.size());
	cursor.y += y;
}

void CSSTableLayout::render_layer_background(CSSLayoutGraphics *graphics, CSSResourceCache *resources, bool root)
{
	Rect border_box(table_x, table_y, table_x + size_grid.get_table_width(), table_y + size_grid.get_table_height());
	border_box.translate(formatting_context->get_x(), formatting_context->get_y());
	border_box.translate(used_to_actual(relative_x), used_to_actual(relative_y));

	Rect padding_box = border_box;
	padding_box.shrink(used_to_actual(border.left), used_to_actual(border.top), used_to_actual(border.right), used_to_actual(border.bottom));

	Rect content_box = padding_box;
	content_box.shrink(used_to_actual(padding.left), used_to_actual(padding.top), used_to_actual(padding.right), used_to_actual(padding.bottom));

	CSSBackgroundRenderer background(graphics, resources, element_node->computed_values.get_background());
	background.set_initial_containing_box(Rect(0, 0, used_to_actual(containing_width.value), used_to_actual(containing_height.value))); // Bug: this is wrong
	background.set_content_box(content_box);
	background.set_padding_box(padding_box);
	background.set_border_box(border_box);
	background.render();

	CSSBorderRenderer border_renderer(graphics, resources, element_node->computed_values.get_border());
	border_renderer.set_border_values(border.left, border.top, border.right, border.bottom);
	border_renderer.set_border_box(border_box);
	border_renderer.render();

	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (get_layout(cell, row))
			{
				render_cell_non_content(graphics, resources, row, cell);
			}
		}
	}
}

void CSSTableLayout::render_layer_non_inline(CSSLayoutGraphics *graphics, CSSResourceCache *resources)
{
	for (size_t i = 0; i < captions.size(); i++)
	{
		bool is_same_stacking_context = (stacking_context == captions[i]->get_stacking_context());
		bool is_positioned = (captions[i]->get_element_node()->computed_values.get_box().position.type != CSSValuePosition::type_static);
		if (is_same_stacking_context && !is_positioned)
		{
			captions[i]->render_layer_background(graphics, resources, false);
			captions[i]->render_layer_non_inline(graphics, resources);
		}
	}

	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (get_layout(cell, row))
			{
				CSSLayoutTreeNode *cell_node = get_layout(cell, row);
				bool is_same_stacking_context = (stacking_context == cell_node->get_stacking_context());
				bool is_positioned = (cell_node->get_element_node()->computed_values.get_box().position.type != CSSValuePosition::type_static);
				bool is_float = cell_node->get_element_node()->is_float();
				if (is_same_stacking_context && !is_positioned && !is_float)
					cell_node->render_layer_non_inline(graphics, resources);
			}
		}
	}
}

void CSSTableLayout::render_layer_floats(CSSLayoutGraphics *graphics, CSSResourceCache *resources)
{
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (get_layout(cell, row))
			{
				CSSLayoutTreeNode *cell_node = get_layout(cell, row);
				bool is_same_stacking_context = (stacking_context == cell_node->get_stacking_context());
				bool is_positioned = (cell_node->get_element_node()->computed_values.get_box().position.type != CSSValuePosition::type_static);
				bool is_float = cell_node->get_element_node()->is_float();
				if (is_same_stacking_context && !is_positioned)
				{
					if (is_float)
					{
						cell_node->render_layer_background(graphics, resources, false);
						cell_node->render_layer_non_inline(graphics, resources);
						cell_node->render_layer_floats(graphics, resources);
						cell_node->render_layer_inline(graphics, resources);
					}
					else
					{
						cell_node->render_layer_floats(graphics, resources);
					}
				}
			}
		}
	}
}

void CSSTableLayout::render_layer_inline(CSSLayoutGraphics *graphics, CSSResourceCache *resources)
{
	for (size_t i = 0; i < captions.size(); i++)
	{
		bool is_same_stacking_context = (stacking_context == captions[i]->get_stacking_context());
		bool is_positioned = (captions[i]->get_element_node()->computed_values.get_box().position.type != CSSValuePosition::type_static);
		if (is_same_stacking_context && !is_positioned)
		{
			captions[i]->render_layer_inline(graphics, resources);
		}
	}

	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (get_layout(cell, row))
			{
				CSSLayoutTreeNode *cell_node = get_layout(cell, row);
				bool is_same_stacking_context = (stacking_context == cell_node->get_stacking_context());
				bool is_positioned = (cell_node->get_element_node()->computed_values.get_box().position.type != CSSValuePosition::type_static);
				bool is_float = cell_node->get_element_node()->is_float();
				if (is_same_stacking_context && !is_positioned && !is_float)
					cell_node->render_layer_inline(graphics, resources);
			}
		}
	}
}

void CSSTableLayout::render_layer_positioned(CSSLayoutGraphics *graphics, CSSResourceCache *resources)
{
	for (size_t i = 0; i < captions.size(); i++)
	{
		bool is_same_stacking_context = (stacking_context == captions[i]->get_stacking_context());
		bool is_positioned = (captions[i]->get_element_node()->computed_values.get_box().position.type != CSSValuePosition::type_static);
		if (is_same_stacking_context)
		{
			if (is_positioned)
			{
				captions[i]->render_layer_background(graphics, resources, false);
				captions[i]->render_layer_non_inline(graphics, resources);
				captions[i]->render_layer_floats(graphics, resources);
				captions[i]->render_layer_inline(graphics, resources);
			}
			captions[i]->render_layer_positioned(graphics, resources);
		}
	}

	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (get_layout(cell, row))
			{
				CSSLayoutTreeNode *cell_node = get_layout(cell, row);
				bool is_same_stacking_context = (stacking_context == cell_node->get_stacking_context());
				bool is_positioned = (cell_node->get_element_node()->computed_values.get_box().position.type != CSSValuePosition::type_static);
				int level = cell_node->get_stacking_context()->get_level();
				if (is_same_stacking_context)
				{
					if (is_positioned)
					{
						cell_node->render_layer_background(graphics, resources, false);
						cell_node->render_layer_non_inline(graphics, resources);
						cell_node->render_layer_floats(graphics, resources);
						cell_node->render_layer_inline(graphics, resources);
					}
					cell_node->render_layer_positioned(graphics, resources);
				}
				else if (!is_same_stacking_context && level == 0)
				{
					cell_node->get_stacking_context()->render(graphics, resources);
				}
			}
		}
	}
}

bool CSSTableLayout::find_content_box(CSSBoxElement *search_element, Rect &out_rect)
{
	if (get_element_node() == search_element)
	{
		Rect box = content_box;
		box.translate(used_to_actual(relative_x), used_to_actual(relative_y));
		if (!formatting_context_root)
			content_box.translate(formatting_context->get_x(), formatting_context->get_y());
		else if (formatting_context->get_parent())
			content_box.translate(formatting_context->get_parent()->get_x(), formatting_context->get_parent()->get_y());
		out_rect = box;
		return true;
	}

	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (get_layout(cell, row))
			{
				CSSLayoutTreeNode *cell_node = get_layout(cell, row);
				if (cell_node->find_content_box(search_element, out_rect))
					return true;
			}
		}
	}

	return false;
}

}
