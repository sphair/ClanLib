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

#include "Core/precomp.h"
#include "API/Core/XML/xpath_evaluator.h"
#include "API/Core/XML/xpath_object.h"
#include "API/Core/XML/dom_node.h"
#include "API/Core/XML/dom_named_node_map.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/Text/string_help.h"
#include "xpath_evaluator_impl.h"
#include "xpath_token.h"
#include "xpath_location_step.h"

/////////////////////////////////////////////////////////////////////////////
// CL_XPathEvaluator_Impl Operations:

CL_XPathEvaluateResult CL_XPathEvaluator_Impl::evaluate(
	const CL_StringRef &expression,
	CL_DomNode context_node,
	CL_XPathToken prev_token)
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
			prev_token = read_location_path(expression, cur_token, context_node, operand_stack);
			continue;
		}

		// Evaluate expression:

		if (cur_token.type == CL_XPathToken::type_literal)
		{
			CL_XPathObject obj;
			obj.set_string(cur_token.value.str);
			operand_stack.push_back(obj);
		}
		else if (cur_token.type == CL_XPathToken::type_variable_reference)
		{
			CL_XPathObject obj = get_variable(cur_token.value.str);
			operand_stack.push_back(obj);
		}
		else if (cur_token.type == CL_XPathToken::type_number)
		{
			CL_XPathObject obj;
			obj.set_number(CL_StringHelp::text_to_int(cur_token.value.str));
			operand_stack.push_back(obj);
		}
		else if (cur_token.type == CL_XPathToken::type_function_name)
		{
			cur_token = read_token(expression, cur_token);
			if (cur_token.type != CL_XPathToken::type_operator ||
				cur_token.value.oper != CL_XPathToken::operator_parenthesis_begin)
			{
				throw CL_Exception("Syntax error in xpath expression");
			}

			std::vector<CL_XPathObject> parameters;
			while (true)
			{
				CL_XPathEvaluateResult result = evaluate(expression, context_node, cur_token);
				parameters.push_back(result.result);

				cur_token = result.next_token;
				if (cur_token.type == CL_XPathToken::type_operator &&
					cur_token.value.oper == CL_XPathToken::operator_parenthesis_end)
					break;
				if (cur_token.type != CL_XPathToken::type_comma)
					throw CL_Exception("Syntax error in xpath expression");
				cur_token = read_token(expression, cur_token);
			}
			cur_token = read_token(expression, cur_token);

			CL_XPathObject obj = call_function(cur_token.value.str, parameters);
			operand_stack.push_back(obj);
		}
		else if (cur_token.type == CL_XPathToken::type_bracket_begin)
		{
			if (operand_stack.empty())
				throw CL_Exception("Syntax error in xpath expression");
			Operand cur_operand = operand_stack.back();
			operand_stack.pop_back();
			if (cur_operand.get_type() != CL_XPathObject::type_node_set)
				throw CL_Exception("Syntax error in xpath expression");

			std::vector<CL_DomNode> nodes = cur_operand.get_node_set();
			std::vector<CL_DomNode> filtered_nodes;
			std::vector<CL_DomNode>::size_type index, size;
			size = nodes.size();
			for (index = 0; index < size; index++)
			{
				CL_XPathEvaluateResult result = evaluate(expression, nodes[index], cur_token);
				if (result.result.get_boolean())
					filtered_nodes.push_back(nodes[index]);
				if (index + 1 == size)
					cur_token = result.next_token;
			}
			cur_token = read_token(expression, cur_token);
			CL_XPathObject obj;
			obj.set_node_set(filtered_nodes);
			operand_stack.push_back(obj);
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
			{
				do_npr(operator_stack, operand_stack);
				operator_stack.push_back(cur_operator);
			}
			else
			{
				operator_stack.push_back(cur_operator);
			}
		}
		else
		{
			throw CL_Exception("Syntax error in xpath expression");
		}

		prev_token = cur_token;
	}

	while (do_npr(operator_stack, operand_stack));

	if (operand_stack.empty()) 
		throw CL_Exception("Syntax error in xpath expression");

	CL_XPathEvaluateResult result;
	result.result = operand_stack.back();
	result.next_token = cur_token;
	return result;
}

