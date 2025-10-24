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

class CL_CSSBoxObject;
class CL_CSSReplacedComponent;

class CL_CSSReplacedLayout : public CL_CSSLayoutTreeNode
{
public:
	CL_CSSReplacedLayout(CL_CSSBoxObject *object);
	~CL_CSSReplacedLayout();

	void render(CL_GraphicContext &gc, CL_CSSResourceCache *resources);
	void set_component_geometry();
	int get_first_line_baseline();
	int get_last_line_baseline();

	bool is_replaced() const { return true; }

private:
	void prepare_children();
	void layout_content(CL_GraphicContext &gc, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy);

	CL_Rect box;
	CL_Image image;
	CL_CSSReplacedComponent *component;
};
