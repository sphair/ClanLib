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

#include "CSSLayout/precomp.h"
#include "css_table_layout.h"
#include "css_table_column.h"
#include "css_table_row.h"
#include "css_block_layout.h"
#include "css_layout_cursor.h"
#include "css_block_formatting_context.h"
#include "../css_resource_cache.h"
#include "../BoxTree/css_box_element.h"
#include "clan_image_stretch.h"

CL_CSSTableLayout::CL_CSSTableLayout(CL_CSSBoxElement *block_element)
: CL_CSSLayoutTreeNode(block_element), next_column(0)
{
}

CL_CSSTableLayout::~CL_CSSTableLayout()
{
	for (size_t i = 0; i < columns.size(); i++)
	{
		for (size_t j = 0; j < columns[i].rows.size(); j++)
		{
			delete columns[i].rows[j];
		}
	}
}

void CL_CSSTableLayout::add_row(CL_CSSBoxElement *row_element)
{
	next_column = 0;
	CL_CSSTableRow row;
	row.element = row_element;
	rows.push_back(row);
}

void CL_CSSTableLayout::add_cell(CL_CSSLayoutTreeNode *cell_layout)
{
	// To do: Implement row and cell span
	if (columns.size() < next_column + 1)
		columns.resize(next_column + 1);
	if (columns[next_column].rows.size() + 1 < rows.size())
		columns[next_column].rows.resize(rows.size() - 1);
	columns[next_column].rows.push_back(cell_layout);
	next_column++;
}

void CL_CSSTableLayout::calculate_top_down_sizes()
{
	CL_CSSLayoutTreeNode::calculate_top_down_sizes();
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (columns[cell].rows.size() > row && columns[cell].rows[row])
			{
				columns[cell].rows[row]->used.containing.width = used.width;
				columns[cell].rows[row]->used.containing.height = used.height;
				columns[cell].rows[row]->used.containing.undetermined_width = used.undetermined_width;
				columns[cell].rows[row]->used.containing.undetermined_height = used.undetermined_height;
				columns[cell].rows[row]->calculate_top_down_sizes();
			}
		}
	}
}