CL_XPathObject CL_XPathEvaluator_Impl::call_function(const CL_StringRef &name, std::vector<CL_XPathObject> parameters)
{
	if (name == "last")
		return function_last(parameters);
	else if (name == "position")
		return function_position(parameters);
	else if (name == "count")
		return function_count(parameters);
	else if (name == "id")
		return function_id(parameters);
	else if (name == "local-name")
		return function_local_name(parameters);
	else if (name == "namespace-uri")
		return function_namespace_uri(parameters);
	else if (name == "name")
		return function_name(parameters);
	else if (name == "string")
		return function_string(parameters);
	else if (name == "concat")
		return function_concat(parameters);
	else if (name == "starts-with")
		return function_starts_with(parameters);
	else if (name == "contains")
		return function_contains(parameters);
	else if (name == "substring-before")
		return function_substring_before(parameters);
	else if (name == "substring-after")
		return function_substring_after(parameters);
	else if (name == "substring")
		return function_substring(parameters);
	else if (name == "string-length")
		return function_string_length(parameters);
	else if (name == "normalize-space")
		return function_normalize_space(parameters);
	else if (name == "translate")
		return function_translate(parameters);
	else if (name == "boolean")
		return function_boolean(parameters);
	else if (name == "not")
		return function_not(parameters);
	else if (name == "true")
		return function_true(parameters);
	else if (name == "false")
		return function_false(parameters);
	else if (name == "lang")
		return function_lang(parameters);
	else if (name == "number")
		return function_number(parameters);
	else if (name == "sum")
		return function_sum(parameters);
	else if (name == "floor")
		return function_floor(parameters);
	else if (name == "ceiling")
		return function_ceiling(parameters);
	else if (name == "round")
		return function_round(parameters);

	CL_XPathObject obj;
	obj.set_null();
	return obj;
}

CL_XPathObject CL_XPathEvaluator_Impl::get_variable(const CL_StringRef &name)
{
	CL_XPathObject obj;
	obj.set_string(name);
	return obj;
}

/////////////////////////////////////////////////////////////////////////////
// CL_XPathEvaluator_Impl implementation:

bool CL_XPathEvaluator_Impl::do_npr(
	std::vector<CL_XPathEvaluator_Impl::Operator> &operator_stack,
	std::vector<CL_XPathEvaluator_Impl::Operand> &operand_stack)
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

		result.set_boolean(b.get_boolean() && a.get_boolean());
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

		result.set_boolean(b.get_boolean() || a.get_boolean());
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

		result.set_number((int) b.get_number() % (int) a.get_number());
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

		result.set_number(b.get_number() / a.get_number());
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

		result.set_number(a.get_number() * b.get_number());
		operand_stack.push_back(result);
		break;
/*
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

		// todo: produce union of two node sets
		break;

	case CL_XPathToken::operator_slash:
		break;
	case CL_XPathToken::operator_double_slash:
		break;
*/
	case CL_XPathToken::operator_plus:
		if (operand_stack.empty()) 
			return false;
		a = operand_stack.back();
		operand_stack.pop_back();

		if (operand_stack.empty()) 
			return false;
		b = operand_stack.back();
		operand_stack.pop_back();

		result.set_number(a.get_number() + b.get_number());
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

		result.set_number(b.get_number() - a.get_number());
		operand_stack.push_back(result);
		break;
/*
	case CL_XPathToken::operator_assign:
		if (operand_stack.empty()) 
			return false;
		a = operand_stack.back();
		operand_stack.pop_back();

		if (operand_stack.empty()) 
			return false;
		b = operand_stack.back();
		operand_stack.pop_back();

		operand_stack.push_back(a);
		break;
*/
	case CL_XPathToken::operator_compare_equal:
		if (operand_stack.empty()) 
			return false;
		a = operand_stack.back();
		operand_stack.pop_back();

		if (operand_stack.empty()) 
			return false;
		b = operand_stack.back();
		operand_stack.pop_back();

		result.set_boolean(compare(b, a) == 0);
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

		result.set_boolean(compare(b, a) != 0);
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

		result.set_boolean(compare(b, a) < 0);
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

		result.set_boolean(compare(b, a) <= 0);
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

		result.set_boolean(compare(b, a) > 0);
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

		result.set_boolean(compare(b, a) >= 0);
		operand_stack.push_back(result);
		break;
	default:	// Added to stop compiler warnings for "operator_xxx" not handled in switch
		break;
	}

	return true;
}

