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

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/css_document2.h"
#include "API/CSSLayout/css_property2.h"
#include "API/CSSLayout/css_property_list2.h"
#include "API/CSSLayout/css_select_node2.h"
#include "API/CSSLayout/dom_select_node.h"
#include "css_ruleset2.h"
#include "css_selector_chain2.h"
#include "css_selector_link2.h"
#include "css_ruleset_match2.h"
#include <algorithm>

/////////////////////////////////////////////////////////////////////////////

class CL_CSSDocument2_Impl
{
public:
	std::vector<CL_CSSRulesetMatch2> select_rulesets(CL_CSSSelectNode2 *node, const CL_String &pseudo_element);
	bool try_match_chain(const CL_CSSSelectorChain2 &chain, CL_CSSSelectNode2 *node, size_t chain_index);
	bool try_match_link(const CL_CSSSelectorLink2 &link, const CL_CSSSelectNode2 *node);
	void read_stylesheet(CL_CSSTokenizer &tokenizer);
	void read_at_rule(CL_CSSTokenizer &tokenizer, CL_CSSToken &token);
	void read_statement(CL_CSSTokenizer &tokenizer, CL_CSSToken &token);
	static bool read_end_of_statement(CL_CSSTokenizer &tokenizer, CL_CSSToken &token);
	bool read_selector_chain(CL_CSSTokenizer &tokenizer, CL_CSSToken &token, CL_CSSSelectorChain2 &out_selector_chain);
	static bool read_property_value(CL_CSSTokenizer &tokenizer, CL_CSSToken &token, CL_CSSProperty2 &property);
	CL_String to_string(const CL_CSSToken &token);

	std::vector<CL_CSSRuleset2> rulesets;
};

/////////////////////////////////////////////////////////////////////////////

CL_CSSDocument2::CL_CSSDocument2()
: impl(new CL_CSSDocument2_Impl())
{
}

CL_CSSDocument2::~CL_CSSDocument2()
{
}

void CL_CSSDocument2::add_sheet(const CL_String &filename)
{
	CL_File file(filename, CL_File::open_existing, CL_File::access_read);
	CL_CSSTokenizer tokenizer(file);
	impl->read_stylesheet(tokenizer);
}

CL_CSSPropertyList2 CL_CSSDocument2::select(const CL_DomElement &node, const CL_String &pseudo_element)
{
	CL_DomSelectNode select_node(node);
	return select(&select_node, pseudo_element);
}

CL_CSSPropertyList2 CL_CSSDocument2::select(CL_CSSSelectNode2 *node, const CL_String &pseudo_element)
{
	std::vector<CL_CSSRulesetMatch2> rulesets = impl->select_rulesets(node, pseudo_element);
	CL_CSSPropertyList2 properties;
	for (size_t i = rulesets.size(); i > 0; i--)
	{
		for (size_t j = 0; j < rulesets[i-1].ruleset->properties.size(); j++)
		{
			if (rulesets[i-1].ruleset->properties[j].is_important())
				properties.push_back(rulesets[i-1].ruleset->properties[j]);
		}
	}
	for (size_t i = rulesets.size(); i > 0; i--)
	{
		for (size_t j = 0; j < rulesets[i-1].ruleset->properties.size(); j++)
		{
			if (!rulesets[i-1].ruleset->properties[j].is_important())
				properties.push_back(rulesets[i-1].ruleset->properties[j]);
		}
	}
	return properties;
}

CL_CSSPropertyList2 CL_CSSDocument2::get_style_properties(const CL_String &style_string)
{
	CL_CSSPropertyList2 properties;
	CL_CSSTokenizer tokenizer(style_string);
	CL_CSSToken token;
	while (true)
	{
		tokenizer.read(token, true);
		if (token.type == CL_CSSToken::type_ident)
		{
			CL_String property_name = token.value;
			tokenizer.read(token, true);
			if (token.type == CL_CSSToken::type_colon)
			{
				tokenizer.read(token, true);

				CL_CSSProperty2 property;
				property.set_name(property_name);
				CL_CSSDocument2_Impl::read_property_value(tokenizer, token, property);
				if (!property.get_value_tokens().empty())
					properties.push_back(property);
			}
			else
			{
				bool end_of_scope = CL_CSSDocument2_Impl::read_end_of_statement(tokenizer, token);
				if (end_of_scope)
					break;
			}
		}
		else if (token.type == CL_CSSToken::type_null)
		{
			break;
		}
	}
	return properties;
}

