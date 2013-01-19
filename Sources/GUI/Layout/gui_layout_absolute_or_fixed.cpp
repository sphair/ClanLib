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
#include "gui_layout_absolute_or_fixed.h"
#include "gui_set_initial_used_values.h"
#include "gui_layout_content.h"
#include "../gui_element.h"

namespace clan
{

// This visitor assumes that the flow for the parent node has already been calculated
// and the entire layout for the absolute/fixed component needs to be calculated now.
void GUILayoutAbsoluteOrFixed::node(GUIComponent_Impl *node)
{
	const CSSComputedBox &properties = node->element.get_css_properties();
	if (properties.position.type == CSSValuePosition::type_absolute || properties.position.type == CSSValuePosition::type_fixed)
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
