
#pragma once

#include "gui_component_impl.h"

namespace clan
{

class GUIComponent_Impl;

class CSSClanBoxVisitor
{
public:
	virtual void node(GUIComponent_Impl *node) = 0;
};

class CSSClanBoxInitialUsedValuesVisitor : public CSSClanBoxVisitor
{
public:
	void node(GUIComponent_Impl *node)
	{
		if (node->parent)
		{
			CSSClanBoxInitialUsedValues::visit(node->css_used_values, node->css_properties, node->parent->impl->css_used_values);
		}
		else
		{
			CSSClanBoxUsedValues initial_containing_box;
			initial_containing_box.width = node->geometry.get_width();
			initial_containing_box.height = node->geometry.get_height();

			CSSClanBoxInitialUsedValues::visit(node->css_used_values, node->css_properties, initial_containing_box);
			CSSClanBoxApplyMinMaxConstraints::visit(node->css_used_values, node->css_properties, initial_containing_box);

			if (node->css_used_values.width_undetermined)
			{
				node->css_used_values.width = initial_containing_box.width - node->css_used_values.margin.left - node->css_used_values.margin.right - node->css_used_values.border.left - node->css_used_values.border.right - node->css_used_values.padding.right - node->css_used_values.padding.right;
				node->css_used_values.width_undetermined = false;
			}

			// TBD: this isn't the default in normal CSS
			if (node->css_used_values.height_undetermined)
			{
				node->css_used_values.height = initial_containing_box.height - node->css_used_values.margin.top - node->css_used_values.margin.top - node->css_used_values.border.top - node->css_used_values.border.bottom - node->css_used_values.padding.bottom - node->css_used_values.padding.bottom;
				node->css_used_values.height_undetermined = false;
			}
		}
	}
};

class CSSClanBoxDisplayVisitor : public CSSClanBoxVisitor
{
public:
	void node(GUIComponent_Impl *node)
	{
		switch (node->css_properties.display.type)
		{
		case CSSBoxDisplay::type_clan_box:
			layout_clan_box(node);
			break;
		case CSSBoxDisplay::type_clan_grid:
			layout_clan_grid(node);
			break;
		case CSSBoxDisplay::type_clan_stacked:
			layout_clan_stacked(node);
			break;
		default:
			throw Exception("Unsupported display type for GUI components");
		}
	}

	void layout_clan_box(GUIComponent_Impl *node)
	{
		// -clan-box layout places child boxes horizontally or vertically one after another
		// -clan-box-direction controls the layout direction

		if (node->css_properties.clan_box_direction.type == CSSBoxClanBoxDirection::type_vertical)
		{
			layout_clan_box_vertical(node);
		}
		else if (node->css_properties.clan_box_direction.type == CSSBoxClanBoxDirection::type_horizontal)
		{
			layout_clan_box_horizontal(node);
		}
		else
		{
			throw Exception("Unexpected CSS -clan-box-direction computed value");
		}
	}

	virtual void layout_clan_box_horizontal(GUIComponent_Impl *node) = 0;
	virtual void layout_clan_box_vertical(GUIComponent_Impl *node) = 0;

	void layout_clan_grid(GUIComponent_Impl *node)
	{
		throw Exception("-clan-grid layout not implemented yet");
	}

	void layout_clan_stacked(GUIComponent_Impl *node)
	{
		throw Exception("-clan-stacked layout not implemented yet");
	}
};

class CSSClanBoxPreferredWidthVisitor : public CSSClanBoxDisplayVisitor
{
public:
	void layout_clan_box_horizontal(GUIComponent_Impl *node)
	{
	}

	void layout_clan_box_vertical(GUIComponent_Impl *node)
	{
	}
};

class CSSClanBoxPreferredHeightVisitor : public CSSClanBoxDisplayVisitor
{
public:
	void layout_clan_box_horizontal(GUIComponent_Impl *node)
	{
	}

	void layout_clan_box_vertical(GUIComponent_Impl *node)
	{
	}
};

class CSSClanBoxLayoutVisitor : public CSSClanBoxDisplayVisitor
{
public:
	void find_preferred_width(GUIComponent_Impl *node)
	{
		node->css_used_values.width = node->component->get_preferred_content_width();

		CSSClanBoxPreferredWidthVisitor preferred_visitor;
		node->visit_css(&preferred_visitor);

		CSSClanBoxApplyMinMaxConstraints::visit(node->css_used_values, node->css_properties, node->parent->impl->css_used_values);
	}

