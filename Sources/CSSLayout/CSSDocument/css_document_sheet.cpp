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
#include "css_document_sheet.h"
#include "css_ruleset_match.h"
#include "API/Core/IOData/html_url.h"

namespace clan
{

CSSDocumentSheet::CSSDocumentSheet(CSSSheetOrigin origin, CSSTokenizer &tokenizer, const std::string &base_uri)
: origin(origin), base_uri(base_uri)
{
	read_stylesheet(tokenizer);
}

std::vector<CSSRulesetMatch> CSSDocumentSheet::select_rulesets(CSSSelectNode *node, const std::string &pseudo_element)
{
	std::vector<CSSRulesetMatch> matched_rulesets;
	for (size_t i = 0; i < rulesets.size(); i++)
	{
		CSSRuleset *cur_ruleset = rulesets[i].get();
		for (size_t j = 0; j < cur_ruleset->selectors.size(); j++)
		{
			const CSSSelectorChain &chain = cur_ruleset->selectors[j];
			if (equals(chain.pseudo_element, pseudo_element))
			{
				bool matches = try_match_chain(chain, node, chain.links.size());
				if (matches)
				{
					matched_rulesets.push_back(CSSRulesetMatch(cur_ruleset, j, matched_rulesets.size()));
					break;
				}
			}
		}
	}
	std::sort(matched_rulesets.begin(), matched_rulesets.end());
	return matched_rulesets;
}

bool CSSDocumentSheet::try_match_chain(const CSSSelectorChain &chain, CSSSelectNode *node, size_t chain_index)
{
	bool matches = false;
	if (chain_index > 0)
	{
		const CSSSelectorLink &link = chain.links[chain_index-1];
		if (link.type == CSSSelectorLink::type_child_combinator)
		{
			node->push();
			if (node->parent())
			{
				matches = try_match_chain(chain, node, chain_index-1);
			}
			node->pop();
		}
		else if (link.type == CSSSelectorLink::type_descendant_combinator)
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
		else if (link.type == CSSSelectorLink::type_next_sibling_combinator)
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

bool CSSDocumentSheet::try_match_link(const CSSSelectorLink &link, CSSSelectNode *node)
{
	if ((link.type == CSSSelectorLink::type_simple_selector && equals(link.element_name, node->name())) ||
		link.type == CSSSelectorLink::type_universal_selector)
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

	std::vector<std::string> element_classes;
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

	std::vector<std::string> pseudo_classes;
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
		std::string value = node->get_attribute_value(link.attribute_selectors[k].name, found);
		if (found)
		{
			found = false;
			switch (link.attribute_selectors[k].type)
			{
			case CSSAttributeSelector::type_set:
				found = true;
				break;

			case CSSAttributeSelector::type_exact_value:
				if (equals(link.attribute_selectors[k].value, value))
					found = true;
				break;

			case CSSAttributeSelector::type_space_separated_value:
				{
					std::vector<std::string> values = StringHelp::split_text(value, " ");
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

			case CSSAttributeSelector::type_hyphen_separated_value:
				{
					std::vector<std::string> values = StringHelp::split_text(value, "-");
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

			case CSSAttributeSelector::type_lang_value:
				{
					std::string::size_type len = link.attribute_selectors[k].value.length();
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

void CSSDocumentSheet::read_stylesheet(CSSTokenizer &tokenizer)
{
	CSSToken token;
	while (true)
	{
		tokenizer.read(token, true);
		if (token.type == CSSToken::type_null)
		{
			break;
		}
		else if (token.type == CSSToken::type_atkeyword)
		{
			read_at_rule(tokenizer, token);
		}
		else if (token.type != CSSToken::type_whitespace)
		{
			read_statement(tokenizer, token);
		}
	}
}

void CSSDocumentSheet::read_at_rule(CSSTokenizer &tokenizer, CSSToken &token)
{
	if (equals(token.value, "-clan-base-uri"))
	{
		tokenizer.read(token, true);
		if (token.type == CSSToken::type_string)
		{
			base_uri = token.value;
			tokenizer.read(token, true);
		}
	}
	else if (equals(token.value, "media"))
	{
		tokenizer.read(token, true);

		std::vector<std::string> media_types;
		while (token.type != CSSToken::type_curly_brace_begin)
		{
			if (token.type != CSSToken::type_ident)
			{
				read_end_of_at_rule(tokenizer, token);
				return;
			}
			media_types.push_back(token.value);

			tokenizer.read(token, true);
			if (token.type == CSSToken::type_curly_brace_begin)
			{
				break;
			}
			else if (token.type != CSSToken::type_delim || token.value != ",")
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
				if (token.type == CSSToken::type_null)
				{
					// Invalid media section. Rollback any rulesets added in it:
					rulesets.erase(rulesets.begin() + rollback_index, rulesets.end());
					break;
				}
				else if (token.type == CSSToken::type_curly_brace_end)
				{
					break;
				}
				else if (token.type == CSSToken::type_atkeyword)
				{
					read_end_of_at_rule(tokenizer, token);
				}
				else if (token.type != CSSToken::type_whitespace)
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

void CSSDocumentSheet::read_end_of_at_rule(CSSTokenizer &tokenizer, CSSToken &token)
{
	// Console::write_line("@%1", token.value);
	int curly_count = 0;
	while (true)
	{
		if (token.type == CSSToken::type_null)
		{
			break;
		}
		else if (token.type == CSSToken::type_semi_colon && curly_count == 0)
		{
			break;
		}
		else if (token.type == CSSToken::type_curly_brace_begin)
		{
			curly_count++;
		}
		else if (token.type == CSSToken::type_curly_brace_end)
		{
			curly_count--;
			if (curly_count == 0)
				break;
		}
		tokenizer.read(token, false);
	}
}


bool CSSDocumentSheet::read_selector_chain(CSSTokenizer &tokenizer, CSSToken &token, CSSSelectorChain &out_selector_chain)
{
	out_selector_chain.links.clear();
	while (true)
	{
		if (!out_selector_chain.links.empty())
		{
			bool whitespace = false;
			if (token.type == CSSToken::type_whitespace)
			{
				while (token.type == CSSToken::type_whitespace)
					tokenizer.read(token, true);
				if (token.type != CSSToken::type_curly_brace_begin && !(token.type == CSSToken::type_delim && token.value == ","))
					whitespace = true;
			}

			if (token.type == CSSToken::type_delim && token.value == ">")
			{
				CSSSelectorLink combinator_link;
				combinator_link.type = CSSSelectorLink::type_child_combinator;
				out_selector_chain.links.push_back(combinator_link);
				whitespace = false;
			}
			else if (token.type == CSSToken::type_delim && token.value == "+")
			{
				CSSSelectorLink combinator_link;
				combinator_link.type = CSSSelectorLink::type_next_sibling_combinator;
				out_selector_chain.links.push_back(combinator_link);
				whitespace = false;
			}
			else if (whitespace)
			{
				CSSSelectorLink combinator_link;
				combinator_link.type = CSSSelectorLink::type_descendant_combinator;
				out_selector_chain.links.push_back(combinator_link);
			}
			else
			{
				break;
			}

			if (!whitespace)
				tokenizer.read(token, true);
		}

		CSSSelectorLink selector_link;
		if (token.type == CSSToken::type_ident)
		{
			// Simple Selector
			selector_link.type = CSSSelectorLink::type_simple_selector;
			selector_link.element_name = token.value;
			tokenizer.read(token, false);
		}
		else if (token.type == CSSToken::type_delim && token.value == "*")
		{
			// Universal Selector
			selector_link.type = CSSSelectorLink::type_universal_selector;
			tokenizer.read(token, false);
		}
		else if (token.type == CSSToken::type_hash ||
			token.type == CSSToken::type_colon ||
			token.type == CSSToken::type_square_bracket_begin ||
			(token.type == CSSToken::type_delim && token.value == "."))
		{
			// Implicit Universal Selector
			selector_link.type = CSSSelectorLink::type_universal_selector;
		}
		else
		{
			return false;
		}

		while (true)
		{
			if (token.type == CSSToken::type_hash && selector_link.element_id.empty())
			{
				selector_link.element_id = token.value;
			}
			else if (token.type == CSSToken::type_colon)
			{
				tokenizer.read(token, false);
				if (token.type == CSSToken::type_ident)
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
				else if (token.type == CSSToken::type_colon)
				{
					// CSS3 pseudo-element.  Currently not supported.
					return false;
				}
				else if (token.type == CSSToken::type_function)
				{
					// CSS2 :lang(en).  Lots more in CSS3.  Currently not supported.
					return false;
				}
			}
			else if (token.type == CSSToken::type_delim && token.value == ".")
			{
				tokenizer.read(token, false);
				if (token.type == CSSToken::type_ident)
				{
					selector_link.element_classes.push_back(token.value);
				}
				else
				{
					return false;
				}
			}
			else if (token.type == CSSToken::type_square_bracket_begin)
			{
				CSSAttributeSelector attribute;

				tokenizer.read(token, true);
				if (token.type == CSSToken::type_ident)
				{
					attribute.name = token.value;
				}
				else
				{
					return false;
				}

				tokenizer.read(token, true);

				bool read_value = true;
				if (token.type == CSSToken::type_includes)
				{
					attribute.type = CSSAttributeSelector::type_space_separated_value;
				}
				else if (token.type == CSSToken::type_dashmatch)
				{
					attribute.type = CSSAttributeSelector::type_lang_value;
				}
				else if (token.type == CSSToken::type_delim && token.value == "=")
				{
					attribute.type = CSSAttributeSelector::type_exact_value;
				}
				else if (token.type == CSSToken::type_square_bracket_end)
				{
					attribute.type = CSSAttributeSelector::type_set;
					read_value = false;
				}
				else
				{
					return false;
				}

				if (read_value)
				{
					tokenizer.read(token, true);
					if (token.type == CSSToken::type_ident || token.type == CSSToken::type_string)
					{
						attribute.value = token.value;
					}
					else
					{
						return false;
					}

					tokenizer.read(token, true);
					if (token.type != CSSToken::type_square_bracket_end)
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

void CSSDocumentSheet::read_statement(CSSTokenizer &tokenizer, CSSToken &token)
{
	std::shared_ptr<CSSRuleset> ruleset(new CSSRuleset(this));
	while (true)
	{
		CSSSelectorChain selector_chain;
		if (read_selector_chain(tokenizer, token, selector_chain))
		{
			ruleset->selectors.push_back(selector_chain);
			if (token.type == CSSToken::type_delim && token.value == ",")
			{
				tokenizer.read(token, true);
			}
			else if (token.type == CSSToken::type_curly_brace_begin)
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

	if (token.type == CSSToken::type_curly_brace_begin)
	{
		while (true)
		{
			tokenizer.read(token, true);
			if (token.type == CSSToken::type_ident)
			{
				std::string property_name = token.value;
				tokenizer.read(token, true);
				if (token.type == CSSToken::type_colon)
				{
					tokenizer.read(token, true);

					CSSProperty property;
					property.set_name(property_name);
					bool end_of_scope = read_property_value(tokenizer, token, property, base_uri);
					if (!property.get_value_tokens().empty())
						parsers.parse(property, ruleset->values);
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
			else if (token.type == CSSToken::type_curly_brace_end)
			{
				break;
			}
			else if (token.type == CSSToken::type_null)
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

bool CSSDocumentSheet::read_property_value(CSSTokenizer &tokenizer, CSSToken &token, CSSProperty &property, std::string base_uri)
{
	property.get_value_tokens().clear();
	int curly_count = 0;
	while (true)
	{
		if (token.type == CSSToken::type_null)
		{
			break;
		}
		else if (token.type == CSSToken::type_curly_brace_begin)
		{
			curly_count++;
		}
		else if (token.type == CSSToken::type_curly_brace_end)
		{
			curly_count--;
			if (curly_count <= 0)
				break;
		}
		else if (token.type == CSSToken::type_semi_colon)
		{
			if (curly_count == 0)
				break;
		}

		if (token.type == CSSToken::type_uri)
			token.value = make_absolute_uri(token.value, base_uri);

		property.get_value_tokens().push_back(token);
		tokenizer.read(token, false);
	}

	// Remove the !important flag if found:
	size_t tokens_size = property.get_value_tokens().size();
	if (tokens_size >= 2 &&
		property.get_value_tokens()[tokens_size-2].type == CSSToken::type_delim && property.get_value_tokens()[tokens_size-2].value == "!" &&
		property.get_value_tokens()[tokens_size-1].type == CSSToken::type_ident && equals(property.get_value_tokens()[tokens_size-1].value, "important"))
	{
		property.set_important(true);
		property.get_value_tokens().pop_back();
		property.get_value_tokens().pop_back();
	}
	else if (tokens_size >= 3 &&
		property.get_value_tokens()[tokens_size-3].type == CSSToken::type_delim && property.get_value_tokens()[tokens_size-3].value == "!" &&
		property.get_value_tokens()[tokens_size-2].type == CSSToken::type_whitespace &&
		property.get_value_tokens()[tokens_size-1].type == CSSToken::type_ident && equals(property.get_value_tokens()[tokens_size-1].value, "important"))
	{
		property.set_important(true);
		property.get_value_tokens().pop_back();
		property.get_value_tokens().pop_back();
		property.get_value_tokens().pop_back();
	}

	// Remove any possible whitespace at the beginning of the property:
	while (!property.get_value_tokens().empty() && property.get_value_tokens().front().type == CSSToken::type_whitespace)
		property.get_value_tokens().erase(property.get_value_tokens().begin());

	// Remove any possible whitespace at the end of the property:
	while (!property.get_value_tokens().empty() && property.get_value_tokens().back().type == CSSToken::type_whitespace)
		property.get_value_tokens().pop_back();

	return curly_count < 0;
}

bool CSSDocumentSheet::read_end_of_statement(CSSTokenizer &tokenizer, CSSToken &token)
{
	int curly_count = 0;
	while (true)
	{
		if (token.type == CSSToken::type_null)
		{
			break;
		}
		else if (token.type == CSSToken::type_curly_brace_begin)
		{
			curly_count++;
		}
		else if (token.type == CSSToken::type_curly_brace_end)
		{
			curly_count--;
			if (curly_count <= 0)
				break;
		}
		else if (token.type == CSSToken::type_semi_colon)
		{
			if (curly_count == 0)
				break;
		}
		tokenizer.read(token, true);
	}

	return curly_count < 0;
}

std::string CSSDocumentSheet::to_string(const CSSToken &token)
{
	switch (token.type)
	{
	case CSSToken::type_null:                   return string_format("null");
	case CSSToken::type_ident:                  return string_format("%1", token.value);
	case CSSToken::type_atkeyword:              return string_format("@%1", token.value);
	case CSSToken::type_string:                 return string_format("%1", token.value);
	case CSSToken::type_invalid:                return string_format("invalid");
	case CSSToken::type_hash:                   return string_format("#%1", token.value);
	case CSSToken::type_number:                 return string_format("%1", token.value);
	case CSSToken::type_percentage:             return string_format("%1%%", token.value);
	case CSSToken::type_dimension:              return string_format("%1%2", token.value, token.dimension);
	case CSSToken::type_uri:                    return string_format("uri(%1)", token.value);
	case CSSToken::type_unicode_range:          return string_format("unicode-range");
	case CSSToken::type_cdo:                    return string_format("<!--");
	case CSSToken::type_cdc:                    return string_format("-->");
	case CSSToken::type_colon:                  return string_format(":");
	case CSSToken::type_semi_colon:             return string_format(";");
	case CSSToken::type_curly_brace_begin:      return string_format("{");
	case CSSToken::type_curly_brace_end:        return string_format("}");
	case CSSToken::type_bracket_begin:          return string_format("(");
	case CSSToken::type_bracket_end:            return string_format(")");
	case CSSToken::type_square_bracket_begin:   return string_format("[");
	case CSSToken::type_square_bracket_end:     return string_format("]");
	case CSSToken::type_whitespace:             return string_format(" ");
	case CSSToken::type_comment:                return string_format("/* */");
	case CSSToken::type_function:               return string_format("function");
	case CSSToken::type_includes:               return string_format("includes");
	case CSSToken::type_dashmatch:              return string_format("dashmatch");
	case CSSToken::type_delim:                  return string_format("%1", token.value);
	default:
		return "Unknown";
	}
}

bool CSSDocumentSheet::equals(const std::string &s1, const std::string &s2)
{
	return StringHelp::compare(s1, s2, true) == 0;
}

std::string CSSDocumentSheet::make_absolute_uri(std::string uri, std::string base_uri)
{
	// Hack for avoiding base paths added to resource schemes.
	HTMLUrl url(uri, base_uri);
	if (url.scheme == "res")
		return HTMLUrl(uri).to_string();
	else
		return HTMLUrl(uri, base_uri).to_string();
}

}