/*
void CL_CSSTableLayout::calculate_sizes(CL_GraphicContext &gc, CL_CSSResourceCache *resources, float width, float height)
{
	float table_auto_width = 0.0f;
	if (element_node->computed_properties.width.type == CL_CSSBoxWidth::type_length)
		table_auto_width = element_node->computed_properties.width.length.value;
	else
		table_auto_width = element_node->computed_properties.width.percentage * width / 100.0f;

	if (element_node->computed_properties.table_layout.type == CL_CSSBoxTableLayout::type_fixed)
	{
		float x_spacing = 0.0f;
		if (element_node->computed_properties.border_collapse.type == CL_CSSBoxBorderCollapse::type_separate)
			x_spacing = element_node->computed_properties.border_spacing.length1.value;

		float table_width = x_spacing;
		int auto_columns = 0;
		int column_count = 0;
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (!columns[cell].rows.empty() && columns[cell].rows[0])
			{
				CL_CSSBoxElement *cell_element = columns[cell].rows[0]->get_element_node();
				float cell_width = 0.0f;
				if (cell_element->computed_properties.width.type == CL_CSSBoxWidth::type_auto)
				{
					auto_columns++;
				}
				else if (cell_element->computed_properties.width.type == CL_CSSBoxWidth::type_length)
				{
					cell_width = cell_element->computed_properties.width.length.value;
				}
				else if (cell_element->computed_properties.width.type == CL_CSSBoxWidth::type_percentage)
				{
					cell_width = cell_element->computed_properties.width.percentage * width / 100.0f;
				}
				columns[cell].block_width = (int)(cell_width+0.5f);
				table_width += cell_width;
				column_count++;
			}
			table_width += x_spacing;
		}

		if (table_auto_width > table_width)
		{
			if (auto_columns > 0)
			{
				float cell_width = (table_auto_width - table_width) / auto_columns;
				for (size_t cell = 0; cell < columns.size(); cell++)
				{
					if (!columns[cell].rows.empty() && columns[cell].rows[0])
					{
						CL_CSSBoxElement *cell_element = columns[cell].rows[0]->get_element_node();
						if (cell_element->computed_properties.width.type == CL_CSSBoxWidth::type_auto)
						{
							columns[cell].block_width = (int)(cell_width+0.5f);
						}
					}
				}
			}
			else if (column_count > 0)
			{
				float cell_width = (table_auto_width - table_width) / column_count;
				for (size_t cell = 0; cell < columns.size(); cell++)
					if (!columns[cell].rows.empty() && columns[cell].rows[0])
						columns[cell].block_width += (int)(cell_width+0.5f);
			}
		}
		else if (table_width > table_auto_width)
		{
			table_auto_width = table_width;
		}

		used.calc_table_block_widths(element_node->computed_properties, table_auto_width, width);
		used.calc_table_block_heights(element_node->computed_properties, height);
		used.calc_table_block_offsets(element_node->computed_properties, width, height);

		for (size_t row = 0; row < rows.size(); row++)
		{
			for (size_t cell = 0; cell < columns.size(); cell++)
			{
				if (columns[cell].rows.size() > row)
				{
					if (columns[cell].rows[row])
					{
						columns[cell].rows[row]->used.calc_block_widths(columns[cell].rows[row]->get_element_node()->computed_properties, columns[cell].block_width);
						columns[cell].rows[row]->used.calc_block_heights(columns[cell].rows[row]->get_element_node()->computed_properties, used.height);
						columns[cell].rows[row]->used.calc_block_offsets(columns[cell].rows[row]->get_element_node()->computed_properties, columns[cell].block_width, used.height);
						columns[cell].rows[row]->calculate_children_sizes(gc, resources);
					}
				}
			}
		}
	}
	else
	{
		used.calc_table_block_widths(element_node->computed_properties, table_auto_width, width);
		used.calc_table_block_heights(element_node->computed_properties, height);
		used.calc_table_block_offsets(element_node->computed_properties, width, height);
		for (size_t row = 0; row < rows.size(); row++)
		{
			for (size_t cell = 0; cell < columns.size(); cell++)
			{
				if (columns[cell].rows.size() > row)
				{
					if (columns[cell].rows[row])
					{
						columns[cell].rows[row]->calculate_sizes(gc, resources, width, height);
						columns[cell].block_width = cl_max(columns[cell].block_width, columns[cell].rows[row]->get_block_width());
					}
				}
			}
		}
	}
}
*/
void CL_CSSTableLayout::layout_content(CL_GraphicContext &gc, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy)
{
	cursor.apply_margin();
	CL_CSSUsedValue start_y = cursor.y;
	calculate_cell_widths(gc, cursor);
	layout_cells(gc, cursor);
	position_cells(cursor);
	used.height = cl_max(used.height, cursor.y-start_y);
}

void CL_CSSTableLayout::render(CL_GraphicContext &gc, CL_CSSResourceCache *resources)
{
	render_non_content(gc, resources);
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (columns[cell].rows.size() > row && columns[cell].rows[row])
			{
				render_cell_non_content(gc, resources, row, cell);
				columns[cell].rows[row]->render(gc, resources);
			}
		}
	}
}

CL_Rect CL_CSSTableLayout::get_cell_border_box(size_t row, size_t col)
{
	CL_CSSUsedValue x = 0.0f;
	CL_CSSUsedValue y = 0.0f;
	for (size_t i = 0; i < col; i++)
		x += columns[i].cell_width;
	for (size_t i = 0; i < row; i++)
		y += rows[i].height;
	CL_Rect box((int)(x+0.5f),(int)(y+0.5f),(int)(x+columns[col].cell_width+0.5f),(int)(y+rows[row].height+0.5f));
	box.translate(formatting_context->get_x(), formatting_context->get_y());
	return box;
}