	void find_preferred_height(GUIComponent_Impl *node)
	{
		node->css_used_values.height = node->component->get_preferred_content_height(node->css_used_values.width);

		CSSClanBoxPreferredHeightVisitor preferred_visitor;
		node->visit_css(&preferred_visitor);

		CSSClanBoxApplyMinMaxConstraints::visit(node->css_used_values, node->css_properties, node->parent->impl->css_used_values);
	}

	void layout_clan_box_horizontal(GUIComponent_Impl *node)
	{
		// Calculate min/preferred/max widths of all child boxes
		CSSClanBoxMath box_math;
		for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling())
		{
			if (child->get_css_properties().position.type != CSSBoxPosition::type_absolute && child->get_css_properties().position.type != CSSBoxPosition::type_fixed)
			{
				CSSClanBoxUsedValues &child_used_values = child->impl->css_used_values;

				// If the width of the box cannot be determined from CSS, then ask the component:
				if (child_used_values.width_undetermined)
				{
					find_preferred_width(child->impl.get());
				}

				CSSUsedValue used_noncontent_width = get_used_noncontent_width(child_used_values);

				box_math.used_min_lengths.push_back(used_noncontent_width + child_used_values.min_width);
				box_math.used_lengths.push_back(used_noncontent_width + child_used_values.width);
				box_math.used_max_lengths.push_back(used_noncontent_width + child_used_values.max_width);

				box_math.used_shrink_weights.push_back(get_factor(child->impl->css_properties.clan_box_width_shrink_factor));
				box_math.used_expand_weights.push_back(get_factor(child->impl->css_properties.clan_box_width_expand_factor));
			}
		}

		// Adjust the widths of the boxes
		box_math.adjust(node->css_used_values.width);

		// Save adjusted width values and calculate the resulting box heights
		int i = 0;
		for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling(), i++)
		{
			if (child->get_css_properties().position.type != CSSBoxPosition::type_absolute && child->get_css_properties().position.type != CSSBoxPosition::type_fixed)
			{
				CSSClanBoxUsedValues &child_used_values = child->impl->css_used_values;

				// Save the result of the horizontal adjustment
				child_used_values.width = box_math.used_lengths[i] - child_used_values.margin.left - child_used_values.border.left - child_used_values.padding.left - child_used_values.margin.right - child_used_values.border.right - child_used_values.padding.right;

				// If the height of the box could not be determined from CSS, then ask the component:
				if (child_used_values.height_undetermined)
				{
					find_preferred_height(child->impl.get());
				}

				CSSUsedValue used_noncontent_height = get_used_noncontent_height(child_used_values);

				// Adjust height of box based on the shrink/expand factors
				CSSClanBoxMath perpendicular_math;

				perpendicular_math.used_min_lengths.push_back(used_noncontent_height + child_used_values.min_height);
				perpendicular_math.used_lengths.push_back(used_noncontent_height + child_used_values.height);
				perpendicular_math.used_max_lengths.push_back(used_noncontent_height + child_used_values.max_height);

				perpendicular_math.used_shrink_weights.push_back(get_factor(child->impl->css_properties.clan_box_height_shrink_factor));
				perpendicular_math.used_expand_weights.push_back(get_factor(child->impl->css_properties.clan_box_height_expand_factor));

				perpendicular_math.adjust(node->css_used_values.height);

				// Save the result of the vertical adjustment
				child_used_values.height = perpendicular_math.used_lengths[0] - child_used_values.margin.top - child_used_values.border.top - child_used_values.padding.top - child_used_values.margin.bottom - child_used_values.border.bottom - child_used_values.padding.bottom;
			}
		}

