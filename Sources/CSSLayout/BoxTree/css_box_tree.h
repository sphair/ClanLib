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

#include "API/CSSLayout/css_document2.h"
#include "css_property_parsers.h"

class CL_CSSBoxElement;
class CL_CSSBoxNode;
class CL_CSSResourceCache;
class CL_CSSPropertyList2;

class CL_CSSBoxTree
{
public:
	CL_CSSBoxTree();
	~CL_CSSBoxTree();
	void clear();
	void create(const CL_DomNode &node);
	void set_root_element(CL_CSSBoxElement *new_root_element);
	void prepare(CL_CSSResourceCache *resource_cache);
	void apply_properties(CL_CSSBoxElement *node, const CL_CSSPropertyList2 &properties);
	void set_selection(CL_CSSBoxNode *start, size_t start_text_offset, CL_CSSBoxNode *end, size_t end_text_offset);

	CL_CSSDocument2 css;
	CL_CSSBoxElement *get_root_element() { return root_element; }
	const CL_CSSBoxElement *get_root_element() const { return root_element; }

private:
	void clean(CL_CSSBoxNode *node = 0);
	CL_CSSBoxNode *create_node(const CL_DomNode &node);
	void create_pseudo_element(CL_CSSBoxElement *box_element, const CL_DomElement &dom_element, const CL_String &pseudo_element);
	CL_CSSBoxProperties get_css_properties(const CL_DomElement &element, const CL_String &pseudo_element = CL_String());
	void compute_element(CL_CSSBoxElement *element, CL_CSSResourceCache *resource_cache);
	void create_anonymous_blocks(CL_CSSBoxElement *element, CL_CSSResourceCache *resource_cache);
	void filter_table(CL_CSSBoxElement * element, CL_CSSResourceCache *resource_cache);
	void convert_run_in_blocks(CL_CSSBoxElement *element);
	void apply_selection(CL_CSSBoxNode *node, CL_CSSBoxNode *start, size_t start_offset, CL_CSSBoxNode *end, size_t end_offset, bool clear, bool in_selection = false);

	CL_CSSBoxElement *root_element;
	CL_CSSPropertyParsers property_parsers;
	CL_CSSBoxNode *selection_start;
	CL_CSSBoxNode *selection_end;
	size_t selection_start_text_offset;
	size_t selection_end_text_offset;
};
