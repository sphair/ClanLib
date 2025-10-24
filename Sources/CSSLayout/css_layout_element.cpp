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

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/css_layout_element.h"
#include "API/CSSLayout/css_layout_text.h"
#include "API/CSSLayout/css_layout_object.h"
#include "API/CSSLayout/css_layout.h"
#include "API/CSSLayout/css_property_list2.h"
#include "css_layout_node_impl.h"
#include "BoxTree/css_box_element.h"
#include "BoxTree/css_box_text.h"
#include "css_layout_impl.h"

CL_CSSLayoutElement::CL_CSSLayoutElement()
{
}

CL_String CL_CSSLayoutElement::get_name() const
{
	if (is_null())
		return CL_String();
	else
		return static_cast<CL_CSSBoxElement*>(impl->box_node)->name;
}

void CL_CSSLayoutElement::set_name(const CL_String &name)
{
	if (!is_null())
		static_cast<CL_CSSBoxElement*>(impl->box_node)->name = name;
}

void CL_CSSLayoutElement::set_col_span(int span)
{
	if (!is_null())
		static_cast<CL_CSSBoxElement*>(impl->box_node)->col_span = span;
}

void CL_CSSLayoutElement::set_row_span(int span)
{
	if (!is_null())
		static_cast<CL_CSSBoxElement*>(impl->box_node)->row_span = span;
}

void CL_CSSLayoutElement::apply_properties(const CL_CSSPropertyList2 &properties)
{
	if (!is_null())
	{
		impl->layout_impl.lock()->box_tree.apply_properties(static_cast<CL_CSSBoxElement*>(impl->box_node), properties);

		// Temp hack to support before and after pseudo elements.
		// To do: Make a better more official way to create pseudo elements.
		if (static_cast<CL_CSSBoxElement*>(impl->box_node)->properties.content.type == CL_CSSBoxContent::type_string)
		{
			CL_CSSBoxText *box_text = new CL_CSSBoxText();
			box_text->set_text(static_cast<CL_CSSBoxElement*>(impl->box_node)->properties.content.str);
			impl->box_node->push_back(box_text);
		}
	}
}

void CL_CSSLayoutElement::apply_properties(const CL_String &style_string, const CL_String &base_uri)
{
	apply_properties(CL_CSSDocument2::get_style_properties(style_string, base_uri));
}

void CL_CSSLayoutElement::insert_before(CL_CSSLayoutNode &new_child, CL_CSSLayoutNode &ref_child)
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
				throw CL_Exception("Node already has a parent");
			if (ref_child.impl->box_node->get_parent() != impl->box_node)
				throw CL_Exception("Node is not a direct child of this element");
			impl->box_node->insert(new_child.impl->box_node, ref_child.impl->box_node);
		}
	}
}

void CL_CSSLayoutElement::append_child(CL_CSSLayoutNode &new_child)
{
	if (!is_null() && !new_child.is_null())
	{
		if (new_child.impl->box_node->get_parent())
			throw CL_Exception("Node already has a parent");
		impl->box_node->push_back(new_child.impl->box_node);
	}
}

void CL_CSSLayoutElement::remove_child(CL_CSSLayoutNode &old_child)
{
	if (!is_null() && !old_child.is_null())
	{
		CL_CSSBoxNode *parent = old_child.impl->box_node->get_parent();
		if (parent != impl->box_node)
			throw CL_Exception("Node is not a direct child of this element");
		old_child.impl->box_node->remove();
	}
}

CL_Rect CL_CSSLayoutElement::get_content_box()
{
	if (!is_null())
	{
		return impl->layout_impl.lock()->layout_tree.get_content_box(static_cast<CL_CSSBoxElement*>(impl->box_node));
	}
	else
	{
		return CL_Rect();
	}
}

CL_CSSLayoutObject CL_CSSLayoutElement::create_object()
{
	if (!is_null())
	{
		CL_CSSLayoutObject obj = impl->layout_impl.lock()->get_layout().create_object();
		append_child(obj);
		return obj;
	}
	else
	{
		return CL_CSSLayoutObject();
	}
}

CL_CSSLayoutElement CL_CSSLayoutElement::create_element(const CL_String &name)
{
	if (!is_null())
	{
		CL_CSSLayoutElement obj = impl->layout_impl.lock()->get_layout().create_element(name);
		append_child(obj);
		return obj;
	}
	else
	{
		return CL_CSSLayoutElement();
	}
}

CL_CSSLayoutText CL_CSSLayoutElement::create_text(const CL_String &text)
{
	if (!is_null())
	{
		CL_CSSLayoutText obj = impl->layout_impl.lock()->get_layout().create_text(text);
		append_child(obj);
		return obj;
	}
	else
	{
		return CL_CSSLayoutText();
	}
}

CL_CSSLayoutElement::CL_CSSLayoutElement(const CL_SharedPtr<CL_CSSLayoutNode_Impl> &impl)
: CL_CSSLayoutNode(impl)
{
}
