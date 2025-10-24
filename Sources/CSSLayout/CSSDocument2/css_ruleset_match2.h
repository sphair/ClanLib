
#pragma once

#include "css_ruleset2.h"

class CL_CSSRulesetMatch2
{
public:
	CL_CSSRulesetMatch2(CL_CSSRuleset2 *ruleset, size_t index_matching_chain, size_t document_order)
	: ruleset(ruleset), specificity(0), document_order(document_order)
	{
		specificity = ruleset->selectors[index_matching_chain].get_specificity();
	}

	CL_CSSRuleset2 *ruleset;
	size_t specificity;
	size_t document_order;

	bool operator <(const CL_CSSRulesetMatch2 &other) const
	{
		if (specificity == other.specificity)
			return document_order < other.document_order;
		else
			return specificity < other.specificity;
	}
};
