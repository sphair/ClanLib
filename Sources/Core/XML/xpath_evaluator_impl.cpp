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
**    Thomas Gottschalk Larsen
*/

#include "Core/precomp.h"
#include "API/Core/XML/xpath_evaluator.h"
#include "API/Core/XML/xpath_object.h"
#include "API/Core/XML/dom_node.h"
#include "API/Core/XML/dom_named_node_map.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/XML/xpath_exception.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Math/cl_math.h"
#include "xpath_evaluator_impl.h"
#include "xpath_token.h"
#include "xpath_location_step.h"
#include <cmath>
#include <limits>

/////////////////////////////////////////////////////////////////////////////
// CL_XPathEvaluator_Impl Operations:


CL_XPathEvaluateResult CL_XPathEvaluator_Impl::evaluate(
	const CL_StringRef &expression,
	const CL_XPathNodeSet &context,
	CL_XPathNodeSet::size_type context_node_index,
	CL_XPathToken prev_token) const
{
	std::vector<Operator> operator_stack;
	std::vector<Operand> operand_stack;

	int parenthesis_count = 0;
	CL_XPathToken cur_token;

	while (true)
	{
		cur_token = read_token(expression, prev_token);

		bool end_parenthesis = (
			cur_token.type == CL_XPathToken::type_operator &&
			cur_token.value.oper == CL_XPathToken::operator_parenthesis_end &&
			parenthesis_count == 0);

		if (cur_token.type == CL_XPathToken::type_none ||
			cur_token.type == CL_XPathToken::type_comma ||
			cur_token.type == CL_XPathToken::type_bracket_end ||
			end_parenthesis)
			break;

		// Check if its a location path:
		if (cur_token.type == CL_XPathToken::type_axis_name ||
			cur_token.type == CL_XPathToken::type_name_test ||
			cur_token.type == CL_XPathToken::type_at_sign ||
			cur_token.type == CL_XPathToken::type_dot ||
			cur_token.type == CL_XPathToken::type_double_dot ||
			(cur_token.type == CL_XPathToken::type_operator &&
				(cur_token.value.oper == CL_XPathToken::operator_slash ||
				cur_token.value.oper == CL_XPathToken::operator_double_slash)))
		{
			prev_token = read_location_path(expression, cur_token, context, context_node_index, operand_stack);
			continue;
		}

		// Evaluate expression:

		if (cur_token.type == CL_XPathToken::type_literal)
		{
			operand_stack.push_back(CL_XPathObject(cur_token.value.str));
		}
		else if (cur_token.type == CL_XPathToken::type_variable_reference)
		{
			operand_stack.push_back(CL_XPathObject(get_variable(cur_token.value.str)));
		}
		else if (cur_token.type == CL_XPathToken::type_number)
		{
			operand_stack.push_back(CL_XPathObject(CL_StringHelp::text_to_double(cur_token.value.str)));
		}
		else if (cur_token.type == CL_XPathToken::type_function_name)
		{
			CL_String function_name = cur_token.value.str;
			cur_token = read_token(expression, cur_token);
			if (cur_token.type != CL_XPathToken::type_operator ||
				cur_token.value.oper != CL_XPathToken::operator_parenthesis_begin)
			{
				throw CL_XPathException("Expected '(' after function name", expression, cur_token);
			}

			std::vector<CL_XPathObject> parameters;
			while (true)
			{
				CL_XPathEvaluateResult result = evaluate(expression, context, context_node_index, cur_token);
				if (!result.result.is_null())
					parameters.push_back(result.result);

				cur_token = result.next_token;
				if (cur_token.type == CL_XPathToken::type_operator &&
					cur_token.value.oper == CL_XPathToken::operator_parenthesis_end)
					break;
				if (cur_token.type != CL_XPathToken::type_comma)
					throw CL_XPathException("Expected ',' or ')' in function call", expression, cur_token);
			}

			CL_XPathObject obj = call_function(context, context_node_index, function_name, parameters);
			operand_stack.push_back(obj);
		}
		else if (cur_token.type == CL_XPathToken::type_bracket_begin)
		{
			if (operand_stack.empty())
				throw CL_XPathException("Missing operand before predicate", expression, cur_token);

			Operand cur_operand = operand_stack.back();
			operand_stack.pop_back();
			if (cur_operand.get_type() != CL_XPathObject::type_node_set)
				throw CL_XPathException("Expected node-set operand before '['", expression, cur_token);

			CL_XPathToken end_token = cur_token;
			while (end_token.type != CL_XPathToken::type_bracket_end && end_token.type != CL_XPathToken::type_none)
				end_token = read_token(expression, end_token);

			if (end_token.type == CL_XPathToken::type_none)
				throw CL_XPathException("Missing matching ']' in expression", expression, cur_token);

			CL_XPathLocationStep::Predicate predicate;
			predicate.pos = cur_token.pos + cur_token.length;
			predicate.length = end_token.pos - predicate.pos;

			CL_XPathNodeSet filtered_nodes;
			CL_XPathNodeSet nodes = cur_operand.get_node_set();
			for (CL_XPathNodeSet::size_type node_index = 0, num_nodes = nodes.size(); node_index < num_nodes; node_index++)
			{
				if (confirm_step_predicate(nodes, node_index, predicate, expression))
					filtered_nodes.push_back(nodes[node_index]);
			}

			cur_token = end_token;
			operand_stack.push_back(CL_XPathObject(filtered_nodes));
		}
		else if (cur_token.type == CL_XPathToken::type_operator)
		{
			Operator previous = operator_stack.empty() ? CL_XPathToken::operator_parenthesis_begin : operator_stack.back();
			Operator cur_operator = cur_token.value.oper;
			if (cur_operator == CL_XPathToken::operator_parenthesis_begin)
			{
				parenthesis_count++;
			}
			else if (cur_operator == CL_XPathToken::operator_parenthesis_end)
			{
				parenthesis_count--;
				while(do_npr(operator_stack, operand_stack));
			}

			if (previous < cur_operator)
				do_npr(operator_stack, operand_stack);

			if (cur_operator != CL_XPathToken::operator_parenthesis_end)
				operator_stack.push_back(cur_operator);
		}
		else
		{
			throw CL_XPathException("Unexpected token", expression, cur_token);
		}

		prev_token = cur_token;
	}

	while (do_npr(operator_stack, operand_stack));

	// Allow empty expression between parenthesis (for supporting functions that take no parameters)
	if (operand_stack.empty() &&
			!(prev_token.type == CL_XPathToken::type_operator &&
			prev_token.value.oper == CL_XPathToken::operator_parenthesis_begin &&
			cur_token.type == CL_XPathToken::type_operator &&
			cur_token.value.oper == CL_XPathToken::operator_parenthesis_end))
	{
		throw CL_XPathException("Expected operand", expression, cur_token);
	}

	CL_XPathEvaluateResult result;
	result.result = operand_stack.empty() ? CL_XPathObject() : operand_stack.back();
	result.next_token = cur_token;
	return result;
}


CL_XPathObject CL_XPathEvaluator_Impl::call_function(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const CL_StringRef &name, const std::vector<CL_XPathObject> &parameters) const
{
	if (name == "last")
		return function_last(context, context_node_index, parameters);
	else if (name == "position")
		return function_position(context, context_node_index, parameters);
	else if (name == "count")
		return function_count(context, context_node_index, parameters);
	else if (name == "id")
		return function_id(context, context_node_index, parameters);
	else if (name == "local-name")
		return function_local_name(context, context_node_index, parameters);
	else if (name == "namespace-uri")
		return function_namespace_uri(context, context_node_index, parameters);
	else if (name == "name")
		return function_name(context, context_node_index, parameters);
	else if (name == "string")
		return function_string(context, context_node_index, parameters);
	else if (name == "concat")
		return function_concat(context, context_node_index, parameters);
	else if (name == "starts-with")
		return function_starts_with(context, context_node_index, parameters);
	else if (name == "contains")
		return function_contains(context, context_node_index, parameters);
	else if (name == "substring-before")
		return function_substring_before(context, context_node_index, parameters);
	else if (name == "substring-after")
		return function_substring_after(context, context_node_index, parameters);
	else if (name == "substring")
		return function_substring(context, context_node_index, parameters);
	else if (name == "string-length")
		return function_string_length(context, context_node_index, parameters);
	else if (name == "normalize-space")
		return function_normalize_space(context, context_node_index, parameters);
	else if (name == "translate")
		return function_translate(context, context_node_index, parameters);
	else if (name == "boolean")
		return function_boolean(context, context_node_index, parameters);
	else if (name == "not")
		return function_not(context, context_node_index, parameters);
	else if (name == "true")
		return function_true(context, context_node_index, parameters);
	else if (name == "false")
		return function_false(context, context_node_index, parameters);
	else if (name == "lang")
		return function_lang(context, context_node_index, parameters);
	else if (name == "number")
		return function_number(context, context_node_index, parameters);
	else if (name == "sum")
		return function_sum(context, context_node_index, parameters);
	else if (name == "floor")
		return function_floor(context, context_node_index, parameters);
	else if (name == "ceiling")
		return function_ceiling(context, context_node_index, parameters);
	else if (name == "round")
		return function_round(context, context_node_index, parameters);

	throw CL_Exception(cl_format("Unknown function '%1'", name));
}

CL_XPathObject CL_XPathEvaluator_Impl::get_variable(const CL_StringRef &name) const
{
	return CL_XPathObject(name);
}

/////////////////////////////////////////////////////////////////////////////
// CL_XPathEvaluator_Impl implementation:

bool CL_XPathEvaluator_Impl::do_npr(
	std::vector<CL_XPathEvaluator_Impl::Operator> &operator_stack,
	std::vector<CL_XPathEvaluator_Impl::Operand> &operand_stack) const
{
	if (operator_stack.empty()) 
		return false;

	Operator oper = operator_stack.back();
	operator_stack.pop_back();

	Operand a, b;
	Operand result;

	switch (oper)
	{
	case CL_XPathToken::operator_parenthesis_begin:
		return false;

	case CL_XPathToken::operator_and:
		if (operand_stack.empty()) 
			return false;
		a = operand_stack.back();
		operand_stack.pop_back();

		if (operand_stack.empty()) 
			return false;
		b = operand_stack.back();
		operand_stack.pop_back();

		result.set_boolean(boolean(b).get_boolean() && boolean(a).get_boolean());
		operand_stack.push_back(result);
		break;

	case CL_XPathToken::operator_or:
		if (operand_stack.empty()) 
			return false;
		a = operand_stack.back();
		operand_stack.pop_back();

		if (operand_stack.empty()) 
			return false;
		b = operand_stack.back();
		operand_stack.pop_back();

		result.set_boolean(boolean(b).get_boolean() || boolean(a).get_boolean());
		operand_stack.push_back(result);
		break;

	case CL_XPathToken::operator_mod:
		if (operand_stack.empty()) 
			return false;
		a = operand_stack.back();
		operand_stack.pop_back();

		if (operand_stack.empty()) 
			return false;
		b = operand_stack.back();
		operand_stack.pop_back();

		result.set_number(static_cast<int>(number(b).get_number()) % static_cast<int>(number(a).get_number()));
		operand_stack.push_back(result);
		break;
	case CL_XPathToken::operator_div:
		if (operand_stack.empty()) 
			return false;
		a = operand_stack.back();
		operand_stack.pop_back();

		if (operand_stack.empty()) 
			return false;
		b = operand_stack.back();
		operand_stack.pop_back();

		result.set_number(number(b).get_number() / number(a).get_number());
		operand_stack.push_back(result);
		break;
	case CL_XPathToken::operator_multiply:
		if (operand_stack.empty()) 
			return false;
		a = operand_stack.back();
		operand_stack.pop_back();

		if (operand_stack.empty()) 
			return false;
		b = operand_stack.back();
		operand_stack.pop_back();

		result.set_number(number(a).get_number() * number(b).get_number());
		operand_stack.push_back(result);
		break;
	case CL_XPathToken::operator_union:
		if (operand_stack.empty()) 
			return false;
		a = operand_stack.back();
		operand_stack.pop_back();

		if (operand_stack.empty()) 
			return false;
		b = operand_stack.back();
		operand_stack.pop_back();

		if (a.get_type() != CL_XPathObject::type_node_set)
			return false;
		if (b.get_type() != CL_XPathObject::type_node_set)
			return false;

		{
			CL_XPathNodeSet nodeset_a = a.get_node_set();
			CL_XPathNodeSet nodeset_b = b.get_node_set();
			for (CL_XPathNodeSet::const_iterator ita = nodeset_a.begin(), itaEnd = nodeset_a.end(); ita != itaEnd; ++ita)
			{
				bool found = false;
				for (CL_XPathNodeSet::const_iterator itb = nodeset_b.begin(), itbEnd = nodeset_b.end(); itb != itbEnd; ++itb)
				{
					if (*ita == *itb)
					{
						found = true;
						break;
					}
				}
				if (!found)
					nodeset_b.push_back(*ita);
			}

			result.set_node_set(nodeset_b);
			operand_stack.push_back(result);
		}
		break;

	case CL_XPathToken::operator_plus:
		if (operand_stack.empty()) 
			return false;
		a = operand_stack.back();
		operand_stack.pop_back();

		if (operand_stack.empty()) 
			return false;
		b = operand_stack.back();
		operand_stack.pop_back();

		result.set_number(number(a).get_number() + number(b).get_number());
		operand_stack.push_back(result);
		break;

	case CL_XPathToken::operator_minus:
		if (operand_stack.empty()) 
			return false;
		a = operand_stack.back();
		operand_stack.pop_back();

		if (operand_stack.empty()) 
			return false;
		b = operand_stack.back();
		operand_stack.pop_back();

		result.set_number(number(b).get_number() - number(a).get_number());
		operand_stack.push_back(result);
		break;

	case CL_XPathToken::operator_compare_equal:
		if (operand_stack.empty()) 
			return false;
		a = operand_stack.back();
		operand_stack.pop_back();

		if (operand_stack.empty()) 
			return false;
		b = operand_stack.back();
		operand_stack.pop_back();

		result.set_boolean(compare_operands(b, a, oper));
		operand_stack.push_back(result);
		break;
	case CL_XPathToken::operator_compare_not_equal:
		if (operand_stack.empty()) 
			return false;
		a = operand_stack.back();
		operand_stack.pop_back();

		if (operand_stack.empty()) 
			return false;
		b = operand_stack.back();
		operand_stack.pop_back();

		result.set_boolean(compare_operands(b, a, oper));
		operand_stack.push_back(result);
		break;
	case CL_XPathToken::operator_less:
		if (operand_stack.empty()) 
			return false;
		a = operand_stack.back();
		operand_stack.pop_back();

		if (operand_stack.empty()) 
			return false;
		b = operand_stack.back();
		operand_stack.pop_back();

		result.set_boolean(compare_operands(b, a, oper));
		operand_stack.push_back(result);
		break;
	case CL_XPathToken::operator_less_equal:
		if (operand_stack.empty()) 
			return false;
		a = operand_stack.back();
		operand_stack.pop_back();

		if (operand_stack.empty()) 
			return false;
		b = operand_stack.back();
		operand_stack.pop_back();

		result.set_boolean(compare_operands(b, a, oper));
		operand_stack.push_back(result);
		break;

	case CL_XPathToken::operator_greater:
		if (operand_stack.empty()) 
			return false;
		a = operand_stack.back();
		operand_stack.pop_back();

		if (operand_stack.empty()) 
			return false;
		b = operand_stack.back();
		operand_stack.pop_back();

		result.set_boolean(compare_operands(b, a, oper));
		operand_stack.push_back(result);
		break;

	case CL_XPathToken::operator_greater_equal:
		if (operand_stack.empty()) 
			return false;
		a = operand_stack.back();
		operand_stack.pop_back();

		if (operand_stack.empty()) 
			return false;
		b = operand_stack.back();
		operand_stack.pop_back();

		result.set_boolean(compare_operands(b, a, oper));
		operand_stack.push_back(result);
		break;
	default:	// Added to stop compiler warnings for "operator_xxx" not handled in switch
		break;
	}

	return true;
}

template<>
bool CL_XPathEvaluator_Impl::compare(const CL_String &value1, const CL_String &value2, Operator oper) const
{
	switch(oper)
	{
	case CL_XPathToken::operator_compare_equal:
		if (value1.compare(value2) == 0)
			return true;
		break;
	case CL_XPathToken::operator_compare_not_equal:
		if (value1.compare(value2) != 0)
			return true;
		break;
	case CL_XPathToken::operator_less_equal:
		if (value1.compare(value2) <= 0)
			return true;
		break;
	case CL_XPathToken::operator_less:
		if (value1.compare(value2) < 0)
			return true;
		break;
	case CL_XPathToken::operator_greater_equal:
		if (value1.compare(value2) >= 0)
			return true;
		break;
	case CL_XPathToken::operator_greater:
		if (value1.compare(value2) > 0)
			return true;
		break;
	}
	return false;
}

template<>
bool CL_XPathEvaluator_Impl::compare(const CL_StringRef &value1, const CL_StringRef &value2, Operator oper) const
{
	switch(oper)
	{
	case CL_XPathToken::operator_compare_equal:
		if (value1.compare(value2) == 0)
			return true;
		break;
	case CL_XPathToken::operator_compare_not_equal:
		if (value1.compare(value2) != 0)
			return true;
		break;
	case CL_XPathToken::operator_less_equal:
		if (value1.compare(value2) <= 0)
			return true;
		break;
	case CL_XPathToken::operator_less:
		if (value1.compare(value2) < 0)
			return true;
		break;
	case CL_XPathToken::operator_greater_equal:
		if (value1.compare(value2) >= 0)
			return true;
		break;
	case CL_XPathToken::operator_greater:
		if (value1.compare(value2) > 0)
			return true;
		break;
	}
	return false;
}

template<typename T>
bool CL_XPathEvaluator_Impl::compare(const T &value1, const T &value2, Operator oper) const
{
	switch(oper)
	{
	case CL_XPathToken::operator_compare_equal:
		if (value1 == value2)
			return true;
		break;
	case CL_XPathToken::operator_compare_not_equal:
		if (value1 != value2)
			return true;
		break;
	case CL_XPathToken::operator_less_equal:
		if (value1 <= value2)
			return true;
		break;
	case CL_XPathToken::operator_less:
		if (value1 < value2)
			return true;
		break;
	case CL_XPathToken::operator_greater_equal:
		if (value1 >= value2)
			return true;
		break;
	case CL_XPathToken::operator_greater:
		if (value1 > value2)
			return true;
		break;
	}
	return false;
}

bool CL_XPathEvaluator_Impl::compare_operands(const Operand &a, const Operand &b, Operator oper) const
{
	if (a.get_type() == CL_XPathObject::type_node_set)
	{
		return compare_node_set(a, b, oper);
	}
	else if (a.get_type() == CL_XPathObject::type_number)
	{
		return compare_number(a, b, oper);
	}
	else if (a.get_type() == CL_XPathObject::type_boolean)
	{
		return compare_boolean(a, b, oper);
	}
	else if (a.get_type() == CL_XPathObject::type_string)
	{
		return compare_string(a, b, oper);
	}
	else
	{
		throw CL_XPathException("Unknown operand type in compare operation");
	}
}

bool CL_XPathEvaluator_Impl::compare_node_set(const Operand &a, const Operand &b, Operator oper) const
{
	CL_XPathNodeSet nodeset1 = a.get_node_set();
	if (b.get_type() == CL_XPathObject::type_node_set)
	{
		CL_XPathNodeSet nodeset2 = b.get_node_set();
		for (CL_XPathNodeSet::const_iterator it1 = nodeset1.begin(); it1 != nodeset1.end(); ++it1)
		{
			CL_String value1 = string(*it1);
			for (CL_XPathNodeSet::const_iterator it2 = nodeset2.begin(); it2 != nodeset2.end(); ++it2)
			{
				CL_String value2 = string(*it2);
				if (compare(value1, value2, oper))
					return true;
			}
		}
		return false;
	}
	else if (b.get_type() == CL_XPathObject::type_number)
	{
		double value2 = b.get_number();
		for (CL_XPathNodeSet::const_iterator it = nodeset1.begin(), itEnd = nodeset1.end(); it != itEnd; ++it)
		{
			try
			{
				double value1 = CL_StringHelp::text_to_double(string(*it));
				if (compare(value1, value2, oper))
					return true;
			}
			catch(CL_Exception&)
			{
			}
		}
		return false;
	}
	else if (b.get_type() == CL_XPathObject::type_boolean)
	{
		bool value2 = b.get_boolean();
		for (CL_XPathNodeSet::const_iterator it = nodeset1.begin(), itEnd = nodeset1.end(); it != itEnd; ++it)
		{
			bool value1 = boolean(*it);
			if (compare(value1, value2, oper))
				return true;
		}

		return false;
	}
	else if (b.get_type() == CL_XPathObject::type_string)
	{
		CL_String value2 = b.get_string();
		for (CL_XPathNodeSet::const_iterator it = nodeset1.begin(), itEnd = nodeset1.end(); it != itEnd; ++it)
		{
			CL_String value1 = string(*it);
			if (compare(value1, value2, oper))
				return true;
		}
		return false;
	}
	else
	{
		throw CL_XPathException("Unknown operand type in compare operation");
	}
}

bool CL_XPathEvaluator_Impl::compare_boolean(const Operand &a, const Operand &b, Operator oper) const
{
	bool value1 = a.get_boolean();
	if (b.get_type() == CL_XPathObject::type_node_set)
	{
		switch (oper)
		{
			case CL_XPathToken::operator_compare_equal:
			case CL_XPathToken::operator_compare_not_equal:
				if (compare_node_set(b, a, oper))
					return true;
				break;
			case CL_XPathToken::operator_less_equal:
				if (compare_node_set(b, a, CL_XPathToken::operator_greater_equal))
					return true;
				break;
			case CL_XPathToken::operator_less:
				if (compare_node_set(b, a, CL_XPathToken::operator_greater))
					return true;
				break;
			case CL_XPathToken::operator_greater_equal:
				if (compare_node_set(b, a, CL_XPathToken::operator_less_equal))
					return true;
				break;
			case CL_XPathToken::operator_greater:
				if (compare_node_set(b, a, CL_XPathToken::operator_less))
					return true;
				break;
		}
		return false;
	}
	else if (b.get_type() == CL_XPathObject::type_number)
	{
		return compare(value1, boolean(b).get_boolean(), oper);
	}
	else if (b.get_type() == CL_XPathObject::type_boolean)
	{
		return compare(value1, b.get_boolean(), oper);
	}
	else if (b.get_type() == CL_XPathObject::type_string)
	{
		return compare(value1, !b.get_string().empty(), oper);
	}
	else
	{
		throw CL_XPathException("Unknown operand type in compare operation");
	}
}

bool CL_XPathEvaluator_Impl::compare_number(const Operand &a, const Operand &b, Operator oper) const
{
	double value1 = a.get_number();
	if (b.get_type() == CL_XPathObject::type_node_set)
	{
		switch (oper)
		{
		case CL_XPathToken::operator_compare_equal:
		case CL_XPathToken::operator_compare_not_equal:
			if (compare_node_set(b, a, oper))
				return true;
			break;
		case CL_XPathToken::operator_less_equal:
			if (compare_node_set(b, a, CL_XPathToken::operator_greater_equal))
				return true;
			break;
		case CL_XPathToken::operator_less:
			if (compare_node_set(b, a, CL_XPathToken::operator_greater))
				return true;
			break;
		case CL_XPathToken::operator_greater_equal:
			if (compare_node_set(b, a, CL_XPathToken::operator_less_equal))
				return true;
			break;
		case CL_XPathToken::operator_greater:
			if (compare_node_set(b, a, CL_XPathToken::operator_less))
				return true;
			break;
		}
		return false;
	}
	else if (b.get_type() == CL_XPathObject::type_boolean)
	{
		return compare(boolean(a).get_boolean(), b.get_boolean(), oper);
	}
	else if (b.get_type() == CL_XPathObject::type_number)
	{
		return compare(value1, b.get_number(), oper);
	}
	else if (b.get_type() == CL_XPathObject::type_string)
	{
		bool result = false;
		try
		{
			result = compare(value1, number(b).get_number(), oper);
		}
		catch (CL_Exception&)
		{
		}
		return false;
	}
	else
	{
		throw CL_XPathException("Unknown operand type in compare operation");
	}
}

