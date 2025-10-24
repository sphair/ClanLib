
#pragma once

#include "css_selector_chain2.h"
#include "API/CSSLayout/css_property2.h"

class CL_CSSRuleset2
{
public:
	CL_CSSRuleset2(int origin) : origin(origin) { }
	std::vector<CL_CSSSelectorChain2> selectors;
	std::vector<CL_CSSProperty2> properties;
	int origin;
};
