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
#include "css_document2_impl.h"
#include "API/Core/IOData/html_url.h"

std::vector<CL_CSSRulesetMatch2> CL_CSSDocument2_Impl::select_rulesets(CL_CSSSelectNode2 *node, const CL_String &pseudo_element)
{
	std::vector<CL_CSSRulesetMatch2> matched_rulesets;
	for (size_t i = 0; i < rulesets.size(); i++)
	{
		CL_CSSRuleset2 &cur_ruleset = rulesets[i];
		for (size_t j = 0; j < cur_ruleset.selectors.size(); j++)
		{
			const CL_CSSSelectorChain2 &chain = cur_ruleset.selectors[j];
			if (equals(chain.pseudo_element, pseudo_element))
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
	bool matches = false;
	if (chain_index > 0)
	{
		const CL_CSSSelectorLink2 &link = chain.links[chain_index-1];
		if (link.type == CL_CSSSelectorLink2::type_child_combinator)
		{
			node->push();
			if (node->parent())
			{
				matches = try_match_chain(chain, node, chain_index-1);
			}
			node->pop();
		}
		else if (link.type == CL_CSSSelectorLink2::type_descendant_combinator)
		{
			node->push();
			while (node->parent())
			{
				matches = try_match_chain(chain, node, chain_index-1);
				if (matches)
					break;
			}
			node->pop();
		}
		else if (link.type == CL_CSSSelectorLink2::type_next_sibling_combinator)
		{
			node->push();
			if (node->prev_sibling())
			{
				matches = try_match_chain(chain, node, chain_index-1);
			}
			node->pop();
		}
		else
		{
			if (try_match_link(link, node))
			{
				matches = try_match_chain(chain, node, chain_index-1);
			}
		}
	}
	else
	{
		matches = true;
	}
	return matches;
}

bool CL_CSSDocument2_Impl::try_match_link(const CL_CSSSelectorLink2 &link, CL_CSSSelectNode2 *node)
{
	if ((link.type == CL_CSSSelectorLink2::type_simple_selector && equals(link.element_name, node->name())) ||
		link.type == CL_CSSSelectorLink2::type_universal_selector)
	{
	}
	else
	{
		return false;
	}

	if (!link.element_id.empty() && link.element_id != node->id())
	{
		return false;
	}
	if (!link.element_lang.empty() && link.element_lang != node->lang())
	{
		return false;
	}

	std::vector<CL_String> element_classes;
	if (!link.element_classes.empty())
		element_classes = node->element_classes();

	for (size_t k = 0; k < link.element_classes.size(); k++)
	{
		bool found = false;
		for (size_t t = 0; t < element_classes.size(); t++)
		{
			if (equals(link.element_classes[k], element_classes[t]))
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			return false;
		}
	}

	std::vector<CL_String> pseudo_classes;
	if (!link.pseudo_classes.empty())
		pseudo_classes = node->pseudo_classes();

	for (size_t k = 0; k < link.pseudo_classes.size(); k++)
	{
		bool found = false;
		for (size_t t = 0; t < pseudo_classes.size(); t++)
		{
			if (equals(link.pseudo_classes[k], pseudo_classes[t]))
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			return false;
		}
	}

	for (size_t k = 0; k < link.attribute_selectors.size(); k++)
	{
		bool found = false;
		CL_String value = node->get_attribute_value(link.attribute_selectors[k].name, found);
		if (found)
		{
			found = false;
			switch (link.attribute_selectors[k].type)
			{
			case CL_CSSAttributeSelector2::type_set:
				found = true;
				break;

			case CL_CSSAttributeSelector2::type_exact_value:
				if (equals(link.attribute_selectors[k].value, value))
					found = true;
				break;

			case CL_CSSAttributeSelector2::type_space_separated_value:
				{
					std::vector<CL_String> values = CL_StringHelp::split_text(value, " ");
					for (size_t p = 0; p < values.size(); p++)
					{
						if (equals(link.attribute_selectors[k].value, values[p]))
						{
							found = true;
							break;
						}
					}
				}
				break;

			case CL_CSSAttributeSelector2::type_hyphen_separated_value:
				{
					std::vector<CL_String> values = CL_StringHelp::split_text(value, "-");
					for (size_t p = 0; p < values.size(); p++)
					{
						if (equals(link.attribute_selectors[k].value, values[p]))
						{
							found = true;
							break;
						}
					}
				}
				break;

			case CL_CSSAttributeSelector2::type_lang_value:
				{
					CL_String::size_type len = link.attribute_selectors[k].value.length();
					if (equals(link.attribute_selectors[k].value, value) ||
						(value.length() >= len+1 &&
						value[len] == '-' &&
						equals(link.attribute_selectors[k].value, value.substr(0, len))))
					{
						found = true;
					}
				}
				break;
			}

			if (found)
				break;
		}

		if (!found)
		{
			return false;
		}
	}
	return true;
}

void CL_CSSDocument2_Impl::read_stylesheet(CL_CSSTokenizer &tokenizer, const CL_String &new_base_uri)
{
	base_uri = new_base_uri;
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
	next_origin++;
}

void CL_CSSDocument2_Impl::read_at_rule(CL_CSSTokenizer &tokenizer, CL_CSSToken &token)
{
	if (equals(token.value, "-clan-base-uri"))
	{
		tokenizer.read(token, true);
		if (token.type == CL_CSSToken::type_string)
		{
			base_uri = token.value;
			tokenizer.read(token, true);
		}
	}
	else if (equals(token.value, "media"))
	{
		tokenizer.read(token, true);

		std::vector<CL_String> media_types;
		while (token.type != CL_CSSToken::type_curly_brace_begin)
		{
			if (token.type != CL_CSSToken::type_ident)
			{
				read_end_of_at_rule(tokenizer, token);
				return;
			}
			media_types.push_back(token.value);

			tokenizer.read(token, true);
			if (token.type == CL_CSSToken::type_curly_brace_begin)
			{
				break;
			}
			else if (token.type != CL_CSSToken::type_delim || token.value != ",")
			{
				read_end_of_at_rule(tokenizer, token);
				return;
			}
		}

		bool media_matched = false;
		for (size_t i = 0; i < media_types.size(); i++)
		{
			if (equals(media_types[i], "all") || equals(media_types[i], "screen"))
			{
				media_matched = true;
				break;
			}
		}

		if (media_matched)
		{
			size_t rollback_index = rulesets.size();
			while (true)
			{
				tokenizer.read(token, true);
				if (token.type == CL_CSSToken::type_null)
				{
					// Invalid media section. Rollback any rulesets added in it:
					rulesets.erase(rulesets.begin() + rollback_index, rulesets.end());
					break;
				}
				else if (token.type == CL_CSSToken::type_curly_brace_end)
				{
					break;
				}
				else if (token.type == CL_CSSToken::type_atkeyword)
				{
					read_end_of_at_rule(tokenizer, token);
				}
				else if (token.type != CL_CSSToken::type_whitespace)
				{
					read_statement(tokenizer, token);
				}
			}
		}
		else
		{
			read_end_of_at_rule(tokenizer, token);
		}
	}
	else
	{
		read_end_of_at_rule(tokenizer, token);
	}
}

void CL_CSSDocument2_Impl::read_end_of_at_rule(CL_CSSTokenizer &tokenizer, CL_CSSToken &token)
{
	// CL_Console::write_line("@%1", token.value);
	int curly_count = 0;
	while (true)
	{
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
		tokenizer.read(token, false);
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
				while (token.type == CL_CSSToken::type_whitespace)
					tokenizer.read(token, true);
				if (token.type != CL_CSSToken::type_curly_brace_begin && !(token.type == CL_CSSToken::type_delim && token.value == ","))
					whitespace = true;
			}

			if (token.type == CL_CSSToken::type_delim && token.value == ">")
			{
				CL_CSSSelectorLink2 combinator_link;
				combinator_link.type = CL_CSSSelectorLink2::type_child_combinator;
				out_selector_chain.links.push_back(combinator_link);
				whitespace = false;
			}
			else if (token.type == CL_CSSToken::type_delim && token.value == "+")
			{
				CL_CSSSelectorLink2 combinator_link;
				combinator_link.type = CL_CSSSelectorLink2::type_next_sibling_combinator;
				out_selector_chain.links.push_back(combinator_link);
				whitespace = false;
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
			token.type == CL_CSSToken::type_square_bracket_begin ||
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
					if (equals(token.value, "before") ||
						equals(token.value, "after"))
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
			else if (token.type == CL_CSSToken::type_square_bracket_begin)
			{
				CL_CSSAttributeSelector2 attribute;

				tokenizer.read(token, true);
				if (token.type == CL_CSSToken::type_ident)
				{
					attribute.name = token.value;
				}
				else
				{
					return false;
				}

				tokenizer.read(token, true);

				bool read_value = true;
				if (token.type == CL_CSSToken::type_includes)
				{
					attribute.type = CL_CSSAttributeSelector2::type_space_separated_value;
				}
				else if (token.type == CL_CSSToken::type_dashmatch)
				{
					attribute.type = CL_CSSAttributeSelector2::type_lang_value;
				}
				else if (token.type == CL_CSSToken::type_delim && token.value == "=")
				{
					attribute.type = CL_CSSAttributeSelector2::type_exact_value;
				}
				else if (token.type == CL_CSSToken::type_square_bracket_end)
				{
					attribute.type = CL_CSSAttributeSelector2::type_set;
					read_value = false;
				}
				else
				{
					return false;
				}

				if (read_value)
				{
					tokenizer.read(token, true);
					if (token.type == CL_CSSToken::type_ident || token.type == CL_CSSToken::type_string)
					{
						attribute.value = token.value;
					}
					else
					{
						return false;
					}

					tokenizer.read(token, true);
					if (token.type != CL_CSSToken::type_square_bracket_end)
					{
						return false;
					}
				}

				selector_link.attribute_selectors.push_back(attribute);
			}
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
	CL_CSSRuleset2 ruleset(next_origin);
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
					bool end_of_scope = read_property_value(tokenizer, token, property, base_uri);
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

bool CL_CSSDocument2_Impl::read_property_value(CL_CSSTokenizer &tokenizer, CL_CSSToken &token, CL_CSSProperty2 &property, CL_String base_uri)
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

		if (token.type == CL_CSSToken::type_uri)
			token.value = make_absolute_uri(token.value, base_uri);

		property.get_value_tokens().push_back(token);
		tokenizer.read(token, false);
	}

	// Remove the !important flag if found:
	size_t tokens_size = property.get_value_tokens().size();
	if (tokens_size >= 2 &&
		property.get_value_tokens()[tokens_size-2].type == CL_CSSToken::type_delim && property.get_value_tokens()[tokens_size-2].value == "!" &&
		property.get_value_tokens()[tokens_size-1].type == CL_CSSToken::type_ident && equals(property.get_value_tokens()[tokens_size-1].value, "important"))
	{
		property.set_important(true);
		property.get_value_tokens().pop_back();
		property.get_value_tokens().pop_back();
	}
	else if (tokens_size >= 3 &&
		property.get_value_tokens()[tokens_size-3].type == CL_CSSToken::type_delim && property.get_value_tokens()[tokens_size-3].value == "!" &&
		property.get_value_tokens()[tokens_size-2].type == CL_CSSToken::type_whitespace &&
		property.get_value_tokens()[tokens_size-1].type == CL_CSSToken::type_ident && equals(property.get_value_tokens()[tokens_size-1].value, "important"))
	{
		property.set_important(true);
		property.get_value_tokens().pop_back();
		property.get_value_tokens().pop_back();
		property.get_value_tokens().pop_back();
	}

	// Remove any possible whitespace at the beginning of the property:
	while (!property.get_value_tokens().empty() && property.get_value_tokens().front().type == CL_CSSToken::type_whitespace)
		property.get_value_tokens().erase(property.get_value_tokens().begin());

	// Remove any possible whitespace at the end of the property:
	while (!property.get_value_tokens().empty() && property.get_value_tokens().back().type == CL_CSSToken::type_whitespace)
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

bool CL_CSSDocument2_Impl::equals(const CL_String &s1, const CL_String &s2)
{
	return CL_StringHelp::compare(s1, s2, true) == 0;
}

CL_String CL_CSSDocument2_Impl::make_absolute_uri(CL_String uri, CL_String base_uri)
{
	return CL_HTMLUrl(uri, base_uri).to_string();
}