bool CL_XPathEvaluator_Impl::compare_string(const Operand &a, const Operand &b, Operator oper) const
{
	if (b.get_type() == CL_XPathObject::type_node_set)
	{
		switch (oper)
		{
		case CL_XPathToken::operator_compare_equal:
		case CL_XPathToken::operator_compare_not_equal:
			if (compare_node_set(b, a, oper))
				return true;
			break;
		case CL_XPathToken::operator_less_equal:
			if (compare_node_set(b, a, CL_XPathToken::operator_greater_equal))
				return true;
			break;
		case CL_XPathToken::operator_less:
			if (compare_node_set(b, a, CL_XPathToken::operator_greater))
				return true;
			break;
		case CL_XPathToken::operator_greater_equal:
			if (compare_node_set(b, a, CL_XPathToken::operator_less_equal))
				return true;
			break;
		case CL_XPathToken::operator_greater:
			if (compare_node_set(b, a, CL_XPathToken::operator_less))
				return true;
			break;
		}
		return false;
	}
	else if (b.get_type() == CL_XPathObject::type_number)
	{
		return compare(a.get_number(), number(b).get_number(), oper);
	}
	else if (b.get_type() == CL_XPathObject::type_boolean)
	{
		return compare(boolean(a).get_boolean(), b.get_boolean(), oper);
	}
	else if (b.get_type() == CL_XPathObject::type_string)
	{
		return compare(a.get_string(), b.get_string(), oper);
	}
	else
	{
		throw CL_XPathException("Unknown operand type in compare operation");
	}
}

CL_XPathToken CL_XPathEvaluator_Impl::read_location_path(
	const CL_StringRef &expression,
	CL_XPathToken cur_token,
	const CL_XPathNodeSet &context,
	CL_XPathNodeSet::size_type context_node_index,
	std::vector<CL_XPathEvaluator_Impl::Operand> &operand_stack) const
{
/*
	[1] LocationPath               ::= RelativeLocationPath | AbsoluteLocationPath	
	[2] AbsoluteLocationPath       ::= '/' RelativeLocationPath? | AbbreviatedAbsoluteLocationPath
	[3] RelativeLocationPath       ::= Step | RelativeLocationPath '/' Step | AbbreviatedRelativeLocationPath
*/

	if (cur_token.type == CL_XPathToken::type_operator && (cur_token.value.oper == CL_XPathToken::operator_slash || cur_token.value.oper == CL_XPathToken::operator_double_slash))
	{
		// Find root node:
		CL_DomNode root_node = context[context_node_index];
		while (true)
		{
			CL_DomNode parent = root_node.get_parent_node();
			if (!parent.is_null())
				root_node = parent;
			else
				break;
		}

		CL_XPathNodeSet nodeset(1, root_node);

		if (cur_token.value.oper == CL_XPathToken::operator_double_slash)
		{
			return read_location_steps(expression, cur_token, nodeset, 0, operand_stack);
		}
		else
		{
			CL_XPathToken next_token = read_token(expression, cur_token);
			if (next_token.type == CL_XPathToken::type_axis_name ||
				next_token.type == CL_XPathToken::type_name_test ||
				next_token.type == CL_XPathToken::type_node_type ||
				next_token.type == CL_XPathToken::type_at_sign ||
				next_token.type == CL_XPathToken::type_dot ||
				next_token.type == CL_XPathToken::type_double_dot ||
				(next_token.type == CL_XPathToken::type_operator && next_token.value.oper == CL_XPathToken::operator_double_slash))
			{
				cur_token = next_token;
				return read_location_steps(expression, cur_token, nodeset, 0, operand_stack);
			}
			else
			{
				operand_stack.push_back(CL_XPathObject(nodeset));
				return cur_token;
			}
		}
	}
	else
	{
		return read_location_steps(expression, cur_token, context, context_node_index, operand_stack);
	}
}

