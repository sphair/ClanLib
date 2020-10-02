/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
	class XPathToken
	{
	public:
		enum Type
		{
			type_none,
			type_bracket_begin,
			type_bracket_end,
			type_dot,
			type_double_dot,
			type_at_sign,
			type_comma,
			type_double_colon,
			type_name_test,
			type_node_type,
			type_operator,
			type_function_name,
			type_axis_name,
			type_literal,
			type_number,
			type_variable_reference
		};

		enum NodeType
		{
			node_type_comment,
			node_type_text,
			node_type_processing_instruction,
			node_type_node
		};

		enum Operator
		{
			operator_parenthesis_begin,
			operator_mod,
			operator_div,
			operator_multiply,
			operator_union,
			operator_slash,
			operator_double_slash,
			operator_plus,
			operator_minus,
			operator_assign,
			operator_greater_equal,
			operator_greater,
			operator_less_equal,
			operator_less,
			operator_compare_not_equal,
			operator_compare_equal,
			operator_and,
			operator_or,
			operator_parenthesis_end
		};

		struct Value
		{
			NodeType node_type;
			Operator oper;
			std::string str;
		};

		Type type;
		Value value;
		std::string::size_type pos, length;

		XPathToken()
			: type(type_none), pos(0), length(0)
		{
		}
	};
}
