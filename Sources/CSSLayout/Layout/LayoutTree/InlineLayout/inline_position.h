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

#pragma once

#include "CSSLayout/Layout/LayoutTree/css_layout_tree_node.h"
#include "CSSLayout/Layout/BoxTree/css_box_element.h"
#include "CSSLayout/Layout/BoxTree/css_box_text.h"
#include "inline_generated_box.h"

namespace clan
{

class CSSInlineRangeVisitor
{
public:
	virtual bool node(CSSInlineGeneratedBox *box, size_t text_start, size_t text_end) = 0;
	virtual void close_node(CSSInlineGeneratedBox *box) { }
};

class CSSInlinePosition
{
public:
	CSSInlinePosition() : box(0), text_pos(0) { }
	CSSInlinePosition(CSSInlineGeneratedBox *box, size_t text_pos) : box(box), text_pos(text_pos) { }

	bool is_block_level() const
	{
		return box && box->layout_node && box->layout_node->get_element_node()->is_block_level();
	}

	bool is_float() const
	{
		return box && box->layout_node && box->layout_node->get_element_node()->is_float();
	}

	bool operator==(const CSSInlinePosition &other) const { return box == other.box && text_pos == other.text_pos; }
	bool operator!=(const CSSInlinePosition &other) const { return box != other.box || text_pos != other.text_pos; }

	CSSInlineGeneratedBox *box;
	size_t text_pos;

	static bool foreach(CSSInlinePosition start, CSSInlinePosition end, CSSInlineRangeVisitor *visitor);
};

inline bool CSSInlinePosition::foreach(CSSInlinePosition start, CSSInlinePosition end, CSSInlineRangeVisitor *visitor)
{
	CSSInlinePosition cur = start;
	while (cur != end)
	{
		CSSBoxText *text = dynamic_cast<CSSBoxText *>(cur.box->box_node);
		if (text)
		{
			size_t text_start = cur.text_pos;
			size_t text_end;
			if (cur.box != end.box)
				text_end = text->processed_text.length();
			else
				text_end = end.text_pos;
			if (!visitor->node(cur.box, text_start, text_end))
				return false;
		}
		else
		{
			visitor->node(cur.box, 0, 0);
		}

		if (cur.box == end.box)
			break;

		cur.text_pos = 0;
		if (cur.box->first_child)
		{
			cur.box = cur.box->first_child;
		}
		else if (cur.box->next_sibling)
		{
			visitor->close_node(cur.box);
			cur.box = cur.box->next_sibling;
		}
		else
		{
			while (cur.box && !cur.box->next_sibling)
			{
				visitor->close_node(cur.box);
				cur.box = cur.box->parent;
			}
			if (cur.box)
			{
				visitor->close_node(cur.box);
				cur.box = cur.box->next_sibling;
			}
		}
	}
	return true;
}

}
