
#pragma once

#include "css_selector_chain.h"
#include "API/CSSLayout/CSSDocument/css_property_value.h"

namespace clan
{

class CSSDocumentSheet;

class CSSRuleset
{
public:
	CSSRuleset(CSSDocumentSheet *sheet) : sheet(sheet) { }
	std::vector<CSSSelectorChain> selectors;
	std::vector<std::unique_ptr<CSSPropertyValue> > important_values;
	std::vector<std::unique_ptr<CSSPropertyValue> > values;
	CSSDocumentSheet *sheet;
};

}
