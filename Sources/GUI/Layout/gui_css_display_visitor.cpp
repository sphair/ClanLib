
#include "GUI/precomp.h"
#include "gui_css_display_visitor.h"

namespace clan
{

void GUICSSDisplayVisitor::node(GUIComponent_Impl *node)
{
	switch (node->css_properties.display.type)
	{
	case CSSBoxDisplay::type_flex:
		flex_node(node);
		break;
	case CSSBoxDisplay::type_clan_grid:
		clan_grid_node(node);
		break;
	case CSSBoxDisplay::type_clan_stacked:
		clan_stacked_node(node);
		break;
	default:
		throw Exception("Unsupported display type for GUI components");
	}
}

void GUICSSDisplayVisitor::flex_node(GUIComponent_Impl *node)
{
	if (node->css_properties.flex_direction.type == CSSBoxFlexDirection::type_column || node->css_properties.flex_direction.type == CSSBoxFlexDirection::type_column_reverse)
	{
		flex_vertical_node(node);
	}
	else if (node->css_properties.flex_direction.type == CSSBoxFlexDirection::type_row || node->css_properties.flex_direction.type == CSSBoxFlexDirection::type_row_reverse)
	{
		flex_horizontal_node(node);
	}
	else
	{
		throw Exception("Unexpected CSS flex-direction computed value");
	}
}

void GUICSSDisplayVisitor::clan_grid_node(GUIComponent_Impl *node)
{
	throw Exception("-clan-grid layout not implemented yet");
}

void GUICSSDisplayVisitor::clan_stacked_node(GUIComponent_Impl *node)
{
	throw Exception("-clan-stacked layout not implemented yet");
}

CSSUsedValue GUICSSDisplayVisitor::get_used_noncontent_width(const GUICSSUsedValues &values)
{
	return values.margin.left + values.border.left + values.padding.left + values.padding.right + values.border.right + values.margin.right;
}

CSSUsedValue GUICSSDisplayVisitor::get_used_noncontent_height(const GUICSSUsedValues &values)
{
	return values.margin.top + values.border.top + values.padding.top + values.padding.bottom + values.border.bottom + values.margin.bottom;
}

float GUICSSDisplayVisitor::get_css_relative_x(GUIComponent_Impl *node, float containing_width)
{
	if (node->css_properties.position.type == CSSBoxPosition::type_relative)
	{
		if (node->css_properties.left.type == CSSBoxLeft::type_length)
			return node->css_properties.left.length.value;
		else if (node->css_properties.left.type == CSSBoxLeft::type_percentage)
			return node->css_properties.left.percentage / 100.0f * containing_width;
		else
			return 0.0f;
	}
	else
	{
		return 0.0f;
	}
}

float GUICSSDisplayVisitor::get_css_relative_y(GUIComponent_Impl *node, float containing_height)
{
	if (node->css_properties.position.type == CSSBoxPosition::type_relative)
	{
		if (node->css_properties.top.type == CSSBoxTop::type_length)
			return node->css_properties.top.length.value;
		else if (node->css_properties.top.type == CSSBoxTop::type_percentage)
			return node->css_properties.top.percentage / 100.0f * containing_height;
		else
			return 0.0f;
	}
	else
	{
		return 0.0f;
	}
}

}
