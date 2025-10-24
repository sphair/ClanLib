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
#include "API/CSSLayout/css_layout_node.h"
#include "API/CSSLayout/css_layout_text.h"
#include "API/CSSLayout/css_layout_element.h"
#include "API/CSSLayout/css_layout_object.h"
#include "BoxTree/css_box_text.h"
#include "BoxTree/css_box_element.h"
#include "BoxTree/css_box_object.h"
#include "css_layout_node_impl.h"
#include "css_layout_impl.h"

CL_CSSLayoutNode::CL_CSSLayoutNode()
{
}

bool CL_CSSLayoutNode::is_null() const
{
	return !impl || impl->is_disposed();
}

bool CL_CSSLayoutNode::is_text() const
{
	if (is_null())
		return false;
	else
		return dynamic_cast<CL_CSSBoxText*>(impl->box_node) != 0;
}

bool CL_CSSLayoutNode::is_element() const
{
	if (is_null())
		return false;
	else
		return dynamic_cast<CL_CSSBoxElement*>(impl->box_node) != 0;
}

bool CL_CSSLayoutNode::is_object() const
{
	if (is_null())
		return false;
	else
		return dynamic_cast<CL_CSSBoxObject*>(impl->box_node) != 0;
}

CL_CSSLayoutText CL_CSSLayoutNode::to_text() const
{
	if (is_text())
		return CL_CSSLayoutText(impl);
	else
		return CL_CSSLayoutText();
}

CL_CSSLayoutElement CL_CSSLayoutNode::to_element() const
{
	if (is_element())
		return CL_CSSLayoutElement(impl);
	else
		return CL_CSSLayoutElement();
}

CL_CSSLayoutObject CL_CSSLayoutNode::to_object() const
{
	if (is_object())
		return CL_CSSLayoutObject(impl);
	else
		return CL_CSSLayoutObject();
}

CL_CSSLayoutNode CL_CSSLayoutNode::get_parent() const
{
	if (is_null())
		return CL_CSSLayoutNode();
	else
		return impl->get_node(impl->box_node->get_parent());
}

CL_CSSLayoutNode CL_CSSLayoutNode::get_next_sibling() const
{
	if (is_null())
		return CL_CSSLayoutNode();
	else
		return impl->get_node(impl->box_node->get_next_sibling());
}

CL_CSSLayoutNode CL_CSSLayoutNode::get_prev_sibling() const
{
	if (is_null())
		return CL_CSSLayoutNode();
	else
		return impl->get_node(impl->box_node->get_prev_sibling());
}

CL_CSSLayoutNode CL_CSSLayoutNode::get_first_child() const
{
	if (is_null())
		return CL_CSSLayoutNode();
	else
		return impl->get_node(impl->box_node->get_first_child());
}

CL_CSSLayoutNode CL_CSSLayoutNode::get_last_child() const
{
	if (is_null())
		return CL_CSSLayoutNode();
	else
		return impl->get_node(impl->box_node->get_last_child());
}

CL_CSSLayoutNode::CL_CSSLayoutNode(const CL_SharedPtr<CL_CSSLayoutNode_Impl> &impl)
: impl(impl)
{
}

CL_String CL_CSSLayoutNode::print_node() const
{
	if (is_null())
		return "null";
	else
		return impl->print_node(impl->box_node);
}

void CL_CSSLayoutNode::set_user_data(CL_UniquePtr<CL_CSSLayoutUserData> &data)
{
	if (!is_null())
		impl->box_node->set_user_data(data);
}

CL_CSSLayoutUserData *CL_CSSLayoutNode::get_user_data()
{
	if (!is_null())
		return impl->box_node->get_user_data();
	else
		return 0;
}

const CL_CSSLayoutUserData *CL_CSSLayoutNode::get_user_data() const
{
	if (!is_null())
		return impl->box_node->get_user_data();
	else
		return 0;
}

bool CL_CSSLayoutNode::operator==(const CL_CSSLayoutNode &other) const
{
	if (is_null() && other.is_null())
		return true;
	else if (is_null() != other.is_null())
		return false;
	else
		return impl->box_node == other.impl->box_node;
}

bool CL_CSSLayoutNode::operator!=(const CL_CSSLayoutNode &other) const
{
	return !operator==(other);
}

/////////////////////////////////////////////////////////////////////////////

CL_CSSLayoutNode_Impl::~CL_CSSLayoutNode_Impl()
{
	dispose();
}

void CL_CSSLayoutNode_Impl::on_dispose()
{
}

CL_CSSLayoutNode CL_CSSLayoutNode_Impl::get_node(CL_CSSBoxNode *box_node) const
{
	return layout_impl.lock()->get_node(box_node);
}


