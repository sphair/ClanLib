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
#include "API/CSSLayout/Layout/css_layout_element.h"
#include "API/CSSLayout/Layout/css_layout_text.h"
#include "API/CSSLayout/Layout/css_layout_object.h"
#include "API/CSSLayout/Layout/css_layout.h"
#include "API/CSSLayout/CSSDocument/css_property_value.h"
#include "API/Core/Text/string_help.h"
#include "css_layout_node_impl.h"
#include "BoxTree/css_box_element.h"
#include "BoxTree/css_box_text.h"
#include "css_layout_impl.h"

namespace clan
{

CSSLayoutElement::CSSLayoutElement()
{
}

std::string CSSLayoutElement::get_name() const
{
	if (is_null())
		return std::string();
	else
		return static_cast<CSSBoxElement*>(impl->box_node)->name;
}

void CSSLayoutElement::set_name(const std::string &name)
{
	if (!is_null())
		static_cast<CSSBoxElement*>(impl->box_node)->name = name;
}

void CSSLayoutElement::set_attribute(const std::string &name, const std::string &value)
{
	if (!is_null())
	{
		CSSBoxElement *element = static_cast<CSSBoxElement*>(impl->box_node);

		if (StringHelp::compare("name", "colspan"))
		{
			set_col_span(StringHelp::text_to_int(value));
		}
		else if (StringHelp::compare("name", "rowspan"))
		{
			set_row_span(StringHelp::text_to_int(value));
		}

		for (size_t i = 0; i < element->attributes.size(); i++)
		{
			if (StringHelp::compare(element->attributes[i].name, name, true) == 0)
			{
				element->attributes[i].value = value;
				return;
			}
		}
		element->attributes.push_back(CSSBoxElementAttribute(name, value));
	}
}

void CSSLayoutElement::set_col_span(int span)
{
	if (!is_null())
		static_cast<CSSBoxElement*>(impl->box_node)->col_span = span;
}

void CSSLayoutElement::set_row_span(int span)
{
	if (!is_null())
		static_cast<CSSBoxElement*>(impl->box_node)->row_span = span;
}
/*
void CSSLayoutElement::apply_properties(const std::vector<CSSPropertyValue *> &properties)
{
	if (!is_null())
	{
		impl->layout_impl.lock()->box_tree.apply_properties(static_cast<CSSBoxElement*>(impl->box_node), properties);

		// Temp hack to support before and after pseudo elements.
		// To do: Make a better more official way to create pseudo elements.
		if (static_cast<CSSBoxElement*>(impl->box_node)->properties.content.type == CSSValueContent::type_string)
		{
			CSSBoxText *box_text = new CSSBoxText();
			box_text->set_text(static_cast<CSSBoxElement*>(impl->box_node)->properties.content.str);
			impl->box_node->push_back(box_text);
		}
	}
}

void CSSLayoutElement::apply_properties(const std::string &style_string, const std::string &base_uri)
{
	apply_properties(CSSDocument::get_style_properties(style_string, base_uri));
}
*/
void CSSLayoutElement::insert_before(CSSLayoutNode &new_child, CSSLayoutNode &ref_child)
{
	if (!is_null() && !new_child.is_null())
	{
		if (ref_child.is_null())
		{
			append_child(new_child);
		}
		else
		{
			if (new_child.impl->box_node->get_parent())
				throw Exception("Node already has a parent");
			if (ref_child.impl->box_node->get_parent() != impl->box_node)
				throw Exception("Node is not a direct child of this element");
			impl->box_node->insert(new_child.impl->box_node, ref_child.impl->box_node);
		}
	}
}

void CSSLayoutElement::append_child(CSSLayoutNode &new_child)
{
	if (!is_null() && !new_child.is_null())
	{
		if (new_child.impl->box_node->get_parent())
			throw Exception("Node already has a parent");
		impl->box_node->push_back(new_child.impl->box_node);
	}
}

void CSSLayoutElement::remove_child(CSSLayoutNode &old_child)
{
	if (!is_null() && !old_child.is_null())
	{
		CSSBoxNode *parent = old_child.impl->box_node->get_parent();
		if (parent != impl->box_node)
			throw Exception("Node is not a direct child of this element");
		old_child.impl->box_node->remove();
	}
}

Rect CSSLayoutElement::get_content_box()
{
	if (!is_null())
	{
		return impl->layout_impl.lock()->layout_tree.get_content_box(static_cast<CSSBoxElement*>(impl->box_node));
	}
	else
	{
		return Rect();
	}
}

CSSLayoutObject CSSLayoutElement::create_object()
{
	if (!is_null())
	{
		CSSLayoutObject obj = impl->layout_impl.lock()->get_layout().create_object();
		append_child(obj);
		return obj;
	}
	else
	{
		return CSSLayoutObject();
	}
}

CSSLayoutElement CSSLayoutElement::create_element(const std::string &name)
{
	if (!is_null())
	{
		CSSLayoutElement obj = impl->layout_impl.lock()->get_layout().create_element(name);
		append_child(obj);
		return obj;
	}
	else
	{
		return CSSLayoutElement();
	}
}

CSSLayoutText CSSLayoutElement::create_text(const std::string &text)
{
	if (!is_null())
	{
		CSSLayoutText obj = impl->layout_impl.lock()->get_layout().create_text(text);
		append_child(obj);
		return obj;
	}
	else
	{
		return CSSLayoutText();
	}
}

CSSLayoutElement::CSSLayoutElement(const std::shared_ptr<CSSLayoutNode_Impl> &impl)
: CSSLayoutNode(impl)
{
}

}
