
#include "GUI/precomp.h"
#include "gui_set_initial_used_values.h"

namespace clan
{

void GUISetInitialUsedValues::node(GUIComponent_Impl *node)
{
	if (node->parent)
	{
		if (node->css_properties.position.type == CSSBoxPosition::type_absolute || node->css_properties.position.type == CSSBoxPosition::type_fixed)
		{
			// To do: find nearest ancestor with 'position:relative'
			GUICSSUsedValues containing_box = node->parent->impl->css_used_values;
			containing_box.width_undetermined = false;
			containing_box.height_undetermined = false;
			GUICSSInitialUsedValues::visit(node->css_used_values, node->css_properties, containing_box);
		}
		else
		{
			GUICSSInitialUsedValues::visit(node->css_used_values, node->css_properties, node->parent->impl->css_used_values);
		}
	}
	else
	{
		GUICSSUsedValues initial_containing_box;
		initial_containing_box.width = node->geometry.get_width();
		initial_containing_box.height = node->geometry.get_height();

		GUICSSInitialUsedValues::visit(node->css_used_values, node->css_properties, initial_containing_box);
		GUICSSApplyMinMaxConstraints::visit(node->css_used_values, node->css_properties, initial_containing_box);

		if (node->css_used_values.width_undetermined)
		{
			node->css_used_values.width = initial_containing_box.width - node->css_used_values.margin.left - node->css_used_values.margin.right - node->css_used_values.border.left - node->css_used_values.border.right - node->css_used_values.padding.left - node->css_used_values.padding.right;
			node->css_used_values.width_undetermined = false;
		}

		// TBD: this isn't the default in normal CSS
		if (node->css_used_values.height_undetermined)
		{
			node->css_used_values.height = initial_containing_box.height - node->css_used_values.margin.top - node->css_used_values.margin.top - node->css_used_values.border.top - node->css_used_values.border.bottom - node->css_used_values.padding.top - node->css_used_values.padding.bottom;
			node->css_used_values.height_undetermined = false;
		}
	}
}

}