int CL_XPathEvaluator_Impl::compare(Operand &a, Operand b)
{
	if (a.get_type() == CL_XPathObject::type_node_set)
	{
		return compare_node_set(a, b);
	}
	else if (a.get_type() == CL_XPathObject::type_number)
	{
		return compare_number(a, b);
	}
	else if (a.get_type() == CL_XPathObject::type_boolean)
	{
		return compare_boolean(a, b);
	}
	else if (a.get_type() == CL_XPathObject::type_string)
	{
		return compare_string(a, b);
	}
	else
	{
		throw CL_Exception("Error in xpath evaluator");
	}
}

int CL_XPathEvaluator_Impl::compare_node_set(Operand &a, Operand b)
{
	std::vector<CL_DomNode> nodeset1 = a.get_node_set();
	if (b.get_type() == CL_XPathObject::type_node_set)
	{
		std::vector<CL_DomNode> nodeset2 = b.get_node_set();
		for (std::vector<CL_DomNode>::size_type i = 0; i < nodeset1.size(); i++)
		{
			CL_String s1 = nodeset1[i].get_node_value();
			if (nodeset1[i].is_element())
				s1 = nodeset1[i].to_element().get_text();
			for (std::vector<CL_DomNode>::size_type j = 0; j < nodeset2.size(); j++)
			{
				CL_String s2 = nodeset1[i].get_node_value();
				if (nodeset1[i].is_element())
					s2 = nodeset1[i].to_element().get_text();

			}
		}
		return -2;
	}
	else if (b.get_type() == CL_XPathObject::type_number)
	{
		double number2 = b.get_number();
		for (std::vector<CL_DomNode>::size_type i = 0; i < nodeset1.size(); i++)
		{
			CL_String s1 = nodeset1[i].get_node_value();
			if (nodeset1[i].is_element())
				s1 = nodeset1[i].to_element().get_text();
			try
			{
				double number1 = CL_StringHelp::text_to_double(s1);
				if (number1 == number2)
					return 0;
			}
			catch (CL_Exception)
			{
			}
		}
		return -2;
	}
	else if (b.get_type() == CL_XPathObject::type_boolean)
	{
		bool boolean2 = b.get_boolean();
		bool boolean1 = !nodeset1.empty();
		if (boolean1 < boolean2)
			return -1;
		else if (boolean2 > boolean1)
			return 1;
		else
			return 0;
	}
	else if (b.get_type() == CL_XPathObject::type_string)
	{
		CL_String s2 = b.get_string();
		for (std::vector<CL_DomNode>::size_type i = 0; i < nodeset1.size(); i++)
		{
			CL_String s1 = nodeset1[i].get_node_value();
			if (nodeset1[i].is_element())
				s1 = nodeset1[i].to_element().get_text();
			if (s1 == s2)
				return 0;
		}
		return -2;
	}
	else
	{
		throw CL_Exception("Error in xpath evaluator");
	}
}

int CL_XPathEvaluator_Impl::compare_boolean(Operand &a, Operand b)
{
	bool boolean1 = a.get_boolean();
	if (b.get_type() == CL_XPathObject::type_node_set)
	{
		return -compare_node_set(b, a);
	}
	else if (b.get_type() == CL_XPathObject::type_number)
	{
		bool boolean2 = b.get_number() != 0.0;
		if (boolean1 < boolean2)
			return -1;
		else if (boolean2 > boolean1)
			return 1;
		else
			return 0;
	}
	else if (b.get_type() == CL_XPathObject::type_boolean)
	{
		bool boolean2 = b.get_boolean();
		if (boolean1 < boolean2)
			return -1;
		else if (boolean2 > boolean1)
			return 1;
		else
			return 0;
	}
	else if (b.get_type() == CL_XPathObject::type_string)
	{
		bool boolean2 = !b.get_string().empty();
		if (boolean1 < boolean2)
			return -1;
		else if (boolean2 > boolean1)
			return 1;
		else
			return 0;
	}
	else
	{
		throw CL_Exception("Error in xpath evaluator");
	}
}

