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

#include "GUI/precomp.h"
#include "gui_layout_content.h"
#include "gui_set_initial_used_values.h"
#include "gui_layout_normal_flow.h"
#include "gui_layout_absolute_or_fixed.h"

namespace clan
{

// This visitor assumes that the parent node has already calculated the layout
// for the component itself. The only exception to this rule is for the root
// component where the function also has to do the calculations on behalf of the
// "initial containing block" virtual parent.
void GUILayoutContent::node(GUIComponent_Impl *node)
{
	if (node->parent == 0)
	{
		GUISetInitialUsedValues initial_visitor;
		initial_visitor.node(node);
	}

	GUILayoutNormalFlow layout_visitor;
	layout_visitor.node(node);

	GUILayoutAbsoluteOrFixed absolute_visitor;
	node->visit_children(&absolute_visitor, false);
}

}
