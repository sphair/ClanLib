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
#include "inline_generated_box.h"

namespace clan
{

CSSInlineGeneratedBox::CSSInlineGeneratedBox()
: opening(true), closing(true), text_start(0), text_end(0), box_node(0), layout_node(0),
  floated(false), x(0), width(0), y(0), height(0), ascent(0), descent(0), baseline_offset(0),
  relative_x(0), relative_y(0), parent(0), first_child(0), last_child(0), next_sibling(0)
{
}

CSSInlineGeneratedBox::~CSSInlineGeneratedBox()
{
	CSSInlineGeneratedBox *cur = first_child;
	while (cur)
	{
		CSSInlineGeneratedBox *next = cur->next_sibling;
		delete cur;
		cur = next;
	}
}

void CSSInlineGeneratedBox::add_box(CSSInlineGeneratedBox *child)
{
	child->parent = this;
	if (last_child)
	{
		last_child->next_sibling = child;
		last_child = child;
	}
	else
	{
		first_child = child;
		last_child = child;
	}
}

bool CSSInlineGeneratedBox::self_and_descendants(CSSInlineGeneratedBoxVisitor *visitor) const
{
	CSSInlineGeneratedBox *cur = const_cast<CSSInlineGeneratedBox*>(this);
	while (cur)
	{
		if (!visitor->node(cur))
			return false;

		if (cur->first_child)
		{
			cur = cur->first_child;
		}
		else if (cur->next_sibling)
		{
			visitor->close_node(cur);
			cur = cur->next_sibling;
		}
		else
		{
			while (cur && !cur->next_sibling)
			{
				visitor->close_node(cur);
				cur = cur->parent;
			}
			if (cur)
			{
				visitor->close_node(cur);
				cur = cur->next_sibling;
			}
		}
	}
	return true;
}

bool CSSInlineGeneratedBox::descendants(CSSInlineGeneratedBoxVisitor *visitor) const
{
	if (first_child)
		return first_child->self_and_descendants(visitor);
	else
		return true;
}

}