		set_horizontal_geometry(node);
	}

	void layout_clan_box_vertical(GUIComponent_Impl *node)
	{
		CSSClanBoxMath box_math;
		for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling())
		{
			if (child->get_css_properties().position.type != CSSBoxPosition::type_absolute && child->get_css_properties().position.type != CSSBoxPosition::type_fixed)
			{
				CSSClanBoxUsedValues &child_used_values = child->impl->css_used_values;

				// If the width of the box cannot be determined from CSS, then ask the component:
				if (child_used_values.width_undetermined)
				{
					find_preferred_width(child->impl.get());
				}

				CSSUsedValue used_noncontent_width = get_used_noncontent_width(child_used_values);

				// Adjust width of box based on the shrink/expand factors
				CSSClanBoxMath perpendicular_math;

				perpendicular_math.used_min_lengths.push_back(used_noncontent_width + child_used_values.min_width);
				perpendicular_math.used_lengths.push_back(used_noncontent_width + child_used_values.width);
				perpendicular_math.used_max_lengths.push_back(used_noncontent_width + child_used_values.max_height);

				perpendicular_math.used_shrink_weights.push_back(get_factor(child->impl->css_properties.clan_box_width_shrink_factor));
				perpendicular_math.used_expand_weights.push_back(get_factor(child->impl->css_properties.clan_box_width_expand_factor));

				perpendicular_math.adjust(node->css_used_values.width);

				// Save the result of the horizontal adjustment
				child_used_values.width = perpendicular_math.used_lengths[0] - child_used_values.margin.left - child_used_values.border.left - child_used_values.padding.left - child_used_values.margin.right - child_used_values.border.right - child_used_values.padding.right;

				// If the height of the box could not be determined from CSS, then ask the component:
				if (child_used_values.height_undetermined)
				{
					find_preferred_height(child->impl.get());
				}

				// Set up vertical box adjustment math:

				CSSUsedValue used_noncontent_height = get_used_noncontent_height(child_used_values);

				box_math.used_min_lengths.push_back(used_noncontent_height + child_used_values.min_height);
				box_math.used_lengths.push_back(used_noncontent_height + child_used_values.height);
				box_math.used_max_lengths.push_back(used_noncontent_height + child_used_values.max_height);

				box_math.used_shrink_weights.push_back(get_factor(child->impl->css_properties.clan_box_height_shrink_factor));
				box_math.used_expand_weights.push_back(get_factor(child->impl->css_properties.clan_box_height_expand_factor));
			}
		}

		// Adjust the heights of the boxes
		box_math.adjust(node->css_used_values.height);

		// Save adjusted height values
		int i = 0;
		for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling(), i++)
		{
			if (child->get_css_properties().position.type != CSSBoxPosition::type_absolute && child->get_css_properties().position.type != CSSBoxPosition::type_fixed)
			{
				CSSClanBoxUsedValues &child_used_values = child->impl->css_used_values;
				child_used_values.height = box_math.used_lengths[i] - child_used_values.margin.top - child_used_values.border.top - child_used_values.padding.top - child_used_values.margin.bottom - child_used_values.border.bottom - child_used_values.padding.bottom;
			}
		}

		set_vertical_geometry(node);
	}

	void set_horizontal_geometry(GUIComponent_Impl *node)
	{
		// Set the actual geometry
		CSSUsedValue x = 0.0f;
		CSSUsedValue y = 0.0f;
		int i = 0;
		for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling(), i++)
		{
			if (child->get_css_properties().position.type != CSSBoxPosition::type_absolute && child->get_css_properties().position.type != CSSBoxPosition::type_fixed)
			{
				CSSClanBoxUsedValues &child_used_values = child->impl->css_used_values;

				CSSUsedValue used_offset_x = child_used_values.margin.left + child_used_values.border.left + child_used_values.padding.left + get_css_relative_x(child->impl.get(), node->css_used_values.width);
				CSSUsedValue used_offset_y = child_used_values.margin.top + child_used_values.border.top + child_used_values.padding.top + get_css_relative_y(child->impl.get(), node->css_used_values.height);

				// Used to actual mapping
				CSSActualValue x1 = (CSSActualValue)(x + used_offset_x + child_used_values.margin.left);
				CSSActualValue y1 = (CSSActualValue)(y + used_offset_y + child_used_values.margin.top);
				CSSActualValue x2 = (CSSActualValue)(x + used_offset_x + child_used_values.width - child_used_values.margin.left - child_used_values.margin.right + 0.5f);
				CSSActualValue y2 = (CSSActualValue)(y + used_offset_y + child_used_values.height - child_used_values.margin.top - child_used_values.margin.bottom + 0.5f);
				child->set_geometry(Rect(x1, y1, x2, y2));

				x += child_used_values.margin.left + child_used_values.border.left + child_used_values.padding.left + child_used_values.width + child_used_values.padding.right + child_used_values.border.right + child_used_values.margin.right;
			}
		}
	}

	void set_vertical_geometry(GUIComponent_Impl *node)
	{
		CSSUsedValue x = 0.0f;
		CSSUsedValue y = 0.0f;
		int i = 0;
		for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling(), i++)
		{
			if (child->get_css_properties().position.type != CSSBoxPosition::type_absolute && child->get_css_properties().position.type != CSSBoxPosition::type_fixed)
			{
				CSSClanBoxUsedValues &child_used_values = child->impl->css_used_values;

				CSSUsedValue used_offset_x = child_used_values.margin.left + child_used_values.border.left + child_used_values.padding.left + get_css_relative_x(child->impl.get(), node->css_used_values.width);
				CSSUsedValue used_offset_y = child_used_values.margin.top + child_used_values.border.top + child_used_values.padding.top + get_css_relative_y(child->impl.get(), node->css_used_values.height);

				// Used to actual mapping
				CSSActualValue x1 = (CSSActualValue)(x + used_offset_x + child_used_values.margin.left);
				CSSActualValue y1 = (CSSActualValue)(y + used_offset_y + child_used_values.margin.top);
				CSSActualValue x2 = (CSSActualValue)(x + used_offset_x + child_used_values.width - child_used_values.margin.left - child_used_values.margin.right + 0.5f);
				CSSActualValue y2 = (CSSActualValue)(y + used_offset_y + child_used_values.height - child_used_values.margin.top - child_used_values.margin.bottom + 0.5f);
				child->set_geometry(Rect(x1, y1, x2, y2));

				y += child_used_values.margin.top + child_used_values.border.top + child_used_values.padding.top + child_used_values.height + child_used_values.padding.bottom + child_used_values.border.bottom + child_used_values.margin.bottom;
			}
		}
	}

	CSSUsedValue get_used_noncontent_width(const CSSClanBoxUsedValues &values)
	{
		return values.margin.left + values.border.left + values.padding.left + values.padding.right + values.border.right + values.margin.right;
	}

	CSSUsedValue get_used_noncontent_height(const CSSClanBoxUsedValues &values)
	{
		return values.margin.top + values.border.top + values.padding.top + values.padding.bottom + values.border.bottom + values.margin.bottom;
	}

	CSSUsedValue get_factor(const CSSBoxClanBoxSizingFactor &factor)
	{
		switch (factor.type)
		{
		default:
		case CSSBoxClanBoxSizingFactor::type_auto:
			return 0.0f;
		case CSSBoxClanBoxSizingFactor::type_number:
			return factor.number;
		}
	}

	float get_css_relative_x(GUIComponent_Impl *node, float containing_width)
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

	float get_css_relative_y(GUIComponent_Impl *node, float containing_height)
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
};

