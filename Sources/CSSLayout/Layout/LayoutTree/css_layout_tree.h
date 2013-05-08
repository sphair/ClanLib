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

namespace clan
{

class CSSBoxNode;
class CSSBoxElement;
class CSSBoxObject;
class CSSBoxText;
class CSSLayoutTreeNode;
class CSSInlineLayout;
class CSSInlineGeneratedBox;
class CSSReplacedLayout;
class CSSTableLayout;
class CSSResourceCache;
class CSSStackingContext;
class CSSLayoutHitTestResult;
class CSSLayoutGraphics;

class CSSLayoutTree
{
public:
	CSSLayoutTree();
	~CSSLayoutTree();

	void clear();
	void create(CSSBoxElement *element);
	void layout(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache, const Size &viewport);
	void render(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache);
	CSSLayoutHitTestResult hit_test(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache, const Point &pos);
	Rect get_cursor_box(CSSLayoutGraphics *graphics, CSSResourceCache *resources, CSSBoxText *text_node, std::string::size_type pos);
	Rect get_content_box(CSSBoxElement *element);

private:
	CSSLayoutTreeNode *create_layout(CSSBoxElement *element);
	CSSInlineLayout *create_inline_level_layout(CSSBoxElement *element);
	CSSInlineGeneratedBox *create_inline_generated_box(CSSBoxNode *cur);
	CSSReplacedLayout *create_replaced_level_layout(CSSBoxObject *object);
	CSSTableLayout *create_table_level_layout(CSSBoxElement *element);

	CSSLayoutTreeNode *root_layout;
	CSSStackingContext *root_stacking_context;
};

}
