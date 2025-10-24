
#pragma once

#include "css_attribute_selector2.h"

class CL_CSSSelectorLink2
{
public:
	CL_CSSSelectorLink2() : type(type_simple_selector) { }

	enum Type
	{
		type_simple_selector,
		type_universal_selector,
		type_descendant_combinator,
		type_child_combinator,
		type_next_sibling_combinator
	};

	Type type;
	CL_String element_name;  // E
	CL_String element_lang;  // E:lang(en)
	CL_String element_id;    // E#myid
	std::vector<CL_String> element_classes; // E.myclass.yourclass
	std::vector<CL_String> pseudo_classes; // E:active:visited:first-child
	std::vector<CL_CSSAttributeSelector2> attribute_selectors; // E[hello="Cleveland"][goodbye="Columbus"]
};
