
#pragma once

#include "css_selector_chain.h"
#include "API/CSSLayout/css_property.h"

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
