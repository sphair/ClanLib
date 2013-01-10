
#pragma once

#include "API/CSSLayout/CSSDocument/css_document.h"
#include "API/CSSLayout/CSSDocument/css_property.h"
#include "API/CSSLayout/CSSDocument/css_property_value_list.h"
#include "API/CSSLayout/CSSDocument/css_select_node.h"
#include "API/CSSLayout/CSSDocument/dom_select_node.h"
#include "CSSLayout/PropertyParsers/css_property_parsers.h"
#include "css_ruleset.h"
#include "css_selector_chain.h"
#include "css_selector_link.h"
#include <algorithm>

namespace clan
{

class CSSRulesetMatch;

class CSSDocumentSheet
{
public:
	CSSDocumentSheet(CSSSheetOrigin origin, CSSTokenizer &tokenizer, const std::string &base_uri);
	std::vector<CSSRulesetMatch> select_rulesets(CSSSelectNode *node, const std::string &pseudo_element);

	CSSSheetOrigin origin;

	static bool read_end_of_statement(CSSTokenizer &tokenizer, CSSToken &token);
	static bool read_property_value(CSSTokenizer &tokenizer, CSSToken &token, CSSProperty &property, std::string base_uri);

private:
	bool try_match_chain(const CSSSelectorChain &chain, CSSSelectNode *node, size_t chain_index);
	bool try_match_link(const CSSSelectorLink &link, CSSSelectNode *node);
	void read_stylesheet(CSSTokenizer &tokenizer);
	void read_at_rule(CSSTokenizer &tokenizer, CSSToken &token);
	void read_statement(CSSTokenizer &tokenizer, CSSToken &token);
	void read_end_of_at_rule(CSSTokenizer &tokenizer, CSSToken &token);
	bool read_selector_chain(CSSTokenizer &tokenizer, CSSToken &token, CSSSelectorChain &out_selector_chain);
	std::string to_string(const CSSToken &token);
	static bool equals(const std::string &s1, const std::string &s2);
	static std::string make_absolute_uri(std::string uri, std::string base_uri);

	std::string base_uri;
	std::vector<std::shared_ptr<CSSRuleset> > rulesets;

	CSSPropertyParsers parsers;
};

}
