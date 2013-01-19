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

#include "CSSLayout/precomp.h"
#include "css_stacking_context.h"
#include "css_layout_tree_node.h"
#include "../BoxTree/css_box_element.h"
#include "API/CSSLayout/PropertyValues/css_value_z_index.h"
#include <algorithm>

namespace clan
{

CSSStackingContext::CSSStackingContext(CSSLayoutTreeNode *layout)
: level(0), layout(layout), tree_order(0), next_child_tree_order(0)
{
	level = layout->get_element_node()->computed_values.get_box().z_index.value;
}

CSSStackingContext::~CSSStackingContext()
{
	for (size_t i = 0; i < children.size(); i++)
		delete children[i];
}

bool CSSStackingContext::operator <(const CSSStackingContext &other) const
{
	if (level != other.level)
		return level < other.level;
	else
		return tree_order < other.tree_order;
}

void CSSStackingContext::push_back(CSSStackingContext *child)
{
	child->tree_order = next_child_tree_order++;
	children.push_back(child);
}

void CSSStackingContext::sort()
{
	std::sort(children.begin(), children.end(), SortPred());
	for (size_t i = 0; i < children.size(); i++)
		children[i]->sort();
}

void CSSStackingContext::render(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache, bool root)
{
	size_t i;
	layout->render_layer_background(graphics, resource_cache, root);
	for (i = 0; i < children.size() && children[i]->level < 0; i++)
		children[i]->render(graphics, resource_cache);
	layout->render_layer_non_inline(graphics, resource_cache);
	layout->render_layer_floats(graphics, resource_cache);
	layout->render_layer_inline(graphics, resource_cache);
	layout->render_layer_positioned(graphics, resource_cache);

	while (i < children.size())
	{
		if (children[i]->level > 0)
			children[i]->render(graphics, resource_cache);
		i++;
	}
}

}
