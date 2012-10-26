
#pragma once

#include "css_selector_link.h"

namespace clan
{

class CSSSelectorChain
{
public:
	std::vector<CSSSelectorLink> links;
	std::string pseudo_element; // E:before (E::before in CSS3), E:after (E::after in CSS3)

	size_t get_specificity()
	{
		// CSS2.1: 6.4.3 Calculating a selector's specificity:

		size_t b = 0;
		size_t c = 0;
		size_t d = 0;
		for (size_t i = 0; i < links.size(); i++)
		{
			if (links[i].type == CSSSelectorLink::type_simple_selector || links[i].type == CSSSelectorLink::type_universal_selector)
			{
				if (!links[i].element_id.empty())
					b++;
				c += links[i].element_classes.size();
				c += links[i].pseudo_classes.size();
				c += links[i].attribute_selectors.size();
				if (links[i].type == CSSSelectorLink::type_simple_selector)
					d++;
			}
		}
		if (!pseudo_element.empty())
			d++;

		return (b << 24)+(c << 8)+d;
	}
};

}