CL_XPathToken CL_XPathEvaluator_Impl::read_location_steps(
	const CL_StringRef &expression,
	CL_XPathToken cur_token,
	const CL_XPathNodeSet &context,
	CL_XPathNodeSet::size_type context_node_index,
	std::vector<CL_XPathEvaluator_Impl::Operand> &operand_stack) const
{
	std::vector<CL_XPathLocationStep> steps;
	while (true)
	{
		CL_XPathLocationStep step;
		cur_token = read_location_step(expression, cur_token, step);
		steps.push_back(step);

		CL_XPathToken next_token = read_token(expression, cur_token);
		if ((next_token.type == CL_XPathToken::type_operator && next_token.value.oper == CL_XPathToken::operator_slash) ||
			(cur_token.type == CL_XPathToken::type_operator && cur_token.value.oper == CL_XPathToken::operator_double_slash))
		{
			if (next_token.value.oper == CL_XPathToken::operator_slash)
				next_token = read_token(expression, next_token);
			if (next_token.type == CL_XPathToken::type_axis_name ||
				next_token.type == CL_XPathToken::type_name_test ||
				next_token.type == CL_XPathToken::type_node_type ||
				next_token.type == CL_XPathToken::type_at_sign ||
				next_token.type == CL_XPathToken::type_dot ||
				next_token.type == CL_XPathToken::type_double_dot)
			{
				cur_token = next_token;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	CL_XPathNodeSet nodeset;
	evaluate_location_step(context, context_node_index, steps, 0, expression, nodeset);
	operand_stack.push_back(CL_XPathObject(nodeset));
	return cur_token;
}

CL_XPathToken CL_XPathEvaluator_Impl::read_location_step(
	const CL_StringRef &expression,
	CL_XPathToken cur_token,
	CL_XPathLocationStep &step) const
{
/*
	Location Steps
	[4]  Step                      ::= AxisSpecifier NodeTest Predicate* | AbbreviatedStep

	[5]  AxisSpecifier             ::= AxisName '::'	| AbbreviatedAxisSpecifier
	[6]  AxisName                  ::= 'ancestor' | 'ancestor-or-self' | 'attribute' | 'child'
	                                   | 'descendant' | 'descendant-or-self' | 'following'
						               | 'following-sibling' | 'namespace' | 'parent'
						               | 'preceding' | 'preceding-sibling' | 'self'	
	[13] AbbreviatedAxisSpecifier  ::= '@'?

	[7]  NodeTest                  ::= NameTest	
	                                   | NodeType '(' ')'	
	                                   | 'processing-instruction' '(' Literal ')'	
	[37] NameTest                  ::= '*' | NCName ':' '*' | QName	
	[38] NodeType                  ::= 'comment' | 'text' | 'processing-instruction' | 'node'

	[8]  Predicate                 ::= '[' PredicateExpr ']'
	[9]  PredicateExpr             ::= Expr

	[12] AbbreviatedStep           ::= '.' | '..'
*/
	if (cur_token.type == CL_XPathToken::type_dot)
	{
		step.axis = "self";
		step.test_type = CL_XPathLocationStep::type_node;
		step.node_type = CL_XPathToken::node_type_node;
	}
	else if (cur_token.type == CL_XPathToken::type_double_dot)
	{
		step.axis = "parent";
		step.test_type = CL_XPathLocationStep::type_node;
		step.node_type = CL_XPathToken::node_type_node;
	}
	else if (cur_token.type == CL_XPathToken::type_operator && cur_token.value.oper == CL_XPathToken::operator_double_slash)
	{
		step.axis = "descendant-or-self";
		step.test_type = CL_XPathLocationStep::type_node;
		step.node_type = CL_XPathToken::node_type_node;
	}
	else
	{
		// Read AxisSpecifier:
		if (cur_token.type == CL_XPathToken::type_axis_name)
		{
			step.axis = cur_token.value.str;
			cur_token = read_token(expression, cur_token);
			if (cur_token.type != CL_XPathToken::type_double_colon)
				throw CL_XPathException("Expected '::' after axis name", expression, cur_token);
			cur_token = read_token(expression, cur_token);
		}
		else if (cur_token.type == CL_XPathToken::type_at_sign) // Abbreviated axis specifier
		{
			step.axis = "attribute";
			cur_token = read_token(expression, cur_token);
		}
		else // Abbreviated syntax
		{
			step.axis = "child";
		}

		// Read Node Test:
		if (cur_token.type == CL_XPathToken::type_name_test)
		{
			step.test_type = CL_XPathLocationStep::type_name;
			step.test_str = cur_token.value.str;
		}
		else if (cur_token.type == CL_XPathToken::type_node_type)
		{
			step.test_type = CL_XPathLocationStep::type_node;
			step.node_type = cur_token.value.node_type;
			cur_token = read_token(expression, cur_token);
			if (cur_token.type != CL_XPathToken::type_operator || cur_token.value.oper != CL_XPathToken::operator_parenthesis_begin)
				throw CL_XPathException("Expected '(' after node-type test", expression, cur_token);
			cur_token = read_token(expression, cur_token);
			if (cur_token.type == CL_XPathToken::type_literal && step.node_type == CL_XPathToken::node_type_processing_instruction)
			{
				step.test_str = cur_token.value.str;
				cur_token = read_token(expression, cur_token);
			}
			if (cur_token.type != CL_XPathToken::type_operator || cur_token.value.oper != CL_XPathToken::operator_parenthesis_end)
				throw CL_XPathException("Expected ')' after node-type test", expression, cur_token);
		}
		else
		{
			throw CL_XPathException("Unknown node test type", expression, cur_token);
		}

		CL_XPathToken next_token = read_token(expression, cur_token);
		while (next_token.type == CL_XPathToken::type_bracket_begin)
		{
			CL_XPathLocationStep::Predicate predicate;
			predicate.pos = next_token.pos + next_token.length;
			cur_token = skip_predicate_expression(expression, next_token);
			predicate.length = cur_token.pos - predicate.pos;
			step.predicates.push_back(predicate);
			next_token = read_token(expression, cur_token);
		}
	}
	return cur_token;
}

CL_XPathToken CL_XPathEvaluator_Impl::skip_predicate_expression(const CL_StringRef &expression, const CL_XPathToken &previous_token) const
{
	int bracket_count = 1;
	CL_XPathToken cur_token = previous_token;
	while (true)
	{
		cur_token = read_token(expression, cur_token);
		if (cur_token.type == CL_XPathToken::type_bracket_begin)
		{
			bracket_count++;
		}
		else if (cur_token.type == CL_XPathToken::type_bracket_end)
		{
			bracket_count--;
			if (bracket_count == 0)
				break;
		}
		else if (cur_token.type == CL_XPathToken::type_none)
		{
			break;
		}
	}
	return cur_token;
}

void CL_XPathEvaluator_Impl::evaluate_location_step(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, CL_XPathNodeSet &nodes) const
{
	if (step_index < steps.size())
	{
		if (steps[step_index].axis == "ancestor")
			select_nodes_ancestor(context, context_node_index, steps, step_index, expression, nodes);
		else if (steps[step_index].axis == "ancestor-or-self")
			select_nodes_ancestor_or_self(context, context_node_index, steps, step_index, expression, nodes);
		else if (steps[step_index].axis == "attribute")
			select_nodes_attribute(context, context_node_index, steps, step_index, expression, nodes);
		else if (steps[step_index].axis == "child")
			select_nodes_child(context, context_node_index, steps, step_index, expression, nodes);
		else if (steps[step_index].axis == "descendant")
			select_nodes_descendant(context, context_node_index, steps, step_index, expression, nodes);
		else if (steps[step_index].axis == "descendant-or-self")
			select_nodes_descendant_or_self(context, context_node_index, steps, step_index, expression, nodes);
		else if (steps[step_index].axis == "following")
			select_nodes_following(context, context_node_index, steps, step_index, expression, nodes);
		else if (steps[step_index].axis == "follwing-sibling")
			select_nodes_following_sibling(context, context_node_index, steps, step_index, expression, nodes);
		else if (steps[step_index].axis == "namespace")
			select_nodes_namespace(context, context_node_index, steps, step_index, expression, nodes);
		else if (steps[step_index].axis == "parent")
			select_nodes_parent(context, context_node_index, steps, step_index, expression, nodes);
		else if (steps[step_index].axis == "preceding")
			select_nodes_preceding(context, context_node_index, steps, step_index, expression, nodes);
		else if (steps[step_index].axis == "preceding-sibling")
			select_nodes_preceding_sibling(context, context_node_index, steps, step_index, expression, nodes);
		else if (steps[step_index].axis == "self")
			select_nodes_self(context, context_node_index, steps, step_index, expression, nodes);
		else
			throw CL_XPathException(cl_format("Unknown location step axis", steps[step_index].axis), expression);
	}
	else
	{
		nodes.push_back(context[context_node_index]);
	}
}

void CL_XPathEvaluator_Impl::select_nodes_ancestor(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, CL_XPathNodeSet &nodes) const
{
	CL_XPathNodeSet nodeset;
	CL_DomNode parent = context[context_node_index].get_parent_node();
	while (!parent.is_null())
	{
		if (confirm_step_requirements(parent, steps[step_index], expression))
			nodeset.push_back(parent);

		parent = parent.get_parent_node();
	}

	evaluate_location_step_predicates(nodeset, steps, step_index, expression, nodes);
}

void CL_XPathEvaluator_Impl::select_nodes_ancestor_or_self(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, CL_XPathNodeSet &nodes) const
{
	CL_XPathNodeSet nodeset;
	CL_DomNode parent = context[context_node_index];
	while (!parent.is_null())
	{
		if (confirm_step_requirements(parent, steps[step_index], expression))
			nodeset.push_back(parent);

		parent = parent.get_parent_node();
	}

	evaluate_location_step_predicates(nodeset, steps, step_index, expression, nodes);
}

void CL_XPathEvaluator_Impl::select_nodes_attribute(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, CL_XPathNodeSet &nodes) const
{
	CL_XPathNodeSet nodeset;
	CL_DomNamedNodeMap attributes = context[context_node_index].get_attributes();
	unsigned long num_attributes = attributes.get_length();
	for (unsigned long idx = 0; idx < num_attributes; idx++)
	{
		const CL_DomNode &node = attributes.item(idx);
		if (confirm_step_requirements(node, steps[step_index], expression))
			nodeset.push_back(node);
	}

	evaluate_location_step_predicates(nodeset, steps, step_index, expression, nodes);
}

void CL_XPathEvaluator_Impl::select_nodes_child(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, CL_XPathNodeSet &nodes) const
{
	CL_XPathNodeSet nodeset;
	CL_DomNode cur_node = context[context_node_index].get_first_child();
	while (!cur_node.is_null())
	{
		if (confirm_step_requirements(cur_node, steps[step_index], expression))
			nodeset.push_back(cur_node);
		cur_node = cur_node.get_next_sibling();
	}

	evaluate_location_step_predicates(nodeset, steps, step_index, expression, nodes);
}

void CL_XPathEvaluator_Impl::select_nodes_descendant(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, CL_XPathNodeSet &nodes) const
{
	CL_XPathNodeSet parentNodes;
	CL_XPathNodeSet nodeset;

	CL_DomNode cur_node = context[context_node_index].get_first_child();
	while (!cur_node.is_null())
	{
		if (confirm_step_requirements(cur_node, steps[step_index], expression))
			nodeset.push_back(cur_node);

		parentNodes.push_back(cur_node);
		cur_node = cur_node.get_first_child();
		while (cur_node.is_null())
		{
			if (parentNodes.empty())
				break;
			cur_node = parentNodes.back();
			parentNodes.pop_back();
			cur_node = cur_node.get_next_sibling();
		}
	}

	evaluate_location_step_predicates(nodeset, steps, step_index, expression, nodes);
}

void CL_XPathEvaluator_Impl::select_nodes_descendant_or_self(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, CL_XPathNodeSet &nodes) const
{
	CL_XPathNodeSet parentNodes;
	CL_XPathNodeSet nodeset;

	CL_DomNode cur_node = context[context_node_index];
	while (!cur_node.is_null())
	{
		if (confirm_step_requirements(cur_node, steps[step_index], expression))
			nodeset.push_back(cur_node);

		parentNodes.push_back(cur_node);
		cur_node = cur_node.get_first_child();
		while (cur_node.is_null())
		{
			if (parentNodes.empty())
				break;
			cur_node = parentNodes.back();
			parentNodes.pop_back();
			cur_node = cur_node.get_next_sibling();
		}
	}

	evaluate_location_step_predicates(nodeset, steps, step_index, expression, nodes);
}

void CL_XPathEvaluator_Impl::select_nodes_following(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, CL_XPathNodeSet &nodes) const
{
	CL_XPathNodeSet nodeset;

	CL_DomNode cur_node;
	CL_DomNode next_node = context[context_node_index];
	while (!next_node.is_null())
	{
		cur_node = next_node;
		next_node = cur_node.get_next_sibling();
		if (next_node.is_null())
		{
			next_node = cur_node.get_parent_node();
		}
		else
		{
			cur_node = next_node;
			if (confirm_step_requirements(cur_node, steps[step_index], expression))
				nodeset.push_back(cur_node);

			next_node = cur_node.get_first_child();
			while (!next_node.is_null())
			{
				cur_node = next_node;
				if (confirm_step_requirements(cur_node, steps[step_index], expression))
					nodeset.push_back(cur_node);

				next_node = cur_node.get_first_child();
			}
			next_node = cur_node.get_parent_node();
		}
	}

	evaluate_location_step_predicates(nodeset, steps, step_index, expression, nodes);
}

void CL_XPathEvaluator_Impl::select_nodes_following_sibling(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, CL_XPathNodeSet &nodes) const
{
	CL_XPathNodeSet nodeset;
	CL_DomNode cur_node = context[context_node_index].get_next_sibling();
	while (!cur_node.is_null())
	{
		if (confirm_step_requirements(cur_node, steps[step_index], expression))
			nodeset.push_back(cur_node);
		cur_node = cur_node.get_next_sibling();
	}

	evaluate_location_step_predicates(nodeset, steps, step_index, expression, nodes);
}

void CL_XPathEvaluator_Impl::select_nodes_namespace(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, CL_XPathNodeSet &nodes) const
{
}

void CL_XPathEvaluator_Impl::select_nodes_parent(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, CL_XPathNodeSet &nodes) const
{
	CL_XPathNodeSet nodeset;
	CL_DomNode parent = context[context_node_index].get_parent_node();
	if (!parent.is_null())
	{
		if (confirm_step_requirements(parent, steps[step_index], expression))
			nodeset.push_back(parent);
	}

	evaluate_location_step_predicates(nodeset, steps, step_index, expression, nodes);
}

void CL_XPathEvaluator_Impl::select_nodes_preceding(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, CL_XPathNodeSet &nodes) const
{
	CL_XPathNodeSet nodeset;

	CL_DomNode cur_node;
	CL_DomNode next_node = context[context_node_index];
	while (!next_node.is_null())
	{
		cur_node = next_node;
		next_node = cur_node.get_previous_sibling();
		if (next_node.is_null())
		{
			next_node = cur_node.get_parent_node();
		}
		else
		{
			cur_node = next_node;
			if (confirm_step_requirements(cur_node, steps[step_index], expression))
				nodeset.push_back(cur_node);

			next_node = cur_node.get_last_child();
			while (!next_node.is_null())
			{
				cur_node = next_node;
				if (confirm_step_requirements(cur_node, steps[step_index], expression))
					nodeset.push_back(cur_node);

				next_node = cur_node.get_last_child();
			}
			next_node = cur_node.get_parent_node();
		}
	}

	evaluate_location_step_predicates(nodeset, steps, step_index, expression, nodes);
}

void CL_XPathEvaluator_Impl::select_nodes_preceding_sibling(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, CL_XPathNodeSet &nodes) const
{
	CL_XPathNodeSet nodeset;
	CL_DomNode cur_node = context[context_node_index].get_previous_sibling();
	while (!cur_node.is_null())
	{
		if (confirm_step_requirements(cur_node, steps[step_index], expression))
			nodeset.push_back(cur_node);
		cur_node = cur_node.get_previous_sibling();
	}

	evaluate_location_step_predicates(nodeset, steps, step_index, expression, nodes);
}

void CL_XPathEvaluator_Impl::select_nodes_self(const CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, CL_XPathNodeSet &nodes) const
{
	CL_DomNode cur_node = context[context_node_index];
	if (!cur_node.is_null())
	{
		if (confirm_step_requirements(cur_node, steps[step_index], expression))
			evaluate_location_step_predicates(CL_XPathNodeSet(1, cur_node), steps, step_index, expression, nodes);
	}
}

bool CL_XPathEvaluator_Impl::confirm_step_requirements(const CL_DomNode &node, const CL_XPathLocationStep &step, const CL_StringRef &expression) const
{
	bool test_passed = false;
	switch (step.test_type)
	{
	default:
	case CL_XPathLocationStep::type_none:
		test_passed = true;
		break;
	case CL_XPathLocationStep::type_name:
		test_passed = step.test_str == "*" || node.get_node_name() == step.test_str;
		break;
	case CL_XPathLocationStep::type_node:
		if (step.node_type != CL_XPathToken::node_type_node)
		{
			switch (node.get_node_type())
			{
			case CL_DomNode::COMMENT_NODE:
				test_passed = step.node_type == CL_XPathToken::node_type_comment;
				break;
			case CL_DomNode::TEXT_NODE:
				test_passed = step.node_type == CL_XPathToken::node_type_text;
				break;
			case CL_DomNode::PROCESSING_INSTRUCTION_NODE:
				test_passed = step.node_type == CL_XPathToken::node_type_processing_instruction;
				break;
			default:
				break;
			}
		}
		else
		{
			test_passed = true;
		}
	}

	return test_passed;
}

bool CL_XPathEvaluator_Impl::confirm_step_predicate(CL_XPathNodeSet &context, CL_XPathNodeSet::size_type context_node_index, const CL_XPathLocationStep::Predicate &predicate, const CL_StringRef &expression) const
{
	CL_XPathEvaluateResult result = evaluate(expression.substr(predicate.pos, predicate.length), context, context_node_index, CL_XPathToken());
	bool include_in_nodeset = false;
	switch (result.result.get_type())
	{
	case CL_XPathObject::type_null:
		break;
	case CL_XPathObject::type_node_set:
		include_in_nodeset = !result.result.get_node_set().empty();
		break;
	case CL_XPathObject::type_boolean:
		include_in_nodeset = result.result.get_boolean();
		break;
	case CL_XPathObject::type_number:
		include_in_nodeset = result.result.get_number() == context_node_index+1;
		break;
	case CL_XPathObject::type_string:
		include_in_nodeset = !result.result.get_string().empty();
		break;
	}
	return include_in_nodeset;
}

void CL_XPathEvaluator_Impl::evaluate_location_step_predicates(const CL_XPathNodeSet &context, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, CL_XPathNodeSet &nodes) const
{
	CL_XPathNodeSet nodeset = context;
	for (std::vector<CL_XPathLocationStep::Predicate>::const_iterator pit = steps[step_index].predicates.begin(), pEnd = steps[step_index].predicates.end(); pit != pEnd; ++pit)
	{
		CL_XPathNodeSet filtered_nodes;
		for (CL_XPathNodeSet::size_type node_index = 0, num_nodes = nodeset.size(); node_index < num_nodes; node_index++)
		{
			if (confirm_step_predicate(nodeset, node_index, *pit, expression))
				filtered_nodes.push_back(nodeset[node_index]);
		}
		nodeset = filtered_nodes;
	}

	for (CL_XPathNodeSet::size_type node_index = 0, num_nodes = nodeset.size(); node_index < num_nodes; node_index++)
		evaluate_location_step(nodeset, node_index, steps, step_index+1, expression, nodes);
}

CL_XPathToken CL_XPathEvaluator_Impl::read_token(
	const CL_StringRef &expression,
	const CL_XPathToken &previous_token) const
{
	CL_StringRef::size_type pos = previous_token.pos + previous_token.length;
	pos = expression.find_first_not_of(" \t\r\n", pos);
	if (pos == CL_StringRef::npos || expression.length() == pos)
	{
		CL_XPathToken token;
		token.pos = pos;
		token.length = 0;
		return token;
	}

	CL_XPathToken token;

	CL_StringRef::size_type available_data = expression.length() - pos;
	CL_StringRef::char_type first_char = expression[pos];
	CL_StringRef::char_type second_char = (available_data > 1) ? expression[pos + 1] : 0;

	token.pos = pos;
	token.length = 1;
	switch (first_char)
	{
	case '(':
		token.type = CL_XPathToken::type_operator;
		token.value.oper = CL_XPathToken::operator_parenthesis_begin;
		return token;
	case ')':
		token.type = CL_XPathToken::type_operator;
		token.value.oper = CL_XPathToken::operator_parenthesis_end;
		return token;
	case '[':
		token.type = CL_XPathToken::type_bracket_begin;
		return token;
	case ']':
		token.type = CL_XPathToken::type_bracket_end;
		return token;
	case '.':
		if (second_char == '.')
		{
			token.type = CL_XPathToken::type_double_dot;
			token.length = 2;
			return token;
		}
		else if (!is_digit(second_char))
		{
			token.type = CL_XPathToken::type_dot;
			return token;
		}
		else
		{
			token.type = CL_XPathToken::type_number;
			for (pos = token.pos + 1; pos < expression.length(); pos++)
			{
				if (!is_digit(expression[pos]))
					break;
			}
			token.length = pos - token.pos;
			token.value.str = expression.substr(token.pos, token.length);
			return token;
		}
	case '@':
		token.type = CL_XPathToken::type_at_sign;
		return token;
	case ',':
		token.type = CL_XPathToken::type_comma;
		return token;
	case ':':
		if (second_char == ':')
		{
			token.type = CL_XPathToken::type_double_colon;
			token.length = 2;
			return token;
		}
		break;
	case '/':
		token.type = CL_XPathToken::type_operator;
		if (second_char == '/')
		{
			token.value.oper = CL_XPathToken::operator_double_slash;
			token.length = 2;
		}
		else
		{
			token.value.oper = CL_XPathToken::operator_slash;
		}
		return token;
	case '|':
		token.type = CL_XPathToken::type_operator;
		token.value.oper = CL_XPathToken::operator_union;
		return token;
	case '+':
		token.type = CL_XPathToken::type_operator;
		token.value.oper = CL_XPathToken::operator_plus;
		return token;
	case '-':
		token.type = CL_XPathToken::type_operator;
		token.value.oper = CL_XPathToken::operator_minus;
		return token;
	case '=':
		token.type = CL_XPathToken::type_operator;
		token.value.oper = CL_XPathToken::operator_compare_equal;
		return token;
	case '!':
		token.type = CL_XPathToken::type_operator;
		if (second_char == '=')
		{
			token.value.oper = CL_XPathToken::operator_compare_not_equal;
			token.length = 2;
			return token;
		}
		break;
	case '<':
		token.type = CL_XPathToken::type_operator;
		if (second_char == '=')
		{
			token.value.oper = CL_XPathToken::operator_less_equal;
			token.length = 2;
		}
		else
		{
			token.value.oper = CL_XPathToken::operator_less;
		}
		return token;
	case '>':
		token.type = CL_XPathToken::type_operator;
		if (second_char == '=')
		{
			token.value.oper = CL_XPathToken::operator_greater_equal;
			token.length = 2;
		}
		else
		{
			token.value.oper = CL_XPathToken::operator_greater;
		}
		return token;
	case '*':
		// If there is a preceding token and the preceding token is not one
		// of @, ::, (, [, , or an Operator, then a * must be recognized as
		// a MultiplyOperator, otherwise it is a name test.
		if (
			previous_token.type == CL_XPathToken::type_none ||
			previous_token.type == CL_XPathToken::type_at_sign ||
			previous_token.type == CL_XPathToken::type_double_colon ||
			(previous_token.type == CL_XPathToken::type_operator &&
			previous_token.value.oper == CL_XPathToken::operator_parenthesis_begin) ||
			previous_token.type == CL_XPathToken::type_bracket_begin ||
			previous_token.type == CL_XPathToken::type_comma ||
			previous_token.type == CL_XPathToken::type_operator)
		{
			token.type = CL_XPathToken::type_name_test;
			token.value.str = "*";
		}
		else
		{
			token.type = CL_XPathToken::type_operator;
			token.value.oper = CL_XPathToken::operator_multiply;
		}
		return token;
	case '"':
		token.type = CL_XPathToken::type_literal;
		pos = expression.find_first_of('"', token.pos + 1);
		if (pos == CL_StringRef::npos)
			throw CL_XPathException("Expected ending quotation sign", expression, token);
		token.length = pos - token.pos + 1;
		token.value.str = expression.substr(token.pos + 1, token.length - 2);
		return token;
	case '\'':
		token.type = CL_XPathToken::type_literal;
		pos = expression.find_first_of('\'', token.pos + 1);
		if (pos == CL_StringRef::npos)
			throw CL_XPathException("Expected ending quotation sign", expression, token);
		token.length = pos - token.pos + 1;
		token.value.str = expression.substr(token.pos + 1, token.length - 2);
		return token;
	case '$':
		token.type = CL_XPathToken::type_variable_reference;
		for (pos = token.pos + 1; pos < expression.length(); pos++)
		{
			if (!is_letter(expression[pos]) &&
				!is_digit(expression[pos]) &&
				expression[pos] != '.' &&
				expression[pos] != '-' &&
				expression[pos] != '_' &&
				!is_combining_char(expression[pos]) &&
				!is_extender(expression[pos]) &&
				expression[pos] != ':')
				break;
		}
		token.length = pos - token.pos;
		token.value.str = expression.substr(token.pos + 1, token.length - 1);
		break;
	}

	if (is_digit(first_char))
	{
		token.type = CL_XPathToken::type_number;
		for (pos = token.pos + 1; pos < expression.length(); pos++)
		{
			if (!is_digit(expression[pos]) &&
				expression[pos] != '.')
				break;
		}
		token.length = pos - token.pos;
		token.value.str = expression.substr(token.pos, token.length);
		return token;
	}

	// Find NCName:

	for (pos = token.pos; pos < expression.length(); pos++)
	{
		if (!is_letter(expression[pos]) &&
			!is_digit(expression[pos]) &&
			expression[pos] != '.' &&
			expression[pos] != '-' &&
			expression[pos] != '_' &&
			!is_combining_char(expression[pos]) &&
			!is_extender(expression[pos]))
			break;
	}
	token.length = pos - token.pos;
	token.value.str = expression.substr(token.pos, token.length);

	if (token.length == 0)
		throw CL_XPathException("Invalid token", expression, token);

	// Find the next two token chars:

	CL_StringRef::size_type next_pos = expression.find_first_not_of(" \t\r\n", pos);
	if (next_pos == CL_StringRef::npos)
		next_pos = expression.length();
	CL_StringRef::size_type next_available_data = expression.length() - next_pos;
	CL_StringRef::char_type next_char1 = next_available_data > 0 ? expression[next_pos] : 0;
	CL_StringRef::char_type next_char2 = next_available_data > 1 ? expression[next_pos + 1] : 0;

	// If there is a preceding token and the preceding token is not one
	// of @, ::, (, [, , or an Operator, then an NCName must be recognized
	// as an OperatorName.
	if (
		previous_token.type != CL_XPathToken::type_none &&
		previous_token.type != CL_XPathToken::type_at_sign &&
		previous_token.type != CL_XPathToken::type_double_colon &&
		previous_token.type != CL_XPathToken::type_bracket_begin &&
		previous_token.type != CL_XPathToken::type_comma &&
		!(previous_token.type == CL_XPathToken::type_operator && previous_token.value.oper != CL_XPathToken::operator_parenthesis_end))
	{
		// Check for OperatorName:

		if (token.value.str == "and")
		{
			token.type = CL_XPathToken::type_operator;
			token.value.oper = CL_XPathToken::operator_and;
			return token;
		}
		else if (token.value.str == "or")
		{
			token.type = CL_XPathToken::type_operator;
			token.value.oper = CL_XPathToken::operator_or;
			return token;
		}
		else if (token.value.str == "mod")
		{
			token.type = CL_XPathToken::type_operator;
			token.value.oper = CL_XPathToken::operator_mod;
			return token;
		}
		else if (token.value.str == "div")
		{
			token.type = CL_XPathToken::type_operator;
			token.value.oper = CL_XPathToken::operator_div;
			return token;
		}
	}

	// If the character following an NCName (possibly after intervening ExprWhitespace)
	// is (, then the token must be recognized as a NodeType or a FunctionName.
	if (next_char1 == '(')
	{
		// Check for NodeType:
		if (token.value.str == "comment")
		{
			token.type = CL_XPathToken::type_node_type;
			token.value.node_type = CL_XPathToken::node_type_comment;
			return token;
		}
		else if (token.value.str == "text")
		{
			token.type = CL_XPathToken::type_node_type;
			token.value.node_type = CL_XPathToken::node_type_text;
			return token;
		}
		else if (token.value.str == "processing-instruction")
		{
			token.type = CL_XPathToken::type_node_type;
			token.value.node_type = CL_XPathToken::node_type_processing_instruction;
			return token;
		}
		else if (token.value.str == "node")
		{
			token.type = CL_XPathToken::type_node_type;
			token.value.node_type = CL_XPathToken::node_type_node;
			return token;
		}

		token.type = CL_XPathToken::type_function_name;
		return token;
	}

	// If the two characters following an NCName (possibly after intervening ExprWhitespace)
	// are ::, then the token must be recognized as an AxisName.
	if (next_char1 == ':' && next_char2 == ':')
	{
		token.type = CL_XPathToken::type_axis_name;
		return token;
	}

	// Find NameTest:

	if (next_pos == pos && next_char1 == ':' && next_char2 == '*')
	{
		token.length += 2;
	}
	else if (next_pos == pos && next_char1 == ':')
	{
		for (pos = token.pos + token.length+1; pos < expression.length(); pos++)
		{
			if (!is_letter(expression[pos]) &&
				!is_digit(expression[pos]) &&
				expression[pos] != '.' &&
				expression[pos] != '-' &&
				expression[pos] != '_' &&
				!is_combining_char(expression[pos]) &&
				!is_extender(expression[pos]))
				break;
		}
		token.length = pos - token.pos;
		token.value.str = expression.substr(token.pos, token.length);
		if (token.length == 0)
			throw CL_XPathException("Invalid token", expression, token);
	}

	token.type = CL_XPathToken::type_name_test;
	return token;
}

CL_XPathObject CL_XPathEvaluator_Impl::function_last(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	return CL_XPathObject(context.size());
}

CL_XPathObject CL_XPathEvaluator_Impl::function_position(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	return CL_XPathObject(context_node_index+1);
}

CL_XPathObject CL_XPathEvaluator_Impl::function_count(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	if (parameters.size() != 1 || parameters.front().get_type() != CL_XPathObject::type_node_set)
		throw CL_XPathException("Function count(node-set) expects a node-set parameter");

	CL_XPathObject obj = parameters.front();
	return CL_XPathObject(obj.get_node_set().size());
}

CL_XPathObject CL_XPathEvaluator_Impl::function_id(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	if (parameters.empty())
		throw CL_XPathException("Function id(object) expects an object parameter");

	CL_XPathObject obj = parameters[0];
	std::vector<CL_String> strings;
	if (obj.get_type() == CL_XPathObject::type_node_set)
	{
		CL_XPathNodeSet nodeset = obj.get_node_set();
		for (CL_XPathNodeSet::const_iterator it = nodeset.begin(), itEnd = nodeset.end(); it != itEnd; ++it)
		{
			CL_String str = CL_StringHelp::trim(string(*it));
			CL_String::size_type pos = str.find_first_not_of(" \r\n\t", 0);
			CL_String::size_type end_pos = CL_String::npos;
			while (pos != CL_String::npos)
			{
				end_pos = str.find_first_of(" \r\n\t", pos);
				strings.push_back(str.substr(pos, end_pos - pos));
				pos = str.find_first_not_of(" \r\n\t", end_pos);
			}
		}
	}
	else
	{
		CL_String str = string(obj).get_string();
		CL_String::size_type pos = str.find_first_not_of(" \r\n\t", 0);
		CL_String::size_type end_pos = CL_String::npos;
		while (pos != CL_String::npos)
		{
			end_pos = str.find_first_of(" \r\n\t", pos);
			strings.push_back(str.substr(pos, end_pos - pos));
			pos = str.find_first_not_of(" \r\n\t", end_pos);
		}
	}

	CL_DomNode root_node = context[context_node_index];
	while (!root_node.get_parent_node().is_null())
		root_node = root_node.get_parent_node();

	CL_XPathNodeSet filtered_nodes;
	for (std::vector<CL_String>::const_iterator it = strings.begin(), itEnd = strings.end(); it != itEnd; ++it)
	{
		CL_XPathNodeSet parent_nodes;
		CL_DomNode cur_node = root_node;
		while (!cur_node.is_null())
		{
			if (cur_node.has_attributes())
			{
				CL_DomNamedNodeMap attributes = cur_node.get_attributes();
				if (attributes.get_named_item("ID").get_node_value() == *it)
				{
					filtered_nodes.push_back(cur_node);
					break;
				}
			}
			parent_nodes.push_back(cur_node);
			cur_node = cur_node.get_first_child();
			while (cur_node.is_null())
			{
				if (parent_nodes.empty())
					break;
				cur_node = parent_nodes.back();
				parent_nodes.pop_back();
				cur_node = cur_node.get_next_sibling();
			}
		}
	}

	return CL_XPathObject(filtered_nodes);
}

CL_XPathObject CL_XPathEvaluator_Impl::function_local_name(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	CL_XPathNodeSet nodeset;
	if (!parameters.empty())
	{
		CL_XPathObject obj = parameters.front();
		if (obj.get_type() != CL_XPathObject::type_node_set)
			throw CL_XPathException("Function local-name(node-set) expects a node-set parameter");
		CL_XPathNodeSet nodeset2 = obj.get_node_set();
		nodeset.insert(nodeset.end(), nodeset2.begin(), nodeset2.end());
	}
	else
	{
		nodeset.push_back(context[context_node_index]);
	}

	if (nodeset.empty())
		return CL_XPathObject("");
	else
		return CL_XPathObject(nodeset.front().get_local_name());
}

CL_XPathObject CL_XPathEvaluator_Impl::function_namespace_uri(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	CL_XPathNodeSet nodeset;
	if (!parameters.empty())
	{
		CL_XPathObject obj = parameters.front();
		if (obj.get_type() != CL_XPathObject::type_node_set)
			throw CL_XPathException("Function namespace-uri(node-set) expects a node-set parameter");
		CL_XPathNodeSet nodeset2 = obj.get_node_set();
		nodeset.insert(nodeset.end(), nodeset2.begin(), nodeset2.end());
	}
	else
	{
		nodeset.push_back(context[context_node_index]);
	}

	if (nodeset.empty())
		return CL_XPathObject("");
	else
		return CL_XPathObject(nodeset.front().get_namespace_uri());
}

CL_XPathObject CL_XPathEvaluator_Impl::function_name(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	CL_XPathNodeSet nodeset;
	if (!parameters.empty())
	{
		CL_XPathObject obj = parameters[0];
		if (obj.get_type() != CL_XPathObject::type_node_set)
			throw CL_XPathException("Function name(node-set) expects a node-set parameter");
		CL_XPathNodeSet nodeset2 = obj.get_node_set();
		nodeset.insert(nodeset.end(), nodeset2.begin(), nodeset2.end());
	}
	else
	{
		nodeset.push_back(context[context_node_index]);
	}

	if (nodeset.empty())
		return CL_XPathObject("");
	else
		return CL_XPathObject(nodeset.front().get_node_name());
}

CL_XPathObject CL_XPathEvaluator_Impl::function_string(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	CL_XPathObject obj;
	if (parameters.empty())
	{
		CL_XPathNodeSet nodeset(1, context[context_node_index]);
		obj = CL_XPathObject(nodeset);
	}
	else
	{
		obj = parameters.front();
	}

	return string(obj);
}

CL_XPathObject CL_XPathEvaluator_Impl::function_concat(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	if (parameters.size() < 2)
		throw CL_XPathException("Function concat(string, string, string*) expects 2 or more string parameters");

	CL_String result;
	for (std::vector<CL_XPathObject>::const_iterator it = parameters.begin(), itEnd = parameters.end(); it != itEnd; ++it)
	{
		CL_XPathObject obj = *it;
		if (obj.get_type() != CL_XPathObject::type_string)
			obj = string(obj);
		result.append(obj.get_string());
	}
	return CL_XPathObject(result);
}

CL_XPathObject CL_XPathEvaluator_Impl::function_starts_with(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	if (parameters.size() != 2)
		throw CL_XPathException("Function starts-with(string, string) expects 2 string parameters");

	CL_XPathObject p1 = string(parameters[0]);
	CL_XPathObject p2 = string(parameters[1]);
	return CL_XPathObject(p1.get_string().find(p2.get_string()) == 0);
}

CL_XPathObject CL_XPathEvaluator_Impl::function_contains(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	if (parameters.size() != 2)
		throw CL_XPathException("Function contains(string, string) expects 2 string parameters");

	CL_XPathObject p1 = string(parameters[0]);
	CL_XPathObject p2 = string(parameters[1]);
	return CL_XPathObject(p1.get_string().find(p2.get_string()) != CL_String::npos);
}

CL_XPathObject CL_XPathEvaluator_Impl::function_substring_before(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	if (parameters.size() != 2)
		throw CL_XPathException("Function substring-before(string, string) expects 2 string parameters");

	CL_XPathObject p1 = string(parameters[0]);
	CL_XPathObject p2 = string(parameters[1]);

	CL_String::size_type pos = p1.get_string().find_first_of(p2.get_string());
	if (pos != CL_String::npos)
		return CL_XPathObject(p1.get_string().substr(0, pos));
	else
		return CL_XPathObject("");
}

CL_XPathObject CL_XPathEvaluator_Impl::function_substring_after(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	if (parameters.size() != 2)
		throw CL_XPathException("Function substring_after(string, string) expects 2 string parameters");

	CL_String p1 = string(parameters[0]).get_string();
	CL_String p2 = string(parameters[1]).get_string();

	CL_String::size_type pos = p1.find_first_of(p2);
	if (pos != CL_String::npos)
		return CL_XPathObject(p1.substr(pos+1));
	else
		return CL_XPathObject("");
}

CL_XPathObject CL_XPathEvaluator_Impl::function_substring(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	if (parameters.size() < 2)
		throw CL_XPathException("Function substring(string, number, number?) expects 2 or 3 parameters");

	CL_String str = string(parameters[0]).get_string();
	CL_String::size_type pos = cl_max(0, static_cast<CL_String::size_type>(number(parameters[1]).get_number()+0.5)-1);
	CL_String::size_type num = CL_String::npos;
	if (parameters.size() == 3)
		num = cl_max(0, static_cast<CL_String::size_type>(number(parameters[2]).get_number() + 0.5));

	return CL_XPathObject(str.substr(pos, num));
}

CL_XPathObject CL_XPathEvaluator_Impl::function_string_length(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	CL_String str;
	if (parameters.empty())
		str = string(context[context_node_index]);
	else
		str = string(parameters.front()).get_string();

	return CL_XPathObject( (size_t) str.length());
}

CL_XPathObject CL_XPathEvaluator_Impl::function_normalize_space(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	CL_String src, dst;
	if (parameters.empty())
		src = CL_StringHelp::trim(string(context[context_node_index]));
	else
		src = CL_StringHelp::trim(string(parameters.front()).get_string());

	CL_String::size_type pos = 0, end_pos = 0;
	while (pos != CL_String::npos)
	{
		end_pos = src.find_first_of(" \r\n\t", pos);
		if (end_pos != CL_String::npos)
			dst.append(src.substr(pos, end_pos - pos) + " ");
		else
			break;
		pos = src.find_first_not_of(" \r\n\t", end_pos);
	}
	dst.append(src.substr(pos));

	return CL_XPathObject(dst);
}

CL_XPathObject CL_XPathEvaluator_Impl::function_translate(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	if (parameters.size() != 3)
		throw CL_XPathException("Function translate(string, string, string) expects 3 string parameters");

	CL_String str = string(parameters[0]).get_string();
	CL_String search = string(parameters[1]).get_string();
	CL_String replace = string(parameters[2]).get_string();

	CL_String::size_type num_searches = search.length();
	for (CL_String::size_type search_idx = 0; search_idx < num_searches; ++search_idx)
	{
		CL_String::size_type real_search_idx = search.find_first_of(search[search_idx]);
		CL_String::char_type c = search[real_search_idx];

		for (CL_String::size_type str_idx = 0; str_idx < str.length(); str_idx++)
		{
			if (str[str_idx] == c) 
			{
				if(real_search_idx < replace.length())
				{
					str[str_idx] = replace[real_search_idx];
				}
				else
				{
					str = str.erase(str_idx--, 1);
				}
			}
		}
	}
	return CL_XPathObject(str);
}

CL_XPathObject CL_XPathEvaluator_Impl::function_boolean(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	if (parameters.empty())
		throw CL_XPathException("Function boolean(object) expects an object parameter");

	return boolean(parameters[0]);
}

CL_XPathObject CL_XPathEvaluator_Impl::function_not(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	if (parameters.empty())
		throw CL_XPathException("Function not(boolean) expects a boolean parameter");

	return CL_XPathObject(!boolean(parameters[0]).get_boolean());
}

CL_XPathObject CL_XPathEvaluator_Impl::function_true(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	return CL_XPathObject(true);
}

CL_XPathObject CL_XPathEvaluator_Impl::function_false(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	return CL_XPathObject(false);
}

CL_XPathObject CL_XPathEvaluator_Impl::function_lang(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	if (parameters.empty())
		throw CL_XPathException("Function lang(string) expects a string parameter");

	bool found = false;
	CL_String search = string(parameters[0]).get_string();
	CL_DomNode cur_node = context[context_node_index];
	while (!found && !cur_node.is_null())
	{
		if (cur_node.has_attributes())
		{
			CL_DomNode lang_attribute = cur_node.get_attributes().get_named_item("xml:lang");
			if (!lang_attribute.is_null())
			{
				CL_String lang = lang_attribute.get_node_value();
				if (CL_StringHelp::compare(search, lang, true) == 0 || CL_StringHelp::compare(search, lang.substr(0, lang.find_first_of('-')), true) == 0)
					found = true;
			}
		}
		cur_node = cur_node.get_parent_node();
	}

	return found;
}

CL_XPathObject CL_XPathEvaluator_Impl::function_number(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	if (parameters.empty())
		throw CL_XPathException("Function number(object) expects 1 object parameter");

	return number(parameters[0]);
}

CL_XPathObject CL_XPathEvaluator_Impl::function_sum(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	if (parameters.empty() || parameters[0].get_type() != CL_XPathObject::type_node_set)
		throw CL_XPathException("Function sum(node-set) expects a node-set parameter");

	double result = 0.0;
	CL_XPathNodeSet nodeset = parameters[0].get_node_set();
	for (CL_XPathNodeSet::const_iterator it = nodeset.begin(), itEnd = nodeset.end(); it != itEnd; ++it)
		result += number(*it);

	return CL_XPathObject(result);
}

CL_XPathObject CL_XPathEvaluator_Impl::function_floor(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	if (parameters.empty())
		throw CL_XPathException("Function floor(number) expects a number parameter");

	return CL_XPathObject(floor(number(parameters[0]).get_number()));
}

CL_XPathObject CL_XPathEvaluator_Impl::function_ceiling(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	if (parameters.empty())
		throw CL_XPathException("Function ceiling(number) expects a number parameter.");

	return CL_XPathObject(ceil(number(parameters[0]).get_number()));
}

CL_XPathObject CL_XPathEvaluator_Impl::function_round(const CL_XPathNodeSet& context, CL_XPathNodeSet::size_type context_node_index, const std::vector<CL_XPathObject> &parameters) const
{
	if (parameters.empty())
		throw CL_XPathException("Function round(number) expects a number parameter.");

	return CL_XPathObject(floor(number(parameters[0]).get_number()+0.5));
}

inline bool CL_XPathEvaluator_Impl::is_letter(const CL_StringRef::char_type &c)
{
/*
	// the letter check from hell!  I wonder if the compiler will eat it :)
	if (
		// BaseChar:
		(c >= 0x0041 && c <= 0x005A) ||
		(c >= 0x0061 && c <= 0x007A) ||
		(c >= 0x00C0 && c <= 0x00D6) ||
		(c >= 0x00D8 && c <= 0x00F6) ||
		(c >= 0x00F8 && c <= 0x00FF) ||
		(c >= 0x0100 && c <= 0x0131) ||
		(c >= 0x0134 && c <= 0x013E) ||
		(c >= 0x0141 && c <= 0x0148) ||
		(c >= 0x014A && c <= 0x017E) ||
		(c >= 0x0180 && c <= 0x01C3) ||
		(c >= 0x01CD && c <= 0x01F0) ||
		(c >= 0x01F4 && c <= 0x01F5) ||
		(c >= 0x01FA && c <= 0x0217) ||
		(c >= 0x0250 && c <= 0x02A8) ||
		(c >= 0x02BB && c <= 0x02C1) ||
		c == 0x0386 ||
		(c >= 0x0388 && c <= 0x038A) ||
		c == 0x038C ||
		(c >= 0x038E && c <= 0x03A1) ||
		(c >= 0x03A3 && c <= 0x03CE) ||
		(c >= 0x03D0 && c <= 0x03D6) ||
		c == 0x03DA ||
		c == 0x03DC ||
		c == 0x03DE ||
		c == 0x03E0 ||
		(c >= 0x03E2 && c <= 0x03F3) ||
		(c >= 0x0401 && c <= 0x040C) ||
		(c >= 0x040E && c <= 0x044F) ||
		(c >= 0x0451 && c <= 0x045C) ||
		(c >= 0x045E && c <= 0x0481) ||
		(c >= 0x0490 && c <= 0x04C4) ||
		(c >= 0x04C7 && c <= 0x04C8) ||
		(c >= 0x04CB && c <= 0x04CC) ||
		(c >= 0x04D0 && c <= 0x04EB) ||
		(c >= 0x04EE && c <= 0x04F5) ||
		(c >= 0x04F8 && c <= 0x04F9) ||
		(c >= 0x0531 && c <= 0x0556) ||
		c == 0x0559 ||
		(c >= 0x0561 && c <= 0x0586) ||
		(c >= 0x05D0 && c <= 0x05EA) ||
		(c >= 0x05F0 && c <= 0x05F2) ||
		(c >= 0x0621 && c <= 0x063A) ||
		(c >= 0x0641 && c <= 0x064A) ||
		(c >= 0x0671 && c <= 0x06B7) ||
		(c >= 0x06BA && c <= 0x06BE) ||
		(c >= 0x06C0 && c <= 0x06CE) ||
		(c >= 0x06D0 && c <= 0x06D3) ||
		c == 0x06D5 ||
		(c >= 0x06E5 && c <= 0x06E6) ||
		(c >= 0x0905 && c <= 0x0939) ||
		c == 0x093D ||
		(c >= 0x0958 && c <= 0x0961) ||
		(c >= 0x0985 && c <= 0x098C) ||
		(c >= 0x098F && c <= 0x0990) ||
		(c >= 0x0993 && c <= 0x09A8) ||
		(c >= 0x09AA && c <= 0x09B0) ||
		c == 0x09B2 ||
		(c >= 0x09B6 && c <= 0x09B9) ||
		(c >= 0x09DC && c <= 0x09DD) ||
		(c >= 0x09DF && c <= 0x09E1) ||
		(c >= 0x09F0 && c <= 0x09F1) ||
		(c >= 0x0A05 && c <= 0x0A0A) ||
		(c >= 0x0A0F && c <= 0x0A10) ||
		(c >= 0x0A13 && c <= 0x0A28) ||
		(c >= 0x0A2A && c <= 0x0A30) ||
		(c >= 0x0A32 && c <= 0x0A33) ||
		(c >= 0x0A35 && c <= 0x0A36) ||
		(c >= 0x0A38 && c <= 0x0A39) ||
		(c >= 0x0A59 && c <= 0x0A5C) ||
		c == 0x0A5E ||
		(c >= 0x0A72 && c <= 0x0A74) ||
		(c >= 0x0A85 && c <= 0x0A8B) ||
		c == 0x0A8D ||
		(c >= 0x0A8F && c <= 0x0A91) ||
		(c >= 0x0A93 && c <= 0x0AA8) ||
		(c >= 0x0AAA && c <= 0x0AB0) ||
		(c >= 0x0AB2 && c <= 0x0AB3) ||
		(c >= 0x0AB5 && c <= 0x0AB9) ||
		c == 0x0ABD ||
		c == 0x0AE0 ||
		(c >= 0x0B05 && c <= 0x0B0C) ||
		(c >= 0x0B0F && c <= 0x0B10) ||
		(c >= 0x0B13 && c <= 0x0B28) ||
		(c >= 0x0B2A && c <= 0x0B30) ||
		(c >= 0x0B32 && c <= 0x0B33) ||
		(c >= 0x0B36 && c <= 0x0B39) ||
		c == 0x0B3D ||
		(c >= 0x0B5C && c <= 0x0B5D) ||
		(c >= 0x0B5F && c <= 0x0B61) ||
		(c >= 0x0B85 && c <= 0x0B8A) ||
		(c >= 0x0B8E && c <= 0x0B90) ||
		(c >= 0x0B92 && c <= 0x0B95) ||
		(c >= 0x0B99 && c <= 0x0B9A) ||
		c == 0x0B9C ||
		(c >= 0x0B9E && c <= 0x0B9F) ||
		(c >= 0x0BA3 && c <= 0x0BA4) ||
		(c >= 0x0BA8 && c <= 0x0BAA) ||
		(c >= 0x0BAE && c <= 0x0BB5) ||
		(c >= 0x0BB7 && c <= 0x0BB9) ||
		(c >= 0x0C05 && c <= 0x0C0C) ||
		(c >= 0x0C0E && c <= 0x0C10) ||
		(c >= 0x0C12 && c <= 0x0C28) ||
		(c >= 0x0C2A && c <= 0x0C33) ||
		(c >= 0x0C35 && c <= 0x0C39) ||
		(c >= 0x0C60 && c <= 0x0C61) ||
		(c >= 0x0C85 && c <= 0x0C8C) ||
		(c >= 0x0C8E && c <= 0x0C90) ||
		(c >= 0x0C92 && c <= 0x0CA8) ||
		(c >= 0x0CAA && c <= 0x0CB3) ||
		(c >= 0x0CB5 && c <= 0x0CB9) ||
		c == 0x0CDE ||
		(c >= 0x0CE0 && c <= 0x0CE1) ||
		(c >= 0x0D05 && c <= 0x0D0C) ||
		(c >= 0x0D0E && c <= 0x0D10) ||
		(c >= 0x0D12 && c <= 0x0D28) ||
		(c >= 0x0D2A && c <= 0x0D39) ||
		(c >= 0x0D60 && c <= 0x0D61) ||
		(c >= 0x0E01 && c <= 0x0E2E) ||
		c == 0x0E30 ||
		(c >= 0x0E32 && c <= 0x0E33) ||
		(c >= 0x0E40 && c <= 0x0E45) ||
		(c >= 0x0E81 && c <= 0x0E82) ||
		c == 0x0E84 ||
		(c >= 0x0E87 && c <= 0x0E88) ||
		c == 0x0E8A ||
		c == 0x0E8D ||
		(c >= 0x0E94 && c <= 0x0E97) ||
		(c >= 0x0E99 && c <= 0x0E9F) ||
		(c >= 0x0EA1 && c <= 0x0EA3) ||
		c == 0x0EA5 ||
		c == 0x0EA7 ||
		(c >= 0x0EAA && c <= 0x0EAB) ||
		(c >= 0x0EAD && c <= 0x0EAE) ||
		c == 0x0EB0 ||
		(c >= 0x0EB2 && c <= 0x0EB3) ||
		c == 0x0EBD ||
		(c >= 0x0EC0 && c <= 0x0EC4) ||
		(c >= 0x0F40 && c <= 0x0F47) ||
		(c >= 0x0F49 && c <= 0x0F69) ||
		(c >= 0x10A0 && c <= 0x10C5) ||
		(c >= 0x10D0 && c <= 0x10F6) ||
		c == 0x1100 ||
		(c >= 0x1102 && c <= 0x1103) ||
		(c >= 0x1105 && c <= 0x1107) ||
		c == 0x1109 ||
		(c >= 0x110B && c <= 0x110C) ||
		(c >= 0x110E && c <= 0x1112) ||
		c == 0x113C ||
		c == 0x113E ||
		c == 0x1140 ||
		c == 0x114C ||
		c == 0x114E ||
		c == 0x1150 ||
		(c >= 0x1154 && c <= 0x1155) ||
		c == 0x1159 ||
		(c >= 0x115F && c <= 0x1161) ||
		c == 0x1163 ||
		c == 0x1165 ||
		c == 0x1167 ||
		c == 0x1169 ||
		(c >= 0x116D && c <= 0x116E) ||
		(c >= 0x1172 && c <= 0x1173) ||
		c == 0x1175 ||
		c == 0x119E ||
		c == 0x11A8 ||
		c == 0x11AB ||
		(c >= 0x11AE && c <= 0x11AF) ||
		(c >= 0x11B7 && c <= 0x11B8) ||
		c == 0x11BA ||
		(c >= 0x11BC && c <= 0x11C2) ||
		c == 0x11EB ||
		c == 0x11F0 ||
		c == 0x11F9 ||
		(c >= 0x1E00 && c <= 0x1E9B) ||
		(c >= 0x1EA0 && c <= 0x1EF9) ||
		(c >= 0x1F00 && c <= 0x1F15) ||
		(c >= 0x1F18 && c <= 0x1F1D) ||
		(c >= 0x1F20 && c <= 0x1F45) ||
		(c >= 0x1F48 && c <= 0x1F4D) ||
		(c >= 0x1F50 && c <= 0x1F57) ||
		c == 0x1F59 ||
		c == 0x1F5B ||
		c == 0x1F5D ||
		(c >= 0x1F5F && c <= 0x1F7D) ||
		(c >= 0x1F80 && c <= 0x1FB4) ||
		(c >= 0x1FB6 && c <= 0x1FBC) ||
		c == 0x1FBE ||
		(c >= 0x1FC2 && c <= 0x1FC4) ||
		(c >= 0x1FC6 && c <= 0x1FCC) ||
		(c >= 0x1FD0 && c <= 0x1FD3) ||
		(c >= 0x1FD6 && c <= 0x1FDB) ||
		(c >= 0x1FE0 && c <= 0x1FEC) ||
		(c >= 0x1FF2 && c <= 0x1FF4) ||
		(c >= 0x1FF6 && c <= 0x1FFC) ||
		c == 0x2126 ||
		(c >= 0x212A && c <= 0x212B) ||
		c == 0x212E ||
		(c >= 0x2180 && c <= 0x2182) ||
		(c >= 0x3041 && c <= 0x3094) ||
		(c >= 0x30A1 && c <= 0x30FA) ||
		(c >= 0x3105 && c <= 0x312C) ||
		(c >= 0xAC00 && c <= 0xD7A3) ||

		// Ideographic:
		(c >= 0x4E00 && c <= 0x9FA5) ||
		c == 0x3007 ||
		(c >= 0x3021 && c <= 0x3029)
		)
	{
		return true;
	}
	return false;
*/
	if ((c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z'))
		return true;
	return false;
}

inline bool CL_XPathEvaluator_Impl::is_combining_char(const CL_StringRef::char_type &c)
{
/*
	if (
		( c >= 0x0300 && c <= 0x0345) ||
		( c >= 0x0360 && c <= 0x0361) ||
		( c >= 0x0483 && c <= 0x0486) ||
		( c >= 0x0591 && c <= 0x05A1) ||
		( c >= 0x05A3 && c <= 0x05B9) ||
		( c >= 0x05BB && c <= 0x05BD) ||
		c == 0x05BF ||
		( c >= 0x05C1 && c <= 0x05C2) ||
		c == 0x05C4 ||
		( c >= 0x064B && c <= 0x0652) ||
		c == 0x0670 ||
		( c >= 0x06D6 && c <= 0x06DC) ||
		( c >= 0x06DD && c <= 0x06DF) ||
		( c >= 0x06E0 && c <= 0x06E4) ||
		( c >= 0x06E7 && c <= 0x06E8) ||
		( c >= 0x06EA && c <= 0x06ED) ||
		( c >= 0x0901 && c <= 0x0903) ||
		c == 0x093C ||
		( c >= 0x093E && c <= 0x094C) ||
		c == 0x094D ||
		( c >= 0x0951 && c <= 0x0954) ||
		( c >= 0x0962 && c <= 0x0963) ||
		( c >= 0x0981 && c <= 0x0983) ||
		c == 0x09BC ||
		c == 0x09BE ||
		c == 0x09BF ||
		( c >= 0x09C0 && c <= 0x09C4) ||
		( c >= 0x09C7 && c <= 0x09C8) ||
		( c >= 0x09CB && c <= 0x09CD) ||
		c == 0x09D7 ||
		( c >= 0x09E2 && c <= 0x09E3) ||
		c == 0x0A02 ||
		c == 0x0A3C ||
		c == 0x0A3E ||
		c == 0x0A3F ||
		( c >= 0x0A40 && c <= 0x0A42) ||
		( c >= 0x0A47 && c <= 0x0A48) ||
		( c >= 0x0A4B && c <= 0x0A4D) ||
		( c >= 0x0A70 && c <= 0x0A71) ||
		( c >= 0x0A81 && c <= 0x0A83) ||
		c == 0x0ABC ||
		( c >= 0x0ABE && c <= 0x0AC5) ||
		( c >= 0x0AC7 && c <= 0x0AC9) ||
		( c >= 0x0ACB && c <= 0x0ACD) ||
		( c >= 0x0B01 && c <= 0x0B03) ||
		c == 0x0B3C ||
		( c >= 0x0B3E && c <= 0x0B43) ||
		( c >= 0x0B47 && c <= 0x0B48) ||
		( c >= 0x0B4B && c <= 0x0B4D) ||
		( c >= 0x0B56 && c <= 0x0B57) ||
		( c >= 0x0B82 && c <= 0x0B83) ||
		( c >= 0x0BBE && c <= 0x0BC2) ||
		( c >= 0x0BC6 && c <= 0x0BC8) ||
		( c >= 0x0BCA && c <= 0x0BCD) ||
		c == 0x0BD7 ||
		( c >= 0x0C01 && c <= 0x0C03) ||
		( c >= 0x0C3E && c <= 0x0C44) ||
		( c >= 0x0C46 && c <= 0x0C48) ||
		( c >= 0x0C4A && c <= 0x0C4D) ||
		( c >= 0x0C55 && c <= 0x0C56) ||
		( c >= 0x0C82 && c <= 0x0C83) ||
		( c >= 0x0CBE && c <= 0x0CC4) ||
		( c >= 0x0CC6 && c <= 0x0CC8) ||
		( c >= 0x0CCA && c <= 0x0CCD) ||
		( c >= 0x0CD5 && c <= 0x0CD6) ||
		( c >= 0x0D02 && c <= 0x0D03) ||
		( c >= 0x0D3E && c <= 0x0D43) ||
		( c >= 0x0D46 && c <= 0x0D48) ||
		( c >= 0x0D4A && c <= 0x0D4D) ||
		c == 0x0D57 ||
		c == 0x0E31 ||
		( c >= 0x0E34 && c <= 0x0E3A) ||
		( c >= 0x0E47 && c <= 0x0E4E) ||
		c == 0x0EB1 ||
		( c >= 0x0EB4 && c <= 0x0EB9) ||
		( c >= 0x0EBB && c <= 0x0EBC) ||
		( c >= 0x0EC8 && c <= 0x0ECD) ||
		( c >= 0x0F18 && c <= 0x0F19) ||
		c == 0x0F35 ||
		c == 0x0F37 ||
		c == 0x0F39 ||
		c == 0x0F3E ||
		c == 0x0F3F ||
		( c >= 0x0F71 && c <= 0x0F84) ||
		( c >= 0x0F86 && c <= 0x0F8B) ||
		( c >= 0x0F90 && c <= 0x0F95) ||
		c == 0x0F97 ||
		( c >= 0x0F99 && c <= 0x0FAD) ||
		( c >= 0x0FB1 && c <= 0x0FB7) ||
		c == 0x0FB9 ||
		( c >= 0x20D0 && c <= 0x20DC) ||
		c == 0x20E1 ||
		( c >= 0x302A && c <= 0x302F) ||
		c == 0x3099 ||
		c == 0x309A
		)
	{
		return true;
	}
	return false;
*/
	return false;
}

inline bool CL_XPathEvaluator_Impl::is_digit(const CL_StringRef::char_type &c)
{
/*
	if (
		( c >= 0x0030 && c <= 0x0039) ||
		( c >= 0x0660 && c <= 0x0669) ||
		( c >= 0x06F0 && c <= 0x06F9) ||
		( c >= 0x0966 && c <= 0x096F) ||
		( c >= 0x09E6 && c <= 0x09EF) ||
		( c >= 0x0A66 && c <= 0x0A6F) ||
		( c >= 0x0AE6 && c <= 0x0AEF) ||
		( c >= 0x0B66 && c <= 0x0B6F) ||
		( c >= 0x0BE7 && c <= 0x0BEF) ||
		( c >= 0x0C66 && c <= 0x0C6F) ||
		( c >= 0x0CE6 && c <= 0x0CEF) ||
		( c >= 0x0D66 && c <= 0x0D6F) ||
		( c >= 0x0E50 && c <= 0x0E59) ||
		( c >= 0x0ED0 && c <= 0x0ED9) ||
		( c >= 0x0F20 && c <= 0x0F29)
		)
	{
		return true;
	}
	return false;
*/

	if (c >= '0' && c <= '9')
		return true;
	return false;
}

inline bool CL_XPathEvaluator_Impl::is_extender(const CL_StringRef::char_type &c)
{
/*
	if (
		c == 0x00B7 ||
		c == 0x02D0 ||
		c == 0x02D1 ||
		c == 0x0387 ||
		c == 0x0640 ||
		c == 0x0E46 ||
		c == 0x0EC6 ||
		c == 0x3005 ||
		(c >= 0x3031 && c <= 0x3035) ||
		(c >= 0x309D && c <= 0x309E) ||
		(c >= 0x30FC && c <= 0x30FE)
		)
	{
		return true;
	}
	return false;
*/
	return ((unsigned char) c == 0xb7);
}

inline CL_XPathObject CL_XPathEvaluator_Impl::boolean(const CL_XPathObject &object)
{
	switch(object.get_type())
	{
	case CL_XPathObject::type_null:
		return CL_XPathObject(false);
	case CL_XPathObject::type_node_set:
		return CL_XPathObject(!object.get_node_set().empty());
	case CL_XPathObject::type_boolean:
		return object;
	case CL_XPathObject::type_number:
		{
			double number = object.get_number();
			bool NaN = (std::numeric_limits<double>::has_quiet_NaN && number == std::numeric_limits<double>::quiet_NaN()) ||
				(std::numeric_limits<double>::has_signaling_NaN && number == std::numeric_limits<double>::signaling_NaN());
			return CL_XPathObject(!NaN && number != 0.0);
		}
	case CL_XPathObject::type_string:
		return CL_XPathObject(object.get_string().length() != 0);
	}
	return CL_XPathObject(false);
}


bool CL_XPathEvaluator_Impl::boolean(const CL_DomNode &node)
{
	return !string(node).empty();
}

inline CL_XPathObject CL_XPathEvaluator_Impl::number(CL_XPathObject object)
{
	double result = std::numeric_limits<double>::has_quiet_NaN ? std::numeric_limits<double>::quiet_NaN() : 0.0;
	switch(object.get_type())
	{
	case CL_XPathObject::type_null:
		return CL_XPathObject(result);

	case CL_XPathObject::type_boolean:
		return CL_XPathObject(object.get_boolean() ? 1.0 : 0.0);

	case CL_XPathObject::type_number:
		return object;

	case CL_XPathObject::type_node_set:
		object = string(object);

	case CL_XPathObject::type_string:
		{
			try
			{
				result = CL_StringHelp::text_to_double(object.get_string());
			}
			catch (CL_Exception&)
			{
			}
		}
	}
	return CL_XPathObject(result);
}

double CL_XPathEvaluator_Impl::number(const CL_DomNode &node)
{
	double result = std::numeric_limits<double>::has_quiet_NaN ? std::numeric_limits<double>::quiet_NaN() : 0.0;
	try
	{
		result = CL_StringHelp::text_to_double(string(node));
	}
	catch (CL_Exception&)
	{
	}
	return result;
}

inline CL_XPathObject CL_XPathEvaluator_Impl::string(const CL_XPathObject &object)
{
	switch(object.get_type())
	{
	case CL_XPathObject::type_node_set:
		{
			CL_String result;
			CL_XPathNodeSet nodeset = object.get_node_set();
			for (CL_XPathNodeSet::const_iterator it = nodeset.begin(), itEnd = nodeset.end(); it != itEnd; ++it)
				result.append(string(*it));

			return CL_XPathObject(result);
		}

	case CL_XPathObject::type_boolean:
		return CL_XPathObject(object.get_boolean() ? "true" : "false");

	case CL_XPathObject::type_number:
		{
			CL_String value;
			double number = object.get_number();

			if (std::numeric_limits<double>::has_infinity && std::abs(object.get_number()) == std::numeric_limits<double>::infinity())
				value = "Infinity";
			else if (std::numeric_limits<double>::has_quiet_NaN && object.get_number() == std::numeric_limits<double>::quiet_NaN())
				value = "NaN";
			else if (std::numeric_limits<double>::has_signaling_NaN && object.get_number() == std::numeric_limits<double>::signaling_NaN())
				value = "NaN";
			else
				value = CL_StringHelp::double_to_text(std::abs(object.get_number()));

			if (number < 0.0)
				value = "-" + value;

			return CL_XPathObject(value);
		}

	case CL_XPathObject::type_string:
		return object;

	case CL_XPathObject::type_null:
	default:
		return CL_XPathObject("");
	}
}

CL_String CL_XPathEvaluator_Impl::string(const CL_DomNode &node)
{
	CL_String s = node.get_node_value();
	if (node.is_element())
		s = node.to_element().get_text();
	return s;
}
