/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "../BoxTree/PropertyTypes/css_box_z_index.h"
#include <algorithm>

CL_CSSStackingContext::CL_CSSStackingContext(CL_CSSLayoutTreeNode *layout)
: level(0), layout(layout), tree_order(0), next_child_tree_order(0)
{
	level = layout->get_element_node()->computed_properties.z_index.value;
}

CL_CSSStackingContext::~CL_CSSStackingContext()
{
	for (size_t i = 0; i < children.size(); i++)
		delete children[i];
}

bool CL_CSSStackingContext::operator <(const CL_CSSStackingContext &other) const
{
	if (level != other.level)
		return level < other.level;
	else
		return tree_order < other.tree_order;
}

void CL_CSSStackingContext::push_back(CL_CSSStackingContext *child)
{
	child->tree_order = next_child_tree_order++;
	children.push_back(child);
}

void CL_CSSStackingContext::sort()
{
	std::sort(children.begin(), children.end());
}

void CL_CSSStackingContext::render(CL_GraphicContext &gc, CL_CSSResourceCache *resource_cache)
{
	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i]->level >= 0)
			break;
		children[i]->render(gc, resource_cache);
	}
	layout->render(gc, resource_cache);
	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i]->level >= 0)
		{
			children[i]->render(gc, resource_cache);
		}
	}
}
