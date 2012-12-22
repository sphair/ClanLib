
#pragma once

#include "css_selector_chain.h"
#include "API/CSSLayout/css_property.h"

namespace clan
{

class CSSDocumentSheet;

class CSSRuleset
{
public:
	CSSRuleset(CSSDocumentSheet *sheet) : sheet(sheet) { }
	std::vector<CSSSelectorChain> selectors;
	std::vector<CSSProperty> properties;
	CSSDocumentSheet *sheet;
};

}
