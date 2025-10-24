/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

class CL_CSSBoxNode;
class CL_CSSBoxElement;
class CL_CSSBoxObject;
class CL_CSSBoxText;
class CL_CSSLayoutTreeNode;
class CL_CSSInlineLayout;
class CL_CSSInlineGeneratedBox;
class CL_CSSReplacedLayout;
class CL_CSSTableLayout;
class CL_CSSResourceCache;
class CL_CSSStackingContext;
class CL_CSSLayoutHitTestResult;
class CL_CSSLayoutGraphics;

class CL_CSSLayoutTree
{
public:
	CL_CSSLayoutTree();
	~CL_CSSLayoutTree();

	void clear();
	void create(CL_CSSBoxElement *element);
	void layout(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resource_cache, const CL_Size &viewport);
	void render(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resource_cache);
	CL_CSSLayoutHitTestResult hit_test(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resource_cache, const CL_Point &pos);
	CL_Rect get_cursor_box(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources, CL_CSSBoxText *text_node, CL_String::size_type pos);
	CL_Rect get_content_box(CL_CSSBoxElement *element);

private:
	CL_CSSLayoutTreeNode *create_layout(CL_CSSBoxElement *element);
	CL_CSSInlineLayout *create_inline_level_layout(CL_CSSBoxElement *element);
	CL_CSSInlineGeneratedBox *create_inline_generated_box(CL_CSSBoxNode *cur);
	CL_CSSReplacedLayout *create_replaced_level_layout(CL_CSSBoxObject *object);
	CL_CSSTableLayout *create_table_level_layout(CL_CSSBoxElement *element);

	CL_CSSLayoutTreeNode *root_layout;
	CL_CSSStackingContext *root_stacking_context;
};
