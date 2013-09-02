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

#include "API/CSSLayout/CSSDocument/css_document.h"
#include "CSSLayout/PropertyParsers/css_property_parsers.h"

namespace clan
{

class CSSBoxElement;
class CSSBoxNode;
class CSSResourceCache;
class CSSPropertyValue;

class CSSBoxTree
{
public:
	CSSBoxTree();
	~CSSBoxTree();
	void clear();
	void set_root_element(CSSBoxElement *new_root_element);
	void prepare(CSSResourceCache *resource_cache);

	CSSDocument css;
	CSSBoxElement *get_root_element() { return root_element; }
	const CSSBoxElement *get_root_element() const { return root_element; }

private:
	void compute(CSSResourceCache *cache);
	void compute(CSSResourceCache *cache, CSSBoxNode *node, bool &collapse_space);

	std::string normalize_newlines(const std::string &text);
	std::string remove_whitespace_around_linefeed(const std::string &text);
	std::string convert_linefeed_to_space(const std::string &text);
	std::string convert_tab_to_space(const std::string &text);
	std::string collapse_spaces(const std::string &text, bool &collapsing);

	CSSBoxElement *root_element;
	CSSPropertyParsers property_parsers;
};

}
