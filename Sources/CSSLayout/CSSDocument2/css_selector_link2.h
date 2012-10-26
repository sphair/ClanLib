
#pragma once

#include "css_attribute_selector2.h"

namespace clan
{

class CSSSelectorLink2
{
public:
	CSSSelectorLink2() : type(type_simple_selector) { }

	enum Type
	{
		type_simple_selector,
		type_universal_selector,
		type_descendant_combinator,
		type_child_combinator,
		type_next_sibling_combinator
	};

	Type type;
	std::string element_name;  // E
	std::string element_lang;  // E:lang(en)
	std::string element_id;    // E#myid
	std::vector<std::string> element_classes; // E.myclass.yourclass
	std::vector<std::string> pseudo_classes; // E:active:visited:first-child
	std::vector<CSSAttributeSelector2> attribute_selectors; // E[hello="Cleveland"][goodbye="Columbus"]
};

}
