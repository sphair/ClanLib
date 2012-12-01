
#include "GUI/precomp.h"
#include "gui_find_preferred_height.h"

namespace clan
{

void GUIFindPreferredHeight::flex_horizontal_node(GUIComponent_Impl *node)
{
	node->css_used_values.height = node->component->get_preferred_content_height(node->css_used_values.width);

	CSSUsedValue preferred_height = 0.0f;
	for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling())
	{
		if (child->get_css_properties().position.type != CSSBoxPosition::type_absolute && child->get_css_properties().position.type != CSSBoxPosition::type_fixed)
		{
			GUICSSUsedValues &child_used_values = child->impl->css_used_values;

			// If the height of the box cannot be determined from CSS, then ask the component:
			if (child_used_values.height_undetermined)
			{
				GUICSSDisplayVisitor::node(child->impl.get());
			}

			preferred_height = std::max(preferred_height, get_used_noncontent_height(child_used_values) + child_used_values.height);
		}
	}

	node->css_used_values.height = std::max(node->css_used_values.height, preferred_height);
	GUICSSApplyMinMaxConstraints::visit(node->css_used_values, node->css_properties, node->parent->impl->css_used_values);
}

void GUIFindPreferredHeight::flex_vertical_node(GUIComponent_Impl *node)
{
	node->css_used_values.height = node->component->get_preferred_content_height(node->css_used_values.width);

	CSSUsedValue preferred_height = 0.0f;
	for (GUIComponent *child = node->first_child; child != 0; child = child->get_next_sibling())
	{
		if (child->get_css_properties().position.type != CSSBoxPosition::type_absolute && child->get_css_properties().position.type != CSSBoxPosition::type_fixed)
		{
			GUICSSUsedValues &child_used_values = child->impl->css_used_values;

			// If the height of the box cannot be determined from CSS, then ask the component:
			if (child_used_values.height_undetermined)
			{
				GUICSSDisplayVisitor::node(child->impl.get());
			}

			preferred_height += get_used_noncontent_height(child_used_values) + child_used_values.height;
		}
	}

	node->css_used_values.height = std::max(node->css_used_values.height, preferred_height);
	GUICSSApplyMinMaxConstraints::visit(node->css_used_values, node->css_properties, node->parent->impl->css_used_values);
}

}