void CL_CSSTableLayout::render_cell_non_content(CL_GraphicContext &gc, CL_CSSResourceCache *resources, size_t row, size_t col)
{
	CL_CSSLayoutTreeNode *cell = columns[col].rows[row];

	CL_Rect border_box = get_cell_border_box(row, col);
	if (cell->get_element_node()->computed_properties.background_color.type == CL_CSSBoxBackgroundColor::type_color)
		CL_Draw::fill(gc, border_box, cell->get_element_node()->computed_properties.background_color.color);

	if (cell->get_element_node()->computed_properties.background_image.type == CL_CSSBoxBackgroundImage::type_uri)
	{
		CL_Image &image = resources->get_image(gc, cell->get_element_node()->computed_properties.background_image.url);
		if (!image.is_null())
		{
			if (cell->get_element_node()->computed_properties.background_repeat.type == CL_CSSBoxBackgroundRepeat::type_repeat)
			{
				image.draw(gc, border_box);
			}
			else if (cell->get_element_node()->computed_properties.background_repeat.type == CL_CSSBoxBackgroundRepeat::type_clan_stretch)
			{
				int sizing_left = (int)(cell->get_element_node()->computed_properties.clan_background_border_left.length.value+0.5f);
				int sizing_top = (int)(cell->get_element_node()->computed_properties.clan_background_border_top.length.value+0.5f);
				int sizing_right = (int)(cell->get_element_node()->computed_properties.clan_background_border_right.length.value+0.5f);
				int sizing_bottom = (int)(cell->get_element_node()->computed_properties.clan_background_border_bottom.length.value+0.5f);
				CL_ClanImageStretch::draw_image(gc, border_box, image, sizing_left, sizing_top, sizing_right, sizing_bottom);
			}
		}
	}
}

void CL_CSSTableLayout::set_component_geometry()
{
	for (size_t row = 0; row < rows.size(); row++)
		for (size_t cell = 0; cell < columns.size(); cell++)
			if (columns[cell].rows.size() > row && columns[cell].rows[row])
				columns[cell].rows[row]->set_component_geometry();
}

int CL_CSSTableLayout::get_first_line_baseline()
{
	int first_line_baseline = -1;
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (columns[cell].rows.size() > row && columns[cell].rows[row])
			{
				int baseline = columns[cell].rows[row]->get_first_line_baseline();
				if (first_line_baseline == -1)
					first_line_baseline = baseline;
				else if (baseline != -1)
					first_line_baseline = cl_min(first_line_baseline, baseline);
			}
		}
	}
	if (first_line_baseline != -1 && formatting_context_root)
		first_line_baseline += formatting_context->get_local_y();
	return first_line_baseline;
}

int CL_CSSTableLayout::get_last_line_baseline()
{
	int last_line_baseline = -1;
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (columns[cell].rows.size() > row && columns[cell].rows[row])
			{
				int baseline = columns[cell].rows[row]->get_last_line_baseline();
				if (last_line_baseline == -1)
					last_line_baseline = baseline;
				else if (baseline != -1)
					last_line_baseline = cl_max(last_line_baseline, baseline);
			}
		}
	}
	if (last_line_baseline != -1 && formatting_context_root)
		last_line_baseline += formatting_context->get_local_y();
	return last_line_baseline;
}

void CL_CSSTableLayout::prepare_children()
{
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (columns[cell].rows.size() > row && columns[cell].rows[row])
			{
				columns[cell].rows[row]->prepare(formatting_context, stacking_context);
			}
		}
	}
}

void CL_CSSTableLayout::calculate_minimum_cell_widths(CL_GraphicContext & gc, CL_CSSLayoutCursor & cursor)
{
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (row == 0)
				columns[cell].minimum_width = 0;

			if (columns[cell].rows.size() > row)
			{
				if (columns[cell].rows[row])
				{
					columns[cell].rows[row]->calc_minimum(gc, cursor);
					CL_CSSUsedValue cell_width = columns[cell].rows[row]->used.min_width;
					columns[cell].minimum_width = cl_max(columns[cell].minimum_width, cell_width);
				}
			}
		}
	}
}

void CL_CSSTableLayout::calculate_preferred_cell_widths(CL_GraphicContext & gc, CL_CSSLayoutCursor & cursor)
{
	for (size_t row = 0; row < rows.size(); row++)
	{
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (row == 0)
				columns[cell].maximum_width = 0;

			if (columns[cell].rows.size() > row)
			{
				if (columns[cell].rows[row])
				{
					CL_CSSUsedValue cell_width = 0;
					if (columns[cell].rows[row]->get_element_node()->computed_properties.width.type == CL_CSSBoxWidth::type_auto)
					{
						columns[cell].rows[row]->calc_preferred(gc, cursor);
						cell_width = columns[cell].rows[row]->used.preferred_width;
					}
					else
					{
						cell_width = columns[cell].rows[row]->used.width;
					}

					columns[cell].maximum_width = cl_max(columns[cell].maximum_width, cell_width);
				}
			}
		}
	}
}