int CL_XPathEvaluator_Impl::compare_number(Operand &a, Operand b)
{
	double number1 = a.get_number();
	if (b.get_type() == CL_XPathObject::type_node_set)
	{
		return -compare_node_set(b, a);
	}
	else if (b.get_type() == CL_XPathObject::type_boolean)
	{
		return -compare_boolean(b, a);
	}
	else if (b.get_type() == CL_XPathObject::type_number)
	{
		double number2 = b.get_number();
		if (number1 < number2)
			return -1;
		else if (number1 > number2)
			return 1;
		else
			return 0;
	}
	else if (b.get_type() == CL_XPathObject::type_string)
	{
		try
		{
			double number2 = CL_StringHelp::text_to_double(b.get_string());
			if (number1 < number2)
				return -1;
			else if (number1 > number2)
				return 1;
			else
				return 0;
		}
		catch (CL_Exception)
		{
			return -2;
		}
	}
	else
	{
		throw CL_Exception("Error in xpath evaluator");
	}
}

int CL_XPathEvaluator_Impl::compare_string(Operand &a, Operand b)
{
	CL_String string1 = a.get_string();
	if (b.get_type() == CL_XPathObject::type_node_set)
	{
		return -compare_node_set(b, a);
	}
	else if (b.get_type() == CL_XPathObject::type_number)
	{
		return -compare_number(b, a);
	}
	else if (b.get_type() == CL_XPathObject::type_boolean)
	{
		return -compare_boolean(b, a);
	}
	else if (b.get_type() == CL_XPathObject::type_string)
	{
		CL_String string2 = b.get_string();
		if (string1 < string2)
			return -1;
		else if (string1 > string2)
			return 1;
		else
			return 0;
	}
	else
	{
		throw CL_Exception("Error in xpath evaluator");
	}
}

CL_XPathToken CL_XPathEvaluator_Impl::read_location_path(
	const CL_StringRef &expression,
	CL_XPathToken cur_token,
	CL_DomNode &context_node,
	std::vector<CL_XPathEvaluator_Impl::Operand> &operand_stack)
{
/*
	[1] LocationPath               ::= RelativeLocationPath | AbsoluteLocationPath	
	[2] AbsoluteLocationPath       ::= '/' RelativeLocationPath? | AbbreviatedAbsoluteLocationPath
	[3] RelativeLocationPath       ::= Step | RelativeLocationPath '/' Step | AbbreviatedRelativeLocationPath
*/

	if (cur_token.type == CL_XPathToken::type_operator && cur_token.value.oper == CL_XPathToken::operator_slash)
	{
		// Find root node:
		CL_DomNode root_node = context_node;
		while (true)
		{
			CL_DomNode parent = root_node.get_parent_node();
			if (!parent.is_null())
				root_node = parent;
			else
				break;
		}

		CL_XPathToken next_token = read_token(expression, cur_token);
		if (next_token.type == CL_XPathToken::type_axis_name ||
			next_token.type == CL_XPathToken::type_name_test ||
			next_token.type == CL_XPathToken::type_at_sign ||
			next_token.type == CL_XPathToken::type_dot ||
			next_token.type == CL_XPathToken::type_double_dot ||
			(next_token.type == CL_XPathToken::type_operator && next_token.value.oper == CL_XPathToken::operator_double_slash))
		{
			cur_token = next_token;
			return read_location_steps(expression, cur_token, root_node, operand_stack);
		}
		else
		{
			std::vector<CL_DomNode> nodeset;
			nodeset.push_back(root_node);
			CL_XPathObject object;
			object.set_node_set(nodeset);
			operand_stack.push_back(object);
			return cur_token;
		}
	}
	else
	{
		return read_location_steps(expression, cur_token, context_node, operand_stack);
	}
}