class CSSClanBoxAbsoluteOrFixedVisitor : public CSSClanBoxVisitor
{
public:
	void node(GUIComponent_Impl *node)
	{
		if (node->css_properties.position.type == CSSBoxPosition::type_absolute || node->css_properties.position.type == CSSBoxPosition::type_fixed)
		{
			float containing_width = node->parent->get_width();
			float containing_height = node->parent->get_height();

			float left = 0.0f;
			if (node->css_properties.left.type == CSSBoxLeft::type_length)
				left = node->css_properties.left.length.value;
			else if (node->css_properties.left.type == CSSBoxLeft::type_percentage)
				left = node->css_properties.left.percentage * containing_width / 100.0f;

			float right = 0.0f;
			if (node->css_properties.right.type == CSSBoxRight::type_length)
				right = node->css_properties.right.length.value;
			else if (node->css_properties.right.type == CSSBoxRight::type_percentage)
				right = node->css_properties.right.percentage * containing_width / 100.0f;

			// to do: implement all the complicated rules from CSSLayoutTreeNode::layout_absolute_or_fixed
/*
			CSSClanBoxInitialUsedValuesVisitor initial_visitor;
			node->visit_css(&initial_visitor);

			CSSClanBoxLayoutVisitor layout_visitor;
			node->visit_css(&layout_visitor);

			CSSClanBoxAbsoluteOrFixedVisitor absolute_visitor;
			node->visit_css(&absolute_visitor);
*/
		}
	}
};

}