void CL_CSSTableLayout::calculate_cell_widths(CL_GraphicContext & gc, CL_CSSLayoutCursor & cursor)
{
	calculate_minimum_cell_widths(gc, cursor);
	calculate_preferred_cell_widths(gc, cursor);

	// Calculate cell widths:
	CL_CSSUsedValue content_width = 0;
	for (size_t cell = 0; cell < columns.size(); cell++)
		content_width += columns[cell].maximum_width;

	CL_CSSUsedValue available_width = used.width;
	if (used.undetermined_width)
		available_width = content_width;

	int columns_minimum = 0;
	for (size_t cell = 0; cell < columns.size(); cell++)
	{
		columns[cell].cell_width = columns[cell].maximum_width;
		if (columns[cell].cell_width < columns[cell].minimum_width+0.1f)
			columns_minimum++;
	}

	while (columns_minimum < columns.size())
	{
		content_width = 0;
		for (size_t cell = 0; cell < columns.size(); cell++)
			content_width += columns[cell].cell_width;
		if (content_width <= available_width)
			break;

		CL_CSSUsedValue extra = 0.0f;
		for (size_t cell = 0; cell < columns.size(); cell++)
			if (columns[cell].cell_width != columns[cell].minimum_width)
				extra += columns[cell].cell_width;
		if (extra == 0.0f)
			break;

		extra = (extra - content_width + available_width) / extra;
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (columns[cell].cell_width != columns[cell].minimum_width)
			{
				columns[cell].cell_width = cl_max(columns[cell].minimum_width, columns[cell].cell_width * extra);
				if (columns[cell].cell_width < columns[cell].minimum_width+0.1f)
					columns_minimum++;
			}
		}
	}
}

void CL_CSSTableLayout::layout_cells(CL_GraphicContext & gc, CL_CSSLayoutCursor & cursor)
{
	for (size_t row = 0; row < rows.size(); row++)
	{
		switch (rows[row].element->computed_properties.height.type)
		{
		default:
		case CL_CSSBoxHeight::type_auto:
			rows[row].height = 0.0f;
		case CL_CSSBoxHeight::type_length:
			rows[row].height = rows[row].element->computed_properties.height.length.value;
			break;
		case CL_CSSBoxHeight::type_percentage:
			rows[row].height = used.undetermined_height ? 0.0f : used.height * rows[row].element->computed_properties.height.percentage / 100.0f;
			break;
		}

		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (columns[cell].rows.size() > row)
			{
				if (columns[cell].rows[row])
				{
					columns[cell].rows[row]->calculate_top_down_sizes();
					columns[cell].rows[row]->set_auto_width(columns[cell].cell_width);
					columns[cell].rows[row]->layout_formatting_root_helper(gc, cursor, normal_strategy);
					rows[row].height = cl_max(rows[row].height, columns[cell].rows[row]->used.height);
				}
			}
		}
	}
}

void CL_CSSTableLayout::position_cells(CL_CSSLayoutCursor &cursor)
{
	CL_CSSUsedValue y = 0;
	for (size_t row = 0; row < rows.size(); row++)
	{
		CL_CSSUsedValue x = 0;
		for (size_t cell = 0; cell < columns.size(); cell++)
		{
			if (columns[cell].rows.size() > row)
			{
				if (columns[cell].rows[row])
				{
					if (columns[cell].rows[row]->get_element_node()->computed_properties.vertical_align.type == CL_CSSBoxVerticalAlign::type_middle)
					{
						CL_CSSUsedValue offset = rows[row].height/2.0f - columns[cell].rows[row]->used.height/2.0f;
						columns[cell].rows[row]->set_root_block_position(cursor.x + x, cursor.y + y + offset);
					}
					else
					{
						columns[cell].rows[row]->set_root_block_position(cursor.x + x, cursor.y + y);
					}
				}
			}
			x += columns[cell].cell_width;
		}
		cursor.apply_written_width(cursor.x + x);
		y += rows[row].height;
	}
	cursor.y += y;
}