/////////////////////////////////////////////////////////////////////////////

std::vector<CL_CSSRulesetMatch2> CL_CSSDocument2_Impl::select_rulesets(CL_CSSSelectNode2 *node, const CL_String &pseudo_element)
{
	std::vector<CL_CSSRulesetMatch2> matched_rulesets;
	for (size_t i = 0; i < rulesets.size(); i++)
	{
		CL_CSSRuleset2 &cur_ruleset = rulesets[i];
		for (size_t j = 0; j < cur_ruleset.selectors.size(); j++)
		{
			const CL_CSSSelectorChain2 &chain = cur_ruleset.selectors[j];
			if (chain.pseudo_element == pseudo_element)
			{
				bool matches = try_match_chain(chain, node, chain.links.size());
				if (matches)
				{
					matched_rulesets.push_back(CL_CSSRulesetMatch2(&cur_ruleset, j, matched_rulesets.size()));
					break;
				}
			}
		}
	}
	std::sort(matched_rulesets.begin(), matched_rulesets.end());
	return matched_rulesets;
}

bool CL_CSSDocument2_Impl::try_match_chain(const CL_CSSSelectorChain2 &chain, CL_CSSSelectNode2 *node, size_t chain_index)
{
	if (chain_index > 0)
	{
		const CL_CSSSelectorLink2 &link = chain.links[chain_index-1];
		if (link.type == CL_CSSSelectorLink2::type_child_combinator)
		{
			CL_CSSSelectNode2 *parent_node = node->get_parent();
			if (parent_node)
			{
				bool matches = try_match_chain(chain, parent_node, chain_index-1);
				delete parent_node;
				return matches;
			}
			else
			{
				return false;
			}
		}
		else if (link.type == CL_CSSSelectorLink2::type_descendant_combinator)
		{
			CL_CSSSelectNode2 *parent_node = node->get_parent();
			while (parent_node)
			{
				bool matches = try_match_chain(chain, parent_node, chain_index-1);
				if (matches)
				{
					delete parent_node;
					return true;
				}
				else
				{
					CL_CSSSelectNode2 *cur = parent_node;
					parent_node = cur->get_parent();
					delete cur;
				}
			}
			return false;
		}
		else if (link.type == CL_CSSSelectorLink2::type_next_sibling_combinator)
		{
			CL_CSSSelectNode2 *sibling_node = node->get_prev_sibling();
			if (sibling_node)
			{
				bool matches = try_match_chain(chain, sibling_node, chain_index-1);
				delete sibling_node;
				return matches;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (try_match_link(link, node))
				return try_match_chain(chain, node, chain_index-1);
			else
				return false;
		}
	}
	else
	{
		return true;
	}
}

bool CL_CSSDocument2_Impl::try_match_link(const CL_CSSSelectorLink2 &link, const CL_CSSSelectNode2 *node)
{
	bool match = false;
	if ((link.type == CL_CSSSelectorLink2::type_simple_selector && link.element_name == node->name) ||
		link.type == CL_CSSSelectorLink2::type_universal_selector)
	{
		match = true;
	}

	if (!link.element_id.empty() && link.element_id != node->id)
	{
		match = false;
	}
	if (!link.element_lang.empty() && link.element_lang != node->lang)
	{
		match = false;
	}
	for (size_t k = 0; k < link.element_classes.size(); k++)
	{
		bool found = false;
		for (size_t t = 0; t < node->element_classes.size(); t++)
		{
			if (link.element_classes[k] == node->element_classes[t])
			{
				found = true;
				break;
			}
		}
		if (!found)
			match = false;
	}
	for (size_t k = 0; k < link.pseudo_classes.size(); k++)
	{
		bool found = false;
		for (size_t t = 0; t < node->pseudo_classes.size(); t++)
		{
			if (link.pseudo_classes[k] == node->pseudo_classes[t])
			{
				found = true;
				break;
			}
		}
		if (!found)
			match = false;
	}
	for (size_t k = 0; k < link.attribute_selectors.size(); k++)
	{
		bool found = false;
		for (size_t t = 0; t < node->attributes.size(); t++)
		{
			if (link.attribute_selectors[k].type == CL_CSSAttributeSelector2::type_set &&
				link.attribute_selectors[k].name == node->attributes[t].name &&
				link.attribute_selectors[k].value == node->attributes[t].value)
			{
				found = true;
				break;
			}
		}
		if (!found)
			match = false;
	}
	return match;
}

void CL_CSSDocument2_Impl::read_stylesheet(CL_CSSTokenizer &tokenizer)
{
	CL_CSSToken token;
	while (true)
	{
		tokenizer.read(token, true);
		if (token.type == CL_CSSToken::type_null)
		{
			break;
		}
		else if (token.type == CL_CSSToken::type_atkeyword)
		{
			read_at_rule(tokenizer, token);
		}
		else if (token.type != CL_CSSToken::type_whitespace)
		{
			read_statement(tokenizer, token);
		}
	}
}

void CL_CSSDocument2_Impl::read_at_rule(CL_CSSTokenizer &tokenizer, CL_CSSToken &token)
{
	// CL_Console::write_line("@%1", token.value);
	int curly_count = 0;
	while (true)
	{
		tokenizer.read(token, false);
		if (token.type == CL_CSSToken::type_null)
		{
			break;
		}
		else if (token.type == CL_CSSToken::type_semi_colon && curly_count == 0)
		{
			break;
		}
		else if (token.type == CL_CSSToken::type_curly_brace_begin)
		{
			curly_count++;
		}
		else if (token.type == CL_CSSToken::type_curly_brace_end)
		{
			curly_count--;
			if (curly_count == 0)
				break;
		}
	}
}

bool CL_CSSDocument2_Impl::read_selector_chain(CL_CSSTokenizer &tokenizer, CL_CSSToken &token, CL_CSSSelectorChain2 &out_selector_chain)
{
	out_selector_chain.links.clear();
	while (true)
	{
		if (!out_selector_chain.links.empty())
		{
			bool whitespace = false;
			if (token.type == CL_CSSToken::type_whitespace)
			{
				tokenizer.read(token, true);
				if (token.type != CL_CSSToken::type_curly_brace_begin)
					whitespace = true;
			}

			if (token.type == CL_CSSToken::type_delim && token.value == ">")
			{
				CL_CSSSelectorLink2 combinator_link;
				combinator_link.type = CL_CSSSelectorLink2::type_child_combinator;
				out_selector_chain.links.push_back(combinator_link);
			}
			else if (token.type == CL_CSSToken::type_delim && token.value == "+")
			{
				CL_CSSSelectorLink2 combinator_link;
				combinator_link.type = CL_CSSSelectorLink2::type_next_sibling_combinator;
				out_selector_chain.links.push_back(combinator_link);
			}
			else if (whitespace)
			{
				CL_CSSSelectorLink2 combinator_link;
				combinator_link.type = CL_CSSSelectorLink2::type_descendant_combinator;
				out_selector_chain.links.push_back(combinator_link);
			}
			else
			{
				break;
			}

			if (!whitespace)
				tokenizer.read(token, true);
		}

		CL_CSSSelectorLink2 selector_link;
		if (token.type == CL_CSSToken::type_ident)
		{
			// Simple Selector
			selector_link.type = CL_CSSSelectorLink2::type_simple_selector;
			selector_link.element_name = token.value;
			tokenizer.read(token, false);
		}
		else if (token.type == CL_CSSToken::type_delim && token.value == "*")
		{
			// Universal Selector
			selector_link.type = CL_CSSSelectorLink2::type_universal_selector;
			tokenizer.read(token, false);
		}
		else if (token.type == CL_CSSToken::type_hash ||
			token.type == CL_CSSToken::type_colon ||
			(token.type == CL_CSSToken::type_delim && token.value == "."))
		{
			// Implicit Universal Selector
			selector_link.type = CL_CSSSelectorLink2::type_universal_selector;
		}
		else
		{
			return false;
		}

		while (true)
		{
			if (token.type == CL_CSSToken::type_hash && selector_link.element_id.empty())
			{
				selector_link.element_id = token.value;
			}
			else if (token.type == CL_CSSToken::type_colon)
			{
				tokenizer.read(token, false);
				if (token.type == CL_CSSToken::type_ident)
				{
					if (token.value == "before" ||
						token.value == "after")
					{
						// CSS2 pseudo-element
						out_selector_chain.pseudo_element = token.value;
					}
					else
					{
						selector_link.pseudo_classes.push_back(token.value);
					}
				}
				else if (token.type == CL_CSSToken::type_colon)
				{
					// CSS3 pseudo-element.  Currently not supported.
					return false;
				}
				else if (token.type == CL_CSSToken::type_function)
				{
					// CSS2 :lang(en).  Lots more in CSS3.  Currently not supported.
					return false;
				}
			}
			else if (token.type == CL_CSSToken::type_delim && token.value == ".")
			{
				tokenizer.read(token, false);
				if (token.type == CL_CSSToken::type_ident)
				{
					selector_link.element_classes.push_back(token.value);
				}
				else
				{
					return false;
				}
			}
			/*else if (token.type == CL_CSSToken::type_square_bracket_begin)
			{
				if (read_attribute_selector(tokenizer, token, selector_link) == false)
					return false;
			}*/
			else
			{
				break;
			}

			tokenizer.read(token, false);
		}

		out_selector_chain.links.push_back(selector_link);
	}
	return true;
}

void CL_CSSDocument2_Impl::read_statement(CL_CSSTokenizer &tokenizer, CL_CSSToken &token)
{
	CL_CSSRuleset2 ruleset;
	while (true)
	{
		CL_CSSSelectorChain2 selector_chain;
		if (read_selector_chain(tokenizer, token, selector_chain))
		{
			ruleset.selectors.push_back(selector_chain);
			if (token.type == CL_CSSToken::type_delim && token.value == ",")
			{
				tokenizer.read(token, true);
			}
			else if (token.type == CL_CSSToken::type_curly_brace_begin)
			{
				break;
			}
			else
			{
				read_end_of_statement(tokenizer, token);
				return;
			}
		}
		else
		{
			read_end_of_statement(tokenizer, token);
			return;
		}
	}

	if (token.type == CL_CSSToken::type_curly_brace_begin)
	{
		while (true)
		{
			tokenizer.read(token, true);
			if (token.type == CL_CSSToken::type_ident)
			{
				CL_String property_name = token.value;
				tokenizer.read(token, true);
				if (token.type == CL_CSSToken::type_colon)
				{
					tokenizer.read(token, true);

					CL_CSSProperty2 property;
					property.set_name(property_name);
					bool end_of_scope = read_property_value(tokenizer, token, property);
					if (!property.get_value_tokens().empty())
						ruleset.properties.push_back(property);
					if (end_of_scope)
						break;
				}
				else
				{
					bool end_of_scope = read_end_of_statement(tokenizer, token);
					if (end_of_scope)
						break;
				}
			}
			else if (token.type == CL_CSSToken::type_curly_brace_end)
			{
				break;
			}
			else if (token.type == CL_CSSToken::type_null)
			{
				return;
			}
		}
		rulesets.push_back(ruleset);
	}
	else
	{
		read_end_of_statement(tokenizer, token);
	}
}

bool CL_CSSDocument2_Impl::read_property_value(CL_CSSTokenizer &tokenizer, CL_CSSToken &token, CL_CSSProperty2 &property)
{
	property.get_value_tokens().clear();
	int curly_count = 0;
	while (true)
	{
		if (token.type == CL_CSSToken::type_null)
		{
			break;
		}
		else if (token.type == CL_CSSToken::type_curly_brace_begin)
		{
			curly_count++;
		}
		else if (token.type == CL_CSSToken::type_curly_brace_end)
		{
			curly_count--;
			if (curly_count <= 0)
				break;
		}
		else if (token.type == CL_CSSToken::type_semi_colon)
		{
			if (curly_count == 0)
				break;
		}
		property.get_value_tokens().push_back(token);
		tokenizer.read(token, false);
	}

	// Remove the !important flag if found:
	size_t tokens_size = property.get_value_tokens().size();
	if (tokens_size >= 2 &&
		property.get_value_tokens()[tokens_size-2].type == CL_CSSToken::type_delim && property.get_value_tokens()[tokens_size-2].value == "!" &&
		property.get_value_tokens()[tokens_size-1].type == CL_CSSToken::type_ident && property.get_value_tokens()[tokens_size-1].value == "important")
	{
		property.set_important(true);
		property.get_value_tokens().pop_back();
		property.get_value_tokens().pop_back();
	}

	// Remove any possible whitespace at the beginning of the property:
	while (!property.get_value_tokens().empty() && property.get_value_tokens().front().type == CL_CSSToken::type_whitespace)
		property.get_value_tokens().erase(property.get_value_tokens().begin());

	// Remove any possible whitespace at the end of the property:
	if (!property.get_value_tokens().empty() && property.get_value_tokens().back().type == CL_CSSToken::type_whitespace)
		property.get_value_tokens().pop_back();

	return curly_count < 0;
}

bool CL_CSSDocument2_Impl::read_end_of_statement(CL_CSSTokenizer &tokenizer, CL_CSSToken &token)
{
	int curly_count = 0;
	while (true)
	{
		if (token.type == CL_CSSToken::type_null)
		{
			break;
		}
		else if (token.type == CL_CSSToken::type_curly_brace_begin)
		{
			curly_count++;
		}
		else if (token.type == CL_CSSToken::type_curly_brace_end)
		{
			curly_count--;
			if (curly_count <= 0)
				break;
		}
		else if (token.type == CL_CSSToken::type_semi_colon)
		{
			if (curly_count == 0)
				break;
		}
		tokenizer.read(token, true);
	}

	return curly_count < 0;
}

CL_String CL_CSSDocument2_Impl::to_string(const CL_CSSToken &token)
{
	switch (token.type)
	{
	case CL_CSSToken::type_null:                   return cl_format("null");
	case CL_CSSToken::type_ident:                  return cl_format("%1", token.value);
	case CL_CSSToken::type_atkeyword:              return cl_format("@%1", token.value);
	case CL_CSSToken::type_string:                 return cl_format("%1", token.value);
	case CL_CSSToken::type_invalid:                return cl_format("invalid");
	case CL_CSSToken::type_hash:                   return cl_format("#%1", token.value);
	case CL_CSSToken::type_number:                 return cl_format("%1", token.value);
	case CL_CSSToken::type_percentage:             return cl_format("%1%%", token.value);
	case CL_CSSToken::type_dimension:              return cl_format("%1%2", token.value, token.dimension);
	case CL_CSSToken::type_uri:                    return cl_format("uri(%1)", token.value);
	case CL_CSSToken::type_unicode_range:          return cl_format("unicode-range");
	case CL_CSSToken::type_cdo:                    return cl_format("<!--");
	case CL_CSSToken::type_cdc:                    return cl_format("-->");
	case CL_CSSToken::type_colon:                  return cl_format(":");
	case CL_CSSToken::type_semi_colon:             return cl_format(";");
	case CL_CSSToken::type_curly_brace_begin:      return cl_format("{");
	case CL_CSSToken::type_curly_brace_end:        return cl_format("}");
	case CL_CSSToken::type_bracket_begin:          return cl_format("(");
	case CL_CSSToken::type_bracket_end:            return cl_format(")");
	case CL_CSSToken::type_square_bracket_begin:   return cl_format("[");
	case CL_CSSToken::type_square_bracket_end:     return cl_format("]");
	case CL_CSSToken::type_whitespace:             return cl_format(" ");
	case CL_CSSToken::type_comment:                return cl_format("/* */");
	case CL_CSSToken::type_function:               return cl_format("function");
	case CL_CSSToken::type_includes:               return cl_format("includes");
	case CL_CSSToken::type_dashmatch:              return cl_format("dashmatch");
	case CL_CSSToken::type_delim:                  return cl_format("%1", token.value);
	default:
		return "Unknown";
	}
}
