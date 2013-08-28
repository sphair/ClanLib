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
#include "css_box_select_node.h"
#include "css_box_element.h"
#include "API/Core/Text/string_help.h"

namespace clan
{

CSSBoxSelectNode::CSSBoxSelectNode(const CSSBoxElement *element)
: dom_element(element), pos(element)
{
}

CSSBoxSelectNode::CSSBoxSelectNode(const CSSBoxElement *element, const std::string &pseudo_tag_name)
: dom_element(element), pos(element), pseudo_tag_name(pseudo_tag_name)
{
}

bool CSSBoxSelectNode::parent()
{
	const CSSBoxElement *parent_node = static_cast<const CSSBoxElement*>(pos->get_parent());
	if (parent_node)
	{
		pos = parent_node;
		return true;
	}
	else
	{
		return false;
	}
}

bool CSSBoxSelectNode::prev_sibling()
{
	const CSSBoxNode *prev = pos->get_prev_sibling();
	while (prev && !dynamic_cast<const CSSBoxElement*>(prev))
		prev = prev->get_prev_sibling();

	if (prev)
	{
		pos = static_cast<const CSSBoxElement*>(prev);
		return true;
	}
	else
	{
		return false;
	}
}

std::string CSSBoxSelectNode::name()
{
	return pos->name;
}

std::string CSSBoxSelectNode::lang()
{
	for (size_t i = 0; i < pos->attributes.size(); i++)
	{
		if (StringHelp::compare(pos->attributes[i].name, "lang", true) == 0)
			return pos->attributes[i].value;
	}
	return "en-US";
}

std::string CSSBoxSelectNode::id()
{
	for (size_t i = 0; i < pos->attributes.size(); i++)
	{
		if (StringHelp::compare(pos->attributes[i].name, "id", true) == 0)
			return pos->attributes[i].value;
	}
	return std::string();
}

std::vector<std::string> CSSBoxSelectNode::element_classes()
{
	for (size_t i = 0; i < pos->attributes.size(); i++)
	{
		if (StringHelp::compare(pos->attributes[i].name, "class", true) == 0)
			return StringHelp::split_text(pos->attributes[i].value, " ", true);
	}

	return std::vector<std::string>();
}

std::vector<std::string> CSSBoxSelectNode::pseudo_classes()
{
	std::vector<std::string> classes;
	if (!pseudo_tag_name.empty())
		classes.push_back(pseudo_tag_name);
	if (!pos->get_prev_sibling())
		classes.push_back("first-child");
	return classes;
}

std::string CSSBoxSelectNode::get_attribute_value(const std::string &name, bool &out_found)
{
	for (size_t i = 0; i < pos->attributes.size(); i++)
	{
		if (StringHelp::compare(pos->attributes[i].name, name, true))
		{
			out_found = true;
			return pos->attributes[i].value;
		}
	}

	out_found = false;
	return std::string();
}

int CSSBoxSelectNode::child_index()
{
	int index = 0;
	const CSSBoxNode *cur = pos;
	while (cur)
	{
		if (dynamic_cast<const CSSBoxElement*>(cur))
			index++;
		cur = cur->get_prev_sibling();
	}
	return index;
}

void CSSBoxSelectNode::push()
{
	saved_elements.push_back(pos);
}

void CSSBoxSelectNode::pop()
{
	pos = saved_elements.back();
	saved_elements.pop_back();
}

}
