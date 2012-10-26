/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

namespace clan
{

class CSSBoxElement;
class CSSBoxNode;
class CSSResourceCache;
class CSSPropertyList2;

class CSSBoxTree
{
public:
	CSSBoxTree();
	~CSSBoxTree();
	void clear();
	void create(const DomNode &node);
	void set_root_element(CSSBoxElement *new_root_element);
	void set_html_body_element(CSSBoxElement *new_html_body_element);
	void prepare(CSSResourceCache *resource_cache);
	void apply_properties(CSSBoxElement *node, const CSSPropertyList2 &properties);
	void set_selection(CSSBoxNode *start, size_t start_text_offset, CSSBoxNode *end, size_t end_text_offset);

	CSSDocument2 css;
	CSSBoxElement *get_root_element() { return root_element; }
	const CSSBoxElement *get_root_element() const { return root_element; }
	CSSBoxElement *get_html_body_element() { return html_body_element; }
	const CSSBoxElement *get_html_body_element() const { return html_body_element; }

private:
	void clean(CSSBoxNode *node = 0);
	CSSBoxNode *create_node(const DomNode &node);
	void create_pseudo_element(CSSBoxElement *box_element, const DomElement &dom_element, const std::string &pseudo_element);
	CSSBoxProperties get_css_properties(const DomElement &element, const std::string &pseudo_element = std::string());
	void compute_element(CSSBoxElement *element, CSSResourceCache *resource_cache);
	void propagate_html_body();
	void create_anonymous_blocks(CSSBoxElement *element, CSSResourceCache *resource_cache);
	void filter_table(CSSResourceCache *resource_cache);
	void convert_run_in_blocks(CSSBoxElement *element);
	void apply_selection(CSSBoxNode *start, size_t start_offset, CSSBoxNode *end, size_t end_offset, bool clear);

	CSSBoxElement *root_element;
	CSSBoxElement *html_body_element;
	CSSPropertyParsers property_parsers;
	CSSBoxNode *selection_start;
	CSSBoxNode *selection_end;
	size_t selection_start_text_offset;
	size_t selection_end_text_offset;
};

}