CL_XPathToken CL_XPathEvaluator_Impl::read_location_steps(
	const CL_StringRef &expression,
	CL_XPathToken cur_token,
	CL_DomNode &context_node,
	std::vector<CL_XPathEvaluator_Impl::Operand> &operand_stack)
{
	std::vector<CL_XPathLocationStep> steps;
	while (true)
	{
		CL_XPathLocationStep step;
		cur_token = read_location_step(expression, cur_token, step);
		steps.push_back(step);

		CL_XPathToken next_token = read_token(expression, cur_token);
		if (next_token.type == CL_XPathToken::type_operator && next_token.value.oper == CL_XPathToken::operator_slash)
		{
			next_token = read_token(expression, next_token);
			if (next_token.type == CL_XPathToken::type_axis_name ||
				next_token.type == CL_XPathToken::type_name_test ||
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
		else if (next_token.type == CL_XPathToken::type_operator && next_token.value.oper == CL_XPathToken::operator_double_slash)
		{
			cur_token = next_token;
			step.axis = "descendant-or-self";
			step.test_type = CL_XPathLocationStep::type_none;
			steps.push_back(step);
			next_token = read_token(expression, next_token);
			if (next_token.type == CL_XPathToken::type_axis_name ||
				next_token.type == CL_XPathToken::type_name_test ||
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

	std::vector<CL_DomNode> nodeset;
	evaluate_location_step(context_node, steps, 0, expression, nodeset);
	CL_XPathObject object;
	object.set_node_set(nodeset);
	operand_stack.push_back(object);
	return cur_token;
}

CL_XPathToken CL_XPathEvaluator_Impl::read_location_step(
	const CL_StringRef &expression,
	CL_XPathToken cur_token,
	CL_XPathLocationStep &step)
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
		step.test_type = CL_XPathLocationStep::type_none;
	}
	else if (cur_token.type == CL_XPathToken::type_double_dot)
	{
		step.axis = "parent";
		step.test_type = CL_XPathLocationStep::type_none;
	}
	else
	{
		// Read AxisSpecifier:
		if (cur_token.type == CL_XPathToken::type_axis_name)
		{
			step.axis = cur_token.value.str;
			cur_token = read_token(expression, cur_token);
			if (cur_token.type != CL_XPathToken::type_double_colon)
				throw CL_Exception("Syntax error in xpath expression");
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
				throw CL_Exception("Syntax error in xpath expression");
			cur_token = read_token(expression, cur_token);
			if (cur_token.type == CL_XPathToken::type_literal && step.node_type == CL_XPathToken::node_type_processing_instruction)
			{
				step.test_str = cur_token.value.str;
				cur_token = read_token(expression, cur_token);
			}
			if (cur_token.type != CL_XPathToken::type_operator || cur_token.value.oper != CL_XPathToken::operator_parenthesis_end)
				throw CL_Exception("Syntax error in xpath expression");
		}
		else
		{
			throw CL_Exception("Syntax error in xpath expression");
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

CL_XPathToken CL_XPathEvaluator_Impl::skip_predicate_expression(const CL_StringRef &expression, const CL_XPathToken &previous_token)
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

void CL_XPathEvaluator_Impl::evaluate_location_step(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &nodeset)
{
	if (step_index < steps.size())
	{
		if (steps[step_index].axis == "ancestor")
			select_nodes_ancestor(context_node, steps, step_index, expression, nodeset);
		else if (steps[step_index].axis == "ancestor-or-self")
			select_nodes_ancestor_or_self(context_node, steps, step_index, expression, nodeset);
		else if (steps[step_index].axis == "attribute")
			select_nodes_attribute(context_node, steps, step_index, expression, nodeset);
		else if (steps[step_index].axis == "child")
			select_nodes_child(context_node, steps, step_index, expression, nodeset);
		else if (steps[step_index].axis == "descendant")
			select_nodes_descendant(context_node, steps, step_index, expression, nodeset);
		else if (steps[step_index].axis == "descendant-or-self")
			select_nodes_descendant_or_self(context_node, steps, step_index, expression, nodeset);
		else if (steps[step_index].axis == "following")
			select_nodes_following(context_node, steps, step_index, expression, nodeset);
		else if (steps[step_index].axis == "follwing-sibling")
			select_nodes_following_sibling(context_node, steps, step_index, expression, nodeset);
		else if (steps[step_index].axis == "namespace")
			select_nodes_namespace(context_node, steps, step_index, expression, nodeset);
		else if (steps[step_index].axis == "parent")
			select_nodes_parent(context_node, steps, step_index, expression, nodeset);
		else if (steps[step_index].axis == "preceding")
			select_nodes_preceding(context_node, steps, step_index, expression, nodeset);
		else if (steps[step_index].axis == "preceding-sibling")
			select_nodes_preceding_sibling(context_node, steps, step_index, expression, nodeset);
		else if (steps[step_index].axis == "self")
			select_nodes_self(context_node, steps, step_index, expression, nodeset);
		else
			throw CL_Exception("Syntax error in xpath expression");
	}
	else
	{
		nodeset.push_back(context_node);
	}
}

void CL_XPathEvaluator_Impl::select_nodes_ancestor(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &nodes)
{
	CL_DomNode parent = context_node.get_parent_node();
	int node_index = 1;
	while (!parent.is_null())
	{
		if (confirm_step_requirements(parent, node_index, steps[step_index], expression))
			evaluate_location_step(parent, steps, step_index+1, expression, nodes);
		parent = parent.get_parent_node();
	}
}

void CL_XPathEvaluator_Impl::select_nodes_ancestor_or_self(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &nodes)
{
	int node_index = 1;
	CL_DomNode parent = context_node.get_parent_node();
	if (!context_node.is_null())
	{
		if (confirm_step_requirements(context_node, node_index, steps[step_index], expression))
			evaluate_location_step(context_node, steps, step_index+1, expression, nodes);
	}
	while (!parent.is_null())
	{
		if (confirm_step_requirements(parent, node_index, steps[step_index], expression))
			evaluate_location_step(parent, steps, step_index+1, expression, nodes);
		parent = parent.get_parent_node();
	}
}

void CL_XPathEvaluator_Impl::select_nodes_attribute(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &nodes)
{
	int node_index = 1;
	CL_DomNamedNodeMap attributes = context_node.get_attributes();
	int num_attributes = attributes.get_length();
	for (int i = 0; i < num_attributes; i++)
	{
		if (confirm_step_requirements(attributes.item(i), node_index, steps[step_index], expression))
			evaluate_location_step(attributes.item(i), steps, step_index+1, expression, nodes);
	}
}

void CL_XPathEvaluator_Impl::select_nodes_child(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &nodes)
{
	int node_index = 1;
	CL_DomNode cur_node = context_node.get_first_child();
	while (!cur_node.is_null())
	{
		if (confirm_step_requirements(cur_node, node_index, steps[step_index], expression))
			evaluate_location_step(cur_node, steps, step_index+1, expression, nodes);
		cur_node = cur_node.get_next_sibling();
	}
}

void CL_XPathEvaluator_Impl::select_nodes_descendant(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &nodes)
{
	int node_index = 1;
	CL_DomNode cur_node = context_node;
	while (true)
	{
		CL_DomNode next = cur_node.get_first_child();
		if (next.is_null())
		{
			next = cur_node.get_next_sibling();
			while (next.is_null())
			{
				cur_node = cur_node.get_parent_node();
				if (cur_node.is_null() || cur_node == context_node)
					break;
				next = cur_node.get_next_sibling();
			}
			if (next.is_null() || cur_node == context_node)
				break;
		}
		cur_node = next;
		if (confirm_step_requirements(cur_node, node_index, steps[step_index], expression))
			evaluate_location_step(cur_node, steps, step_index+1, expression, nodes);
	}
}

void CL_XPathEvaluator_Impl::select_nodes_descendant_or_self(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &nodes)
{
	int node_index = 1;
	if (!context_node.is_null())
	{
		if (confirm_step_requirements(context_node, node_index, steps[step_index], expression))
			evaluate_location_step(context_node, steps, step_index+1, expression, nodes);
	}
	CL_DomNode cur_node = context_node;
	while (true)
	{
		CL_DomNode next = cur_node.get_first_child();
		if (next.is_null())
		{
			next = cur_node.get_next_sibling();
			while (next.is_null())
			{
				cur_node = cur_node.get_parent_node();
				if (cur_node.is_null() || cur_node == context_node)
					break;
				next = cur_node.get_next_sibling();
			}
			if (next.is_null() || cur_node == context_node)
				break;
		}
		cur_node = next;
		if (confirm_step_requirements(cur_node, node_index, steps[step_index], expression))
			evaluate_location_step(cur_node, steps, step_index+1, expression, nodes);
	}
}

void CL_XPathEvaluator_Impl::select_nodes_following(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &nodes)
{
	int node_index = 1;
	CL_DomNode cur_node = context_node;
	while (true)
	{
		CL_DomNode next = cur_node.get_first_child();
		if (next.is_null())
		{
			next = cur_node.get_next_sibling();
			while (next.is_null())
			{
				cur_node = cur_node.get_parent_node();
				if (cur_node.is_null())
					break;
				next = cur_node.get_next_sibling();
			}
			if (next.is_null())
				break;
		}
		cur_node = next;
		if (confirm_step_requirements(cur_node, node_index, steps[step_index], expression))
			evaluate_location_step(cur_node, steps, step_index+1, expression, nodes);
	}
}

void CL_XPathEvaluator_Impl::select_nodes_following_sibling(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &nodes)
{
	int node_index = 1;
	CL_DomNode cur_node = context_node.get_next_sibling();
	while (!cur_node.is_null())
	{
		if (confirm_step_requirements(cur_node, node_index, steps[step_index], expression))
			evaluate_location_step(cur_node, steps, step_index+1, expression, nodes);
		cur_node = cur_node.get_next_sibling();
	}
}

void CL_XPathEvaluator_Impl::select_nodes_namespace(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &nodes)
{
}

void CL_XPathEvaluator_Impl::select_nodes_parent(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &nodes)
{
	int node_index = 1;
	CL_DomNode parent = context_node.get_parent_node();
	if (!parent.is_null())
	{
		if (confirm_step_requirements(parent, node_index, steps[step_index], expression))
			evaluate_location_step(parent, steps, step_index+1, expression, nodes);
	}
}

void CL_XPathEvaluator_Impl::select_nodes_preceding(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &nodes)
{
	int node_index = 1;
	CL_DomNode cur_node = context_node;
	while (true)
	{
		CL_DomNode prev = cur_node.get_last_child();
		if (prev.is_null())
		{
			prev = cur_node.get_previous_sibling();
			while (prev.is_null())
			{
				cur_node = cur_node.get_parent_node();
				if (cur_node.is_null())
					break;
				prev = cur_node.get_previous_sibling();
			}
			if (prev.is_null())
				break;
		}
		cur_node = prev;
		if (confirm_step_requirements(cur_node, node_index, steps[step_index], expression))
			evaluate_location_step(cur_node, steps, step_index+1, expression, nodes);
	}
}

void CL_XPathEvaluator_Impl::select_nodes_preceding_sibling(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &nodes)
{
	int node_index = 1;
	CL_DomNode cur_node = context_node.get_previous_sibling();
	while (!cur_node.is_null())
	{
		if (confirm_step_requirements(cur_node, node_index, steps[step_index], expression))
			evaluate_location_step(cur_node, steps, step_index+1, expression, nodes);
		cur_node = cur_node.get_previous_sibling();
	}
}

void CL_XPathEvaluator_Impl::select_nodes_self(const CL_DomNode &context_node, const std::vector<CL_XPathLocationStep> &steps, unsigned int step_index, const CL_StringRef &expression, std::vector<CL_DomNode> &nodes)
{
	int node_index = 1;
	if (!context_node.is_null())
	{
		if (confirm_step_requirements(context_node, node_index, steps[step_index], expression))
			evaluate_location_step(context_node, steps, step_index+1, expression, nodes);
	}
}

bool CL_XPathEvaluator_Impl::confirm_step_requirements(const CL_DomNode &node, int &node_index, const CL_XPathLocationStep &step, const CL_StringRef &expression)
{
	bool test_passed = false;
	switch (step.test_type)
	{
	default:
	case CL_XPathLocationStep::type_none:
		test_passed = true;
		break;
	case CL_XPathLocationStep::type_name:
		test_passed = node.get_node_name() == step.test_str;
		break;
	case CL_XPathLocationStep::type_node:
		switch (node.get_node_type())
		{
		case CL_DomNode::COMMENT_NODE:
			test_passed = step.node_type == CL_XPathToken::node_type_comment || step.node_type == CL_XPathToken::node_type_node;
			break;
		case CL_DomNode::TEXT_NODE:
			test_passed = step.node_type == CL_XPathToken::node_type_text || step.node_type == CL_XPathToken::node_type_node;
			break;
		case CL_DomNode::PROCESSING_INSTRUCTION_NODE:
			if (step.node_type == CL_XPathToken::node_type_processing_instruction)
			{
				test_passed = true;
			}
			else if (step.node_type == CL_XPathToken::node_type_node)
			{
				test_passed = true;
			}
			break;
		case CL_DomNode::NULL_NODE:
		case CL_DomNode::ELEMENT_NODE:
		case CL_DomNode::ATTRIBUTE_NODE:
		case CL_DomNode::CDATA_SECTION_NODE:
		case CL_DomNode::ENTITY_REFERENCE_NODE:
		case CL_DomNode::ENTITY_NODE:
		case CL_DomNode::DOCUMENT_NODE:
		case CL_DomNode::DOCUMENT_TYPE_NODE:
		case CL_DomNode::DOCUMENT_FRAGMENT_NODE:
		case CL_DomNode::NOTATION_NODE:
			test_passed = step.node_type == CL_XPathToken::node_type_node;
			break;
		default:
			break;
		}
	}

	if (test_passed)
	{
		int cur_node_index = node_index;
		node_index++;
		for (std::vector<CL_XPathLocationStep::Predicate>::size_type predicate_index = 0; predicate_index < step.predicates.size(); predicate_index++)
		{
			const CL_XPathLocationStep::Predicate &predicate = step.predicates[predicate_index];

			CL_XPathEvaluateResult result = evaluate(expression.substr(predicate.pos, predicate.length), node, CL_XPathToken());
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
				include_in_nodeset = result.result.get_number() == cur_node_index;
				break;
			case CL_XPathObject::type_string:
				include_in_nodeset = !result.result.get_string().empty();
				break;
			}
			if (!include_in_nodeset)
				return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}

CL_XPathToken CL_XPathEvaluator_Impl::read_token(
	const CL_StringRef &expression,
	const CL_XPathToken &previous_token)
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
		/*if (second_char == '=')
		{*/
			token.value.oper = CL_XPathToken::operator_compare_equal;
		/*	token.length = 2;
		}
		else
		{
			token.value.oper = CL_XPathToken::operator_assign;
		}*/
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
			throw CL_Exception("Missing ending quotation sign in XPath expression");
		token.length = pos - token.pos + 1;
		token.value.str = expression.substr(token.pos + 1, token.length - 2);
		return token;
	case '\'':
		token.type = CL_XPathToken::type_literal;
		pos = expression.find_first_of('\'', token.pos + 1);
		if (pos == CL_StringRef::npos)
			throw CL_Exception("Missing ending quotation sign in XPath expression");
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
		throw CL_Exception("Syntax error in xpath expression");

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
			throw CL_Exception("Syntax error in xpath expression");
	}

	token.type = CL_XPathToken::type_name_test;
	return token;
}

CL_XPathObject CL_XPathEvaluator_Impl::function_last(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_position(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_count(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_id(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_local_name(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_namespace_uri(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_name(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_string(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_concat(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_starts_with(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_contains(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_substring_before(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_substring_after(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_substring(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_string_length(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_normalize_space(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_translate(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_boolean(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_not(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_true(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_false(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_lang(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_number(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_sum(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_floor(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_ceiling(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
}

CL_XPathObject CL_XPathEvaluator_Impl::function_round(const std::vector<CL_XPathObject> &parameters)
{
	return CL_XPathObject();
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

