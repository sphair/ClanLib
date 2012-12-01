
#include "GUI/precomp.h"
#include "gui_find_preferred_width.h"

namespace clan
{

void GUIFindPreferredWidth::flex_horizontal_node(GUIComponent_Impl *node)
{
	node->css_used_values.width = node->component->get_preferred_content_width();

	CSSUsedValue preferred_width = 0.0f;
	for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling())
	{
		if (child->get_css_properties().position.type != CSSBoxPosition::type_absolute && child->get_css_properties().position.type != CSSBoxPosition::type_fixed)
		{
			GUICSSUsedValues &child_used_values = child->impl->css_used_values;

			// If the width of the box cannot be determined from CSS, then ask the component:
			if (child_used_values.width_undetermined)
			{
				GUICSSDisplayVisitor::node(child->impl.get());
			}

			preferred_width += get_used_noncontent_width(child_used_values) + child_used_values.width;
		}
	}

	node->css_used_values.width = std::max(node->css_used_values.width, preferred_width);
	GUICSSApplyMinMaxConstraints::visit(node->css_used_values, node->css_properties, node->parent->impl->css_used_values);
}

void GUIFindPreferredWidth::flex_vertical_node(GUIComponent_Impl *node)
{
	node->css_used_values.width = node->component->get_preferred_content_width();

	CSSUsedValue preferred_width = 0.0f;
	for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling())
	{
		if (child->get_css_properties().position.type != CSSBoxPosition::type_absolute && child->get_css_properties().position.type != CSSBoxPosition::type_fixed)
		{
			GUICSSUsedValues &child_used_values = child->impl->css_used_values;

			// If the width of the box cannot be determined from CSS, then ask the component:
			if (child_used_values.width_undetermined)
			{
				GUICSSDisplayVisitor::node(child->impl.get());
			}

			preferred_width = std::max(preferred_width, get_used_noncontent_width(child_used_values) + child_used_values.width);
		}
	}

	node->css_used_values.width = std::max(node->css_used_values.width, preferred_width);
	GUICSSApplyMinMaxConstraints::visit(node->css_used_values, node->css_properties, node->parent->impl->css_used_values);
}

}
