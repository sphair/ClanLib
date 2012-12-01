
#include "GUI/precomp.h"
#include "gui_layout_absolute_or_fixed.h"
#include "gui_set_initial_used_values.h"
#include "gui_layout_content.h"

namespace clan
{

// This visitor assumes that the flow for the parent node has already been calculated
// and the entire layout for the absolute/fixed component needs to be calculated now.
void GUILayoutAbsoluteOrFixed::node(GUIComponent_Impl *node)
{
	if (node->css_properties.position.type == CSSBoxPosition::type_absolute || node->css_properties.position.type == CSSBoxPosition::type_fixed)
	{
		// to do: implement all the complicated rules from CSSLayoutTreeNode::layout_absolute_or_fixed
/*
		GUISetInitialUsedValues initial_visitor;
		initial_visitor.node(node);

		GUILayoutContent content_visitor;
		content_visitor.node(node);
*/
	}
}

}
