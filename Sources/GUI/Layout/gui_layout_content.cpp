
#include "GUI/precomp.h"
#include "gui_layout_content.h"
#include "gui_set_initial_used_values.h"
#include "gui_layout_box_content.h"
#include "gui_layout_absolute_or_fixed.h"

namespace clan
{

// This visitor assumes that the parent node has already calculated the layout
// for the component itself. The only exception to this rule is for the root
// component where the function also has to do the calculations on behalf of the
// "initial containing block" virtual parent.
void GUILayoutContent::node(GUIComponent_Impl *node)
{
	GUISetInitialUsedValues initial_visitor;
	if (node->parent == 0)
		initial_visitor.node(node);
	node->visit_children(&initial_visitor, true);

	GUILayoutBoxContent layout_visitor;
	if (node->parent == 0)
		layout_visitor.node(node);
	node->visit_children(&layout_visitor, true);

	GUILayoutAbsoluteOrFixed absolute_visitor;
	if (node->parent == 0)
		absolute_visitor.node(node);
	node->visit_children(&absolute_visitor, true);
}

}
