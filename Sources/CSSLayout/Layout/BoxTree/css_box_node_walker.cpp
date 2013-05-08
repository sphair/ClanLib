/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "css_box_node_walker.h"
#include "css_box_element.h"
#include "css_box_text.h"
#include "css_box_object.h"

namespace clan
{

CSSBoxNodeWalker::CSSBoxNodeWalker(CSSBoxNode *node, bool allow_upwards_walking)
: cur(node), level(allow_upwards_walking ? 4096 : 0)
{
	if (cur)
	{
		CSSBoxElement *cur_element = dynamic_cast<CSSBoxElement*>(cur);
		if (cur_element && cur_element->is_display_none())
			cur = 0;
	}
}

bool CSSBoxNodeWalker::is_node() const
{
	return cur != 0;
}

bool CSSBoxNodeWalker::is_element() const
{
	return cur != 0 && dynamic_cast<CSSBoxElement*>(cur) != 0;
}

bool CSSBoxNodeWalker::is_text() const
{
	return cur != 0 && dynamic_cast<CSSBoxText*>(cur) != 0;
}

bool CSSBoxNodeWalker::is_object() const
{
	return cur != 0 && dynamic_cast<CSSBoxObject*>(cur) != 0;
}

CSSBoxNode *CSSBoxNodeWalker::get() const
{
	return cur;
}

CSSBoxElement *CSSBoxNodeWalker::get_element() const
{
	return dynamic_cast<CSSBoxElement*>(cur);
}

CSSBoxText *CSSBoxNodeWalker::get_text() const
{
	return dynamic_cast<CSSBoxText*>(cur);
}

CSSBoxObject *CSSBoxNodeWalker::get_object() const
{
	return dynamic_cast<CSSBoxObject*>(cur);
}

bool CSSBoxNodeWalker::next(bool traverse_children)
{
	CSSBoxNode *next = 0;
	if (traverse_children && is_element() && !get_element()->is_display_none())
		next = cur->get_first_child();
	if (next)
		level++;
	else
		next = cur->get_next_sibling();
	while (cur && !next && level > 0)
	{
		level--;
		cur = cur->get_parent();
		if (cur)
			next = cur->get_next_sibling();
	}
	cur = next;
	return is_node();
}

}
