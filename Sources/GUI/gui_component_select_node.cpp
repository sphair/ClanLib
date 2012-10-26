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

#include "GUI/precomp.h"
#include "gui_component_select_node.h"
#include "API/GUI/gui_component.h"
#include "API/Core/Text/string_help.h"

namespace clan
{

GUIComponentSelectNode::GUIComponentSelectNode(const GUIComponent *element)
: dom_element(element), pos(element)
{
}

bool GUIComponentSelectNode::parent()
{
	const GUIComponent *parent_node = pos->get_parent_component();
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

bool GUIComponentSelectNode::prev_sibling()
{
	const GUIComponent *prev_node = pos->get_previous_sibling();
	if (prev_node)
	{
		pos = prev_node;
		return true;
	}
	else
	{
		return false;
	}
}

std::string GUIComponentSelectNode::name()
{
	return pos->get_tag_name();
}

std::string GUIComponentSelectNode::lang()
{
	// "We all live in America! (this is not a love song)" ;)
	return "en-US";//pos.get_attribute("lang");
}

std::string GUIComponentSelectNode::id()
{
	return pos->get_id();
}

std::vector<std::string> GUIComponentSelectNode::element_classes()
{
	return StringHelp::split_text(pos->get_class(), " ");
}

std::vector<std::string> GUIComponentSelectNode::pseudo_classes()
{
	std::vector<std::string> classes = pos->get_pseudo_classes();
	if (!pos->get_previous_sibling())
		classes.push_back("first-child");
	return classes;
}

std::string GUIComponentSelectNode::get_attribute_value(const std::string &name, bool &out_found)
{
	out_found = false;
	return std::string();
}

int GUIComponentSelectNode::child_index()
{
	int index = 0;
	const GUIComponent *cur = pos;
	while (cur)
	{
		index++;
		cur = cur->get_previous_sibling();
	}
	return index;
}

void GUIComponentSelectNode::push()
{
	saved_elements.push_back(pos);
}

void GUIComponentSelectNode::pop()
{
	pos = saved_elements.back();
	saved_elements.pop_back();
}

}
