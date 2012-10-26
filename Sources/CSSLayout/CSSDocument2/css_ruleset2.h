
#pragma once

#include "css_selector_chain2.h"
#include "API/CSSLayout/css_property2.h"

namespace clan
{

class CSSRuleset2
{
public:
	CSSRuleset2(int origin) : origin(origin) { }
	std::vector<CSSSelectorChain2> selectors;
	std::vector<CSSProperty2> properties;
	int origin;
};

}
