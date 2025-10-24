/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "API/Core/XML/dom_node.h"
#include "API/Core/Text/string_help.h"
#include "xpath_evaluator_impl.h"
#include "xpath_token.h"

/////////////////////////////////////////////////////////////////////////////
// CL_XPathEvaluator Construction:

CL_XPathEvaluator::CL_XPathEvaluator()
: impl(new CL_XPathEvaluator_Impl)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_XPathEvaluator Operations:

CL_XPathObject CL_XPathEvaluator::evaluate(const CL_StringRef &expression, CL_DomNode context_node)
{
	CL_XPathToken prev_token;
	CL_XPathEvaluateResult result = impl->evaluate(expression, context_node, prev_token);
	if (result.next_token.type != CL_XPathToken::type_none)
		throw CL_Exception("Syntax error in xpath expression");
	return result.result;
}

/*
std::vector<CL_DomNode> XPathEvaluator::evaluate(
	const CL_StringRef &expression, CL_DomNode context_node, int context_position, int context_size)
{
	int tokenizer_pos = 0;
	CL_StringRef token = read_token(expression, tokenizer_pos);

	if (token == "/")
	{
		context_node = context_node.ownerDocument();
		token = read_token(expression, tokenizer_pos);
	}

	if (token.empty())
	{
		std::vector<CL_DomNode> nodes;
		nodes.push_back(context_node);
		return nodes;
	}

	CL_StringRef axis_name;
	CL_StringRef node_test;

	CL_StringRef next_token = peek_token(expression, tokenizer_pos);
	if (next_token == "::")
	{
		axis_name = token;
		node_test = read_token(expression, tokenzier_pos);
	}
	else
	{
		if (token[0] == '@')
		{
			axis_name = "attribute";
			node_test = token.substr(1);
		}
		else
		{
			axis_name = "child";
			node_test = token;
		}
	}

	std::vector<CL_DomNode> location_nodes = find_location_nodes(axis_name, node_test);
	
	next_token = peek_token(expression, tokenizer_pos);
	while (next_token == "[")
	{
		read_token(expression, tokenizer_pos);
		filter_location_nodes(location_nodes, expression, tokenizer_pos);
	}

	int tokenizer_pos = 0;
	CL_StringRef token = read_token(expression, tokenizer_pos);
	if (token.empty())
	{
		return location_nodes;
	}
	else if (token == "/")
	{
		std::vector<CL_DomNode> union_nodes;
		std::vector<CL_DomNode>::size_type i = 0, size = location_nodes.size();
		for (i = 0; i < size; i++)
		{
			std::vector<CL_DomNode> nodes = evaluate(
				expression.substr(tokenizer_pos), location_nodes[i], i, size);
			union_nodes.append(nodes);
		}
		return union_nodes;
	}
	else
	{
		throw Exception("XPath syntax error");
	}
}
*/
