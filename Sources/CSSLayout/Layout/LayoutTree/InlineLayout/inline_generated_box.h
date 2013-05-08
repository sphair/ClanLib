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

#include "CSSLayout/Layout/LayoutTree/css_used_value.h"

namespace clan
{

class CSSBoxNode;
class CSSLayoutTreeNode;
class CSSInlineGeneratedBox;

class CSSInlineGeneratedBoxVisitor
{
public:
	virtual bool node(CSSInlineGeneratedBox *box) { return true; }
	virtual void close_node(CSSInlineGeneratedBox *box) { }
};

class CSSInlineGeneratedBox
{
public:
	CSSInlineGeneratedBox();
	~CSSInlineGeneratedBox();

	bool self_and_descendants(CSSInlineGeneratedBoxVisitor *visitor) const;
	bool descendants(CSSInlineGeneratedBoxVisitor *visitor) const;

	void add_box(CSSInlineGeneratedBox *child);
	bool is_block_line() const { return first_child == 0; }

	bool opening, closing;
	size_t text_start, text_end;
	CSSBoxNode *box_node;
	CSSLayoutTreeNode *layout_node;
	bool floated;
	CSSActualValue x, width;
	CSSActualValue y, height, ascent, descent, baseline_offset;
	CSSUsedValue relative_x, relative_y;
	CSSInlineGeneratedBox *parent;
	CSSInlineGeneratedBox *first_child;
	CSSInlineGeneratedBox *last_child;
	CSSInlineGeneratedBox *next_sibling;
};

}
