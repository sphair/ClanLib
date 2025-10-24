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

#pragma once

#include "css_layout_tree_node.h"

class CL_CSSBoxElement;
class CL_CSSBoxText;
class CL_CSSBoxMarginWidth;
class CL_CSSBoxPaddingWidth;
class CL_CSSBoxWidth;
class CL_CSSBoxHeight;
class CL_CSSBlockFormattingContext;
class CL_CSSLayoutCursor;
class CL_CSSResourceCache;
class CL_CSSInlineLayout;
class CL_GraphicContext;

class CL_CSSBlockLayout : public CL_CSSLayoutTreeNode
{
public:
	CL_CSSBlockLayout(CL_CSSBoxElement *element_node);
	~CL_CSSBlockLayout();

	void calculate_top_down_sizes();
	void set_auto_width(CL_CSSUsedValue width);
	void render(CL_GraphicContext &gc, CL_CSSResourceCache *resource_cache);
	void set_component_geometry();
	int get_first_line_baseline();
	int get_last_line_baseline();
	bool find_content_box(CL_CSSBoxElement *element, CL_Rect &out_rect);

	CL_CSSInlineLayout *find_inline_layout(CL_CSSBoxText *text_node);
	CL_CSSLayoutHitTestResult hit_test(const CL_Point &pos) const;

	std::vector<CL_CSSLayoutTreeNode *> children;

private:
	bool add_content_margin_top(CL_CSSLayoutCursor &cursor);
	void prepare_children();
	void layout_content(CL_GraphicContext &gc, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy);
};