CL_String CL_CSSLayoutNode_Impl::print_node(CL_CSSBoxNode *node, int indent)
{
	CL_String output_string;
	output_string += CL_String(indent*2, ' ');

	CL_CSSBoxElement *element = dynamic_cast<CL_CSSBoxElement*>(node);
	CL_CSSBoxText *text = dynamic_cast<CL_CSSBoxText*>(node);

	if (element)
	{
		//if (element->computed_properties.font_family.names.empty())
			output_string += cl_format("%1 { display: %2; float: %3; width: %4 }\r\n", element->name, to_string(element->computed_properties.display), to_string(element->computed_properties.float_box), to_string(element->computed_properties.width));
		//else
		//	output_string += cl_format("%1 { font-family: %2 }\r\n", element->name, element->computed_properties.font_family.names[0].name);
	}
	else if (text)
	{
		output_string += cl_format("[%1]\r\n", escape_text(text->text));
	}
	else
	{
		output_string += cl_format("unknown\r\n");
	}

	CL_CSSBoxNode *cur = node->get_first_child();
	while (cur)
	{
		output_string += print_node(cur, indent+1);
		cur = cur->get_next_sibling();
	}
	return output_string;
}

CL_String CL_CSSLayoutNode_Impl::escape_text(CL_String text)
{
	while (true)
	{
		CL_String::size_type pos = text.find('\r');
		if (pos == CL_String::npos)
			break;
		text.replace(pos, 1, "\\r");
	}
	while (true)
	{
		CL_String::size_type pos = text.find('\n');
		if (pos == CL_String::npos)
			break;
		text.replace(pos, 1, "\\n");
	}
	while (true)
	{
		CL_String::size_type pos = text.find('\t');
		if (pos == CL_String::npos)
			break;
		text.replace(pos, 1, "\\t");
	}
	return text;
}

CL_String CL_CSSLayoutNode_Impl::to_string(const CL_CSSBoxDisplay &display)
{
	switch (display.type)
	{
	default: return "unknown";
	case CL_CSSBoxDisplay::type_inline: return "inline";
	case CL_CSSBoxDisplay::type_block: return "block";
	case CL_CSSBoxDisplay::type_list_item: return "list-item";
	case CL_CSSBoxDisplay::type_run_in: return "run-in";
	case CL_CSSBoxDisplay::type_inline_block: return "inline-block";
	case CL_CSSBoxDisplay::type_table: return "table";
	case CL_CSSBoxDisplay::type_inline_table: return "inline-table";
	case CL_CSSBoxDisplay::type_table_row_group: return "table-row-group";
	case CL_CSSBoxDisplay::type_table_header_group: return "table-header-group";
	case CL_CSSBoxDisplay::type_table_footer_group: return "table-footer-group";
	case CL_CSSBoxDisplay::type_table_row: return "table-row";
	case CL_CSSBoxDisplay::type_table_column_group: return "table-column-group";
	case CL_CSSBoxDisplay::type_table_column: return "table-column";
	case CL_CSSBoxDisplay::type_table_cell: return "table-cell";
	case CL_CSSBoxDisplay::type_table_caption: return "table-caption";
	case CL_CSSBoxDisplay::type_none: return "none";
	case CL_CSSBoxDisplay::type_inherit: return "inherit";
	}
}

CL_String CL_CSSLayoutNode_Impl::to_string(const CL_CSSBoxColor &color)
{
	switch (color.type)
	{
	default: return "unknown";
	case CL_CSSBoxColor::type_color: return cl_format("rgb(%1,%2,%3)", (int)(color.color.r*255.0f), (int)(color.color.g*255.0f), (int)(color.color.b*255.0f));
	case CL_CSSBoxColor::type_inherit: return "inherit";
	}
}

CL_String CL_CSSLayoutNode_Impl::to_string(const CL_CSSBoxFontSize &font_size)
{
	switch (font_size.type)
	{
	default: return "unknown";
	case CL_CSSBoxFontSize::type_xx_small: return "xx-small";
	case CL_CSSBoxFontSize::type_x_small: return "x-small";
	case CL_CSSBoxFontSize::type_small: return "small";
	case CL_CSSBoxFontSize::type_medium: return "medium";
	case CL_CSSBoxFontSize::type_large: return "large";
	case CL_CSSBoxFontSize::type_x_large: return "x-large";
	case CL_CSSBoxFontSize::type_xx_large: return "xx-large";
	case CL_CSSBoxFontSize::type_larger: return "larger";
	case CL_CSSBoxFontSize::type_smaller: return "smaller";
	case CL_CSSBoxFontSize::type_length: return cl_format("%1", font_size.length.value);
	case CL_CSSBoxFontSize::type_percentage: return cl_format("%1%%", font_size.percentage);
	case CL_CSSBoxFontSize::type_inherit: return "inherit";
	}
}

CL_String CL_CSSLayoutNode_Impl::to_string(const CL_CSSBoxFloat &float_box)
{
	switch (float_box.type)
	{
	default: return "unknown";
	case CL_CSSBoxFloat::type_none: return "none";
	case CL_CSSBoxFloat::type_left: return "left";
	case CL_CSSBoxFloat::type_right: return "right";
	case CL_CSSBoxFloat::type_inherit: return "inherit";
	}
}

CL_String CL_CSSLayoutNode_Impl::to_string(const CL_CSSBoxWidth &width)
{
	switch (width.type)
	{
	default: return "unknown";
	case CL_CSSBoxWidth::type_auto: return "auto";
	case CL_CSSBoxWidth::type_length: return "length";
	case CL_CSSBoxWidth::type_percentage: return "percentage";
	case CL_CSSBoxWidth::type_inherit: return "inherit";
	}
}
