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

#pragma once

#include "API/CSSLayout/css_property2.h"
#include "API/CSSLayout/css_property_list2.h"
#include "API/CSSLayout/css_select_node2.h"
#include "API/CSSLayout/dom_select_node.h"
#include "css_ruleset2.h"
#include "css_selector_chain2.h"
#include "css_selector_link2.h"
#include "css_ruleset_match2.h"
#include <algorithm>

class CL_CSSDocument2_Impl
{
public:
	CL_CSSDocument2_Impl() : next_origin(0) { }
	std::vector<CL_CSSRulesetMatch2> select_rulesets(CL_CSSSelectNode2 *node, const CL_String &pseudo_element);
	bool try_match_chain(const CL_CSSSelectorChain2 &chain, CL_CSSSelectNode2 *node, size_t chain_index);
	bool try_match_link(const CL_CSSSelectorLink2 &link, CL_CSSSelectNode2 *node);
	void read_stylesheet(CL_CSSTokenizer &tokenizer, const CL_String &base_uri);
	void read_at_rule(CL_CSSTokenizer &tokenizer, CL_CSSToken &token);
	void read_statement(CL_CSSTokenizer &tokenizer, CL_CSSToken &token);
	void read_end_of_at_rule(CL_CSSTokenizer &tokenizer, CL_CSSToken &token);
	static bool read_end_of_statement(CL_CSSTokenizer &tokenizer, CL_CSSToken &token);
	bool read_selector_chain(CL_CSSTokenizer &tokenizer, CL_CSSToken &token, CL_CSSSelectorChain2 &out_selector_chain);
	static bool read_property_value(CL_CSSTokenizer &tokenizer, CL_CSSToken &token, CL_CSSProperty2 &property, CL_String base_uri);
	CL_String to_string(const CL_CSSToken &token);
	static bool equals(const CL_String &s1, const CL_String &s2);
	static CL_String make_absolute_uri(CL_String uri, CL_String base_uri);

	CL_String base_uri;
	std::vector<CL_CSSRuleset2> rulesets;
	int next_origin;
};
