
#pragma once

#include "css_ruleset.h"

namespace clan
{

class CSSRulesetMatch
{
public:
	CSSRulesetMatch(CSSRuleset *ruleset, size_t index_matching_chain, size_t document_order)
	: ruleset(ruleset), specificity(0), document_order(document_order)
	{
		specificity = ruleset->selectors[index_matching_chain].get_specificity();
	}

	CSSRuleset *ruleset;
	size_t specificity;
	size_t document_order;

	bool operator <(const CSSRulesetMatch &other) const
	{
		if (ruleset->origin == other.ruleset->origin)
		{
			if (specificity == other.specificity)
				return document_order < other.document_order;
			else
				return specificity < other.specificity;
		}
		else
		{
			return ruleset->origin < other.ruleset->origin;
		}
	}
};

}
