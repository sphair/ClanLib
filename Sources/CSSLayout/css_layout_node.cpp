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

namespace clan
{

CSSLayoutNode::CSSLayoutNode()
{
}

bool CSSLayoutNode::is_null() const
{
	return !impl || impl->is_disposed();
}

bool CSSLayoutNode::is_text() const
{
	if (is_null())
		return false;
	else
		return dynamic_cast<CSSBoxText*>(impl->box_node) != 0;
}

bool CSSLayoutNode::is_element() const
{
	if (is_null())
		return false;
	else
		return dynamic_cast<CSSBoxElement*>(impl->box_node) != 0;
}

bool CSSLayoutNode::is_object() const
{
	if (is_null())
		return false;
	else
		return dynamic_cast<CSSBoxObject*>(impl->box_node) != 0;
}

CSSLayoutText CSSLayoutNode::to_text() const
{
	if (is_text())
		return CSSLayoutText(impl);
	else
		return CSSLayoutText();
}

CSSLayoutElement CSSLayoutNode::to_element() const
{
	if (is_element())
		return CSSLayoutElement(impl);
	else
		return CSSLayoutElement();
}

CSSLayoutObject CSSLayoutNode::to_object() const
{
	if (is_object())
		return CSSLayoutObject(impl);
	else
		return CSSLayoutObject();
}

CSSLayoutNode CSSLayoutNode::get_parent() const
{
	if (is_null())
		return CSSLayoutNode();
	else
		return impl->get_node(impl->box_node->get_parent());
}

CSSLayoutNode CSSLayoutNode::get_next_sibling() const
{
	if (is_null())
		return CSSLayoutNode();
	else
		return impl->get_node(impl->box_node->get_next_sibling());
}

CSSLayoutNode CSSLayoutNode::get_prev_sibling() const
{
	if (is_null())
		return CSSLayoutNode();
	else
		return impl->get_node(impl->box_node->get_prev_sibling());
}

CSSLayoutNode CSSLayoutNode::get_first_child() const
{
	if (is_null())
		return CSSLayoutNode();
	else
		return impl->get_node(impl->box_node->get_first_child());
}

CSSLayoutNode CSSLayoutNode::get_last_child() const
{
	if (is_null())
		return CSSLayoutNode();
	else
		return impl->get_node(impl->box_node->get_last_child());
}

CSSLayoutNode::CSSLayoutNode(const std::shared_ptr<CSSLayoutNode_Impl> &impl)
: impl(impl)
{
}

std::string CSSLayoutNode::print_node() const
{
	if (is_null())
		return "null";
	else
		return impl->print_node(impl->box_node);
}

void CSSLayoutNode::set_user_data(std::unique_ptr<CSSLayoutUserData> &data)
{
	if (!is_null())
		impl->box_node->set_user_data(data);
}

CSSLayoutUserData *CSSLayoutNode::get_user_data()
{
	if (!is_null())
		return impl->box_node->get_user_data();
	else
		return 0;
}

const CSSLayoutUserData *CSSLayoutNode::get_user_data() const
{
	if (!is_null())
		return impl->box_node->get_user_data();
	else
		return 0;
}

bool CSSLayoutNode::operator==(const CSSLayoutNode &other) const
{
	if (is_null() && other.is_null())
		return true;
	else if (is_null() != other.is_null())
		return false;
	else
		return impl->box_node == other.impl->box_node;
}

bool CSSLayoutNode::operator!=(const CSSLayoutNode &other) const
{
	return !operator==(other);
}

/////////////////////////////////////////////////////////////////////////////

CSSLayoutNode_Impl::~CSSLayoutNode_Impl()
{
	dispose();
}

void CSSLayoutNode_Impl::on_dispose()
{
}

CSSLayoutNode CSSLayoutNode_Impl::get_node(CSSBoxNode *box_node) const
{
	return layout_impl.lock()->get_node(box_node);
}


std::string CSSLayoutNode_Impl::print_node(CSSBoxNode *node, int indent)
{
	std::string output_string;
	output_string += std::string(indent*2, ' ');

	CSSBoxElement *element = dynamic_cast<CSSBoxElement*>(node);
	CSSBoxText *text = dynamic_cast<CSSBoxText*>(node);

	if (element)
	{
		//if (element->computed_properties.font_family.names.empty())
			output_string += string_format("%1 { display: %2; float: %3; width: %4 }\r\n", element->name, to_string(element->computed_properties.display), to_string(element->computed_properties.float_box), to_string(element->computed_properties.width));
		//else
		//	output_string += string_format("%1 { font-family: %2 }\r\n", element->name, element->computed_properties.font_family.names[0].name);
	}
	else if (text)
	{
		output_string += string_format("[%1]\r\n", escape_text(text->text));
	}
	else
	{
		output_string += string_format("unknown\r\n");
	}

	CSSBoxNode *cur = node->get_first_child();
	while (cur)
	{
		output_string += print_node(cur, indent+1);
		cur = cur->get_next_sibling();
	}
	return output_string;
}

std::string CSSLayoutNode_Impl::escape_text(std::string text)
{
	while (true)
	{
		std::string::size_type pos = text.find('\r');
		if (pos == std::string::npos)
			break;
		text.replace(pos, 1, "\\r");
	}
	while (true)
	{
		std::string::size_type pos = text.find('\n');
		if (pos == std::string::npos)
			break;
		text.replace(pos, 1, "\\n");
	}
	while (true)
	{
		std::string::size_type pos = text.find('\t');
		if (pos == std::string::npos)
			break;
		text.replace(pos, 1, "\\t");
	}
	return text;
}

std::string CSSLayoutNode_Impl::to_string(const CSSBoxDisplay &display)
{
	switch (display.type)
	{
	default: return "unknown";
	case CSSBoxDisplay::type_inline: return "inline";
	case CSSBoxDisplay::type_block: return "block";
	case CSSBoxDisplay::type_list_item: return "list-item";
	case CSSBoxDisplay::type_run_in: return "run-in";
	case CSSBoxDisplay::type_inline_block: return "inline-block";
	case CSSBoxDisplay::type_table: return "table";
	case CSSBoxDisplay::type_inline_table: return "inline-table";
	case CSSBoxDisplay::type_table_row_group: return "table-row-group";
	case CSSBoxDisplay::type_table_header_group: return "table-header-group";
	case CSSBoxDisplay::type_table_footer_group: return "table-footer-group";
	case CSSBoxDisplay::type_table_row: return "table-row";
	case CSSBoxDisplay::type_table_column_group: return "table-column-group";
	case CSSBoxDisplay::type_table_column: return "table-column";
	case CSSBoxDisplay::type_table_cell: return "table-cell";
	case CSSBoxDisplay::type_table_caption: return "table-caption";
	case CSSBoxDisplay::type_none: return "none";
	case CSSBoxDisplay::type_inherit: return "inherit";
	}
}

std::string CSSLayoutNode_Impl::to_string(const CSSBoxColor &color)
{
	switch (color.type)
	{
	default: return "unknown";
	case CSSBoxColor::type_color: return string_format("rgb(%1,%2,%3)", (int)(color.color.r*255.0f), (int)(color.color.g*255.0f), (int)(color.color.b*255.0f));
	case CSSBoxColor::type_inherit: return "inherit";
	}
}

std::string CSSLayoutNode_Impl::to_string(const CSSBoxFontSize &font_size)
{
	switch (font_size.type)
	{
	default: return "unknown";
	case CSSBoxFontSize::type_xx_small: return "xx-small";
	case CSSBoxFontSize::type_x_small: return "x-small";
	case CSSBoxFontSize::type_small: return "small";
	case CSSBoxFontSize::type_medium: return "medium";
	case CSSBoxFontSize::type_large: return "large";
	case CSSBoxFontSize::type_x_large: return "x-large";
	case CSSBoxFontSize::type_xx_large: return "xx-large";
	case CSSBoxFontSize::type_larger: return "larger";
	case CSSBoxFontSize::type_smaller: return "smaller";
	case CSSBoxFontSize::type_length: return string_format("%1", font_size.length.value);
	case CSSBoxFontSize::type_percentage: return string_format("%1%%", font_size.percentage);
	case CSSBoxFontSize::type_inherit: return "inherit";
	}
}

std::string CSSLayoutNode_Impl::to_string(const CSSBoxFloat &float_box)
{
	switch (float_box.type)
	{
	default: return "unknown";
	case CSSBoxFloat::type_none: return "none";
	case CSSBoxFloat::type_left: return "left";
	case CSSBoxFloat::type_right: return "right";
	case CSSBoxFloat::type_inherit: return "inherit";
	}
}

std::string CSSLayoutNode_Impl::to_string(const CSSBoxWidth &width)
{
	switch (width.type)
	{
	default: return "unknown";
	case CSSBoxWidth::type_auto: return "auto";
	case CSSBoxWidth::type_length: return "length";
	case CSSBoxWidth::type_percentage: return "percentage";
	case CSSBoxWidth::type_inherit: return "inherit";
	}
}

}
