
#pragma once

#include "css_selector_chain2.h"
#include "API/CSSLayout/css_property2.h"

namespace clan
{

class CSSRuleset
{
public:
	CSSRuleset(int origin) : origin(origin) { }
	std::vector<CSSSelectorChain> selectors;
	std::vector<CSSProperty> properties;
	int origin;
};

}
